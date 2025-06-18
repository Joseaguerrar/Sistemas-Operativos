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
    // 1. Show detailed execution
    std::vector<Process> fcfs_p = processes;
    std::vector<Process> sjf_p = processes;
    std::vector<Process> rr_p = processes;
    std::vector<Process> prio_p = processes;

    fcfs(fcfs_p);
    sjf(sjf_p);
    round_robin(rr_p, 2);
    priority_scheduling(prio_p);

    // 2. Make new copies to extract metrics while keeping the output clean
    std::vector<Process> m_fcfs = processes;
    std::vector<Process> m_sjf = processes;
    std::vector<Process> m_rr = processes;
    std::vector<Process> m_prio = processes;

    Metrics fcfs_metrics = captureAndParse(fcfs, m_fcfs);
    Metrics sjf_metrics = captureAndParse(sjf, m_sjf);
    Metrics rr_metrics = captureAndParse([](std::vector<Process>& p) { round_robin(p, 2); }, m_rr);
    Metrics prio_metrics = captureAndParse(priority_scheduling, m_prio);

    // 3. Show final summary
    std::cout << "\n--- Summary ---\n";
    std::cout << "Algorithm     | Avg WT | Avg TT\n";
    std::cout << "--------------|--------|--------\n";
    std::cout << "FCFS          | " << fcfs_metrics.avg_waiting_time << "    | " << fcfs_metrics.avg_turnaround_time << "\n";
    std::cout << "SJF           | " << sjf_metrics.avg_waiting_time << "    | " << sjf_metrics.avg_turnaround_time << "\n";
    std::cout << "Round Robin   | " << rr_metrics.avg_waiting_time << "    | " << rr_metrics.avg_turnaround_time << "\n";
    std::cout << "Priority      | " << prio_metrics.avg_waiting_time << "    | " << prio_metrics.avg_turnaround_time << "\n";
  } else {
    std::cerr << "Unrecognized algorithm: " << algorithm << std::endl;
  }

  return 0;
}
