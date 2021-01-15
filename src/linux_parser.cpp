#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <unordered_map>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

namespace fs = std::filesystem;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  auto CleanValue = [](string& dirty_value) {
    dirty_value = std::regex_replace(dirty_value, std::regex("_kB"), "");
    std::replace(dirty_value.begin(), dirty_value.end(), '_', ' ');
    return std::stof(dirty_value);
  };

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  float mem_util = 0.0;
  if (filestream) {
    string line;
    string key;
    string dirty_value;
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream line_stream(line);
      while (line_stream >> key >> dirty_value) {
        if (key == "MemTotal") {
          mem_util += CleanValue(dirty_value);
        } else if (key == "dirty_value") {
          mem_util -= CleanValue(dirty_value);
        }
      }
    }
  }
  return mem_util;
}

long LinuxParser::UpTime() {
  long up_time = 0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream) {
    filestream >> up_time;
  }
  return up_time;
}

std::unordered_map<LinuxParser::CPUStates, long> LinuxParser::CpuUtilization() {
  std::unordered_map<LinuxParser::CPUStates, long> cpu_utilization;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream) {
    string line;
    getline(filestream, line);
    line = std::regex_replace(line, std::regex("cpu "), "");
    std::istringstream line_stream(line);
    long value;
    for (int state = CPUStates::kUser_; state <= CPUStates::kGuestNice_;
         ++state) {
      if (line_stream >> value) {
        cpu_utilization.insert_or_assign((CPUStates)state, value);
      } else {
        throw std::runtime_error("Failed to parse cpu state.");
      }
    }
  }
  return cpu_utilization;
}

template <typename T>
T LinuxParser::Helpers::GetValue(const string& path, const string& name) {
  T value;
  string line;
  string key;
  std::ifstream filestream(path);
  if (filestream) {
    while (getline(filestream, line)) {
      std::istringstream line_stream(line);
      if (line_stream >> key >> value) {
        if (key == name) {
          return value;
        }
      }
    }
  }
  return T();
}

int LinuxParser::Helpers::GetProcesses(const std::string& name) {
  return GetValue<int>(kProcDirectory + kStatFilename, name);
}

int LinuxParser::TotalProcesses() { return Helpers::GetProcesses("processes"); }

int LinuxParser::RunningProcesses() {
  return Helpers::GetProcesses("procs_running");
}

string LinuxParser::Command(int pid) {
  auto path = kProcDirectory + to_string(pid) + kCmdlineFilename;
  std::ifstream filestream(path);
  if (filestream) {
    string command;
    getline(filestream, command);
    return command;
  }
  throw std::runtime_error("Could not open: " + path);
}

string LinuxParser::Ram(int pid) {
  auto path = kProcDirectory + to_string(pid) + kStatusFilename;
  return Helpers::GetValue<string>(path, "VmSize:");
}

string LinuxParser::Uid(int pid) {
  auto path = kProcDirectory + to_string(pid) + kStatusFilename;
  return Helpers::GetValue<string>(path, "Uid:");
}

string LinuxParser::User(int pid) {
  auto path = kPasswordPath;
  std::ifstream filestream(path);
  if (filestream) {
    auto uid = Uid(pid);
    string line;
    while (getline(filestream, line)) {
      string user, delim, uid_found;
      std::replace(line.begin(), line.end(), ':', ' ');
      std::stringstream line_stream(line);
      if (line_stream >> user >> delim >> uid_found) {
        if (uid == uid_found) {
          return user;
        }
      }
    }
    throw std::runtime_error("Could not find Uid " + uid);
  }
  throw std::runtime_error("Could not open " + path);
}

vector<string> LinuxParser::Helpers::GetStats(int pid) {
  auto path = kProcDirectory + to_string(pid) + kStatFilename;
  std::ifstream filestream(path);
  if (filestream) {
    string line, word;
    getline(filestream, line);
    std::stringstream line_stream(line);
    vector<string> stats;
    while (line_stream >> word) {
      stats.emplace_back(word);
    }
    return stats;
  }
  throw std::runtime_error("Could not open " + path);
}

long LinuxParser::UpTime(int pid) {
  auto stats = Helpers::GetStats(pid);
  auto start_time = std::stol(stats[ProcessStats::kStarttime]);
  return start_time / sysconf(_SC_CLK_TCK);
}

float LinuxParser::CpuUtilization(int pid) {
  auto stats = Helpers::GetStats(pid);
  vector times{ProcessStats::kUtime, ProcessStats::kStime,
               ProcessStats::kCutime, ProcessStats::kCstime};
  float total_time = 0;
  for (auto& time : times) {
    total_time += (float)std::stol(stats[time]);
  }
  total_time /= (float)sysconf(_SC_CLK_TCK);
  auto seconds = (float)(UpTime() - UpTime(pid));
  return total_time / seconds;
}