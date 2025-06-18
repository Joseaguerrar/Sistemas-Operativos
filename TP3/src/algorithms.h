#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <regex>
#include <map>

/**
 * @struct Process
 * @brief Represents a process in a scheduling algorithm.
 */
struct Process {
  int pid;                  // Unique process ID.
  int burst_time;           // CPU burst time required by the process.
  int priority;             // Priority of the process (lower value = higher priority).
  int arrival;              // Arrival time of the process.
  int start = 0;            // Time at which the process starts execution.
  int end = 0;              // Time at which the process finishes execution.
  int waiting_time = 0;     // Total waiting time of the process.
  int turnaround_time = 0;  // Turnaround time = end - arrival.
};

/**
 * @struct Metrics
 * @brief Holds average metrics calculated from a scheduling algorithm.
 *
 * This struct stores the average waiting time and average turnaround time
 * produced by a CPU scheduling algorithm.
 */
struct Metrics {
  double avg_waiting_time;
  double avg_turnaround_time;
};

/**
 * @brief Simulates the First-Come First-Served (FCFS) scheduling algorithm.
 *
 * @param processes Vector of processes to schedule.
 */
void fcfs(std::vector<Process>& processes);

/**
 * @brief Simulates the Shortest Job First (SJF) scheduling algorithm (non-preemptive).
 *
 * @param processes Vector of processes to schedule.
 */
void sjf(std::vector<Process> processes);

/**
 * @brief Simulates the Round Robin scheduling algorithm.
 *
 * @param processes Vector of processes to schedule.
 * @param quantum Time quantum for each process (default is 2).
 */
void round_robin(std::vector<Process> processes, int quantum = 2);

/**
 * @brief Simulates the Priority scheduling algorithm (non-preemptive).
 *
 * @param processes Vector of processes to schedule.
 */
void priority_scheduling(std::vector<Process> processes);

/**
 * @brief Simula el algoritmo Multilevel Feedback Queue con tres niveles.
 *
 * Nivel 0: Round Robin (Q = 2)
 * Nivel 1: Round Robin (Q = 4)
 * Nivel 2: FCFS
 *
 * @param processes Vector de procesos a planificar.
 */
void multilevel_feedback_queue(std::vector<Process> processes);