#ifndef PAGE_REPLACEMENT_HPP
#define PAGE_REPLACEMENT_HPP

#include <vector>
#include <string>
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
    MFU,
    All
};

// Estructura principal para el simulador
struct PageReplacementSimulator
{
    int frameCount;
    ReplacementAlgorithm algorithm;
    std::vector<PageFrame> memory;
    int clockHand = 0; // Para Clock
    int pageFaults = 0;
    int globalTime = 0;
    std::unordered_map<int, int> globalFrequencies;

    PageReplacementSimulator(int count, ReplacementAlgorithm algo)
        : frameCount(count), algorithm(algo), memory(count) {}
};

// Función para convertir string a ReplacementAlgorithm
ReplacementAlgorithm parseAlgorithm(const std::string &name);

// Función para parsear la secuencia de accesos (e.g. [1*,2,3])
void parseAccessSequence(
    const std::string &input,
    std::vector<int> &pages,
    std::vector<bool> &modifiedFlags);

// Función para imprimir el estado actual de la memoria
void printMemory(const std::vector<PageFrame> &memory);

// Función para convertir un valor del enum a su representación en texto (string)
std::string to_string(ReplacementAlgorithm algo);

inline std::string to_string(ReplacementAlgorithm algo) {
    switch (algo) {
        case ReplacementAlgorithm::FIFO: return "FIFO";
        case ReplacementAlgorithm::SecondChance: return "SecondChance";
        case ReplacementAlgorithm::NRU: return "NRU";
        case ReplacementAlgorithm::LRU: return "LRU";
        case ReplacementAlgorithm::Clock: return "Clock";
        case ReplacementAlgorithm::LFU: return "LFU";
        case ReplacementAlgorithm::MFU: return "MFU";
        case ReplacementAlgorithm::All: return "All";
        default: return "Desconocido";
    }
}

#endif // PAGE_REPLACEMENT_HPP
