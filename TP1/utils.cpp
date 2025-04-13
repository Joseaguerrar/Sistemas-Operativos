#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdexcept>

#include "utils.hpp"

/**
 * Divide un string en un vector de strings utilizando un delimitador específico.
 * 
 * @param str Cadena de entrada.
 * @param delimiter Carácter separador.
 * @return Vector con las subcadenas resultantes.
 */
std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;
    while (getline(ss, item, delimiter)) {
        // Eliminar espacios al inicio y final
        item.erase(0, item.find_first_not_of(' '));
        item.erase(item.find_last_not_of(' ') + 1);
        result.push_back(item);
    }
    return result;
}

/**
 * Convierte una cadena con números separados por comas en un vector de enteros.
 * 
 * @param part Cadena con los números (ejemplo: "98, 183, 37").
 * @return Vector de enteros con los números parseados.
 */
std::vector<int> parseRequests(const std::string& part) {
    std::vector<std::string> nums = split(part, ',');
    std::vector<int> requests;
    for (auto& num : nums) {
        requests.push_back(std::stoi(num));
    }
    return requests;
}

/**
 * Convierte todos los caracteres de una cadena a mayúsculas.
 * 
 * @param str Cadena de entrada.
 * @return Cadena convertida a mayúsculas.
 */
std::string toUpper(const std::string& str) {
    std::string res = str;
    std::transform(res.begin(), res.end(), res.begin(), ::toupper);
    return res;
}

/**
 * Extrae y convierte a entero la posición de la cabeza desde una cadena tipo "Head:53".
 * 
 * @param part Cadena con el formato "Head:valor".
 * @return Entero con el valor de la cabeza.
 */
int parseHead(const std::string& part) {
    size_t pos = part.find(":");
    if (pos == std::string::npos)
        throw std::invalid_argument("Formato incorrecto de Head");
    return std::stoi(part.substr(pos + 1));
}

/**
 * Convierte la dirección de entrada a mayúsculas (ASC o DESC).
 * 
 * @param part Cadena con la dirección.
 * @return Cadena convertida a mayúsculas.
 */
std::string parseDirection(const std::string& part) {
    return toUpper(part);
}
