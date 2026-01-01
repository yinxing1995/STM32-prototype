#ifndef RTC_SLEEP_H
#define RTC_SLEEP_H
#include <stdint.h>

void rtc_init(void);

void enter_low_power(uint32_t sleep_time);

#endif