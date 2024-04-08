#include "client.h"

void unixTimeToHoursMinutes(time_t unixTime, int *hours, int *minutes) {
    struct tm *timeinfo = localtime(&unixTime);
    *hours = timeinfo->tm_hour;
    *minutes = timeinfo->tm_min;
}

