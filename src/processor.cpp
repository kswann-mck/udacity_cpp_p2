#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>
#include <chrono>
#include <thread>

using std::vector;
using std::string;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    // Formula take from here as suggested in project instructions: 
    // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux



    // get the aggregate utilization values from the LinuxParser
    vector<string> cpu_utilization_values = LinuxParser::CpuUtilization();

    // convert string values in vector to named values 
    long int prev_user = std::stol(cpu_utilization_values[0]);
    long int prev_nice = std::stol(cpu_utilization_values[1]);
    long int prev_system = std::stol(cpu_utilization_values[2]);
    long int prev_idle = std::stol(cpu_utilization_values[3]);
    long int prev_iowait = std::stol(cpu_utilization_values[4]);
    long int prev_irq = std::stol(cpu_utilization_values[5]);
    long int prev_softirq = std::stol(cpu_utilization_values[6]);
    long int prev_steal = std::stol(cpu_utilization_values[7]);
    //long int prev_guest = std::stol(cpu_utilization_values[8]);
    //long int prev_guest_nice = std::stol(cpu_utilization_values[9]);

    // pause for a second
    sleep_for(seconds(1));

    // get the aggregate utilization values from the LinuxParser
    cpu_utilization_values = LinuxParser::CpuUtilization();

    // convert string values in vector to named values 
    long int user = std::stol(cpu_utilization_values[0]);
    long int nice = std::stol(cpu_utilization_values[1]);
    long int system = std::stol(cpu_utilization_values[2]);
    long int idle = std::stol(cpu_utilization_values[3]);
    long int iowait = std::stol(cpu_utilization_values[4]);
    long int irq = std::stol(cpu_utilization_values[5]);
    long int softirq = std::stol(cpu_utilization_values[6]);
    long int steal = std::stol(cpu_utilization_values[7]);
    //long int guest = std::stol(cpu_utilization_values[8]);
    //long int guest_nice = std::stol(cpu_utilization_values[9]);

    // calculate idle and non-idle time
    long int prev_total_idle = prev_idle + prev_iowait;
    long int total_idle = idle + iowait;
    long int prev_non_idle = prev_user + prev_nice + prev_system + prev_irq + prev_softirq + prev_steal;
    long int non_idle = user + nice + system + irq + softirq + steal;
    long int prev_total = prev_total_idle + prev_non_idle;
    long int total = total_idle + non_idle;
    
    // calculate difference between point in time measurements
    long int idle_delta = total_idle - prev_total_idle;
    long int total_delta = total - prev_total;

    return float(total_delta - idle_delta)/float(total_delta);
}