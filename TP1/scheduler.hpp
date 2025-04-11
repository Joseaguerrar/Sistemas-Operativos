#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <vector>
#include <string>

//Algoritmos a utilizar


/**
 * Ejecuta el algoritmo FCFS (First Come First Served).
 * Atiende las peticiones en el orden en que fueron solicitadas.
 * 
 * @param requests Vector de cilindros a atender.
 * @param head Posición inicial de la cabeza de lectura.
 * @return Total de cilindros recorridos.
 */
int fcfs(const std::vector<int>& requests, int head);

/**
 * Ejecuta el algoritmo SSTF (Shortest Seek Time First).
 * Atiende siempre la petición más cercana a la posición actual.
 * 
 * @param requests Vector de cilindros a atender.
 * @param head Posición inicial de la cabeza de lectura.
 * @return Total de cilindros recorridos.
 */
int sstf(const std::vector<int>& requests, int head);

/**
 * Ejecuta el algoritmo SCAN (Elevator Algorithm).
 * La cabeza se mueve en una dirección hasta el borde del disco o hasta completar las peticiones,
 * luego cambia de dirección.
 * 
 * @param requests Vector de cilindros a atender.
 * @param head Posición inicial de la cabeza de lectura.
 * @param direction Dirección inicial del movimiento ("ASC" o "DESC").
 * @param maxCylinder Número máximo de cilindros disponibles en el disco.
 * @return Total de cilindros recorridos.
 */
int scan(const std::vector<int>& requests, int head, std::string direction, int maxCylinder);

/**
 * Ejecuta el algoritmo C-SCAN (Circular SCAN).
 * La cabeza se mueve en una dirección hasta el borde del disco y luego vuelve al extremo opuesto sin atender peticiones.
 * 
 * @param requests Vector de cilindros a atender.
 * @param head Posición inicial de la cabeza de lectura.
 * @param direction Dirección inicial del movimiento ("ASC" o "DESC").
 * @param maxCylinder Número máximo de cilindros disponibles en el disco.
 * @return Total de cilindros recorridos.
 */
int c_scan(const std::vector<int>& requests, int head, std::string direction, int maxCylinder);

/**
 * Ejecuta el algoritmo LOOK.
 * Similar a SCAN pero la cabeza solo se mueve hasta la última petición en cada dirección, sin llegar al borde del disco.
 * 
 * @param requests Vector de cilindros a atender.
 * @param head Posición inicial de la cabeza de lectura.
 * @param direction Dirección inicial del movimiento ("ASC" o "DESC").
 * @return Total de cilindros recorridos.
 */
int look(const std::vector<int>& requests, int head, std::string direction);

/**
 * Ejecuta el algoritmo C-LOOK.
 * Similar a C-SCAN pero la cabeza solo se mueve entre las peticiones, saltando del último al primero si es necesario.
 * 
 * @param requests Vector de cilindros a atender.
 * @param head Posición inicial de la cabeza de lectura.
 * @param direction Dirección inicial del movimiento ("ASC" o "DESC").
 * @return Total de cilindros recorridos.
 */
int c_look(const std::vector<int>& requests, int head, std::string direction);

#endif // SCHEDULER_HPP
