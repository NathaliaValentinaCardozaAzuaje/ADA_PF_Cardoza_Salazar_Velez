# Plan de implementación — Módulo C: Asignación de Ancho de Banda
## Programación Dinámica (Mochila 0-1)
### Proyecto ADA — Universidad EAFIT 2026/01

---

## Contexto del módulo

Este módulo recibe como entrada el arreglo ya ordenado por tenure descendente producido por el Módulo A. Selecciona las **50 primeras solicitudes activas** (Churn = "No") de ese arreglo. Sobre ese subconjunto aplica la Mochila 0-1 por tabulación para maximizar el ingreso mensual total asignando ancho de banda limitado W = 500.

---

## Archivos a crear

```
src/knapsack.hpp
src/knapsack.cpp
```

El módulo se invoca desde `main.cpp` y escribe su salida en `results/asignacion_bw.txt`.

---

## Estructuras de datos necesarias

Define en `knapsack.hpp` una estructura para representar cada ítem de la mochila:

```cpp
struct KnapsackItem {
    std::string customerID;
    int    weight;   // TotalCharges redondeado al entero más cercano
    int    value;    // MonthlyCharges * 10, redondeado al entero más cercano
    double totalCharges;
    double monthlyCharges;
};
```

También declara estas funciones:

```cpp
// Construye el vector de ítems a partir de las solicitudes ordenadas del Módulo A
std::vector<KnapsackItem> buildKnapsackItems(
    const std::vector<ServiceRequest>& sortedRequests  // tipo del Módulo A
);

// Resuelve la mochila 0-1 por tabulación completa
// Devuelve el valor óptimo y, por referencia, los índices seleccionados
int solveKnapsack(
    const std::vector<KnapsackItem>& items,
    int W,
    std::vector<int>& selectedIndices   // backtracking: índices de ítems incluidos
);

// Genera el reporte completo en results/asignacion_bw.txt
void writeKnapsackReport(
    const std::vector<KnapsackItem>& items,
    const std::vector<int>& selected,
    int optimalValue,
    int W,
    const std::string& outputPath
);
```

---

## Paso 1 — Selección de los 50 ítems

En `buildKnapsackItems`:

1. Recorrer el arreglo `sortedRequests` (ya ordenado por tenure DESC desde el Módulo A).
2. Filtrar únicamente los registros donde `churn == "No"` (solicitudes activas).
3. Tomar los **primeros 50** de esos registros filtrados.
4. Para cada uno, calcular:
   - `weight = (int) round(totalCharges)`  → si TotalCharges es 0.0 (tenure=0), weight = 0
   - `value  = (int) round(monthlyCharges * 10.0)`
5. Retornar el vector de 50 `KnapsackItem`.

> **Advertencia**: algunos registros tienen `TotalCharges = 0.0` (los 11 con tenure=0 tratados en el parseo). Si alguno de esos 50 ítems tiene weight=0, es válido incluirlo siempre (no ocupa espacio). Manéjalo con normalidad; la recurrencia lo incluirá automáticamente.

---

## Paso 2 — Tabulación de la Mochila 0-1

En `solveKnapsack`, implementa la tabla `dp[i][w]` de dimensiones **(51) × (W+1)** donde W=500.

### Recurrencia

```
dp[0][w] = 0  para todo w en [0, W]

Para i = 1..n:
  Para w = 0..W:
    Si items[i-1].weight > w:
      dp[i][w] = dp[i-1][w]
    Si no:
      dp[i][w] = max(
          dp[i-1][w],
          dp[i-1][w - items[i-1].weight] + items[i-1].value
      )
```

### Implementación sugerida en C++

```cpp
const int n = items.size();   // debe ser <= 50
const int rows = n + 1;
const int cols = W + 1;

// Tabla dp como vector 2D
std::vector<std::vector<int>> dp(rows, std::vector<int>(cols, 0));

for (int i = 1; i < rows; ++i) {
    int wi = items[i-1].weight;
    int vi = items[i-1].value;
    for (int w = 0; w < cols; ++w) {
        if (wi > w) {
            dp[i][w] = dp[i-1][w];
        } else {
            dp[i][w] = std::max(dp[i-1][w], dp[i-1][w - wi] + vi);
        }
    }
}
```

El valor óptimo es `dp[n][W]`.

---

## Paso 3 — Backtracking para reconstruir la solución

Después de llenar la tabla, recorre al revés para saber qué ítems fueron incluidos:

```cpp
std::vector<int> selected;
int w = W;
for (int i = n; i >= 1; --i) {
    if (dp[i][w] != dp[i-1][w]) {
        selected.push_back(i - 1);        // índice en el vector items
        w -= items[i-1].weight;
    }
}
// Opcional: invertir para tener el orden original
std::reverse(selected.begin(), selected.end());
```

Guarda en `selectedIndices` los índices resultantes.

---

## Paso 4 — Contraejemplo codicioso

Este paso es **obligatorio** para la rúbrica. Usando exactamente **3 solicitudes del mismo conjunto de 50**, construye un caso donde el codicioso por ratio `v_i / w_i` no produce el óptimo.

### Cómo encontrar el contraejemplo automáticamente

1. Recorre las combinaciones de 3 ítems del conjunto de 50.
2. Para cada trío `(a, b, c)` con capacidad `W' = w_a + w_b - 1` (o similar):
   - Aplica codicioso por ratio sobre los 3 ítems.
   - Aplica mochila 0-1 exacta sobre los 3 ítems.
   - Si los resultados difieren, guardas ese trío como contraejemplo.
3. Imprime la tabla comparativa.

> **Alternativa manual**: puedes construirlo a mano eligiendo 3 ítems donde uno tenga ratio alto pero peso alto, y dos tengan ratios menores pero pesos bajos que juntos caben y tienen más valor.

### Formato de la tabla comparativa (para el reporte)

```
--- CONTRAEJEMPLO CODICIOSO ---
Capacidad de prueba: W' = XXX

Item    CustomerID    Weight    Value    Ratio v/w
  1     XXXX-XXXX       aa       bb       c.cc
  2     XXXX-XXXX       dd       ee       f.ff
  3     XXXX-XXXX       gg       hh       i.ii

Enfoque               Seleccionados    Valor Total    Optimo?
Codicioso (ratio)     {1}              bb             No
PD (Mochila 0-1)      {2, 3}           ee+hh          Si
```

---

## Paso 5 — Escritura del archivo de salida

El archivo `results/asignacion_bw.txt` debe contener:

```
=== MODULO C: ASIGNACION DE ANCHO DE BANDA ===

Capacidad total W = 500
Numero de solicitudes candidatas (Churn=No, top 50 por tenure): 50

--- SOLUCION OPTIMA (Mochila 0-1) ---
Valor optimo total: XXXXX centavos
Numero de solicitudes seleccionadas: XX
Peso total utilizado: XXX / 500

Solicitudes seleccionadas:
  i  CustomerID        Weight  Value
  -  ----------        ------  -----
  1  XXXX-XXXX           XXX    XXX
  ...

--- CONTRAEJEMPLO CODICIOSO ---
[tabla comparativa como se describio arriba]

--- ANALISIS DE COMPLEJIDAD ---
Tiempo:  Theta(n * W) = Theta(50 * 500) = Theta(25000)
Espacio: Theta(n * W) = Theta(25000) celdas en la tabla dp

Discusion de pseudopolinomialidad:
La complejidad Theta(n*W) es polinomial en los VALORES de n y W,
pero W no esta acotado por el tamano del input en bits (log2(W)).
Si W = 2^k, el algoritmo requiere 2^k columnas → exponencial en k.
Por esto se clasifica como pseudopolinomial, no polinomial estricto.
El problema de la Mochila 0-1 es NP-completo; no se conoce algoritmo
polinomial en sentido estricto.
```

---

## Integración en main.cpp

El flujo desde `main.cpp` debe ser:

```cpp
// 1. Parseo (Módulo A)
auto requests = parseCSV(argv[1]);

// 2. MergeSort (Módulo A)
mergeSort(requests, 0, requests.size() - 1);

// 3. Búsquedas binarias (Módulo A) → escribe busquedas_A.txt

// 4. Construcción del grafo y Kruskal (Módulo B) → escribe mst_red.txt

// 5. Módulo C: usa el arreglo YA ORDENADO de Módulo A
auto knapsackItems = buildKnapsackItems(requests);  // filtra Churn=No, toma 50
std::vector<int> selected;
int optVal = solveKnapsack(knapsackItems, 500, selected);
writeKnapsackReport(knapsackItems, selected, optVal, 500, "results/asignacion_bw.txt");
```

---

## Checklist de verificación antes de entregar

- [ ] La tabla dp tiene exactamente dimensiones `(n+1) × (501)` con `n <= 50`
- [ ] El caso base `dp[0][w] = 0` está correctamente inicializado (vector de ceros lo hace automáticamente)
- [ ] El backtracking recorre desde `i = n` hasta `i = 1` (no hasta 0)
- [ ] El contraejemplo usa ítems del mismo conjunto de 50 (no inventados)
- [ ] El peso total de los ítems seleccionados ≤ 500 (verificar con assert o impresión)
- [ ] La discusión de pseudopolinomialidad está en el archivo de salida Y en el informe
- [ ] `results/asignacion_bw.txt` se genera en una sola ejecución desde `main.cpp`
- [ ] El módulo compila con `-std=c++17 -O2` sin warnings

---

## Notas sobre el informe (Sección Módulo C)

El informe debe incluir:

1. **Formulación de la recurrencia**: escribir explícitamente la ecuación dp[i][w] con casos.
2. **Pseudocódigo** de la tabulación y el backtracking (no el código C++, sino pseudocódigo legible).
3. **Tabla comparativa del contraejemplo** con los 3 ítems reales del dataset.
4. **Discusión de pseudopolinomialidad**: por qué Θ(n·W) no es polinomial estricto, y relación con NP-completitud de la Mochila 0-1.
5. **Justificación del Teorema Maestro** no aplica aquí (aplica en Módulo A); en Módulo C la justificación es directa: dos loops anidados de n y W iteraciones.
