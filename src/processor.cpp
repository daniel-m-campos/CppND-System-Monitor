#include "processor.h"

#include "linux_parser.h"

float Processor::Utilization() {
  auto cpu_util = LinuxParser::CpuUtilization();
  float user = std::stof(cpu_util[0]);
  float nice = std::stof(cpu_util[1]);
  float system = std::stof(cpu_util[2]);
  float idle = std::stof(cpu_util[3]);
  float iowait = std::stof(cpu_util[4]);
  float irq = std::stof(cpu_util[5]);
  float softirq = std::stof(cpu_util[6]);
  float steal = std::stof(cpu_util[7]);

  float total_idle = idle + iowait;
  float total_active = user + nice + system + irq + softirq + steal;

  return total_active / (total_idle + total_active);
}