# Práctica Final de Análisis y Diseño de Algoritmos

## Requisitos

- Compilador C++ compatible con C++17, por ejemplo `g++`.
- Sistema operativo macOS, Linux o Windows con entorno de compilación equivalente.

## Compilación

Desde la raíz del proyecto, compila todos los archivos fuente con uno de los comandos siguientes (elige el que se ajuste a tu entorno):

- Compilación rápida con `g++` (Unix/macOS/WSL/MinGW):

```bash
g++ -std=c++17 -O2 -o ada_pf src/*.cpp
```

- Usando `clang++` (macOS, similar a `g++`):

```bash
clang++ -std=c++17 -O2 -o ada_pf src/*.cpp
```

Notas útiles:

- Si recibes errores de "símbolo redefinido" o "multiple definition", evita incluir `src/main.cpp` dos veces en la lista de fuentes; usar `src/*.cpp` es suficiente.
- Para compilar en Windows con MinGW desde PowerShell o CMD, reemplaza `g++` por la ruta a `g++.exe` si no está en el PATH, y el ejecutable será `ada_pf.exe`.
- Para compilación en modo depuración (simula con menos optimizaciones y símbolos de depuración):

```bash
g++ -std=c++17 -g -O0 -o ada_pf_debug src/*.cpp
```

## Ejecución

Después de compilar, ejecuta el binario desde la raíz del proyecto:

```bash
./ada_pf
```

En Windows (PowerShell/CMD) ejecuta:

```powershell
.\ada_pf.exe
```

## Resultado esperado

Al finalizar la ejecución, la consola muestra la cantidad de registros cargados, la cantidad de nulos detectados, los primeros registros ordenados, las búsquedas realizadas y las mediciones de tiempo.
