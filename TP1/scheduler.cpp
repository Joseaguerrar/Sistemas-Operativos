#include <iostream> 
#include <cmath> //abs
#include <algorithm> //std::min_element
#include <vector>

#include"scheduler.hpp"

/**
 * Simula el algoritmo FCFS (First Come First Served).
 * Atiende las peticiones en el orden que llegan.
 * 
 * @param requests Vector con los cilindros a visitar.
 * @param head Posición inicial de la cabeza.
 * @return Total de cilindros recorridos.
 */
int fcfs(const std::vector<int>& requests, int head) {
    int total = 0;
    int current = head;

    std::cout << "Recorrido: " << current;

    for (int next : requests) {
        total += std::abs(next - current);
        current = next;
        std::cout << " -> " << current;
    }

    std::cout << "\n";

    return total;
}

/**
 * Simula el algoritmo SSTF (Shortest Seek Time First).
 * Atiende siempre la petición más cercana a la posición actual.
 * 
 * @param requests Vector con los cilindros a visitar.
 * @param head Posición inicial de la cabeza.
 * @return Total de cilindros recorridos.
 */
int sstf(const std::vector<int>& requests, int head) {
    int total = 0;
    int current = head;
    std::vector<int> pending = requests; // Copia de los requests

    std::cout << "Recorrido: " << current;

    while (!pending.empty()) {
        auto next_it = std::min_element(pending.begin(), pending.end(),
            [current](int a, int b) {
                return std::abs(a - current) < std::abs(b - current);
            });

        int next = *next_it;
        total += std::abs(next - current);
        current = next;
        std::cout << " -> " << current;

        pending.erase(next_it); // Eliminamos el atendido
    }

    std::cout << "\n";
    return total;
}