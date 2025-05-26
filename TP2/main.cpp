#include "PageReplacement.hpp"
#include "Algorithms.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

/**
 * @brief Programa principal que ejecuta la simulación de reemplazo de páginas.
 *        Usa valores por defecto si el usuario solo presiona Enter.
 */
int main()
{
    std::string algoritmoStr = "FIFO";
    std::string accesoStr = "[2,6,1*,8,2,6,2,0,5*,3,1]";
    std::string estadoInicialStr = "0";
    int marcoCount = 4;

    std::string input;

    std::cout << "Presiona Enter para usar valores por defecto o introduce los valores:\n";

    std::cout << "Cantidad de marcos (default 4): ";
    std::getline(std::cin, input);
    if (!input.empty())
        marcoCount = std::stoi(input);

    std::cout << "Estado inicial de memoria (default vacía): ";
    std::getline(std::cin, input);
    if (!input.empty())
        estadoInicialStr = input;

    std::cout << "Algoritmo a utilizar (default FIFO): ";
    std::getline(std::cin, input);
    if (!input.empty())
        algoritmoStr = input;

    std::cout << "Cadena de accesos (default [2,6,1*,8,2,6,2,0,5*,3,1]): ";
    std::getline(std::cin, input);
    if (!input.empty())
        accesoStr = input;

    // Procesar entradas
    if (marcoCount == 0)
        marcoCount = DEFAULT_FRAME_COUNT;

    ReplacementAlgorithm algoritmo = parseAlgorithm(algoritmoStr);
    PageReplacementSimulator sim(marcoCount, algoritmo);

    // Cargar estado inicial si se indicó
    if (estadoInicialStr != "0")
    {
        std::vector<int> estadoInicial;
        std::vector<bool> dummyMods;
        parseAccessSequence(estadoInicialStr, estadoInicial, dummyMods);

        for (size_t i = 0; i < estadoInicial.size() && i < static_cast<size_t>(marcoCount); ++i) {
            int page = estadoInicial[i];
            sim.memory[i].pageNumber = page;
            sim.memory[i].bits = {true, false, true};  // R=1, M=0, V=1
            sim.memory[i].frequency = sim.globalFrequencies[page] = 1;
        }
    }

    // Parsear la secuencia de accesos
    std::vector<int> paginas;
    std::vector<bool> modificadas;
    parseAccessSequence(accesoStr, paginas, modificadas);

    // Ejecutar el algoritmo correspondiente
    if (algoritmo == ReplacementAlgorithm::FIFO)
        runFIFO(sim, paginas, modificadas);
    else if (algoritmo == ReplacementAlgorithm::SecondChance)
        runSecondChance(sim, paginas, modificadas);
    else if (algoritmo == ReplacementAlgorithm::NRU)
        runNRU(sim, paginas, modificadas);
    else if (algoritmo == ReplacementAlgorithm::LRU)
        runLRU(sim, paginas, modificadas);
    else if (algoritmo == ReplacementAlgorithm::Clock)
        runClock(sim, paginas, modificadas);
    else if (algoritmo == ReplacementAlgorithm::LFU)
        runLFU(sim, paginas, modificadas);
    else if (algoritmo == ReplacementAlgorithm::MFU)
        runMFU(sim, paginas, modificadas);
    else if (algoritmo == ReplacementAlgorithm::All) {  // Modo ranking
        std::vector<std::pair<std::string, int>> resultados;

        for (ReplacementAlgorithm alg : {
            ReplacementAlgorithm::FIFO,
            ReplacementAlgorithm::SecondChance,
            ReplacementAlgorithm::NRU,
            ReplacementAlgorithm::LRU,
            ReplacementAlgorithm::Clock,
            ReplacementAlgorithm::LFU,
            ReplacementAlgorithm::MFU
        }) {
            PageReplacementSimulator simTmp(marcoCount, alg);

            // Cargar estado inicial si se indicó
            if (estadoInicialStr != "0") {
                std::vector<int> estadoInicial;
                std::vector<bool> dummyMods;
                parseAccessSequence(estadoInicialStr, estadoInicial, dummyMods);

                for (size_t i = 0; i < estadoInicial.size() && i < static_cast<size_t>(marcoCount); ++i) {
                    int page = estadoInicial[i];
                    simTmp.memory[i].pageNumber = page;
                    simTmp.memory[i].bits = {true, false, true};  // R=1, M=0, V=1
                    simTmp.memory[i].frequency = simTmp.globalFrequencies[page] = 1;
                }
            }

            // Ejecutar el algoritmo
            if (alg == ReplacementAlgorithm::FIFO)
                runFIFO(simTmp, paginas, modificadas);
            else if (alg == ReplacementAlgorithm::SecondChance)
                runSecondChance(simTmp, paginas, modificadas);
            else if (alg == ReplacementAlgorithm::NRU)
                runNRU(simTmp, paginas, modificadas);
            else if (alg == ReplacementAlgorithm::LRU)
                runLRU(simTmp, paginas, modificadas);
            else if (alg == ReplacementAlgorithm::Clock)
                runClock(simTmp, paginas, modificadas);
            else if (alg == ReplacementAlgorithm::LFU)
                runLFU(simTmp, paginas, modificadas);
            else if (alg == ReplacementAlgorithm::MFU)
                runMFU(simTmp, paginas, modificadas);

            resultados.emplace_back(to_string(alg), simTmp.pageFaults);
        }

        // Ordenar ranking por número de fallos (ascendente)
        std::sort(resultados.begin(), resultados.end(), [](auto &a, auto &b) {
            return a.second < b.second;
        });

        std::cout << "\n=== Ranking de algoritmos (menor a mayor cantidad de fallos) ===\n";
        for (size_t i = 0; i < resultados.size(); ++i) {
            std::cout << i + 1 << ". " << resultados[i].first << ": " << resultados[i].second << " fallos\n";
        }
    } else {
        std::cerr << "Error: Algoritmo no implementado todavía.\n";
        return 1;
    }

    // Mostrar resultados
    if (algoritmo != ReplacementAlgorithm::All) {
        std::cout << "\nCantidad de fallos de página: " << sim.pageFaults << "\n";
        printMemory(sim.memory);
    }

    return 0;
}
