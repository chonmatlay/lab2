#ifndef LAB_H
#define LAB_H
#include <Arduino.h>


extern int enable_stopwatch;
extern int count  ;
void enable_timer_1();
void enable_timer_0();
void init_watch();
void read_button_1();
void read_button_2();
void update_state();
void init_gpio();
void increase_watch();
void increase_stopwatch();
typedef struct Watchs {
    int hour ;
    int minute;
    int second;
  } Watch;

typedef struct stopwatchs {
  int minute;
  int second;
  int minisecond;
  } stopwatch;
#endif
