#include "parser.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

vector<Solicitud> cargarCSV(const string& nombreArchivo,int& registrosNulos) 
{
    vector<Solicitud> solicitudes;
    ifstream archivo(nombreArchivo);

    // Control de Error en caso de que haya un error al abrir el archivo
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
        s.churn = columnas[20];

        if (columnas[19].empty() || columnas[19] == " ") {
            s.totalCharges = 0.0;
            registrosNulos++;
        }
        else {
            s.totalCharges = stod(columnas[19]);
        }
        solicitudes.push_back(s);
    }
    archivo.close();
    return solicitudes;
}