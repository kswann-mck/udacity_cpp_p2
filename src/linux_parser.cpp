#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include "linux_parser.h"
#include "format.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  string kb;
  float memtotal = 0.0f;
  float memfree = 0.0f;
  float utilization;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
      line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
      line.pop_back();
      line.pop_back(); //remove kB
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          memtotal = std::stof(value);
        }
        if (key == "MemFree") {
          memfree = std::stof(value);
        }
      }
    }
  }

  // edge case where divide by zero would give nan
  if (memtotal == 0.0) {
    return 0.0;
  }

  utilization = (memtotal-memfree)/memtotal;
  return utilization;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime_str;
  string uptime_str2;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime_str >> uptime_str2;
  }

  // edge case if value was not found
  if (uptime_str.empty()) {
    return long(0);
  }

  return long(std::stof(uptime_str));
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
    return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
    string line;
    string key;
    string value;
    vector<string> values;

    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (filestream.is_open()) {
      while(std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> value) {
          values.push_back(value);
        }
      }
    }
    long int utime = std::stol(values[13]);
    long int stime = std::stol(values[14]);
    long int cutime = std::stol(values[15]);
    long int cstime = std::stol(values[16]);
    return utime + stime + cutime + cstime;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
    vector<string> cpu_values = LinuxParser::CpuUtilization();
    long int user = std::stol(cpu_values[kUser_]);
    long int nice = std::stol(cpu_values[kNice_]);
    long int system = std::stol(cpu_values[kSystem_]);
    long int irq = std::stol(cpu_values[kIRQ_]);
    long int softirq = std::stol(cpu_values[kSoftIRQ_]);
    long int steal = std::stol(cpu_values[kSteal_]);
    return user + nice + system + irq + softirq + steal;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
    vector<string> cpu_values = LinuxParser::CpuUtilization();
    long int idle = std::stol(cpu_values[kIdle_]);
    long int iowait = std::stol(cpu_values[kIOwait_]);
    return idle + iowait;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string value;
  string line;
  vector<string> cpu_utilization_values;
  std::ifstream stream(kProcDirectory + kStatFilename);
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

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  int total_processes = 0;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          total_processes = std::stoi(value);
        }
      }
    }
  }

  return total_processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  int running_processes = 0;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          running_processes = std::stoi(value);
        }
      }
    }
  }
  return running_processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  string key;
  string value;
  string command;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    command = line;
  }
  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  float memory_used = 0.0f;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          memory_used = std::stof(value);
        }
      }
    }
  }

  // if value was not found
  if (memory_used == 0.0f) {
    return "0.00";
  }

  memory_used = memory_used/1000; // convert to megabytes
  return Format::FloatToStringWithTwoDecimals(memory_used);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  string uid;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          uid = value;
        }
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line;
  string key;
  string value;
  string username;
  string file_username;
  string x;

  string uid = LinuxParser::Uid(pid);
  string file_uid;

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> file_username >> x >> file_uid) {
        if (uid == file_uid) {
          username = file_username;
        }
      }
    }
  }
  return username;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  string key;
  string value;
  vector<string> values;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        values.push_back(value);
      }
    }
  }

  return LinuxParser::UpTime() - std::stol(values[21])/sysconf(_SC_CLK_TCK);
}
