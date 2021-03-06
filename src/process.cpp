#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include "process.h"
#include "linux_parser.h"
#include "format.h"

using std::string;
using std::to_string;
using std::vector;

// Define a constructor to generate a process from a PID.
Process::Process(int pid) {
    pid_ = pid;
}

// TODO: Return this process's ID
int Process::Pid() { return pid_;}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    long active_time = LinuxParser::ActiveJiffies(pid_)/sysconf(_SC_CLK_TCK);
    return std::stof(Format::FloatToStringWithTwoDecimals(float(active_time)/float(LinuxParser::UpTime())));
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    if (std::stof(Ram()) < std::stof(a.Ram())) {
        return true;
    }
    else {
        return false;
    }
}