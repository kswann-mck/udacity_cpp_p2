#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() {
    return cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    vector<int> current_pids = LinuxParser::Pids();
    vector<Process> updated_processes;
    
    // create the list of updated processes
    for (int pid : current_pids) {
        bool in_list = false;
        for (Process p : updated_processes) {
            if (p.Pid() == pid) {
                in_list = true;
            }
        }
        if (!in_list) {
            updated_processes.push_back(Process(pid));
        }
    }

    // sort the processes by the amount of ram used in descending order
    std::sort(updated_processes.begin(), updated_processes.end());
    std::reverse(updated_processes.begin(), updated_processes.end());
    processes_ = updated_processes;

    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
    return LinuxParser::MemoryUtilization();
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }