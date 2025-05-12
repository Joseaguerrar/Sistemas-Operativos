#ifndef PAGE_REPLACEMENT_HPP
#define PAGE_REPLACEMENT_HPP

#include <vector>
#include <string>
#include <deque>
#include <unordered_map>

// Número de marcos por defecto
const int DEFAULT_FRAME_COUNT = 4;

// Bits de control
struct PageBits
{
    bool R = false; // Referenciado
    bool M = false; // Modificado
    bool V = true;  // Válido
};

// Representación de una página en memoria
struct PageFrame
{
    int pageNumber = -1; // Número de página
    PageBits bits;       // Bits R, M, V
    int timestamp = 0;   // Para LRU
    int frequency = 0;   // Para LFU/MFU
};

// Enum con los algoritmos disponibles
enum class ReplacementAlgorithm
{
    FIFO,
    SecondChance,
    NRU,
    LRU,
    Clock,
    LFU,
    MFU
};

// Estructura principal para simular la memoria
struct PageReplacementSimulator
{
    int frameCount;
    ReplacementAlgorithm algorithm;
    std::vector<PageFrame> memory;
    std::deque<int> accessSequence;     // Secuencia de solicitudes (sin modificar)
    std::deque<bool> modificationFlags; // Marca si fue modificada (*)
    std::vector<PageFrame> swapSpace;   // Simulación del swap
    int clockHand = 0;                  // Para el algoritmo Clock
    int pageFaults = 0;

    PageReplacementSimulator(int count, ReplacementAlgorithm algo)
        : frameCount(count), algorithm(algo), memory(count) {}
};

// Función para convertir string a ReplacementAlgorithm
ReplacementAlgorithm parseAlgorithm(const std::string &name);

// Función para parsear la entrada como cadena de acceso
void parseAccessSequence(const std::string &input, std::deque<int> &sequence, std::deque<bool> &flags);

// Función para imprimir el estado actual de la memoria (para depuración)
void printMemory(const std::vector<PageFrame> &memory);

#endif // PAGE_REPLACEMENT_HPP
