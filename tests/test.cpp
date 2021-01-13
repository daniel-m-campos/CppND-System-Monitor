#define CATCH_CONFIG_MAIN
#include <iostream>
#include <string>
#include <vector>

#include "catch2/catch.hpp"
using std::vector;
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
}