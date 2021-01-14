#include "format.h"

#include <string>

using std::string;

string Format::ElapsedTime(long seconds) {
  auto ToString = [](long digit) {
    const string& s_digit = std::to_string(digit);
    return digit < 10 ? "0" + s_digit : s_digit;
  };

  auto hours = seconds / kSecondsInHour;
  auto minutes = (seconds / kSecondsInMinute) % kMinutesInHour;
  auto seconds_left = seconds % kSecondsInMinute;
  return ToString(hours) + ":" + ToString(minutes) + ":" +
         ToString(seconds_left);
}