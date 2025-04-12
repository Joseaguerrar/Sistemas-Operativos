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

    std::cout << "Entrada: " << input << "\n";
    std::cout << "Head inicial: " << head << "\n";
    std::cout << "Requests: ";
    for (auto r : requests) std::cout << r << " ";
    std::cout << "\n\n";

    std::cout << "================= FCFS =================\n";
    int total_fcfs = fcfs(requests, head);
    std::cout << "Total del recorrido FCFS: " << total_fcfs << "\n\n";

    std::cout << "================= SSTF =================\n";
    int total_sstf = sstf(requests, head);
    std::cout << "Total del recorrido SSTF: " << total_sstf << "\n";

    return 0;
}
