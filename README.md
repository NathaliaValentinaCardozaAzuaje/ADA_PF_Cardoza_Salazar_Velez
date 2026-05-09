# Practica Final Integradora - Analisis y Diseno de Algoritmos

Repositorio del proyecto **ADA_PF_Cardoza_Salazar_Velez**.

El proyecto integra 3 modulos sobre el dataset de Telco Customer Churn:

- **Modulo A (Divide y Venceras):** parseo CSV, MergeSort por `tenure` descendente y busqueda binaria recursiva.
- **Modulo B (Codicioso):** construccion de grafo determinista y MST con Kruskal + Union-Find.
- **Modulo C (Programacion Dinamica):** Mochila 0-1 con backtracking y contraejemplo del enfoque codicioso.

## Requisitos

- Compilador C++ con soporte C++17 (`g++` recomendado).
- Dataset en la ruta:
  - `data/WA_Fn-UseC_-Telco-Customer-Churn.csv`

## Estructura esperada

```text
ADA_PF_Cardoza_Salazar_Velez/
|-- data/
|   `-- WA_Fn-UseC_-Telco-Customer-Churn.csv
|-- src/
|   |-- main.cpp
|   |-- parser.cpp / parser.hpp
|   |-- mergesort.cpp / mergesort.hpp
|   |-- binary_search.cpp / binary_search.hpp
|   |-- graph.cpp / graph.hpp
|   |-- kruskal.cpp / kruskal.hpp
|   `-- knapsack.cpp / knapsack.hpp
|-- results/
|-- report/
`-- README.md
```

## Compilacion

### Linux / macOS / WSL / Git Bash

```bash
g++ -std=c++17 -O2 -o ada_pf src/*.cpp
```

### Windows PowerShell (MinGW)

```powershell
g++ -std=c++17 -O2 -o ada_pf.exe src/*.cpp
```

## Ejecucion

### Linux / macOS / WSL / Git Bash

```bash
./ada_pf data/WA_Fn-UseC_-Telco-Customer-Churn.csv
```

### Compilar y ejecutar de una vez (Linux / macOS / WSL / Git Bash/Windows PowerShell)

```bash
g++ -std=c++17 -O2 -o ada_pf src/*.cpp
./ada_pf data/WA_Fn-UseC_-Telco-Customer-Churn.csv
```

### Windows PowerShell

```powershell
.\ada_pf.exe data\WA_Fn-UseC_-Telco-Customer-Churn.csv
```

> El programa tambien funciona sin argumento y usa por defecto:
> `data/WA_Fn-UseC_-Telco-Customer-Churn.csv`

## Salidas generadas (una sola ejecucion)

Al correr el ejecutable se generan estos archivos en `results/`:

- `solicitudes_ordenadas.csv` (Modulo A)
- `busquedas_A.txt` (Modulo A)
- `mst_red.txt` (Modulo B)
- `asignacion_bw_500.txt` (Modulo C, escenario W=500)
- `asignacion_bw_5000.txt` (Modulo C, escenario W=5000)

## Nota importante de compilacion

No compiles con `src/main.cpp src/*.cpp` al mismo tiempo, porque `main.cpp` queda duplicado.
Usa:

```bash
g++ -std=c++17 -O2 -o ada_pf src/*.cpp
```

## Ejecucion esperada

En consola veras, en orden:

1. Modulo A: ordenamiento, busquedas y medicion de tiempos.
2. Modulo B: datos del grafo y MST por Kruskal.
3. Modulo C: resultados de Mochila para W=500 y W=5000.

Al final aparece:

- `Archivos generados correctamente.`

## Informe

El informe en PDF se encuentra en `report/`.
