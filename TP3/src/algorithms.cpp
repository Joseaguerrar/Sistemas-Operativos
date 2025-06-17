#include "algorithms.h"

void fcfs(std::vector<Process>& processes) {
  // Sort processes by arrival time
  std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
    return a.arrival < b.arrival;
  });

  int current_time = 0;
  float total_waiting = 0, total_turnaround = 0;

  // Schedule each process in order
  for (auto& p : processes) {
    // If CPU is idle, move to the arrival time of the next process
    if (current_time < p.arrival) {
      current_time = p.arrival;
    }

    p.start = current_time;
    p.end = current_time + p.burst_time;
    p.turnaround_time = p.end - p.arrival;
    p.waiting_time = p.start - p.arrival;

    current_time += p.burst_time;
    total_waiting += p.waiting_time;
    total_turnaround += p.turnaround_time;
  }

  float throughput = static_cast<float>(processes.size()) / current_time;

  // Output results
  std::cout << "\n--- FCFS ---" << std::endl;
  for (const auto& p : processes) {
    std::cout << "P" << p.pid
      << " -> Start: " << p.start
      << ", End: " << p.end
      << ", Waiting Time: " << p.waiting_time
      << ", Turnaround Time: " << p.turnaround_time
      << std::endl;
  }

  std::cout << "Average Waiting Time: " << total_waiting / processes.size() << std::endl;
  std::cout << "Average Turnaround Time: " << total_turnaround / processes.size() << std::endl;
  std::cout << "Throughput: " << throughput << " processes/time unit\n" << std::endl;
}

void sjf(std::vector<Process> processes) {
  std::vector<Process> ready;
  std::vector<Process> result;
  int current_time = 0;
  size_t completed = 0;
  float total_waiting = 0, total_turnaround = 0;

  // Repeat until all processes are completed
  while (completed < processes.size()) {
    // Move all arrived processes to the ready queue
    for (auto it = processes.begin(); it != processes.end();) {
      if (it->arrival <= current_time) {
        ready.push_back(*it);
        it = processes.erase(it);
      } else {
        ++it;
      }
    }

    if (!ready.empty()) {
      // Pick the process with the shortest burst time
      auto it = std::min_element(ready.begin(), ready.end(), [](const Process& a, const Process& b) {
        return a.burst_time < b.burst_time;
      });

      Process p = *it;
      ready.erase(it);

      p.start = current_time;
      p.end = p.start + p.burst_time;
      p.turnaround_time = p.end - p.arrival;
      p.waiting_time = p.start - p.arrival;

      current_time = p.end;
      result.push_back(p);
      total_waiting += p.waiting_time;
      total_turnaround += p.turnaround_time;
      completed++;
    } else {
      // No process is ready, CPU idle
      current_time++;
    }
  }

  // Output results
  std::cout << "\n--- SJF ---" << std::endl;
  for (const auto& p : result) {
    std::cout << "P" << p.pid
      << " -> Start: " << p.start
      << ", End: " << p.end
      << ", Waiting Time: " << p.waiting_time
      << ", Turnaround Time: " << p.turnaround_time
      << std::endl;
  }

  float throughput = static_cast<float>(result.size()) / current_time;
  std::cout << "Average Waiting Time: " << total_waiting / result.size() << std::endl;
  std::cout << "Average Turnaround Time: " << total_turnaround / result.size() << std::endl;
  std::cout << "Throughput: " << throughput << " processes/time unit\n" << std::endl;
}

void round_robin(std::vector<Process> processes, int quantum) {
  std::queue<Process> queue;
  int current_time = 0;
  float total_waiting = 0, total_turnaround = 0;
  std::map<int, int> remaining_time;    // Remaining burst time per process
  std::map<int, int> first_start_time;  // Time of first execution per process
  std::vector<Process> result;
  size_t completed = 0;

  // Initialize remaining time map
  for (const auto& p : processes) {
    remaining_time[p.pid] = p.burst_time;
  }

  // Sort processes by arrival time
  std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
    return a.arrival < b.arrival;
  });

  size_t index = 0;

  // Main scheduling loop
  while (completed < processes.size()) {
    // Add all processes that have arrived up to current_time
    while (index < processes.size() && processes[index].arrival <= current_time) {
      queue.push(processes[index]);
      index++;
    }

    // CPU is idle
    if (queue.empty()) {
      current_time++;
      continue;
    }

    Process current = queue.front();
    queue.pop();

    // Record the first time the process runs
    if (first_start_time.count(current.pid) == 0)
      first_start_time[current.pid] = current_time;

    // Run the process for a time slice
    int exec_time = std::min(quantum, remaining_time[current.pid]);
    current_time += exec_time;
    remaining_time[current.pid] -= exec_time;

    // Add newly arrived processes during this time slice
    while (index < processes.size() && processes[index].arrival <= current_time) {
      queue.push(processes[index]);
      index++;
    }

    // If the process has remaining time, enqueue again
    if (remaining_time[current.pid] > 0) {
      queue.push(current);
    } else {
      completed++;
      current.start = first_start_time[current.pid];
      current.end = current_time;
      current.turnaround_time = current.end - current.arrival;
      current.waiting_time = current.turnaround_time - current.burst_time;

      total_turnaround += current.turnaround_time;
      total_waiting += current.waiting_time;
      result.push_back(current);
    }
  }

  // Output results
  std::cout << "\n--- Round Robin (Q=" << quantum << ") ---" << std::endl;
  for (const auto& p : result) {
    std::cout << "P" << p.pid
      << " -> Start: " << p.start
      << ", End: " << p.end
      << ", Waiting Time: " << p.waiting_time
      << ", Turnaround Time: " << p.turnaround_time
      << std::endl;
  }

  float throughput = static_cast<float>(result.size()) / current_time;
  std::cout << "Average Waiting Time: " << total_waiting / result.size() << std::endl;
  std::cout << "Average Turnaround Time: " << total_turnaround / result.size() << std::endl;
  std::cout << "Throughput: " << throughput << " processes/time unit\n" << std::endl;
}

void priority_scheduling(std::vector<Process> processes) {
  std::vector<Process> ready;
  std::vector<Process> result;
  int current_time = 0;
  size_t completed = 0;
  float total_waiting = 0, total_turnaround = 0;

  // Main scheduling loop
  while (completed < processes.size()) {
    // Move all processes that have arrived to the ready queue
    for (auto it = processes.begin(); it != processes.end();) {
      if (it->arrival <= current_time) {
        ready.push_back(*it);
        it = processes.erase(it);
      } else {
        ++it;
      }
    }

    // If ready queue is not empty, pick process with highest priority
    if (!ready.empty()) {
      auto it = std::min_element(ready.begin(), ready.end(), [](const Process& a, const Process& b) {
        return a.priority < b.priority;
      });

      Process p = *it;
      ready.erase(it);

      p.start = current_time;
      p.end = p.start + p.burst_time;
      p.turnaround_time = p.end - p.arrival;
      p.waiting_time = p.start - p.arrival;

      current_time = p.end;
      result.push_back(p);
      total_waiting += p.waiting_time;
      total_turnaround += p.turnaround_time;
      completed++;
    } else {
      // CPU is idle
      current_time++;
    }
  }

  // Output results
  std::cout << "\n--- Priority Scheduling ---" << std::endl;
  for (const auto& p : result) {
    std::cout << "P" << p.pid
      << " -> Start: " << p.start
      << ", End: " << p.end
      << ", Waiting Time: " << p.waiting_time
      << ", Turnaround Time: " << p.turnaround_time
      << std::endl;
  }

  float throughput = static_cast<float>(result.size()) / current_time;
  std::cout << "Average Waiting Time: " << total_waiting / result.size() << std::endl;
  std::cout << "Average Turnaround Time: " << total_turnaround / result.size() << std::endl;
  std::cout << "Throughput: " << throughput << " processes/time unit\n" << std::endl;
}
