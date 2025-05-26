#include "Algorithms.hpp"
#include <unordered_set>
#include <queue>
#include <iostream>
#include <climits>

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
 * @brief Simula el algoritmo de reemplazo de páginas Second Chance (FIFO + bit R).
 *
 * Utiliza una cola circular y el bit R para decidir si una página debe ser reemplazada
 * o reinsertada al final con su bit R en 0.
 *
 * @param sim   Referencia al simulador con la memoria, contador de fallos, etc.
 * @param pages Secuencia de números de páginas solicitadas.
 * @param mods  Flags que indican si cada página fue modificada (*).
 */
void runSecondChance(PageReplacementSimulator &sim, const std::vector<int> &pages, const std::vector<bool> &mods)
{
    std::queue<int> fifoQueue; // Orden FIFO para candidatos
    std::unordered_set<int> loadedPages;

    // Prellenar cola si hay estado inicial para evitar errores
    for (const auto &frame : sim.memory)
    {
        if (frame.pageNumber != -1)
        {
            fifoQueue.push(frame.pageNumber);
            loadedPages.insert(frame.pageNumber);
        }
    }

    for (size_t i = 0; i < pages.size(); ++i)
    {
        int page = pages[i];
        bool modified = mods[i];
        bool hit = false;

        // Verificar si la página ya está cargada
        for (auto &frame : sim.memory)
        {
            if (frame.pageNumber == page)
            {
                hit = true;
                frame.bits.R = true;
                if (modified)
                    frame.bits.M = true;
                break;
            }
        }

        if (hit)
            continue; // No hay fallo

        // Fallo de página
        sim.pageFaults++;

        // Buscar marco libre
        bool placed = false;
        for (auto &frame : sim.memory)
        {
            if (frame.pageNumber == -1)
            {
                frame.pageNumber = page;
                frame.bits = {true, modified, true};
                fifoQueue.push(page);
                loadedPages.insert(page);
                placed = true;
                break;
            }
        }

        if (placed)
            continue;

        // No hay marcos libres: aplicar Second Chance
        while (true)
        {
            if (fifoQueue.empty())
            {
                std::cerr << "Error: fifoQueue quedó vacía, posible corrupción de estado.\n";
                std::exit(EXIT_FAILURE);
            }
            int candidate = fifoQueue.front();
            fifoQueue.pop();

            // Buscar el marco correspondiente al candidato
            for (auto &frame : sim.memory)
            {
                if (frame.pageNumber == candidate)
                {
                    if (frame.bits.R)
                    {
                        frame.bits.R = false;      // Segunda oportunidad
                        fifoQueue.push(candidate); // Lo movemos al final
                    }
                    else
                    {
                        // Reemplazo: quitar víctima
                        loadedPages.erase(candidate);
                        frame.pageNumber = page;
                        frame.bits = {true, modified, true};
                        fifoQueue.push(page);
                        loadedPages.insert(page);
                        break;
                    }
                    break;
                }
            }

            // Salimos si ya insertamos la nueva página
            if (loadedPages.count(page))
                break;
        }
    }
}

/**
 * @brief Simula el algoritmo NRU (Not Recently Used) de forma determinista.
 *
 * Clasifica las páginas en cuatro clases basadas en los bits R (referenciado)
 * y M (modificado). Al ocurrir un fallo de página, se selecciona determinísticamente
 * la primera página encontrada en la clase más baja disponible para su reemplazo.
 * Periódicamente se reinician los bits R para simular el paso del tiempo.
 *
 * Clases:
 *  - Clase 0: R = 0, M = 0  → mejor candidata
 *  - Clase 1: R = 0, M = 1
 *  - Clase 2: R = 1, M = 0
 *  - Clase 3: R = 1, M = 1  → peor candidata
 *
 * @param sim   Referencia al simulador con marcos de página, contador de fallos, etc.
 * @param pages Vector con la secuencia de páginas solicitadas.
 * @param mods  Vector que indica si la página correspondiente fue modificada (con '*').
 */
void runNRU(PageReplacementSimulator &sim, const std::vector<int> &pages, const std::vector<bool> &mods)
{
    for (size_t i = 0; i < pages.size(); ++i)
    {
        int page = pages[i];
        bool modified = mods[i];
        bool hit = false;

        // Buscar si la página ya está en memoria
        for (auto &frame : sim.memory)
        {
            if (frame.pageNumber == page)
            {
                // Página encontrada → hit
                hit = true;
                frame.bits.R = true; // Se marca como referenciada
                if (modified)
                    frame.bits.M = true; // Se marca como modificada si corresponde
                break;
            }
        }

        if (hit)
            continue; // Si fue hit, pasamos al siguiente acceso

        // Fallo de página
        sim.pageFaults++;

        // Buscar un marco libre
        bool placed = false;
        for (auto &frame : sim.memory)
        {
            if (frame.pageNumber == -1)
            {
                // Marco vacío → cargar página sin reemplazar
                frame.pageNumber = page;
                frame.bits = {true, modified, true};
                frame.frequency = 1;
                placed = true;
                break;
            }
        }

        if (placed)
            continue; // Ya se colocó sin necesidad de reemplazo

        // No hay marcos libres: clasificar páginas en clases NRU
        std::vector<int> class0, class1, class2, class3;

        for (size_t j = 0; j < sim.memory.size(); ++j)
        {
            const auto &frame = sim.memory[j];
            bool R = frame.bits.R;
            bool M = frame.bits.M;

            if (!R && !M)
                class0.push_back(j);
            else if (!R && M)
                class1.push_back(j);
            else if (R && !M)
                class2.push_back(j);
            else
                class3.push_back(j);
        }

        // Elegir clase con mayor prioridad disponible
        std::vector<int> *victimClass = nullptr;
        if (!class0.empty())
            victimClass = &class0;
        else if (!class1.empty())
            victimClass = &class1;
        else if (!class2.empty())
            victimClass = &class2;
        else
            victimClass = &class3;

        // Selección determinista: tomar el primer índice de la mejor clase
        int victimIndex = (*victimClass)[0];

        // Reemplazar la página víctima
        sim.memory[victimIndex].pageNumber = page;
        sim.memory[victimIndex].bits = {true, modified, true};
        sim.memory[victimIndex].frequency = 1;

        // Simulación del tiempo: reinicio periódico del bit R (cada 5 accesos)
        if ((i + 1) % 5 == 0)
        {
            for (auto &frame : sim.memory)
                frame.bits.R = false;
        }
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

/**
 * @brief Simula el algoritmo Clock (Reloj), variante eficiente de Second Chance.
 *
 * Usa un puntero circular (clock hand) para recorrer los marcos.
 * Si la página apuntada tiene R=1, se le da una segunda oportunidad (R=0 y se avanza).
 * Si tiene R=0, se reemplaza inmediatamente.
 *
 * @param sim   Referencia al simulador con marcos, contador de fallos, etc.
 * @param pages Secuencia de páginas solicitadas.
 * @param mods  Vector que indica si cada página fue modificada (*).
 */
void runClock(PageReplacementSimulator &sim, const std::vector<int> &pages, const std::vector<bool> &mods)
{
    // Asegurar que el puntero de reloj esté dentro de los límites
    sim.clockHand %= sim.frameCount;

    for (size_t i = 0; i < pages.size(); ++i)
    {
        int page = pages[i];     // Página solicitada en esta iteración
        bool modified = mods[i]; // ¿La página fue modificada en este acceso?
        bool hit = false;        // ¿La página ya estaba en memoria?

        // Verificar si la página ya se encuentra en memoria (HIT)
        for (auto &frame : sim.memory)
        {
            if (frame.pageNumber == page)
            {
                // La página ya está cargada
                hit = true;
                frame.bits.R = true; // Marcar como referenciada
                if (modified)
                    frame.bits.M = true; // Marcar como modificada si aplica
                break;
            }
        }

        if (hit)
            continue; // No hay fallo, se continúa con el siguiente acceso

        // Fallo de página: la página no estaba cargada
        sim.pageFaults++;

        // Buscar un marco libre
        bool placed = false;
        for (auto &frame : sim.memory)
        {
            if (frame.pageNumber == -1)
            {
                // Encontramos un marco libre → colocamos la nueva página aquí
                frame.pageNumber = page;
                frame.bits = {true, modified, true}; // R=1, M=modificado, V=1
                placed = true;
                break;
            }
        }

        if (placed)
            continue; // Se usó un marco libre, no se requiere reemplazo

        // No hay marcos libres → aplicar algoritmo Clock
        while (true)
        {
            // Obtener el marco apuntado por el reloj
            PageFrame &candidate = sim.memory[sim.clockHand];

            if (!candidate.bits.R)
            {
                // Si R=0 → no ha sido referenciada recientemente → reemplazar
                candidate.pageNumber = page;
                candidate.bits = {true, modified, true}; // Nuevo estado de la página cargada

                // Avanzar el reloj para la siguiente iteración futura
                sim.clockHand = (sim.clockHand + 1) % sim.frameCount;
                break;
            }
            else
            {
                // Si R=1 → darle una segunda oportunidad
                candidate.bits.R = false;                             // Limpiar R
                sim.clockHand = (sim.clockHand + 1) % sim.frameCount; // Avanzar el puntero
            }
        }
    }
}

/**
 * @brief Ejecuta el algoritmo de reemplazo de páginas LFU (Least Frequently Used).
 *
 * Este algoritmo selecciona para reemplazo el marco con la menor frecuencia de uso.
 * En caso de empate en la frecuencia, se aplica desempate por orden de llegada (FIFO).
 *
 * @param sim   Referencia al simulador con marcos, contador de fallos, etc.
 * @param pages Secuencia de páginas solicitadas.
 * @param mods  Vector que indica si cada página fue modificada (*).
 */
void runLFU(PageReplacementSimulator &sim, const std::vector<int> &pages, const std::vector<bool> &mods) {
    for (size_t i = 0; i < pages.size(); ++i) {
        int page = pages[i];
        bool modified = mods[i];
        bool hit = false;
        sim.globalTime++;  // Avanzar tiempo global

        // Buscar si la página ya está cargada
        for (auto &frame : sim.memory) {
            if (frame.pageNumber == page) {
                hit = true;
                frame.bits.R = true;
                if (modified)
                    frame.bits.M = true;
                frame.frequency++;  // Aumentar frecuencia
                break;
            }
        }

        if (hit)
            continue;

        // Fallo de página
        sim.pageFaults++;

        bool placed = false;
        for (auto &frame : sim.memory) {
            if (frame.pageNumber == -1) {
                frame.pageNumber = page;
                frame.bits = {true, modified, true};
                frame.frequency = ++sim.globalFrequencies[page];
                frame.timestamp = sim.globalTime;
                placed = true;
                break;
            }
        }

        if (!placed) {
            // Reemplazo LFU con desempate por timestamp (FIFO)
            int minFreq = INT_MAX;
            int oldestTime = INT_MAX;
            int victimIndex = -1;

            for (int j = 0; j < sim.frameCount; ++j) {
                const auto &frame = sim.memory[j];
                if (frame.frequency < minFreq || (frame.frequency == minFreq && frame.timestamp < oldestTime)) {
                    minFreq = frame.frequency;
                    oldestTime = frame.timestamp;
                    victimIndex = j;
                }
            }

            sim.memory[victimIndex].pageNumber = page;
            sim.memory[victimIndex].bits = {true, modified, true};
            sim.memory[victimIndex].frequency = ++sim.globalFrequencies[page];
            sim.memory[victimIndex].timestamp = sim.globalTime;
        }
    }
}

/**
 * @brief Ejecuta el algoritmo de reemplazo de páginas MFU (Most Frequently Used).
 *
 * Este algoritmo selecciona para reemplazo el marco con la mayor frecuencia de uso.
 * En caso de empate en la frecuencia, se aplica desempate por orden de llegada (FIFO).
 *
 * @param sim   Referencia al simulador con marcos, contador de fallos, etc.
 * @param pages Secuencia de páginas solicitadas.
 * @param mods  Vector que indica si cada página fue modificada (*).
 */
void runMFU(PageReplacementSimulator &sim, const std::vector<int> &pages, const std::vector<bool> &mods) {
    for (size_t i = 0; i < pages.size(); ++i) {
        int page = pages[i];
        bool modified = mods[i];
        bool hit = false;
        sim.globalTime++;  // Avanzar tiempo global

        // Buscar si la página ya está cargada
        for (auto &frame : sim.memory) {
            if (frame.pageNumber == page) {
                hit = true;
                frame.bits.R = true;
                if (modified)
                    frame.bits.M = true;
                frame.frequency++;  // Aumentar frecuencia
                break;
            }
        }

        if (hit)
            continue;

        // Fallo de página
        sim.pageFaults++;

        bool placed = false;
        for (auto &frame : sim.memory) {
            if (frame.pageNumber == -1) {
                frame.pageNumber = page;
                frame.bits = {true, modified, true};
                frame.frequency = ++sim.globalFrequencies[page];
                frame.timestamp = sim.globalTime;
                placed = true;
                break;
            }
        }

        if (!placed) {
            // Reemplazo MFU con desempate por timestamp (FIFO inverso)
            int maxFreq = -1;
            int oldestTime = INT_MAX;
            int victimIndex = -1;

            for (int j = 0; j < sim.frameCount; ++j) {
                const auto &frame = sim.memory[j];
                if (frame.frequency > maxFreq || (frame.frequency == maxFreq && frame.timestamp < oldestTime)) {
                    maxFreq = frame.frequency;
                    oldestTime = frame.timestamp;
                    victimIndex = j;
                }
            }

            sim.memory[victimIndex].pageNumber = page;
            sim.memory[victimIndex].bits = {true, modified, true};
            sim.memory[victimIndex].frequency = ++sim.globalFrequencies[page];
            sim.memory[victimIndex].timestamp = sim.globalTime;
        }
    }
}
