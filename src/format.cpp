#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    
    int seconds_per_minute = 60;
    int minutes_per_hour = 60;

    int total_minutes = seconds / seconds_per_minute;
    int seconds_remainder = seconds % seconds_per_minute;
    int hours = total_minutes / minutes_per_hour;
    int minutes = total_minutes % minutes_per_hour;

    string hours_str = std::to_string(hours);
    string minutes_str = std::to_string(minutes);
    string seconds_str = std::to_string(seconds_remainder);

    // pad hours minutes and seconds when less than 2 digits with a leading zero
    if (hours_str.length() < 2) {
        hours_str = "0"+hours_str;
    }

    if (minutes_str.length() < 2) {
        minutes_str = "0"+minutes_str;
    }

    if (seconds_str.length() < 2) {
        seconds_str = "0"+seconds_str;
    }

    return hours_str + ":" + minutes_str + ":" + seconds_str;
}