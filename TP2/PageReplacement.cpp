#include "PageReplacement.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

// Convierte string a enum ReplacementAlgorithm
ReplacementAlgorithm parseAlgorithm(const std::string &name)
{
    std::string lower = name;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if (lower == "fifo")
        return ReplacementAlgorithm::FIFO;
    if (lower == "secondchance")
        return ReplacementAlgorithm::SecondChance;
    if (lower == "nru")
        return ReplacementAlgorithm::NRU;
    if (lower == "lru")
        return ReplacementAlgorithm::LRU;
    if (lower == "clock")
        return ReplacementAlgorithm::Clock;
    if (lower == "lfu")
        return ReplacementAlgorithm::LFU;
    if (lower == "mfu")
        return ReplacementAlgorithm::MFU;

    std::cerr << "Error: algoritmo desconocido: " << name << std::endl;
    std::exit(EXIT_FAILURE);
}

// Parsea una cadena como "[2,6,1*,8,...]" en páginas y flags de modificación
void parseAccessSequence(
    const std::string &input,
    std::vector<int> &pages,
    std::vector<bool> &modifiedFlags)
{
    std::string cleaned;
    for (char c : input)
    {
        if (!std::isspace(c) && c != '[' && c != ']')
        {
            cleaned += c;
        }
    }

    std::stringstream ss(cleaned);
    std::string token;

    while (std::getline(ss, token, ','))
    {
        bool modified = false;
        if (!token.empty() && token.back() == '*')
        {
            modified = true;
            token.pop_back();
        }

        try
        {
            int page = std::stoi(token);
            pages.push_back(page);
            modifiedFlags.push_back(modified);
        }
        catch (...)
        {
            std::cerr << "Error: entrada inválida en secuencia: " << token << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
}

// Imprime el estado actual de la memoria
void printMemory(const std::vector<PageFrame> &memory)
{
    std::cout << "Estado de la memoria:\n";
    for (const auto &frame : memory)
    {
        if (frame.pageNumber == -1)
        {
            std::cout << "[Vacío]\n";
        }
        else
        {
            std::cout << "Página " << frame.pageNumber
                      << " | R: " << frame.bits.R
                      << " M: " << frame.bits.M
                      << " V: " << frame.bits.V
                      << " | Timestamp: " << frame.timestamp
                      << " | Freq: " << frame.frequency << "\n";
        }
    }
    std::cout << std::endl;
}
