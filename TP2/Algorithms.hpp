#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "PageReplacement.hpp"
#include <vector>

// Una función por algoritmo
void runFIFO(PageReplacementSimulator &sim, const std::vector<int> &pages, const std::vector<bool> &mods);
void runSecondChance(PageReplacementSimulator &sim, const std::vector<int> &pages, const std::vector<bool> &mods);
void runNRU(PageReplacementSimulator &sim, const std::vector<int> &pages, const std::vector<bool> &mods);
void runLRU(PageReplacementSimulator &sim, const std::vector<int> &pages, const std::vector<bool> &mods);
void runClock(PageReplacementSimulator &sim, const std::vector<int> &pages, const std::vector<bool> &mods);
void runLFU(PageReplacementSimulator &sim, const std::vector<int> &pages, const std::vector<bool> &mods);
void runMFU(PageReplacementSimulator &sim, const std::vector<int> &pages, const std::vector<bool> &mods);

#endif // ALGORITHMS_HPP
