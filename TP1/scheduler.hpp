#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <vector>
#include <string>

//Algoritmos a utilizar

int fcfs(const std::vector<int>& requests, int head);

int sstf(const std::vector<int>& requests, int head);

int scan(const std::vector<int>& requests, int head, std::string direction, int maxCylinder);

int c_scan(const std::vector<int>& requests, int head, std::string direction, int maxCylinder);

int look(const std::vector<int>& requests, int head, std::string direction);

int c_look(const std::vector<int>& requests, int head, std::string direction);

#endif // SCHEDULER_HPP
