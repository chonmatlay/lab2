
#include "lab.h"
int enable_stopwatch=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  init_gpio();
  init_watch();
  enable_timer_1();
  enable_timer_0();
    Serial.println("enable timer 1 ok"); 
}

void loop() {
  // put your main code here, to run repeatedly:
  update_state();
 
}
