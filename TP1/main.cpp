#include <iostream>
#include "utils.hpp"

int main() {
    // Entrada de prueba (podrías luego pedirla con cin si querés)
    std::string input = "98, 183, 37, 122, 14, 124, 65, 67 | FCFS | Head:53 | ASC";

    std::cout << "Entrada: " << input << "\n";

    // Parseo con utils
    auto tokens = split(input, '|');

    if(tokens.size() < 2){
        std::cerr << "Error: Entrada inválida\n";
        return 1;
    }

    auto requests = parseRequests(tokens[0]);
    auto algorithm = toUpper(tokens[1]);
    int head = tokens.size() > 2 ? parseHead(tokens[2]) : 0;
    std::string direction = tokens.size() > 3 ? parseDirection(tokens[3]) : "ASC";

    // Mostrar resultados
    std::cout << "Algoritmo: " << algorithm << "\n";
    std::cout << "Head inicial: " << head << "\n";
    std::cout << "Dirección: " << direction << "\n";
    std::cout << "Requests: ";
    for (auto r : requests) {
        std::cout << r << " ";
    }
    std::cout << "\n";

    return 0;
}
