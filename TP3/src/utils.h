#include "algorithms.h"

/**
 * @brief Parses a comma-separated string of integers.
 *
 * @param str Input string (e.g. "3,5,2").
 * @return Vector of integers parsed from the string.
 */
std::vector<int> parseValues(const std::string& str);

/**
 * @brief Parses the full input string to extract processes and the selected algorithm.
 *
 * @param input Input string in the format: "n | burst_times | priorities | arrivals | algorithm".
 * @param processes Vector to fill with parsed processes.
 * @param algorithm String to store the parsed algorithm name.
 */
void parseInput(const std::string& input, std::vector<Process>& processes, std::string& algorithm);

/**
 * @brief Prints the scheduling results: metrics and averages.
 *
 * @param processes Vector of scheduled processes.
 * @param algorithm Name of the scheduling algorithm used.
 */
void printProcess(const std::vector<Process>& processes, const std::string& algorithm);
