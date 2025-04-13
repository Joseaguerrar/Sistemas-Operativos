#include <iostream>
#include <map>  // Para el ranking
#include <algorithm>  // sort

#include "utils.hpp"
#include "scheduler.hpp"

int main() {
    // Recibir la entrada del usuario
    std::string input;
    std::cout << "\nIngrese la entrada como el siguiente ejemplo: "
        "98, 183, 37, 122, 14, 124, 65, 67 | FCFS | Head:53 | ASC\n";
    std::getline(std::cin, input);

    auto tokens = split(input, '|');

    if (tokens.size() < 2) {
        std::cerr << "\nError: Entrada inválida\n";
        return 1;
    }

    // Interpretar cada parte de la entrada
    auto requests = parseRequests(tokens[0]);
    auto algorithm = toUpper(tokens[1]);
    int head = tokens.size() > 2 ? parseHead(tokens[2]) : 0;
    std::string direction = "ASC";
    if (tokens.size() > 3) {
        direction = parseDirection(tokens[3]);
        if (direction != "ASC" && direction != "DESC") {
            std::cout << "Dirección inválida: '" << direction << "'. "
                "Usando dirección por defecto: ASC\n";
            direction = "ASC";
        }
    }

    // Datos de la entrada para hacer la simulación
    std::cout << "\nEntrada: " << input << "\n";
    std::cout << "Algoritmo: " << algorithm << "\n";
    std::cout << "Head inicial: " << head << "\n";
    std::cout << "Dirección: " << direction << "\n";
    std::cout << "Requests: ";
    for (auto r : requests) std::cout << r << " ";
    std::cout << "\n\n";

    // Ejecutar los algoritmos según el caso
    if (algorithm == "FCFS") {
        std::cout << "================= FCFS =================\n";
        int total_fcfs = fcfs(requests, head);
        std::cout << "Total de cilindros recorridos con FCFS: " << total_fcfs << "\n\n";
    } else if (algorithm == "SSTF") {
        std::cout << "================= SSTF =================\n";
        int total_sstf = sstf(requests, head);
        std::cout << "Total de cilindros recorridos con SSTF: " << total_sstf << "\n\n";
    } else if (algorithm == "SCAN") {
        std::cout << "================= Scan =================\n";
        int total_scan = scan(requests, head, direction, 199);  // 199 es maxCylinder (puede cambiarlo)
        std::cout << "Total de cilindros recorridos con Scan: " << total_scan << "\n\n";
    } else if (algorithm == "C-SCAN") {
        std::cout << "================= C-Scan =================\n";
        int total_c_scan = c_scan(requests, head, direction, 199);
        std::cout << "Total de cilindros recorridos con C-Scan: " << total_c_scan << "\n\n";
    } else if (algorithm == "LOOK") {
        std::cout << "================= Look =================\n";
        int total_look = look(requests, head, direction);
        std::cout << "Total de cilindros recorridos con Look: " << total_look << "\n\n";
    } else if (algorithm == "C-LOOK") {
        std::cout << "================= C-Look =================\n";
        int total_c_look = c_look(requests, head, direction);
        std::cout << "Total de cilindros recorridos con C-Look: " << total_c_look << "\n\n";
    } else if (algorithm == "ALL") {
        // Si se escribe "ALL" o "all", se ejecutarán todos los algoritmos y se creará un ranking
        std::map<std::string, int> results;

        results["FCFS"] = fcfs(requests, head);
        results["SSTF"] = sstf(requests, head);
        results["SCAN"] = scan(requests, head, direction, 199);
        results["C-SCAN"] = c_scan(requests, head, direction, 199);
        results["LOOK"] = look(requests, head, direction);
        results["C-LOOK"] = c_look(requests, head, direction);

        // Ordenar los resultados
        std::vector<std::pair<std::string, int>> ranking(results.begin(), results.end());
        std::sort(ranking.begin(), ranking.end(), [](auto& a, auto& b) {
            return a.second < b.second;
        });

        std::cout << "\n================= Ranking de todos los algoritmos =================\n";
        int rankingPos = 1;
        for (const auto& [nombre, recorrido] : ranking) {
            std::cout << rankingPos << ". " << nombre << " -> " << recorrido << " cilindros\n";
            rankingPos++;
        }

    } else {
        std::cerr << "Algoritmo no reconocido: " << algorithm << "\n";
        return 1;
    }

    return 0;
}
