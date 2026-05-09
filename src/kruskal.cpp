#include "kruskal.hpp"
#include <algorithm>

UnionFind::UnionFind(int n) {
    padre.resize(n);
    rango.resize(n, 0);

    for (int i = 0; i < n; i++) {
        padre[i] = i;
    }
}

int UnionFind::find(int x) {
    if (padre[x] != x) {
        padre[x] = find(padre[x]);
    }

    return padre[x];
}

bool UnionFind::conectados(int a, int b) {
    return find(a) == find(b);
}

void UnionFind::unir(int a, int b) 
{
    int raizA = find(a);
    int raizB = find(b);

    if (raizA == raizB) {
        return;
    }

    if (rango[raizA] < rango[raizB]) {
        padre[raizA] = raizB;
    }
    else if (rango[raizA] > rango[raizB]) {
        padre[raizB] = raizA;
    }
    else {
        padre[raizB] = raizA;
        rango[raizA]++;
    }
}

bool compararAristas(const Arista& a, const Arista& b) {
    return a.peso < b.peso;
}

vector<Arista> kruskal(const Grafo& grafo, int& pesoTotal) {

    vector<Arista> mst;
    pesoTotal = 0;
    vector<Arista> aristasOrdenadas = grafo.aristas;

    sort(aristasOrdenadas.begin(), aristasOrdenadas.end(), compararAristas);

    UnionFind uf(grafo.numNodos);

    for (const Arista& arista : aristasOrdenadas) 
    {

        if (!uf.conectados(arista.origen, arista.destino)) 
        {
            uf.unir(arista.origen, arista.destino);
            mst.push_back(arista);
            pesoTotal += arista.peso;

            if (mst.size() == grafo.numNodos - 1) 
            {
                break;
            }
        }
    }
    return mst;
}