#include <assert.h>
#include "linux_parser.h"
#include <iostream>

void TestMemoryUtilization() {
    float memory_utiliztion = LinuxParser::MemoryUtilization();
    std::cout << "Memory Utilization" << memory_utiliztion << "\n";
    assert(memory_utiliztion != NULL);
}

int main() {
    TestMemoryUtilization();
}