#include "Algorithms.hpp"
#include <unordered_set>
#include <queue>
#include <iostream>

/**
 * Simula el algoritmo de reemplazo de páginas FIFO.
 *
 * @param sim   Referencia al simulador con la memoria, contador de fallos, etc.
 * @param pages Secuencia de números de páginas solicitadas.
 * @param mods  Flags que indican si cada página fue modificada (*).
 */
void runFIFO(PageReplacementSimulator &sim, const std::vector<int> &pages, const std::vector<bool> &mods)
{
    std::unordered_set<int> loadedPages; // Para saber qué páginas están en memoria (acceso rápido)
    std::queue<int> fifoQueue;           // Cola para llevar el orden de llegada (FIFO)

    for (size_t i = 0; i < pages.size(); ++i)
    {
        int page = pages[i];     // Página solicitada en esta iteración
        bool modified = mods[i]; // ¿La página fue modificada? (tiene '*')

        bool hit = false; // ¿Ya está en memoria?

        // Verificar si la página ya está cargada (HIT)
        for (auto &frame : sim.memory)
        {
            if (frame.pageNumber == page)
            {
                hit = true;
                frame.bits.R = true; // Marcar que fue referenciada
                if (modified)
                    frame.bits.M = true; // Marcar como modificada si corresponde
                break;
            }
        }

        if (hit)
            continue; // Si ya está, no hay fallo. Pasamos al siguiente acceso.

        // Fallo de página: la página no está en memoria
        sim.pageFaults++;

        // Buscar un marco libre (sin página asignada)
        bool placed = false;
        for (auto &frame : sim.memory)
        {
            if (frame.pageNumber == -1)
            {
                // Cargar la nueva página en un marco vacío
                frame.pageNumber = page;
                frame.bits = {true, modified, true}; // R=1, M según bandera, V=1
                fifoQueue.push(page);                // Registrar el orden de llegada
                loadedPages.insert(page);            // Registrar que está en memoria
                placed = true;
                break;
            }
        }

        if (placed)
            continue; // Si hubo marco libre, saltamos reemplazo.

        // No hay marcos libres: aplicar reemplazo FIFO
        int victim = fifoQueue.front();
        fifoQueue.pop();           // Sacar la página más antigua
        loadedPages.erase(victim); // Eliminar de registro de cargadas

        // Reemplazar la víctima por la nueva página
        for (auto &frame : sim.memory)
        {
            if (frame.pageNumber == victim)
            {
                frame.pageNumber = page;
                frame.bits = {true, modified, true}; // R=1, M según bandera, V=1
                break;
            }
        }

        // Registrar la nueva página como recién cargada
        fifoQueue.push(page);
        loadedPages.insert(page);
    }
}

/**
 * @brief Simula el algoritmo de reemplazo de páginas LRU (Least Recently Used).
 *
 * Este algoritmo reemplaza la página menos recientemente utilizada, usando un contador
 * de tiempo global que se actualiza en cada acceso. Cada marco mantiene un campo `timestamp`
 * que indica el momento del último uso.
 *
 * @param sim   Referencia al simulador con memoria, algoritmo, contadores, etc.
 * @param pages Secuencia de números de página solicitadas.
 * @param mods  Flags que indican si cada página fue modificada (ej. símbolo '*').
 */
void runLRU(PageReplacementSimulator &sim, const std::vector<int> &pages, const std::vector<bool> &mods)
{
    int currentTime = 0; // Contador de tiempo para actualizar los timestamps.

    for (size_t i = 0; i < pages.size(); ++i)
    {
        int page = pages[i];     // Página solicitada en esta iteración.
        bool modified = mods[i]; // Indica si esta página fue modificada.
        bool hit = false;        // Indica si la página ya está en memoria.

        // Verificar si la página ya se encuentra cargada (HIT).
        for (auto &frame : sim.memory)
        {
            if (frame.pageNumber == page)
            {
                hit = true;
                frame.bits.R = true; // Se marca como referenciada.
                if (modified)
                    frame.bits.M = true;         // Se marca como modificada si aplica.
                frame.timestamp = currentTime++; // Actualiza timestamp por acceso.
                break;
            }
        }

        if (hit)
            continue; // No hay fallo, se continúa con el siguiente acceso.

        // Fallo de página: la página no estaba cargada.
        sim.pageFaults++;

        // Buscar un marco libre (sin página cargada).
        bool placed = false;
        for (auto &frame : sim.memory)
        {
            if (frame.pageNumber == -1)
            {
                // Cargar la nueva página en un marco libre.
                frame.pageNumber = page;
                frame.bits = {true, modified, true}; // R=1, M=modificado, V=1
                frame.timestamp = currentTime++;     // Se actualiza el timestamp.
                frame.frequency = 1;                 // Inicializa frecuencia (LFU/MFU).
                placed = true;
                break;
            }
        }

        if (placed)
            continue; // Se usó un marco libre, sin necesidad de reemplazo.

        // No hay marcos libres: buscar la página menos recientemente usada (menor timestamp).
        int lruIndex = 0;
        int minTime = sim.memory[0].timestamp;
        for (int j = 1; j < sim.frameCount; ++j)
        {
            if (sim.memory[j].timestamp < minTime)
            {
                minTime = sim.memory[j].timestamp;
                lruIndex = j;
            }
        }

        // Reemplazar la página menos recientemente usada.
        sim.memory[lruIndex].pageNumber = page;
        sim.memory[lruIndex].bits = {true, modified, true}; // Se actualiza el estado.
        sim.memory[lruIndex].timestamp = currentTime++;     // Nuevo acceso, nuevo timestamp.
        sim.memory[lruIndex].frequency = 1;                 // Reinicia frecuencia.
    }
}
