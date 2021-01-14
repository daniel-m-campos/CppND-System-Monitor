#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
static const long kSecondsInMinute = 60;
static const long kMinutesInHour = 60;
static const long kSecondsInHour = kSecondsInMinute * kMinutesInHour;
std::string ElapsedTime(long times);
}  // namespace Format

#endif