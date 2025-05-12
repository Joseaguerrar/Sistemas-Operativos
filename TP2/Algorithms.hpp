#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "PageReplacement.hpp"
#include <vector>

// Una función por algoritmo
void runFIFO(PageReplacementSimulator &sim, const std::vector<int> &pages, const std::vector<bool> &mods);
void runLRU(PageReplacementSimulator &sim, const std::vector<int> &pages, const std::vector<bool> &mods);
// faltan

#endif // ALGORITHMS_HPP
