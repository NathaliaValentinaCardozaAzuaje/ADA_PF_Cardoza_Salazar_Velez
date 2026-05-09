#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "binary_search.hpp"
#include "graph.hpp"
#include "knapsack.hpp"
#include "kruskal.hpp"
#include "mergesort.hpp"
#include "output.hpp"
#include "parser.hpp"
#include "solicitud.hpp"

using namespace std;

void guardarMST(const Grafo &grafo, const vector<Arista> &mst, int pesoTotal,
                double costoPromedio, const string &nombreArchivo);

// Punto de entrada del programa.
// Flujo general:
// 1) Carga y ordena solicitudes por tenure.
// 2) Ejecuta busquedas binarias y mediciones de tiempo.
// 3) Ejecuta el modulo B (grafo + Kruskal MST).
// 4) Ejecuta el modulo C (mochila) para W=500 y W=5000 y escribe reportes.
int main() {
  int registrosNulos = 0;
  vector<Solicitud> solicitudes =
      cargarCSV("data/WA_Fn-UseC_-Telco-Customer-Churn.csv", registrosNulos);

  if (solicitudes.empty()) {
    cerr << "No se cargaron datos.\n";
    return 1;
  }

  cout << "Registros cargados: " << solicitudes.size() << endl;
  cout << "Registros nulos: " << registrosNulos << endl;

  // --- MODULO B: Grafo y Kruskal ---
  Grafo grafo = construirGrafo(solicitudes);

  cout << "\nMODULO B\n";
  cout << "Nodos: " << grafo.numNodos << endl;
  cout << "Aristas: " << grafo.aristas.size() << endl;

  int sumaPesos = 0;

  for (const Arista &arista : grafo.aristas) {
    sumaPesos += arista.peso;
  }

  double costoPromedio = (double)sumaPesos / grafo.aristas.size();

  cout << fixed << setprecision(2);
  cout << "Costo promedio de arista: " << costoPromedio << endl;

  int pesoTotalMST = 0;
  vector<Arista> mst = kruskal(grafo, pesoTotalMST);

  cout << "\nMST generado por Kruskal:\n";
  cout << "Aristas en MST: " << mst.size() << endl;
  cout << "Peso total MST: " << pesoTotalMST << endl;

  for (const Arista &arista : mst) {
    cout << arista.origen << " - " << arista.destino << " : " << arista.peso
         << endl;
  }

  guardarMST(grafo, mst, pesoTotalMST, costoPromedio, "results/mst_red.txt");

  cout << "\nPrimeras 5 aristas:\n";

  for (int i = 0; i < 5; i++) {
    cout << grafo.aristas[i].origen << " - " << grafo.aristas[i].destino
         << " : " << grafo.aristas[i].peso << endl;
  }

  // --- MODULO A: Ordenamiento y busqueda ---
  mergeSort(solicitudes, 0, solicitudes.size() - 1);

  cout << "\nPrimeros 10 registros ordenados:\n";

  for (int i = 0; i < 10; i++) {
    cout << solicitudes[i].customerID << " " << solicitudes[i].tenure << endl;
  }

  guardarCSVOrdenado(solicitudes, "results/solicitudes_ordenadas.csv");

  vector<int> consultas = {72, 60, 45, 30, 12};
  vector<string> resultadosBusquedas;

  cout << "\nResultados de busquedas:\n";

  for (int k : consultas) {

    int indice =
        busquedaBinariaRecursiva(solicitudes, 0, solicitudes.size() - 1, k);

    string resultado;

    if (indice != -1) {
      resultado = "k = " + to_string(k) + " -> " +
                  solicitudes[indice].customerID +
                  " tenure: " + to_string(solicitudes[indice].tenure);
    } else {
      resultado = "k = " + to_string(k) + " -> no encontrado";
    }

    resultadosBusquedas.push_back(resultado);
    cout << resultado << endl;
  }

  guardarBusquedas(resultadosBusquedas, "results/busquedas_A.txt");

  // --- MEDICION DE TIEMPOS ---
  cout << "\nMEDICION DE TIEMPOS\n";

  vector<int> tamanos = {1000, 3500, 7043};
  const int repeticionesBusqueda = 1000000;
  volatile int acumuladorBusqueda = 0;
  vector<int> clavesMedicionBusqueda = {72, 60, 45, 30, 12};

  for (int n : tamanos) {

    vector<Solicitud> subset(solicitudes.begin(), solicitudes.begin() + n);

    auto inicioMerge = chrono::high_resolution_clock::now();

    mergeSort(subset, 0, subset.size() - 1);

    auto finMerge = chrono::high_resolution_clock::now();

    auto tiempoMerge = chrono::duration<double, micro>(finMerge - inicioMerge);

    auto inicioBusqueda = chrono::high_resolution_clock::now();

    for (int r = 0; r < repeticionesBusqueda; ++r) {
      int clave = clavesMedicionBusqueda[r % clavesMedicionBusqueda.size()];
      acumuladorBusqueda +=
          busquedaBinariaRecursiva(subset, 0, subset.size() - 1, clave);
    }

    auto finBusqueda = chrono::high_resolution_clock::now();

    auto tiempoBusquedaTotal = chrono::duration_cast<chrono::microseconds>(
        finBusqueda - inicioBusqueda);
    double tiempoBusquedaPromedio =
        static_cast<double>(tiempoBusquedaTotal.count()) / repeticionesBusqueda;

    cout << "\nTamano: " << n << endl;

    cout << "MergeSort: " << fixed << setprecision(0) << tiempoMerge.count()
         << " microsegundos" << endl;

    cout << "Busqueda binaria: " << fixed << setprecision(4)
         << tiempoBusquedaPromedio << " microsegundos (promedio de "
         << repeticionesBusqueda << " ejecuciones)" << endl;
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
    escribirReporteMochila(itemsMochila, indicesSeleccionados, valorOptimo, W,
                           contraejemplo, "results/asignacion_bw_500.txt");
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
    escribirReporteMochila(itemsMochila, indicesSeleccionados, valorOptimo, W,
                           contraejemplo, "results/asignacion_bw_5000.txt");
    cout << "[Modulo C] W=5000: valor optimo = " << valorOptimo
         << ", items seleccionados = " << indicesSeleccionados.size() << "\n";
  }

  cout << "\nArchivos generados correctamente.\n";

  return 0;
}

void guardarMST(const Grafo &grafo, const vector<Arista> &mst, int pesoTotal,
                double costoPromedio, const string &nombreArchivo) {
  ofstream archivo(nombreArchivo);

  if (!archivo.is_open()) {
    cerr << "Error creando archivo MST\n";
    return;
  }

  archivo << fixed << setprecision(2);

  archivo << "MODULO B - RED DE MINIMO COSTO\n\n";

  archivo << "Numero de nodos: " << grafo.numNodos << "\n";
  archivo << "Numero de aristas: " << grafo.aristas.size() << "\n";
  archivo << "Costo promedio de arista: " << costoPromedio << "\n\n";

  archivo << "MST POR KRUSKAL\n";
  archivo << "Cantidad de aristas en MST: " << mst.size() << "\n";
  archivo << "Peso total del MST: " << pesoTotal << "\n\n";

  archivo << "Aristas incluidas en el MST:\n";

  for (const Arista &arista : mst) {
    archivo << arista.origen << " - " << arista.destino << " : " << arista.peso
            << "\n";
  }

  archivo << "\nPropiedad de eleccion codiciosa:\n";
  archivo << "Kruskal selecciona en cada paso la arista de menor peso que no "
             "forma ciclo. ";
  archivo << "Union-Find permite verificar si los dos nodos de una arista ya "
             "pertenecen ";
  archivo << "al mismo componente. Si ya estan conectados, agregar la arista "
             "produciria ";
  archivo << "un ciclo, por lo tanto se descarta. Si no estan conectados, la "
             "arista se ";
  archivo << "agrega al MST y se unen ambos componentes.\n";

  archivo.close();
}
