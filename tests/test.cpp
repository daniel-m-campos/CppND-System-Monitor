#define CATCH_CONFIG_MAIN
#include <iostream>
#include <string>
#include <vector>

#include "catch2/catch.hpp"
#include "linux_parser.h"

TEST_CASE("LinuxParser Tests", "[LinuxParserTests]") {
  SECTION("MemoryUtilization") {
    auto mem_util = LinuxParser::MemoryUtilization();
    CHECK(mem_util > 0.0);
  }
  SECTION("UpTime") {
    auto up_time = LinuxParser::UpTime();
    CHECK(up_time > 0);
  }
  SECTION("CpuUtilization") {
    auto cpu_utilization = LinuxParser::CpuUtilization();
    CHECK(!cpu_utilization.empty());
    CHECK(cpu_utilization[0] != "cpu");
    CHECK(std::stof(cpu_utilization[0]) > 0);
    CHECK(cpu_utilization.size() == 10);
  }
  SECTION("TotalProcesses") {
    auto total_processes = LinuxParser::TotalProcesses();
    CHECK(total_processes > 0);
  }
  SECTION("RunningProcesses") {
    auto running_processes = LinuxParser::RunningProcesses();
    CHECK(running_processes > 0);
  }
  SECTION("Command") {
    auto command = LinuxParser::Command(1);
    auto pos = command.find("/sbin");
    CHECK(pos == 0);
  }
  SECTION("Ram") {
    auto ram = LinuxParser::Ram(1);
    CHECK(std::stof(ram) > 0);
  }
  SECTION("Uid") {
    auto uid = LinuxParser::Uid(1);
    CHECK(uid == "0");
  }
  SECTION("User") {
    auto user = LinuxParser::User(1);
    CHECK(user == "root");
  }
}