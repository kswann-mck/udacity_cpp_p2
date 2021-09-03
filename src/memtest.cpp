#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
//#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

float MemoryUtilization() {
  string line;
  string key;
  string value;
  string kb;
  string str_memfree;
  string str_memtotal;
  float memtotal = 1.0;
  float memfree = 1.0;
  float utilization = 0.0;
  std::ifstream filestream("/proc/meminfo");
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::cout << line << "\n";
      line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
      std::cout << line << "\n";
      line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
      std::cout << line << "\n";
      line.pop_back();
      std::cout << line << "\n";
      line.pop_back();
      std::cout << line << "\n";
      std::replace(line.begin(), line.end(), ':', ' ');
      std::cout << line << "\n";
      std::istringstream linestream(line);
      std::cout << line << "\n";
      while (linestream >> key >> value) {
        std::cout << key << value << "\n";
        if (key == "MemTotal") {
          memtotal = std::stof(value);
        }
        if (key == "MemFree") {
          memfree = std::stof(value);
        }
      }
    }
  }
  utilization = (memtotal-memfree)/memtotal;
  return utilization;
}

long UpTime() {
  string uptime_str;
  string uptime_str2;
  string line;
  std::ifstream stream("/proc/uptime");
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime_str >> uptime_str2;
  }
  return long(std::stof(uptime_str));
}


// TODO: Read and return CPU utilization
vector<string> CpuUtilization() { 
  string value;
  string line;
  vector<string> cpu_utilization_values;
  std::ifstream stream("/proc/stat");
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      if (value != "cpu") {
        cpu_utilization_values.push_back(value);
      }
    }
  }
  return cpu_utilization_values;
}



int main() {
    //std::cout << MemoryUtilization() << "\n";
    //std::cout << UpTime() << "\n";
    for (string val : CpuUtilization()) {
        std::cout << val << "\n";
    }

}
