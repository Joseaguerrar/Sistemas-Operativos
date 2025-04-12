#include <iostream>
#include "utils.hpp"
#include "scheduler.hpp"

int main() {
    std::string input = "98, 183, 37, 122, 14, 124, 65, 67 | FCFS | Head:53 | ASC";

    auto tokens = split(input, '|');

    if(tokens.size() < 2){
        std::cerr << "Error: Entrada inválida\n";
        return 1;
    }

    auto requests = parseRequests(tokens[0]);
    auto algorithm = toUpper(tokens[1]);
    int head = tokens.size() > 2 ? parseHead(tokens[2]) : 0;
    std::string direction = tokens.size() > 3 ? parseDirection(tokens[3]) : "ASC";

    std::cout << "Algoritmo: " << algorithm << "\n";
    std::cout << "Head inicial: " << head << "\n";
    std::cout << "Dirección: " << direction << "\n";
    std::cout << "Requests: ";
    for (auto r : requests) std::cout << r << " ";
    std::cout << "\n";

    int total = 0;

    if(algorithm == "FCFS") {
        total = fcfs(requests, head);
    } else {
        std::cerr << "Algoritmo no implementado todavía\n";
        return 1;
    }

    std::cout << "Total del recorrido: " << total << "\n";

    return 0;
}
