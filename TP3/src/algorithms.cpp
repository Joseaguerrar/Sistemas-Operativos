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
  std::vector<bool> done(processes.size(), false);
  float total_waiting = 0, total_turnaround = 0;

  while (completed < processes.size()) {
    // Add ready processes to the `ready` vector
    ready.clear();
    for (size_t i = 0; i < processes.size(); ++i) {
      if (!done[i] && processes[i].arrival <= current_time) {
        ready.push_back(processes[i]);
      }
    }

    if (!ready.empty()) {
      // Choose the process with the lowest burst time
      auto shortest_it = std::min_element(ready.begin(), ready.end(), [](const Process& a, const Process& b) {
        return a.burst_time < b.burst_time;
      });

      // Find the original index of the selected process
      int pid = shortest_it->pid;
      size_t idx = 0;
      for (; idx < processes.size(); ++idx) {
        if (processes[idx].pid == pid) break;
      }

      Process& p = processes[idx];
      p.start = current_time;
      p.end = p.start + p.burst_time;
      p.waiting_time = p.start - p.arrival;
      p.turnaround_time = p.end - p.arrival;

      current_time = p.end;
      result.push_back(p);
      done[idx] = true;
      completed++;

      total_waiting += p.waiting_time;
      total_turnaround += p.turnaround_time;
    } else {
      // If no process has arrived, advance time
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
  std::queue<int> queue;  // Only save PID
  int current_time = 0;
  float total_waiting = 0, total_turnaround = 0;
  std::map<int, int> remaining_time;
  std::map<int, int> first_start_time;
  std::map<int, Process> originals;  // To keep the original data
  std::vector<Process> result;
  size_t completed = 0;

  // Save original information and remaining times
  for (const auto& p : processes) {
    originals[p.pid] = p;
    remaining_time[p.pid] = p.burst_time;
  }

  // Sort processes by arrival time
  std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
    return a.arrival < b.arrival;
  });

  size_t index = 0;

  // Add processes that arrive at 0
  while (index < processes.size() && processes[index].arrival <= current_time) {
    queue.push(processes[index].pid);
    index++;
  }

  while (completed < processes.size()) {
    if (queue.empty()) {
      current_time++;
      while (index < processes.size() && processes[index].arrival <= current_time) {
        queue.push(processes[index].pid);
        index++;
      }
      continue;
    }

    int pid = queue.front();
    queue.pop();
    Process& current = originals[pid];

    if (first_start_time.count(pid) == 0) {
      first_start_time[pid] = current_time;
    }

    int exec_time = std::min(quantum, remaining_time[pid]);
    current_time += exec_time;
    remaining_time[pid] -= exec_time;

    std::vector<int> during_slice;
    std::vector<int> at_slice_end;

    while (index < processes.size() && processes[index].arrival <= current_time) {
      if (processes[index].arrival < current_time)
        during_slice.push_back(processes[index].pid);
      else
        at_slice_end.push_back(processes[index].pid);
      index++;
    }

    if (remaining_time[pid] > 0) {
      for (int new_pid : during_slice) queue.push(new_pid);
      queue.push(pid);
      for (int new_pid : at_slice_end) queue.push(new_pid);
    } else {
      current.start = first_start_time[pid];
      current.end = current_time;
      current.turnaround_time = current.end - current.arrival;
      current.waiting_time = current.turnaround_time - current.burst_time;

      total_turnaround += current.turnaround_time;
      total_waiting += current.waiting_time;
      result.push_back(current);
      completed++;

      for (int new_pid : during_slice) queue.push(new_pid);
      for (int new_pid : at_slice_end) queue.push(new_pid);
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
  std::vector<Process> result;
  std::vector<bool> done(processes.size(), false);
  int current_time = 0;
  size_t completed = 0;
  float total_waiting = 0, total_turnaround = 0;

  while (completed < processes.size()) {
    // Find ready processes (they reached current_time)
    std::vector<size_t> ready_indices;
    for (size_t i = 0; i < processes.size(); ++i) {
      if (!done[i] && processes[i].arrival <= current_time) {
        ready_indices.push_back(i);
      }
    }

    if (!ready_indices.empty()) {
      // Choose the index with the highest priority (lowest number)
      size_t selected_idx = ready_indices[0];
      for (size_t idx : ready_indices) {
        if (processes[idx].priority < processes[selected_idx].priority ||
           (processes[idx].priority == processes[selected_idx].priority &&
            processes[idx].arrival < processes[selected_idx].arrival)) {
          selected_idx = idx;
        }
      }

      Process& p = processes[selected_idx];
      p.start = current_time;
      p.end = p.start + p.burst_time;
      p.waiting_time = p.start - p.arrival;
      p.turnaround_time = p.end - p.arrival;

      current_time = p.end;
      result.push_back(p);
      done[selected_idx] = true;
      completed++;
      total_waiting += p.waiting_time;
      total_turnaround += p.turnaround_time;
    } else {
      // There are no processes ready, advance time
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
