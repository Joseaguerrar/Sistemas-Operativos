#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>

/**
 * Divide un string en un vector de strings utilizando un delimitador específico.
 *
 * @param str Cadena de entrada a dividir.
 * @param delimiter Carácter separador.
 * @return Vector con las subcadenas obtenidas.
 */
std::vector<std::string> split(const std::string& str, char delimiter);

/**
 * Convierte una cadena con números separados por comas en un vector de enteros.
 *
 * @param part Cadena con los números (ejemplo: "98, 183, 37").
 * @return Vector de enteros con los números parseados.
 */
std::vector<int> parseRequests(const std::string& part);

/**
 * Convierte todos los caracteres de una cadena a mayúsculas.
 *
 * @param str Cadena de entrada.
 * @return Cadena convertida a mayúsculas.
 */
std::string toUpper(const std::string& str);

/**
 * Extrae y convierte a entero la posición de la cabeza desde una cadena tipo "Head:53".
 *
 * @param part Cadena con el formato "Head:valor".
 * @return Entero con el valor de la cabeza.
 */
int parseHead(const std::string& part);

/**
 * Convierte la dirección de entrada a mayúsculas (ASC o DESC).
 *
 * @param part Cadena con la dirección.
 * @return Cadena convertida a mayúsculas.
 */
std::string parseDirection(const std::string& part);

#endif // UTILS_HPP
