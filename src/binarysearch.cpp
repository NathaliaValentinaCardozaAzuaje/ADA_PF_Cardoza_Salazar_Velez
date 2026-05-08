#include "binarysearch.hpp"

using namespace std;

int busquedaBinariaRecursiva(
    const vector<Solicitud>& arr,
    int izquierda,
    int derecha,
    int k
) {
    // Caso base: rango inválido
    if (izquierda > derecha) {
        return -1;
    }

    int medio = izquierda + (derecha - izquierda) / 2;

    // Orden descendente por tenure.
    if (arr[medio].tenure == k) {
        // Encontramos una aparición y retornamos cualquiera
        return medio;
    }

    // Si el valor en medio es mayor que k, entonces k (si existe)
    // estará en la derecha porque el arreglo es descendente.
    if (arr[medio].tenure > k) {
        return busquedaBinariaRecursiva(arr, medio + 1, derecha, k);
    }

    // Si arr[medio].tenure < k, buscar a la izquierda.
    return busquedaBinariaRecursiva(arr, izquierda, medio - 1, k);
}