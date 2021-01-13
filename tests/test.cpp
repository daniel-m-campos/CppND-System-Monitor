#define CATCH_CONFIG_MAIN
#include <iostream>
#include <string>
#include <vector>

#include "catch2/catch.hpp"
using std::vector;
#include "linux_parser.h"

TEST_CASE("LinuxParser Tests", "[LinuxParserTests]") {
  SECTION("MemoryUtilization") {
    float mem_util = LinuxParser::MemoryUtilization();
    CHECK(mem_util > 0.0);
  }
}