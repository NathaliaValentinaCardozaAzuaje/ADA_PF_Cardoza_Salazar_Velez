#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <chrono>

#include "parser.hpp"
#include "mergesort.hpp"
#include "binarysearch.hpp"
#include "output.hpp"
#include "knapsack.hpp"

using namespace std;

// Punto de entrada del programa.
// Flujo general:
// 1) Carga y ordena solicitudes por tenure.
// 2) Ejecuta busquedas binarias y mediciones de tiempo.
// 3) Ejecuta el modulo C (mochila) para W=500 y W=5000 y escribe reportes.
int main() {
    int registrosNulos = 0;
    vector<Solicitud> solicitudes =
        cargarCSV(
            "data/WA_Fn-UseC_-Telco-Customer-Churn.csv",
            registrosNulos );
    
    if (solicitudes.empty()) {
        cerr << "No se cargaron datos.\n";
        return 1;
    }

    cout << "Registros cargados: "
         << solicitudes.size()
         << endl;

    cout << "Registros nulos: "
         << registrosNulos
         << endl;

    mergeSort(
        solicitudes,
        0,
        solicitudes.size() - 1
    );

    cout << "\nPrimeros 10 registros ordenados:\n";

    for (int i = 0; i < 10; i++) {

        cout
            << solicitudes[i].customerID
            << " "
            << solicitudes[i].tenure
            << endl;
    }

    guardarCSVOrdenado(
        solicitudes,
        "results/solicitudes_ordenadas.csv"
    );

    vector<int> consultas = {
        72,
        60,
        45,
        30,
        12
    };

    vector<string> resultadosBusquedas;

    cout << "\nResultados de busquedas:\n";

    for (int k : consultas) {

        int indice =
            busquedaBinariaRecursiva(
                solicitudes,
                0,
                solicitudes.size() - 1,
                k
            );

        string resultado;

        if (indice != -1) {
            resultado =
                "k = " +
                to_string(k) +
                " -> " +
                solicitudes[indice].customerID +
                " tenure: " +
                to_string(solicitudes[indice].tenure);
        }
        else {
            resultado = "k = " + to_string(k) + " -> no encontrado";
        }

        resultadosBusquedas.push_back(resultado);
        cout << resultado << endl;
    }

    guardarBusquedas(
        resultadosBusquedas,
        "results/busquedas_A.txt"
    );

    cout << "\nMEDICION DE TIEMPOS\n";

    vector<int> tamanos = {
        1000,
        3500,
        7043
    };
    const int repeticionesBusqueda = 1000000;
    volatile int acumuladorBusqueda = 0;
    vector<int> clavesMedicionBusqueda = {72, 60, 45, 30, 12};

    for (int n : tamanos) {

        vector<Solicitud> subset(
            solicitudes.begin(),
            solicitudes.begin() + n
        );

        auto inicioMerge =
            chrono::high_resolution_clock::now();

        mergeSort(
            subset,
            0,
            subset.size() - 1
        );

        auto finMerge =
            chrono::high_resolution_clock::now();

        auto tiempoMerge =
            chrono::duration<double, micro>(finMerge - inicioMerge);

        auto inicioBusqueda =
            chrono::high_resolution_clock::now();

        for (int r = 0; r < repeticionesBusqueda; ++r) {
            int clave = clavesMedicionBusqueda[r % clavesMedicionBusqueda.size()];
            acumuladorBusqueda += busquedaBinariaRecursiva(
                subset,
                0,
                subset.size() - 1,
                clave
            );
        }

        auto finBusqueda =
            chrono::high_resolution_clock::now();

        auto tiempoBusquedaTotal =
            chrono::duration_cast<
                chrono::microseconds
            >(finBusqueda - inicioBusqueda);
        double tiempoBusquedaPromedio =
            static_cast<double>(tiempoBusquedaTotal.count()) / repeticionesBusqueda;

        cout
            << "\nTamano: "
            << n
            << endl;

        cout
            << "MergeSort: "
            << fixed << setprecision(0)
            << tiempoMerge.count()
            << " microsegundos"
            << endl;

        cout
            << "Busqueda binaria: "
            << fixed << setprecision(4)
            << tiempoBusquedaPromedio
            << " microsegundos (promedio de "
            << repeticionesBusqueda
            << " ejecuciones)"
            << endl;
    }

    // Evita que el compilador elimine la medicion por optimizacion agresiva.
    if (acumuladorBusqueda == -1) {
        cout << "";
    }

    // --- MODULO C: dos ejecuciones W=500 y W=5000 ---
    auto itemsMochila = construirItemsMochila(solicitudes);

    // Ejecucion 1: W = 500
    {
        const int W = 500;
        vector<int> indicesSeleccionados;
        int valorOptimo = resolverMochila(itemsMochila, W, indicesSeleccionados);
        ContraejemploCodicioso contraejemplo =
            encontrarContraejemploCodicioso(itemsMochila, W, valorOptimo);
        escribirReporteMochila(
            itemsMochila,
            indicesSeleccionados,
            valorOptimo,
            W,
            contraejemplo,
            "results/asignacion_bw_500.txt"
        );
        cout << "\n[Modulo C] W=500: valor optimo = " << valorOptimo
             << ", items seleccionados = " << indicesSeleccionados.size() << "\n";
    }

    // Ejecucion 2: W = 5000
    {
        const int W = 5000;
        vector<int> indicesSeleccionados;
        int valorOptimo = resolverMochila(itemsMochila, W, indicesSeleccionados);
        ContraejemploCodicioso contraejemplo =
            encontrarContraejemploCodicioso(itemsMochila, W, valorOptimo);
        escribirReporteMochila(
            itemsMochila,
            indicesSeleccionados,
            valorOptimo,
            W,
            contraejemplo,
            "results/asignacion_bw_5000.txt"
        );
        cout << "[Modulo C] W=5000: valor optimo = " << valorOptimo
             << ", items seleccionados = " << indicesSeleccionados.size() << "\n";
    }

    cout << "\nArchivos generados correctamente.\n";

    return 0;
}
