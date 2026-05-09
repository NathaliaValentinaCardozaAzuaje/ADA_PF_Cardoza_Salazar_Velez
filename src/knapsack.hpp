#ifndef KNAPSACK_HPP
#define KNAPSACK_HPP

#include <string>
#include <vector>

#include "solicitud.hpp"

struct ItemMochila {
    std::string idCliente;
    int peso;
    int valor;
    double cargosTotales;
    double cargosMensuales;
};

// Contraejemplo: el greedy selecciona exactamente 3 items del conjunto de 50
// que caben dentro de W, pero su valor combinado es menor al optimo de PD.
struct ContraejemploCodicioso {
    bool encontrado;
    std::vector<int> indicesGreedy;  // indices en items[] de los 3 items que escogio el greedy
    int valorGreedy;                 // valor total de esos 3 items
    int pesoGreedy;                  // peso total de esos 3 items
};

std::vector<ItemMochila> construirItemsMochila(
    const std::vector<Solicitud>& solicitudesOrdenadas
);

int resolverMochila(
    const std::vector<ItemMochila>& items,
    int W,
    std::vector<int>& indicesSeleccionados
);

// Busca 3 items del conjunto que quepan en W (peso total <= W)
// cuyo valor combinado sea menor al valorOptimoPD.
// El greedy los ordena por ratio y toma todos los que quepan (hasta 3).
ContraejemploCodicioso encontrarContraejemploCodicioso(
    const std::vector<ItemMochila>& items,
    int W,
    int valorOptimoPD
);

void escribirReporteMochila(
    const std::vector<ItemMochila>& items,
    const std::vector<int>& seleccionados,
    int valorOptimo,
    int W,
    const ContraejemploCodicioso& contraejemplo,
    const std::string& rutaSalida
);

#endif
