#include "utils.h"

std::vector<int> parseValues(const std::string& str) {
  std::vector<int> values;
  std::stringstream ss(str);
  std::string token;

  while (getline(ss, token, ',')) {
    values.push_back(stoi(token));
  }

  return values;
}

void parseInput(const std::string& input, std::vector<Process>& processes, std::string& algorithm) {
  std::stringstream ss(input);
  std::string part;
  std::vector<std::string> parts;

  // Split input into parts using '|'
  while (getline(ss, part, '|')) {
    parts.push_back(part);
  }

  int n = stoi(parts[0]);  // Number of processes

  // Parse burst times
  std::vector<int> burst_times = parseValues(parts[1]);

  // Parse priorities or default to 0
  std::vector<int> priorities =
    (parts[2] == " 0" || parts[2] == "0") ? std::vector<int>(n, 0) : parseValues(parts[2]);

  // Parse arrival times or default to 0
  std::vector<int> arrivals =
    (parts[3] == " 0" || parts[3] == "0") ? std::vector<int>(n, 0) : parseValues(parts[3]);

  // Parse and clean algorithm name
  algorithm = parts[4];
  algorithm.erase(remove_if(algorithm.begin(), algorithm.end(), ::isspace), algorithm.end());

  // Create and store each process
  for (int i = 0; i < n; ++i) {
    processes.push_back({
      i + 1,           // PID
      burst_times[i],  // CPU burst time
      priorities[i],   // Priority
      arrivals[i]      // Arrival time
    });
  }
}

void printProcess(const std::vector<Process>& processes, const std::string& algorithm) {
  std::cout << "Selected algorithm: " << algorithm << "\n";
  std::cout << "processes:\n";

  for (const auto& p : processes) {
    std::cout << "P" << p.pid
      << ": BT=" << p.burst_time
      << " Priority=" << p.priority
      << " Arrival=" << p.arrival << "\n";
  }
}
