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

/**
 * @brief Executes a scheduling algorithm and extracts average metrics from its output.
 *
 * This function temporarily redirects std::cout to a string buffer, runs the
 * given scheduling algorithm function on the provided process list, and then
 * parses the captured output to extract the average waiting time and
 * turnaround time using regular expressions.
 *
 * @tparam Func A callable type (e.g., function pointer or lambda) that accepts
 *   a reference to a vector of Process objects and performs scheduling.
 * @param algorithmFunc The scheduling algorithm function to execute.
 * @param processes A vector of processes to be passed to the algorithm.
 * @return Metrics struct containing the parsed average waiting and turnaround times.
 */
template <typename Func>
Metrics captureAndParse(Func algorithmFunc, std::vector<Process>& processes) {
  std::streambuf* original_buf = std::cout.rdbuf();  // Save original cout
  std::stringstream buffer;
  std::cout.rdbuf(buffer.rdbuf());  // Redirect cout

  algorithmFunc(processes);  // Execute algorithm

  std::cout.rdbuf(original_buf);  // Restore cout

  // Parse average from buffer
  std::string output = buffer.str();
  std::regex avg_regex(
    R"(Average Waiting Time: ([0-9.]+)\s+Average Turnaround Time: ([0-9.]+))"
  );
  std::smatch match;

  Metrics m{0, 0};
  if (std::regex_search(output, match, avg_regex)) {
    m.avg_waiting_time = std::stod(match[1].str());
    m.avg_turnaround_time = std::stod(match[2].str());
  }
  return m;
}
