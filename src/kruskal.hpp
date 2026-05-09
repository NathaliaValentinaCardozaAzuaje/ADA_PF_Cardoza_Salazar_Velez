#ifndef KRUSKAL_HPP
#define KRUSKAL_HPP

#include "graph.hpp"

using namespace std;

class UnionFind {
private:
    vector<int> padre;
    vector<int> rango;

public:
    UnionFind(int n);

    int find(int x);

    void unir(int a, int b);

    bool conectados(int a, int b);
};

vector<Arista> kruskal(const Grafo& grafo, int& pesoTotal);

#endif