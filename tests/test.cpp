#define CATCH_CONFIG_MAIN
#include <iostream>
#include <string>
#include <vector>

#include "catch2/catch.hpp"
#include "linux_parser.h"
#include "processor.h"

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
    CHECK_THROWS_AS(LinuxParser::Uid(-1), std::runtime_error);
  }
  SECTION("User") {
    auto user = LinuxParser::User(1);
    CHECK(user == "root");
    CHECK_THROWS_AS(LinuxParser::User(-1), std::runtime_error);
  }
  SECTION("UpTime") { CHECK_NOTHROW(LinuxParser::UpTime(1)); }
  SECTION("Pids") {
    auto pids = LinuxParser::Pids();
    CHECK(1 < pids.size());
    CHECK(pids.size() < 32768);
  }
}

TEST_CASE("Processor Tests", "[ProcessorTests]") {
  Processor processor;
  auto util = processor.Utilization();
  CHECK(util > 0.0);
  CHECK(util < 1.0);
}