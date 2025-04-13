#include <iostream>
#include <cmath>  // abs
#include <algorithm>  // sort, min_element
#include <vector>

#include "scheduler.hpp"

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

    std::cout << "Recorrido FCFS: " << current;

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
    std::vector<int> pending = requests;  // Copia de los requests

    std::cout << "Recorrido SSTF: " << current;

    while (!pending.empty()) {
        auto next_it = std::min_element(pending.begin(), pending.end(),
            [current](int a, int b) {
                return std::abs(a - current) < std::abs(b - current);
            });

        int next = *next_it;
        total += std::abs(next - current);
        current = next;
        std::cout << " -> " << current;

        pending.erase(next_it);  // Eliminamos el atendido
    }

    std::cout << "\n";
    return total;
}

/**
 * Simula el algoritmo SCAN (también conocido como el "ascensor").
 * La cabeza se mueve en una dirección hasta el final, atendiendo peticiones en su camino,
 * luego cambia de dirección y continúa atendiendo el resto.
 *
 * @param requests Vector con los cilindros a visitar.
 * @param head Posición inicial de la cabeza.
 * @param direction Dirección inicial del recorrido: "ASC" o "DESC".
 * @param maxCylinder Valor máximo del cilindro en el disco.
 * @return Total de cilindros recorridos.
 */
int scan(const std::vector<int>& requests, int head, std::string direction, int maxCylinder) {
    int total = 0;
    int current = head;
    std::vector<int> left, right;

    for (int r : requests) {
        if (r < current) {
            left.push_back(r);
        } else {
            right.push_back(r);
        }
    }

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    std::cout << "Recorrido Scan: " << current;

    if (direction == "ASC") {
        // Subiendo
        for (int r : right) {
            total += std::abs(r - current);
            current = r;
            std::cout << " -> " << current;
        }

        // Llegar al borde
        if (current != maxCylinder) {
            total += std::abs(maxCylinder - current);
            current = maxCylinder;
            std::cout << " -> " << current;
        }

        // Bajando
        for(auto it = left.rbegin(); it != left.rend(); ++it) {
            total += std::abs(*it - current);
            current = *it;
            std::cout << " -> " << current;
        }
    } else {
        // Bajando
        for(auto it = left.rbegin(); it != left.rend(); ++it) {
            total += std::abs(*it - current);
            current = *it;
            std::cout << " -> " << current;
        }

        // Llegar al borde
        if (current != 0) {
            total += std::abs(0 - current);
            current = 0;
            std::cout << " -> " << current;
        }

        // Subiendo
        for (int r : right) {
            total += std::abs(r - current);
            current = r;
            std::cout << " -> " << current;
        }
    }

    std::cout << "\n";
    return total;
}

/**
 * Simula el algoritmo C-SCAN (Circular SCAN).
 * La cabeza se mueve en una dirección atendiendo peticiones y, al llegar al extremo,
 * salta al inicio (o al final) sin atender, y continúa en la misma dirección.
 *
 * @param requests Vector con los cilindros a visitar.
 * @param head Posición inicial de la cabeza.
 * @param direction Dirección inicial del recorrido: "ASC" o "DESC".
 * @param maxCylinder Valor máximo del cilindro en el disco.
 * @return Total de cilindros recorridos.
 */
int c_scan(const std::vector<int>& requests, int head, std::string direction, int maxCylinder) {
    int total = 0;
    int current = head;

    std::vector<int> left, right;
    for (int r : requests) {
        if (r < current) {
            left.push_back(r);
        } else {
            right.push_back(r);
        }
    }

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    std::cout << "Recorrido C-Scan: " << current;

    if (direction == "ASC") {
        // Subiendo
        for (int r : right) {
            total += std::abs(r - current);
            current = r;
            std::cout << " -> " << current;
        }

        // Llegar al borde superior
        if (current != maxCylinder) {
            total += std::abs(maxCylinder - current);
            current = maxCylinder;
            std::cout << " -> " << current;
        }

        // Salta a 0
        total += maxCylinder;
        current = 0;
        std::cout << " -> " << current;

        // Subir desde 0
        for (int r : left) {
            total += std::abs(r - current);
            current = r;
            std::cout << " -> " << current;
        }
    } else {
        // Bajando
        for (auto it = left.rbegin(); it != left.rend(); ++it) {
            total += std::abs(*it - current);
            current = *it;
            std::cout << " -> " << current;
        }

        // Llegar al borde inferior
        if (current != 0) {
            total += std::abs(0 - current);
            current = 0;
            std::cout << " -> " << current;
        }

        // Salta a maxCylinder
        total += maxCylinder;
        current = maxCylinder;
        std::cout << " -> " << current;

        // Bajar desde maxCylinder
        for (auto it = right.rbegin(); it != right.rend(); ++it) {
            total += std::abs(*it - current);
            current = *it;
            std::cout << " -> " << current;
        }
    }
    std::cout << "\n";
    return total;
}

/**
 * Simula el algoritmo LOOK.
 * Similar a SCAN, pero en lugar de ir hasta el final del disco,
 * se detiene en el último cilindro solicitado en cada dirección.
 *
 * @param requests Vector con los cilindros a visitar.
 * @param head Posición inicial de la cabeza.
 * @param direction Dirección inicial del recorrido: "ASC" o "DESC".
 * @return Total de cilindros recorridos.
 */
int look(const std::vector<int>& requests, int head, std::string direction) {
    int total = 0;
    int current = head;

    std::vector<int> left, right;
    for (int r : requests) {
        if (r < current) {
            left.push_back(r);
        } else {
            right.push_back(r);
        }
    }
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    std::cout << "Recorrido Look: " << current;

    if (direction == "ASC") {
        // Sube
        for (int r : right) {
            total += std::abs(r - current);
            current = r;
            std::cout << " -> " << current;
        }

        // Baja
        for (auto it = left.rbegin(); it != left.rend(); ++it) {
            total += std::abs(*it - current);
            current = *it;
            std::cout << " -> " << current;
        }
    } else {
        // Baja
        for (auto it = left.rbegin(); it != left.rend(); ++it) {
            total += std::abs(*it - current);
            current = *it;
            std::cout << " -> " << current;
        }

        // Sube
        for (int r : right) {
            total += std::abs(r - current);
            current = r;
            std::cout << " -> " << current;
        }
    }
    std::cout << "\n";
    return total;
}

/**
 * Simula el algoritmo C-LOOK (Circular LOOK).
 * Similar a C-SCAN, pero solo se mueve entre las peticiones,
 * saltando del último al primero sin ir a los extremos del disco.
 *
 * @param requests Vector con los cilindros a visitar.
 * @param head Posición inicial de la cabeza.
 * @param direction Dirección inicial del recorrido: "ASC" o "DESC".
 * @return Total de cilindros recorridos.
 */
int c_look(const std::vector<int>& requests, int head, std::string direction) {
    int total = 0;
    int current = head;

    std::vector<int> left, right;
    for (int r : requests) {
        if (r < current) {
            left.push_back(r);
        } else {
            right.push_back(r);
        }
    }

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    std::cout << "Recorrido C-Look: " << current;

    if (direction == "ASC") {
        // Sube
        for (int r : right) {
            total += std::abs(r - current);
            current = r;
            std::cout << " -> " << current;
        }

        // Baja
        if (!left.empty()) {
            total += std::abs(left.front() - current);
            current = left.front();
            std::cout << " -> " << current;
            
            // Sube desde el menor
            for (int r : left) {
                total += std::abs(r - current);
                current = r;
                std::cout << " -> " << current;
            }
        }
    } else {
        // Baja
        for (auto it = left.rbegin(); it != left.rend(); ++it) {
            total += std::abs(*it - current);
            current = *it;
            std::cout << " -> " << current;
        }

        // Salto al mayor

        if (!right.empty()) {
            total += std::abs(right.back() - current);
            current = right.back();
            std::cout << " -> " << current;

            // Baja desde el mayor
            for (auto it = right.rbegin(); it != right.rend(); ++it) {
                total += std::abs(*it - current);
                current = *it;
                std::cout << " -> " << current;
            }
        }
    }
    std::cout << "\n";
    return total;
}
