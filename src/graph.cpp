#include "graph.hpp"
#include <cmath>

Grafo construirGrafo(const vector<Solicitud>& solicitudes) 
{
    const int N = 20;

    vector<double> sumaMonthly(N, 0.0);
    vector<int> cantidadPorGrupo(N, 0);

    for (int i = 0; i < solicitudes.size(); i++) {
    int grupo = i % N;

    sumaMonthly[grupo] += solicitudes[i].monthlyCharges;
    cantidadPorGrupo[grupo]++;

    }

    vector<double> promedio(N, 0.0);

    for (int i = 0; i < N; i++) {
        promedio[i] = sumaMonthly[i] / cantidadPorGrupo[i];
        promedio[i] = round(promedio[i] * 100.0) / 100.0;
    }

    Grafo grafo;
    grafo.numNodos = N;

    for (int u = 0; u < N; u++) {
        for (int v = u + 1; v < N; v++) {
            Arista arista;
            arista.origen = u;
            arista.destino = v;
            arista.peso = floor(promedio[u] + promedio[v]);

            grafo.aristas.push_back(arista);
        }
    }

    return grafo;

}

