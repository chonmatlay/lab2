#include "lab.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define BUTTON_1  D6
#define BUTTON_2 D7

LiquidCrystal_I2C lcd(0x27,16,2);
bool flag3 =1;
bool firstButtonRead1=0 ,firstButtonRead2=0, secondButtonRead1=0, secondButtonRead2=0 ;
bool firstButton1s =0 , firstButton3s=0;
bool secondButton1s =0 , secondButton3s=0;
bool firstButtonPress=0 ,secondButtonPress=0;
unsigned int count1=0 , count2=0, count3=0,count4=0 ;
bool blinking=0;
bool flag,flag1 =0 ;
 int count=0;


enum {
  STATE1 , 
  STATE2, 
  STATE3,
  STATE4
} eState;

  Watch w;
   stopwatch sw;
void blink_char(int x , int y , int z);

void init_watch(){
    w.hour=0;
    w.minute =0;
    w.second=0;
}
void reset_stopwatch(){
    sw.minute=0;
    sw.second=0;
    sw.minisecond=0;
}
void ISR_1(){
  count++;
  
  if (count ==100){
    increase_watch();
    count =0;
  }
  read_button_1();
  read_button_2();
}
void ISR_2(){
  if (enable_stopwatch) increase_stopwatch();
  timer0_write(ESP.getCycleCount()+800000);
}
void increase_watch(){
  w.hour= (w.minute==59&&w.second==59)? ((w.hour==23)?0:w.hour+1):w.hour;
  w.minute=(w.second==59)?((w.minute==59)?0:w.minute+1):w.minute;
  w.second = (w.second==59) ? 0 : w.second+1;
}
void increase_stopwatch(){
  sw.minute= (sw.second==59&&sw.minisecond==99)? ((sw.minute==99)?sw.minute:sw.minute+1):sw.minute;
  sw.second=(sw.minisecond==99)?((sw.second==59)?0:sw.second+1):sw.second;
  sw.minisecond = (sw.minisecond==99) ? 0 : sw.minisecond+1;
}
void display_watch(){
lcd.setCursor(3,0);
  if (w.hour<10){
    lcd.print(0);
    lcd.setCursor(4,0);
    lcd.print(w.hour);
    }
  lcd.setCursor(5,0);
  lcd.print(":");
  lcd.setCursor(6,0);
  if(w.minute<10) {
    lcd.print(0);
    lcd.setCursor(7,0);
  }
  lcd.print(w.minute); 
  lcd.setCursor(8,0);
  lcd.print(":");
  lcd.setCursor(9,0);
  if (w.second<10){
      lcd.print(0);
      lcd.setCursor(10,0);
    }
  lcd.print(w.second);
  
}
void display_stopwatch(){
  lcd.setCursor(3,0);
  if (sw.minute<10){
    lcd.print(0);
    lcd.setCursor(4,0);
    lcd.print(sw.minute);
    }
  lcd.setCursor(5,0);
  lcd.print(":");
  lcd.setCursor(6,0);
  if(sw.second<10) {
    lcd.print(0);
    lcd.setCursor(7,0);
  }
  lcd.print(sw.second); 
  lcd.setCursor(8,0);
  lcd.print(":");
  lcd.setCursor(9,0);
  if (sw.minisecond<10){
      lcd.print(0);
      lcd.setCursor(10,0);
      //lcd.print(sw.minisecond);
    }
  lcd.print(sw.minisecond);
   lcd.setCursor(11,0);
   lcd.print(" ");
}
void modify_h(){
  blink_char(3,0,w.hour);
  lcd.setCursor(5,0);
  lcd.print(":");
  lcd.setCursor(6,0);
  if (w.minute < 10) {
      lcd.print(0);
      lcd.setCursor(7,0);
    }
  lcd.print(w.minute);
  lcd.setCursor(8,0);
  lcd.print(":");
  lcd.setCursor(9,0);
  if (w.second<10){
    lcd.print(0);
    lcd.setCursor(10,0);
    }
  lcd.print(w.second);
}
void modify_m(){
  lcd.setCursor(3,0);
  if (w.hour <10){
      lcd.print(0);
      lcd.setCursor(4,0);
    }
  lcd.print(w.hour);          
  lcd.setCursor(5,0);
  lcd.print(":");
  blink_char(6,0,w.minute);
  lcd.setCursor(8,0);
  lcd.print(":");
  lcd.setCursor(9,0);
  if (w.second<10) {
    lcd.print(0);
    lcd.setCursor(10,0);
    }
  lcd.print(w.second);
}
void modify_s(){
  lcd.setCursor(3,0);
   if (w.hour <10){
      lcd.print(0);
      lcd.setCursor(4,0);
    }
  lcd.print(w.hour);
  lcd.setCursor(5,0);
  lcd.print(":");
  lcd.setCursor(6,0);
   if (w.minute < 10) {
      lcd.print(0);
      lcd.setCursor(7,0);
    }
  lcd.print(w.minute);
  lcd.setCursor(8,0);
  lcd.print(":");
  blink_char(9,0,w.second);
}
void blink_char (int x , int y ,int val ){
  lcd.setCursor(x,y);
  if ((count%5==1)) {
    if (blinking){
      lcd.print("  ");
      blinking=0;
    }else{
      if(val <10){
          lcd.print(0);
          lcd.setCursor(x+1,y);
        }
        lcd.print(val);
        blinking=1;
      }
      Serial.println(count);
    } 
  }

void modify(int count2){
  switch (count2){
    case 2 : 
      w.second++;
      if (w.second==60) {
        w.second=0;
        w.minute+=1;
        if (w.minute==60){
          w.minute=0;
          w.hour++;
          if (w.hour == 24) w.hour=0;
        }
      }
    
    break;
    case 1 : 
      w.minute++ ;
      if (w.minute==60){
        w.minute=0;
        w.hour++;
        if (w.hour==24) w.hour =0;
      }
      break;
    case 0:
      w.hour ++;
      if (w.hour==24 ) w.hour =0; 
      break;
  } 
}
void read_button_1(){
  firstButtonRead1=firstButtonRead2;
  firstButtonRead2 = digitalRead(BUTTON_1);
  if ( firstButtonRead1==firstButtonRead2){
      if(!firstButtonRead2){
          if (count1==0){ firstButtonPress=1;
          count1++;
          }
          else firstButtonPress =0;
        }else{
            firstButtonPress=0;
            firstButton1s=0;
            count1=0 ;
          }
    }
}
void read_button_2(){
   secondButtonRead1=secondButtonRead2;
  secondButtonRead2 = digitalRead(BUTTON_2);
  if ( secondButtonRead1==secondButtonRead2){
      if(!secondButtonRead1&&firstButtonRead1){
          secondButtonPress=1;
          count3++;
          if (count3 >=100){
              secondButton1s=1;
              count4++;
              if (count4>=20){
                count4 =0 ;   
                flag=0;             
              }
            }
        }else{
            secondButtonPress=0;
            secondButton1s=0;
            count3=0 ;
            count4=0;
            flag=0;
            flag1=0;
          }
    }
}
void update_state(){
    switch (eState){
        case STATE1 :
           display_watch();
            if (firstButtonPress) eState = STATE2 ;
            firstButtonPress=0;
            break;
        case STATE2 : 
            //lcd.setCursor(0,3);
            if (count2==0){
              modify_h();
            } else if (count2==1){
              modify_m();
            } else if (count2==2){
              modify_s();
            } 
            if (firstButtonPress) {
              count2++;
              firstButtonPress=0;
              if (count2==3) {
                eState=STATE4;
                count2=0;
              }
            }
            if (secondButtonPress) eState =STATE3;
            break;
        case STATE3: 
           if(!flag1) modify(count2);
            flag1=1;
            if(flag1&&secondButton1s&&!flag){
              if (count4==19) {
              modify(count2);
              flag=1;
                }
            }
            display_watch();
            if (!secondButtonPress) eState = STATE2;
            break;
        case STATE4 :
            display_stopwatch();
            if (secondButtonPress&&flag3){
              enable_stopwatch=!enable_stopwatch;
              flag3=0;
            }
            flag3=1;
            if (firstButtonPress) eState=STATE1;
            break;
    }
}
void init_gpio(){
   pinMode(BUTTON_1 ,INPUT_PULLUP);
  pinMode(BUTTON_2,INPUT_PULLUP);
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  Serial.println("init ok");
  init_watch();
  reset_stopwatch();
}
void enable_timer_1(){
  timer1_isr_init();
  timer1_attachInterrupt(ISR_1);
   timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
   timer1_write(50000);
   //interrupts();
};
void enable_timer_0(){
  timer0_isr_init();
  timer0_attachInterrupt(ISR_2);
  // timer0_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
   timer0_write(ESP.getCycleCount()+160000);
   interrupts();
}
