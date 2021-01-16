#include "format.h"

#include <iomanip>
#include <sstream>
#include <string>

using std::string;

string Format::ElapsedTime(long seconds) {
  auto hours = seconds / kSecondsInHour;
  auto minutes = (seconds / kSecondsInMinute) % kMinutesInHour;
  auto seconds_left = seconds % kSecondsInMinute;
  std::ostringstream elapsed_time;
  elapsed_time << std::setw(2) << std::setfill('0') << hours << ":"
               << std::setw(2) << std::setfill('0') << minutes << ":"
               << std::setw(2) << std::setfill('0') << seconds_left;
  return elapsed_time.str();
}