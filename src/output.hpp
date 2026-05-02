#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <vector>
#include <string>
#include "solicitud.hpp"

using namespace std;

void guardarCSVOrdenado(
    const vector<Solicitud>& solicitudes,
    const string& nombreArchivo
);

void guardarBusquedas(
    const vector<string>& resultados,
    const string& nombreArchivo
);

#endif