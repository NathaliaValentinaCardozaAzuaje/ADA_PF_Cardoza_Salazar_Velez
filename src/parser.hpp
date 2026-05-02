#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>
#include "solicitud.hpp"

std::vector<Solicitud> cargarCSV(
    const std::string& nombreArchivo,
    int& registrosNulos
);

#endif