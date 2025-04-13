#include <iostream>
#include <map>  // Para el ranking
#include <algorithm>  // sort

#include "utils.hpp"
#include "scheduler.hpp"

int main() {
    std::string input;
    std::cout << "Ingrese la entrada como el siguiente ejemplo: "
        "98, 183, 37, 122, 14, 124, 65, 67 | SCAN | Head:53 | ASC\n";
    std::getline(std::cin, input);

    auto tokens = split(input, '|');

    if(tokens.size() < 2){
        std::cerr << "Error: Entrada inválida\n";
        return 1;
    }

    auto requests = parseRequests(tokens[0]);
    auto algorithm = toUpper(tokens[1]);
    int head = tokens.size() > 2 ? parseHead(tokens[2]) : 0;
    std::string direction = tokens.size() > 3 ? parseDirection(tokens[3]) : "ASC";

    std::cout << "Entrada: " << input << "\n";
    std::cout << "Head inicial: " << head << "\n";
    std::cout << "Requests: ";
    for (auto r : requests) std::cout << r << " ";
    std::cout << "\n\n";

    if(algorithm == "FCFS"){
        std::cout << "================= FCFS =================\n";
        int total_fcfs = fcfs(requests, head);
        std::cout << "Total del recorrido FCFS: " << total_fcfs << "\n\n";
    } else if(algorithm == "SSTF") {
        std::cout << "================= SSTF =================\n";
        int total_sstf = sstf(requests, head);
        std::cout << "Total del recorrido SSTF: " << total_sstf << "\n\n";
    } else if(algorithm == "SCAN") {
        std::cout << "================= Scan =================\n";
        int total_scan = scan(requests, head, direction, 199);  // 199 es maxCylinder (puede cambiarlo)
        std::cout << "Total del recorrido Scan: " << total_scan << "\n\n";
    } else if(algorithm == "C-SCAN") {
        std::cout << "================= C-Scan =================\n";
        int total_c_scan = c_scan(requests, head, direction, 199);
        std::cout << "Total del recorrido C-Scan: " << total_c_scan << "\n\n";
    } else if(algorithm == "LOOK") {
        std::cout << "================= Look ================\n";
        int total_look = look(requests, head, direction);
        std::cout << "Total del recorrido Look: " << total_look << "\n\n";
    } else if(algorithm == "C-LOOK") {
        std::cout << "================= C-Look ================\n";
        int total_c_look = c_look(requests, head, direction);
        std::cout << "Total del recorrido C-Look: " << total_c_look << "\n\n";
    } else if(algorithm == "ALL") {
        std::map<std::string, int> results;

        results["FCFS"] = fcfs(requests, head);
        results["SSTF"] = sstf(requests, head);
        results["SCAN"] = scan(requests, head, direction, 199);
        results["C-SCAN"] = c_scan(requests, head, direction, 199);
        results["LOOK"] = look(requests, head, direction);
        results["C-LOOK"] = c_look(requests, head, direction);

        std::vector<std::pair<std::string, int>> ranking(results.begin(), results.end());
        std::sort(ranking.begin(), ranking.end(), [](auto& a, auto& b) {
            return a.second < b.second;
        });

        std::cout << "\n================= Ranking de todos los algoritmos ================\n";
        for (const auto& [nombre, recorrido] : ranking) {
            std::cout << nombre << " -> " << recorrido << " cilindros\n";
        }

    } else {
        std::cerr << "Algoritmo no reconocido: " << algorithm << "\n";
        return 1;
    }

    return 0;
}
