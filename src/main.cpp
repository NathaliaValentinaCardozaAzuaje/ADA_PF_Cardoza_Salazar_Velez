#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "binary_search.hpp"
#include "mergesort.hpp"
#include "parser.hpp"
#include "solicitud.hpp"

using namespace std;

void guardarCSVOrdenado(const vector<Solicitud> &solicitudes, const string &nombreArchivo);

void guardarBusquedas(const vector<string> &resultados, const string &nombreArchivo);

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

  mergeSort(solicitudes, 0, solicitudes.size() - 1);

  cout << "\nPrimeros 10 registros ordenados:\n";

  for (int i = 0; i < 10; i++) {
    cout << solicitudes[i].customerID << " " << solicitudes[i].tenure << endl;
  }

  guardarCSVOrdenado(solicitudes, "results/solicitudes_ordenadas.csv");

  vector<int> consultas = {72, 60, 45, 30, 12};
  vector<string> resultadosBusquedas;

  cout << "\nResultados de búsquedas:\n";

  for (int k : consultas) {

    int indice =
        busquedaBinariaRecursiva(solicitudes, 0, solicitudes.size() - 1, k);

    if (indice != -1) {

      string resultado = "k = " + to_string(k) + " -> " + solicitudes[indice].customerID + " tenure: " + to_string(solicitudes[indice].tenure);
      resultadosBusquedas.push_back(resultado);
      cout << resultado << endl;
    }
  }

  guardarBusquedas(resultadosBusquedas, "results/busquedas_A.txt");

  cout << "\nMEDICION DE TIEMPOS\n";

  vector<int> tamanos = {1000, 3500, 7043};

  for (int n : tamanos) {

    vector<Solicitud> subset(solicitudes.begin(), solicitudes.begin() + n);

    auto inicioMerge = chrono::high_resolution_clock::now();

    mergeSort(subset, 0, subset.size() - 1);

    auto finMerge = chrono::high_resolution_clock::now();

    auto tiempoMerge = chrono::duration_cast<chrono::nanoseconds>(finMerge - inicioMerge);

    auto inicioBusqueda = chrono::high_resolution_clock::now();

    busquedaBinariaRecursiva(subset, 0, subset.size() - 1, 45);

    auto finBusqueda = chrono::high_resolution_clock::now();

    auto tiempoBusqueda = chrono::duration_cast<chrono::nanoseconds>(finBusqueda - inicioBusqueda);

    cout << "\nTamano: " << n << endl;

    cout << "MergeSort: " << tiempoMerge.count() << " nanosegundos" << endl;

    cout << "Busqueda binaria: " << tiempoBusqueda.count() << " nanosegundos" << endl;
  }

  cout << "\nArchivos generados correctamente.\n";

  return 0;
}

void guardarCSVOrdenado(const vector<Solicitud> &solicitudes, const string &nombreArchivo) 
{
  ofstream archivo(nombreArchivo);
  
  if (!archivo.is_open()) {
    cerr << "Error creando archivo CSV\n";
    return;
  }

  archivo << "customerID,tenure,MonthlyCharges," << "TotalCharges,Churn\n";

  for (const Solicitud &s : solicitudes) {
    archivo << s.customerID << "," << s.tenure << "," << s.monthlyCharges << "," << s.totalCharges << "," << s.churn << "\n";
  }
  archivo.close();
}

void guardarBusquedas(const vector<string> &resultados, const string &nombreArchivo) 
{
  ofstream archivo(nombreArchivo);
  
  if (!archivo.is_open()) {
    cerr << "Error creando archivo de búsquedas\n";
    return;
  }

  for (const string &linea : resultados) {
    archivo << linea << "\n";
  }
  archivo.close();
}