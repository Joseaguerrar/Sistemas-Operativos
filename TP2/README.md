# Tarea Programada II: Algoritmos de Reemplazo de Páginas

# Integrantes
- ### José Guerra Rodríguez C33510
- ### Josué Torres Sibaja C37853

# Visión General del Funcionamiento del Programa
Este programa simula el comportamiento de diferentes algoritmos de reemplazo de páginas en memoria virtual, utilizando como entrada una lista de peticiones de acceso a páginas y otros parámetros que definen cómo debe operar el algoritmo. El objetivo principal es determinar cuántos fallos de página ocurren según el algoritmo seleccionado y el patrón de accesos proporcionado.

El programa utiliza la versión estándar de **C++17**, y solo requiere el compilador **g++**, que ya viene instalado por defecto en muchas distribuciones como Ubuntu o Fedora. También puede instalarse fácilmente desde los repositorios oficiales o ejecutando los siguientes comandos en una terminal:

### Instalación en Distribuciones Basadas en Debian/Ubuntu
```bash
sudo apt update
sudo apt install g++
```

### Instalación en Distribuciones Basadas en Fedora
```bash
sudo dnf install gcc-c++
```

### Verificación de Versión
Una vez instalado, puede verificar que tiene soporte para C++17 ejecutando:
```bash
g++ --version
```

Debe mostrar una versión igual o superior a 7.x para garantizar compatibilidad con C++17.

---

# Entrada del Programa
Al iniciar el programa, se le solicitarán al usuario los siguientes parámetros:

    [Cantidad de marcos]
    [Estado inicial de memoria]
    [Algoritmo a utilizar]
    [Cadena de accesos]

- **Cantidad de marcos**: Número de marcos disponibles en la memoria física (debe ser un entero positivo).
- **Estado inicial de memoria**: Secuencia de páginas separadas por comas (ejemplo: `2,4,3`) que estarán cargadas inicialmente.
- **Algoritmo a utilizar**: Nombre del algoritmo de reemplazo a utilizar. Puede ser uno de los siguientes:
  - `FIFO`, `SecondChance`, `NRU`, `LRU`, `Clock`, `LFU`, `MFU`, o `All` para probar todos.
- **Cadena de accesos**: Secuencia de accesos a páginas, separadas por comas (ejemplo: `5,7,6`).

---

# Proceso Interno
Lectura y validación de la entrada:
- Se valida que el número de marcos sea mayor que cero.
- Se interpreta la cadena de accesos, determinando las páginas y si fueron modificadas.
- Se verifica que el algoritmo indicado sea uno de los implementados.

---

# Ejecución del Algoritmo
- Según el nombre del algoritmo ingresado, se invoca la función correspondiente.
- Se calcula el total de fallos de página generados durante la simulación.
- Se imprime el estado final de la memoria y la cantidad de fallos.

---

# Modo Ranking (All)
- Si el usuario especifica el algoritmo como `All`, se ejecutan todos los algoritmos disponibles.
- Se registra el número de fallos de página para cada uno.
- Se ordenan de menor a mayor cantidad de fallos y se presenta un ranking comparativo.

---

# Salidas del Programa
- Cantidad total de fallos de página.
- Estado final de la memoria principal.
- En el modo `All`, se imprime un ranking de los algoritmos ordenados por eficiencia (menos fallos primero).

---

# Compilar y Ejecutar el Programa
Para compilar y correr el programa se deben ejecutar los siguientes comandos:
```bash
make clean
make
make run
```

Cuando el programa se ejecute, solicitará la entrada para realizar la simulación del algoritmo de reemplazo de páginas.  
En la siguiente sección se proveen distintos casos de prueba para probar el programa, el cual debe devolver el resultado esperado según el caso de prueba.

---

# Casos de Prueba
| Algoritmo | Cantidad de marcos | Estado inicial de memoria | Cadena de accesos | Cantidad de fallos |
| --- | --- | --- | --- | --- |
| FIFO | 4 (default) | Vacía (default) | 2,6,1*,8,2,6,2,0,5*,3,1 (default) | 8 |
| SecondChance | 4 (default) | Vacía (default) | 2,6,1*,8,2,6,2,0,5*,3,1 (default) | 8 |
| NRU | 4 (default) | Vacía (default) | 2,6,1*,8,2,6,2,0,5*,3,1 (default) | 7 |
| LRU | 4 (default) | Vacía (default) | 2,6,1*,8,2,6,2,0,5*,3,1 (default) | 8 |
| Clock | 4 (default) | Vacía (default) | 2,6,1*,8,2,6,2,0,5*,3,1 (default) | 8 |
| LFU | 4 (default) | Vacía (default) | 2,6,1*,8,2,6,2,0,5*,3,1 (default) | 8 |
| MFU | 4 (default) | Vacía (default) | 2,6,1*,8,2,6,2,0,5*,3,1 (default) | 8 |
| All (Ranking) | 4 (default) | Vacía (default) | 2,6,1*,8,2,6,2,0,5*,3,1 (default) | ---> |
**Ranking esperado**:
1. NRU: 7 fallos
2. FIFO: 8 fallos
3. SecondChance: 8 fallos
4. LRU: 8 fallos
5. Clock: 8 fallos
6. LFU: 8 fallos
7. MFU: 8 fallos