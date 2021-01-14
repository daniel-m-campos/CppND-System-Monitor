#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  explicit Process(int pid) : pid_(pid) {}
  int Pid() const;
  std::string User();
  std::string Command();
  float CpuUtilization() const;
  std::string Ram() const;
  long int UpTime() const;
  bool operator<(Process const& a) const;

 private:
  int pid_;
  std::string user_;
  std::string command_;
};

#endif