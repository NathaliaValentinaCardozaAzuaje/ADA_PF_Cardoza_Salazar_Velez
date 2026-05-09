#ifndef BINARY_SEARCH_HPP
#define BINARY_SEARCH_HPP

#include <vector>

#include "solicitud.hpp"

using namespace std;

int busquedaBinariaRecursiva(
    const vector<Solicitud>& arr,
    int izquierda,
    int derecha,
    int k
);

#endif