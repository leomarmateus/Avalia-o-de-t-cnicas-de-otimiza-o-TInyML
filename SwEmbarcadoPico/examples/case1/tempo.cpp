#include"tempo.h"

datetime_t t = {
.year = 2022,
 .month = 8,
 .day = 2,
 .dotw = 0, // 0 is Sunday, so 5 is Friday
 .hour = 00,
 .min = 00,
 .sec = 00
};

char datetime_buf[256];

void start_time(){
    rtc_init();
    rtc_set_datetime(&t);
}

void get_time(){
    char *datetime_str = &datetime_buf[0];
    rtc_get_datetime(&t);
    datetime_to_str(datetime_str, sizeof(datetime_buf), &t);
    printf("\r%s\n", datetime_str);
}