#include "process.h"

#include <string>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() const { return pid_; }

float Process::CpuUtilization() const {
  return LinuxParser::CpuUtilization(pid_);
}

string Process::Command() {
  if (command_.empty()) {
    command_ = LinuxParser::Command(pid_);
  }
  return command_;
}

string Process::Ram() const { return LinuxParser::Ram(pid_); }

string Process::User() {
  if (user_.empty()) {
    user_ = LinuxParser::User(pid_);
  }
  return user_;
}

long int Process::UpTime() const { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& other) const {
  return CpuUtilization() > other.CpuUtilization();
}