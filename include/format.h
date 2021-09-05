#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);
std::string FloatToStringWithTwoDecimals(float f);  // TODO: See src/format.cpp
};                                    // namespace Format

#endif