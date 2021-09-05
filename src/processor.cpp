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
    return float(LinuxParser::ActiveJiffies())/float(LinuxParser::Jiffies());
}