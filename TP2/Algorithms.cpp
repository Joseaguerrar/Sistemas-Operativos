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
