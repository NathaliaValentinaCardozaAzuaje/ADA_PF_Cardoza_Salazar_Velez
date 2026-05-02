#include "output.hpp"
#include <fstream>
#include <iostream>

using namespace std;

void guardarCSVOrdenado(
    const vector<Solicitud>& solicitudes,
    const string& nombreArchivo
) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error creando archivo CSV\n";
        return;
    }

    archivo
        << "customerID,tenure,MonthlyCharges,"
        << "TotalCharges,Churn\n";

    for (const Solicitud& s : solicitudes) {
        archivo
            << s.customerID << ","
            << s.tenure << ","
            << s.monthlyCharges << ","
            << s.totalCharges << ","
            << s.churn << "\n";
    }
    archivo.close();
}

void guardarBusquedas(
    const vector<string>& resultados,
    const string& nombreArchivo
) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error creando archivo de búsquedas\n";
        return;
    }

    for (const string& linea : resultados) {
        archivo << linea << "\n";
    }
    archivo.close();
}