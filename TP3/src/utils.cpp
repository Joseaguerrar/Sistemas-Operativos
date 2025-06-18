#include "utils.h"

std::vector<int> parseValues(const std::string& str) {
  std::vector<int> values;
  std::stringstream ss(str);
  std::string token;
  while (getline(ss, token, ',')) {
    if (!token.empty()) values.push_back(std::stoi(token));
  }
  return values;
}

void parseInput(const std::string& input, std::vector<Process>& processes, std::string& algorithm) {
  std::stringstream ss(input);
  std::string part;
  std::vector<std::string> parts;

  // Split input into parts using '|'
  while (getline(ss, part, '|')) {
    part.erase(remove_if(part.begin(), part.end(), ::isspace), part.end());  // Trim spaces
    parts.push_back(part);
  }

  if (parts.size() < 5) {
    std::cerr << "Error: Incomplete input. Expected 5 sections separated by '|'.\n";
    return;
  }

  int n = stoi(parts[0]);  // Number of processes

  // Parse burst times
  std::vector<int> burst_times = parseValues(parts[1]);
  if (burst_times.size() != n) {
    std::cerr << "Error: Incorrect number of burst times.\n";
    return;
  }

  // Parse priorities (pad with 0 if missing)
  std::vector<int> priorities = (parts[2] == "0")
    ? std::vector<int>(n, 0)
    : parseValues(parts[2]);
  if (priorities.size() < n) priorities.resize(n, 0);

  // Parse arrivals (pad with 0 if missing)
  std::vector<int> arrivals = (parts[3] == "0")
    ? std::vector<int>(n, 0) : parseValues(parts[3]);
  if (arrivals.size() == 1) arrivals = std::vector<int>(n, arrivals[0]);
  if (arrivals.size() < n) arrivals.resize(n, 0);

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
  std::cout << "\nSelected algorithm: " << algorithm << "\n";
  std::cout << "Processes:\n";

  for (const auto& p : processes) {
    std::cout << "P" << p.pid
      << ": BT=" << p.burst_time
      << " Priority=" << p.priority
      << " Arrival=" << p.arrival << "\n";
  }
}
