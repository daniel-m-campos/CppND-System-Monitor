#define CATCH_CONFIG_MAIN
#include <iostream>
#include <string>
#include <vector>

#include "catch2/catch.hpp"
#include "format.h"
#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

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
    CHECK(cpu_utilization.size() == 10);
    CHECK(cpu_utilization[LinuxParser::CPUStates::kUser_] > 0);
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
    CHECK(LinuxParser::Uid(-1).empty());
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
  SECTION("CpuUtilization(Pids)") {
    auto pid_cpu_util = LinuxParser::CpuUtilization(1);
    CHECK(pid_cpu_util > 0.0);
    CHECK(pid_cpu_util < 1.0);
  }
}

TEST_CASE("Processor Tests", "[ProcessorTests]") {
  Processor processor;
  auto util = processor.Utilization();
  CHECK(util > 0.0);
  CHECK(util < 1.0);
}

TEST_CASE("Process Tests", "[ProcessTests]") {
  Process process(1);
  CHECK(process.CpuUtilization() > 0.0);
  CHECK(process.Command().find("/sbin") == 0);
  CHECK(!process.Ram().empty());
  CHECK(process.User() == "root");
  CHECK_NOTHROW(process.UpTime());
}

TEST_CASE("System Tests", "[SystemTests]") {
  System system;
  CHECK(system.Cpu().Utilization() > 0.0);
  CHECK(!system.Processes().empty());
  CHECK(system.MemoryUtilization() > 0);
  CHECK(system.TotalProcesses() > 0);
  CHECK(system.RunningProcesses() > 0);
  CHECK(!system.Kernel().empty());
  CHECK(system.OperatingSystem().find("Ubuntu") == 0);
}

TEST_CASE("Format Tests", "[FormatTests]") {
  SECTION("Digits above 10") {
    long seconds =
        40 * Format::kSecondsInHour + 41 * Format::kSecondsInMinute + 42;
    auto time = Format::ElapsedTime(seconds);
    CHECK(Format::ElapsedTime(seconds) == "40:41:42");
  }
  SECTION("Digits under 10") {
    long seconds =
        7 * Format::kSecondsInHour + 8 * Format::kSecondsInMinute + 9;
    auto time = Format::ElapsedTime(seconds);
    CHECK(Format::ElapsedTime(seconds) == "07:08:09");
  }
}