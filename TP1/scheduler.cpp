#include <iostream> 
#include <cmath> //abs

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