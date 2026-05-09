#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include "solicitud.hpp"

using namespace std;

struct Arista {
    int origen;
    int destino;
    int peso;
};

struct Grafo {
    int numNodos;
    vector<Arista> aristas;
};

Grafo construirGrafo(const vector<Solicitud>& solicitudes);

#endif