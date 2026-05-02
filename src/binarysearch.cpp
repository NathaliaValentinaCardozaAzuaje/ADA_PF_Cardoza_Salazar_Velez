#include "binarysearch.hpp"

using namespace std;

int busquedaBinariaRecursiva(
    const vector<Solicitud>& arr,
    int izquierda,
    int derecha,
    int k
) {

    // Caso base
    if (izquierda > derecha) {

        return -1;
    }

    int medio =
        izquierda + (derecha - izquierda) / 2;

    // Si cumple la condicion
    if (arr[medio].tenure >= k) {

        // Verificar si es el PRIMERO
        if (
            medio == 0 ||
            arr[medio - 1].tenure < k
        ) {

            return medio;
        }

        // Buscar más a la izquierda
        return busquedaBinariaRecursiva(
            arr,
            izquierda,
            medio - 1,
            k
        );
    }

    // Como es descendente, si no cumple debemos ir a la izquierda
    return busquedaBinariaRecursiva(
        arr,
        izquierda,
        medio - 1,
        k
    );
}