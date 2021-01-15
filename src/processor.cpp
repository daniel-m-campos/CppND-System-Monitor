#include "processor.h"

#include "linux_parser.h"

using LinuxParser::CPUStates;

float Processor::Utilization() {
  auto cpu_util = LinuxParser::CpuUtilization();
  auto GetState = [&cpu_util](CPUStates state) {
    return std::stof(cpu_util[state]);
  };
  float total_idle =
      GetState(CPUStates::kIdle_) + GetState(CPUStates::kIOwait_);
  float total_active =
      GetState(CPUStates::kUser_) + GetState(CPUStates::kNice_) +
      GetState(CPUStates::kSystem_) + GetState(CPUStates::kIRQ_) +
      GetState(CPUStates::kSoftIRQ_) + GetState(CPUStates::kSteal_);

  return total_active / (total_idle + total_active);
}