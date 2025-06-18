#include "utils.h"

/**
 * @brief Main entry point for the CPU Scheduling Simulator.
 *
 * The program reads a single line of input describing the process list and selected
 * scheduling algorithm, then runs the corresponding simulation.
 *
 * Example input: 5 | 10,1,2,1,3 | 3,1,4,5,2 | 0 | Priority
 *
 * Supported algorithms: FCFS, SJF, RR, Priority, all
 */
int main() {
  std::string input;
  std::vector<Process> processes;
  std::string algorithm;

  // Prompt user for input
  std::cout << "Enter input (e.g.: 5 | 10,1,2,1,3 | 3,1,4,5,2 | 0 | Priority):\n> ";
  std::getline(std::cin, input);  // Read full line including spaces

  // Parse input string
  parseInput(input, processes, algorithm);

  // Print parsed process information
  printProcess(processes, algorithm);

  // Execute the selected scheduling algorithm
  if (algorithm == "FCFS") {
    fcfs(processes);
  } else if (algorithm == "SJF") {
    sjf(processes);
  } else if (algorithm == "RR") {
    round_robin(processes, 2);
  } else if (algorithm == "Priority") {
    priority_scheduling(processes);
  } else if (algorithm == "all") {
    // Make copies to preserve original input for each algorithm
    std::vector<Process> copy1 = processes;
    std::vector<Process> copy2 = processes;
    std::vector<Process> copy3 = processes;
    std::vector<Process> copy4 = processes;

    fcfs(copy1);
    sjf(copy2);
    round_robin(copy3, 2);
    priority_scheduling(copy4);
  } else {
    std::cerr << "Unrecognized algorithm: " << algorithm << std::endl;
  }

  return 0;
}
