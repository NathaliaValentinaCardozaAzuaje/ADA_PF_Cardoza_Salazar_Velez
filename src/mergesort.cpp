#include "mergesort.hpp"

using namespace std;

void merge(vector<Solicitud>& arr, int izquierda, int medio, int derecha) 
{
    int n1 = medio - izquierda + 1;
    int n2 = derecha - medio;

    vector<Solicitud> L(n1);
    vector<Solicitud> R(n2);

    for (int i = 0; i < n1; i++) {
        L[i] = arr[izquierda + i];
    }

    for (int j = 0; j < n2; j++) {
        R[j] = arr[medio + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = izquierda;

    while (i < n1 && j < n2) {
        if (L[i].tenure >= R[j].tenure) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<Solicitud>& arr, int izquierda, int derecha) 
{

    if (izquierda >= derecha) {
        return;
    }

    int medio = izquierda + (derecha - izquierda) / 2;
    mergeSort(arr, izquierda, medio);
    mergeSort(arr, medio + 1, derecha);
    merge(arr, izquierda, medio, derecha);
}