#include "knapsack.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <sstream>

using namespace std;

namespace {

// Suma los pesos de un conjunto de indices.
int sumarPesos(const vector<ItemMochila>& items, const vector<int>& indices) {
    int total = 0;
    for (int idx : indices) total += items[idx].peso;
    return total;
}

// Suma los valores de un conjunto de indices.
int sumarValores(const vector<ItemMochila>& items, const vector<int>& indices) {
    int total = 0;
    for (int idx : indices) total += items[idx].valor;
    return total;
}

}

// Construye hasta 50 items desde solicitudes con churn = "No".
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

// Resuelve mochila 0-1 con Programacion Dinamica.
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

    // Reconstruccion de la solucion optima desde la tabla DP.
    indicesSeleccionados.clear();
    int w = W;
    for (int i = n; i >= 1; --i) {
        if (items[i - 1].peso <= w && dp[i][w] != dp[i - 1][w]) {
            indicesSeleccionados.push_back(i - 1);
            w -= items[i - 1].peso;
        }
    }
    reverse(indicesSeleccionados.begin(), indicesSeleccionados.end());
    return dp[n][W];
}

// Busca el mejor contraejemplo codicioso con exactamente 3 items.
// Recorre todos los trios posibles, aplica greedy por ratio v/w y conserva
// el trio con mayor valor codicioso que siga siendo menor al optimo de PD.
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

    for (int i = 0; i < n - 2; ++i) {
        for (int j = i + 1; j < n - 1; ++j) {
            for (int k = j + 1; k < n; ++k) {
                int trio[3] = {i, j, k};

                // Orden descendente por ratio para simular el codicioso.
                sort(trio, trio + 3, [&](int a, int b) {
                    double ra = (items[a].peso > 0)
                        ? static_cast<double>(items[a].valor) / items[a].peso : 0.0;
                    double rb = (items[b].peso > 0)
                        ? static_cast<double>(items[b].valor) / items[b].peso : 0.0;
                    return ra > rb;
                });

                vector<int> seleccionados;
                int restante = W;
                for (int t = 0; t < 3; ++t) {
                    if (items[trio[t]].peso <= restante) {
                        seleccionados.push_back(trio[t]);
                        restante -= items[trio[t]].peso;
                    }
                }

                if (static_cast<int>(seleccionados.size()) != 3) continue;

                int valorG = sumarValores(items, seleccionados);
                int pesoG  = sumarPesos(items, seleccionados);

                if (valorG >= valorOptimoPD) continue;

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

// Escribe el reporte del modulo C en archivo de texto.
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

    out << "=== MODULO C: ASIGNACION DE ANCHO DE BANDA ===\n\n";
    out << "Capacidad total W = " << W << "\n";
    out << "Numero de solicitudes candidatas (Churn=No, top 50 por tenure): "
        << items.size() << "\n\n";

    out << "--- SOLUCION OPTIMA (Mochila 0-1) ---\n";
    out << "Valor optimo total: " << valorOptimo << " centavos\n";
    out << "Numero de solicitudes seleccionadas: " << seleccionados.size() << "\n";
    out << "Peso total utilizado: " << sumarPesos(items, seleccionados)
        << " / " << W << "\n\n";
    
    if (seleccionados.empty()) {
        out << "No se selecciono ninguna solicitud.\n";
    } else {
        out << "Solicitudes seleccionadas:\n";
        out << "  i  CustomerID  Peso  Valor\n";
        out << "  -  ----------  ----  -----\n";
        for (size_t i = 0; i < seleccionados.size(); ++i) {
            const ItemMochila& item = items[seleccionados[i]];
            out << setw(3) << (i + 1) << "  "
                << setw(10) << item.idCliente << "  "
                << setw(3) << item.peso << "  "
                << setw(0) << item.valor << "\n";
        }
    }
    out << "\n--- CONTRAEJEMPLO CODICIOSO ---\n";
    out << "El greedy por ratio v/w selecciona exactamente 3 solicitudes del\n";
    out << "conjunto de 50 que caben dentro de W = " << W << ", pero su valor\n";
    out << "total es menor al optimo encontrado por la Mochila 0-1.\n\n";

    if (!ce.encontrado) {
        out << "No se encontro contraejemplo de 3 items con valor < " << valorOptimo << ".\n";
    } else {
        out << "Items seleccionados por el Codicioso (ratio v/w):\n";
        out << "  #   CustomerID    Peso    Valor    Ratio v/w\n";
        out << "  -   ----------    ----    -----    ---------\n";
        out << fixed << setprecision(4);
        for (size_t pos = 0; pos < ce.indicesGreedy.size(); ++pos) {
            const ItemMochila& item = items[ce.indicesGreedy[pos]];
            double ratio = (item.peso > 0)
                ? static_cast<double>(item.valor) / item.peso : 0.0;
            out << setw(3) << (pos + 1) << "   "
                << setw(10) << item.idCliente << "    "
                << setw(2) << item.peso << "  "
                << setw(5) << item.valor << "    "
                << setw(8) << ratio << "\n";
        }
        out << "  Peso total: " << ce.pesoGreedy << " / " << W << "\n\n";

        ostringstream pdStr;
        for (size_t i = 0; i < seleccionados.size(); ++i) {
            if (i > 0) pdStr << ", ";
            pdStr << items[seleccionados[i]].idCliente;
        }

        ostringstream greedyStr;
        for (size_t i = 0; i < ce.indicesGreedy.size(); ++i) {
            if (i > 0) greedyStr << ", ";
            greedyStr << items[ce.indicesGreedy[i]].idCliente;
        }

        out << "Enfoque               Solicitudes seleccionadas              Valor Total   Optimo?\n";
        out << "--------------------  ------------------------------------   -----------   -------\n";
        out << left;
        out << "Codicioso (ratio)     " << setw(38) << greedyStr.str()
            << right << setw(4) << ce.valorGreedy << setw(14) << "No\n";
        out << left;
        out << "PD (Mochila 0-1)      " << setw(38) << pdStr.str()
            << right << setw(4) << valorOptimo << setw(14) << "Si\n";
        out << left;
    }

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
