#include "processor.h"

#include "linux_parser.h"

using LinuxParser::CPUStates;

float Processor::Utilization() {
  auto cpu_util = LinuxParser::CpuUtilization();
  float total_idle =
      cpu_util[CPUStates::kIdle_] + cpu_util[CPUStates::kIOwait_];
  float total_active =
      cpu_util[CPUStates::kUser_] + cpu_util[CPUStates::kNice_] +
      cpu_util[CPUStates::kSystem_] + cpu_util[CPUStates::kIRQ_] +
      cpu_util[CPUStates::kSoftIRQ_] + cpu_util[CPUStates::kSteal_];

  return total_active / (total_idle + total_active);
}