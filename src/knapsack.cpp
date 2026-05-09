#include "knapsack.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <sstream>

using namespace std;

namespace {

int sumarPesos(const vector<ItemMochila>& items, const vector<int>& indices) {
    int total = 0;
    for (int idx : indices) total += items[idx].peso;
    return total;
}

int sumarValores(const vector<ItemMochila>& items, const vector<int>& indices) {
    int total = 0;
    for (int idx : indices) total += items[idx].valor;
    return total;
}

} // namespace

// ---------------------------------------------------------------------------

vector<ItemMochila> construirItemsMochila(
    const vector<Solicitud>& solicitudesOrdenadas
) {
    vector<ItemMochila> items;
    items.reserve(50);
    for (const auto& s : solicitudesOrdenadas) {
        if (s.churn != "No") continue;
        ItemMochila item;
        item.idCliente       = s.customerID;
        item.cargosTotales   = s.totalCharges;
        item.cargosMensuales = s.monthlyCharges;
        item.peso  = static_cast<int>(round(s.totalCharges));
        item.valor = static_cast<int>(round(s.monthlyCharges * 10.0));
        items.push_back(item);
        if (items.size() == 50) break;
    }
    return items;
}

// ---------------------------------------------------------------------------

int resolverMochila(
    const vector<ItemMochila>& items,
    int W,
    vector<int>& indicesSeleccionados
) {
    const int n = static_cast<int>(items.size());
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    for (int i = 1; i <= n; ++i) {
        int wi = items[i - 1].peso;
        int vi = items[i - 1].valor;
        for (int w = 0; w <= W; ++w) {
            dp[i][w] = (wi > w)
                ? dp[i - 1][w]
                : max(dp[i - 1][w], dp[i - 1][w - wi] + vi);
        }
    }

    // Backtracking
    indicesSeleccionados.clear();
    int w = W;
    for (int i = n; i >= 1; --i) {
        if (items[i-1].peso <= w && dp[i][w] != dp[i-1][w]) {
            indicesSeleccionados.push_back(i - 1);
            w -= items[i - 1].peso;
        }
    }
    reverse(indicesSeleccionados.begin(), indicesSeleccionados.end());
    return dp[n][W];
}

// ---------------------------------------------------------------------------
// Busca exactamente 3 items del conjunto de 50 que:
//   1. Sean seleccionados por el greedy (ordenados por ratio v/w, tomando
//      los que quepan dentro de W hasta completar 3).
//   2. Su valor combinado sea estrictamente menor al valorOptimoPD.
//
// Estrategia: iterar sobre trios (i,j,k) del conjunto. Para cada trio,
// simular el greedy (ordenar por ratio, tomar todos los que quepan en W).
// Si el greedy selecciona exactamente 3 y su valor < valorOptimoPD → contraejemplo.
// ---------------------------------------------------------------------------
ContraejemploCodicioso encontrarContraejemploCodicioso(
    const vector<ItemMochila>& items,
    int W,
    int valorOptimoPD
) {
    ContraejemploCodicioso resultado;
    resultado.encontrado   = false;
    resultado.valorGreedy  = -1;
    resultado.pesoGreedy   = 0;

    const int n = static_cast<int>(items.size());

    // Para cada trio de items del conjunto de 50
    for (int i = 0; i < n - 2; ++i) {
        for (int j = i + 1; j < n - 1; ++j) {
            for (int k = j + 1; k < n; ++k) {
                // Los 3 indices del trio en orden
                int trio[3] = {i, j, k};

                // Simular greedy por ratio sobre este trio
                // Ordenar el trio por ratio descendente
                sort(trio, trio + 3, [&](int a, int b) {
                    double ra = (items[a].peso > 0)
                        ? (double)items[a].valor / items[a].peso : 0.0;
                    double rb = (items[b].peso > 0)
                        ? (double)items[b].valor / items[b].peso : 0.0;
                    return ra > rb;
                });

                // Greedy: tomar los que quepan dentro de W
                vector<int> seleccionados;
                int restante = W;
                for (int t = 0; t < 3; ++t) {
                    if (items[trio[t]].peso <= restante) {
                        seleccionados.push_back(trio[t]);
                        restante -= items[trio[t]].peso;
                    }
                }

                // Necesitamos exactamente 3 items seleccionados
                if ((int)seleccionados.size() != 3) continue;

                int valorG = sumarValores(items, seleccionados);
                int pesoG  = sumarPesos(items, seleccionados);

                // El valor del greedy debe ser menor al optimo de PD
                if (valorG >= valorOptimoPD) continue;

                // Conservar el mejor contraejemplo: mayor valor greedy,
                // pero siempre estrictamente menor al optimo de PD.
                if (!resultado.encontrado || valorG > resultado.valorGreedy) {
                    resultado.encontrado    = true;
                    resultado.indicesGreedy = seleccionados;
                    resultado.valorGreedy   = valorG;
                    resultado.pesoGreedy    = pesoG;
                }
            }
        }
    }

    return resultado;
}

// ---------------------------------------------------------------------------

void escribirReporteMochila(
    const vector<ItemMochila>& items,
    const vector<int>& seleccionados,
    int valorOptimo,
    int W,
    const ContraejemploCodicioso& ce,
    const string& rutaSalida
) {
    ofstream out(rutaSalida, ios::out | ios::trunc);
    if (!out) return;

    // --- Encabezado ---
    out << "=== MODULO C: ASIGNACION DE ANCHO DE BANDA ===\n\n";
    out << "Capacidad total W = " << W << "\n";
    out << "Numero de solicitudes candidatas (Churn=No, top 50 por tenure): "
        << items.size() << "\n\n";

    // --- Solucion optima (PD, 50 items) ---
    out << "--- SOLUCION OPTIMA (Mochila 0-1) ---\n";
    out << "Valor optimo total: " << valorOptimo << " centavos\n";
    out << "Numero de solicitudes seleccionadas: " << seleccionados.size() << "\n";
    out << "Peso total utilizado: " << sumarPesos(items, seleccionados)
        << " / " << W << "\n\n";

    out << "Solicitudes seleccionadas:\n";
    out << "  i  CustomerID        Peso   Valor\n";
    out << "  -  ----------        ----   -----\n";
    for (size_t i = 0; i < seleccionados.size(); ++i) {
        const ItemMochila& item = items[seleccionados[i]];
        out << setw(3) << (i + 1) << "  "
            << setw(14) << item.idCliente << "  "
            << setw(4) << item.peso << "   "
            << setw(5) << item.valor << "\n";
    }

    // --- Contraejemplo codicioso ---
    out << "\n--- CONTRAEJEMPLO CODICIOSO ---\n";
    out << "El greedy por ratio v/w selecciona exactamente 3 solicitudes del\n";
    out << "conjunto de 50 que caben dentro de W = " << W << ", pero su valor\n";
    out << "total es menor al optimo encontrado por la Mochila 0-1.\n\n";

    if (!ce.encontrado) {
        out << "No se encontro contraejemplo de 3 items con valor < " << valorOptimo << ".\n";
    } else {
        // Tabla de los 3 items seleccionados por el greedy
        out << "Items seleccionados por el Codicioso (ratio v/w):\n";
        out << "  #   CustomerID      Peso    Valor    Ratio v/w\n";
        out << "  -   ----------      ----    -----    ---------\n";
        out << fixed << setprecision(4);
        for (size_t pos = 0; pos < ce.indicesGreedy.size(); ++pos) {
            const ItemMochila& item = items[ce.indicesGreedy[pos]];
            double ratio = (item.peso > 0)
                ? static_cast<double>(item.valor) / item.peso : 0.0;
            out << setw(3) << (pos + 1) << "   "
                << setw(12) << item.idCliente << "    "
                << setw(4) << item.peso << "    "
                << setw(5) << item.valor << "    "
                << setw(9) << ratio << "\n";
        }
        out << "  Peso total: " << ce.pesoGreedy << " / " << W << "\n\n";

        // Tabla comparativa
        // Construir string PD: customerIDs del optimo
        ostringstream pdStr;
        for (size_t i = 0; i < seleccionados.size(); ++i) {
            if (i > 0) pdStr << ", ";
            pdStr << items[seleccionados[i]].idCliente;
        }
        // Construir string Greedy: customerIDs de los 3
        ostringstream greedyStr;
        for (size_t i = 0; i < ce.indicesGreedy.size(); ++i) {
            if (i > 0) greedyStr << ", ";
            greedyStr << items[ce.indicesGreedy[i]].idCliente;
        }

        out << "Enfoque               Solicitudes seleccionadas              Valor Total   Optimo?\n";
        out << "--------------------  ------------------------------------   -----------   -------\n";
        out << left;
        out << "Codicioso (ratio)     " << setw(38) << greedyStr.str()
            << right << setw(7) << ce.valorGreedy << "   No\n";
        out << left;
        out << "PD (Mochila 0-1)      " << setw(38) << pdStr.str()
            << right << setw(7) << valorOptimo    << "   Si\n";
        out << left;
    }

    // --- Analisis de complejidad ---
    const int n = static_cast<int>(items.size());
    out << "\n--- ANALISIS DE COMPLEJIDAD ---\n";
    out << "Tiempo:  Theta(n * W) = Theta(" << n << " * " << W
        << ") = Theta(" << (n * W) << ")\n";
    out << "Espacio: Theta(n * W) = Theta(" << (n * W)
        << ") celdas en la tabla dp\n\n";
    out << "Discusion de pseudopolinomialidad:\n";
    out << "La complejidad Theta(n*W) es polinomial en los VALORES de n y W,\n";
    out << "pero W no esta acotado por el tamano del input en bits (log2(W)).\n";
    out << "Si W = 2^k, el algoritmo requiere 2^k columnas -> exponencial en k.\n";
    out << "Por esto se clasifica como pseudopolinomial, no polinomial estricto.\n";
    out << "El problema de la Mochila 0-1 es NP-completo; no se conoce algoritmo\n";
    out << "polinomial en sentido estricto.\n";
}
