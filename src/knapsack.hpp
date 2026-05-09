#ifndef KNAPSACK_HPP
#define KNAPSACK_HPP

#include <string>
#include <vector>

#include "solicitud.hpp"

// Representa una solicitud convertida al modelo de mochila.
struct ItemMochila {
    std::string idCliente;
    int peso;
    int valor;
    double cargosTotales;
    double cargosMensuales;
};

// Resultado del contraejemplo codicioso con exactamente 3 items.
// Si se encuentra, contiene los indices seleccionados por greedy, su peso
// total y su valor total (estrictamente menor al optimo de PD).
struct ContraejemploCodicioso {
    bool encontrado;
    std::vector<int> indicesGreedy;  // indices en items[] de los 3 items que escogio el greedy
    int valorGreedy;                 // valor total de esos 3 items
    int pesoGreedy;                  // peso total de esos 3 items
};

// Construye hasta 50 items de mochila desde solicitudes ordenadas.
// Toma solicitudes con churn = "No", y transforma:
// peso = round(totalCharges), valor = round(monthlyCharges * 10).
std::vector<ItemMochila> construirItemsMochila(
    const std::vector<Solicitud>& solicitudesOrdenadas
);

// Resuelve Mochila 0-1 con Programacion Dinamica.
// Llena indicesSeleccionados con la solucion optima y retorna su valor.
int resolverMochila(
    const std::vector<ItemMochila>& items,
    int W,
    std::vector<int>& indicesSeleccionados
);

// Busca un contraejemplo del enfoque codicioso por ratio v/w.
// Recorre trios de items, simula greedy en cada trio y conserva el mejor
// contraejemplo (mayor valor greedy) que siga cumpliendo valorGreedy < PD.
ContraejemploCodicioso encontrarContraejemploCodicioso(
    const std::vector<ItemMochila>& items,
    int W,
    int valorOptimoPD
);

// Escribe el reporte final del modulo C en archivo de texto.
// Incluye solucion optima por PD, contraejemplo codicioso y complejidad.
void escribirReporteMochila(
    const std::vector<ItemMochila>& items,
    const std::vector<int>& seleccionados,
    int valorOptimo,
    int W,
    const ContraejemploCodicioso& contraejemplo,
    const std::string& rutaSalida
);

#endif
