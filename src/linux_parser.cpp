#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <filesystem>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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

vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_utilization;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream) {
    string line;
    getline(filestream, line);
    line = std::regex_replace(line, std::regex("cpu "), "");
    std::istringstream line_stream(line);
    string value;
    while (line_stream >> value) {
      cpu_utilization.emplace_back(value);
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
    throw std::runtime_error("Could not find value");
  }
  throw std::runtime_error("Could not open: " + path);
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

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }