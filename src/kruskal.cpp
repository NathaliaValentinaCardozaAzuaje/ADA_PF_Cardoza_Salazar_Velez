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

void UnionFind::unir(int a, int b) {
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