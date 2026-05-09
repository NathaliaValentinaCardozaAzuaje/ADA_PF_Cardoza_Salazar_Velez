#include "parser.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

namespace {

string limpiarCampo(const string& valor) {
    const string espacios = " \t\r\n";
    size_t inicio = valor.find_first_not_of(espacios);
    if (inicio == string::npos) {
        return "";
    }
    size_t fin = valor.find_last_not_of(espacios);
    return valor.substr(inicio, fin - inicio + 1);
}

} // namespace

vector<Solicitud> cargarCSV(
    const string& nombreArchivo,
    int& registrosNulos
) {
    vector<Solicitud> solicitudes;
    ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cerr << "Error abriendo CSV\n";
        return solicitudes;
    }
    string linea;
    getline(archivo, linea);

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string campo;
        vector<string> columnas;
        while (getline(ss, campo, ',')) {
            columnas.push_back(campo);
        }
        Solicitud s;

        s.customerID = columnas[0];
        s.tenure = stoi(columnas[5]);
        s.monthlyCharges = stod(columnas[18]);
        s.churn = limpiarCampo(columnas[20]);

        string cargosTotales = limpiarCampo(columnas[19]);
        if (cargosTotales.empty()) {
            s.totalCharges = 0.0;
            registrosNulos++;
        }
        else {
            s.totalCharges = stod(cargosTotales);
        }
        solicitudes.push_back(s);
    }
    archivo.close();
    return solicitudes;
}