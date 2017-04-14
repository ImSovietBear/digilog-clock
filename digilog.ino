//Digilog clock firmware v0.5 By Elijah Reeds
//This software is under the by-nc-sa 4.0 license.
//License details: https://creativecommons.org/licenses/by-nc-sa/4.0/
#include <FastLED.h>

#define LED_COUNT 36 // 36 total LEDS 0-11 are for hour, and 12 - 35 are minutes. one minute led for every number and inbetween every two numbers on a clock face (12 hr clock).
#define LED_PIN 4//The datapin for the WS2812 pin.
#define B_UP 8//button for brightness up.
#define B_DOWN 9//button for brightness down.

long previousTime = 0;
long interval = 200;

int curSec = 28;  //current time variables
int curHour = 11; 
float curMin = 01;

int mnl = 0; //The current minute LED

int hourR = 0; //hour LED color.
int hourG = 0;
int hourB = 255;

int minR = 255; //minute LED color.
int minG = 0;
int minB = 0;

int potIn = 0;
int rBright = 10; //Raw brightness.
int bright = 0; //Acts as the reverse of the brightness input so that setting a high brightness lowers the amount to devide the color values

CRGB leds[LED_COUNT]; //just initializing the leds.

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_COUNT);//fastled setup.
  
  Serial.begin(9600);//start serial communications for debugging and other uses.

  pinMode(B_UP, INPUT);//sets pinmodes for brightness buttons.
  pinMode(B_DOWN, INPUT);
  
  for(int z = 0; z < 2; z++){//Startup animaiton... Pretty.
    for(int i = 0; i < LED_COUNT; i++){
        leds[i] = CRGB(255 / bright,255 / bright,255 / bright);
        FastLED.show();
        delay(10);
        leds[i] = CRGB(0,0,0);
        FastLED.show();
    }
  }
  for (int x = 0; x<2; x++){
    for (int i = 0; i < LED_COUNT; i++){
      leds[i] = CRGB(255 / bright,255 / bright,255 / bright);
    }
    FastLED.show();
    delay(150);
    for (int i = 0; i < LED_COUNT; i++){
      leds[i] = CRGB(0,0,0);
    }
    FastLED.show();
    delay(150);
  }
  
  
  

}

void loop() {

  unsigned long currentTime = millis();//keeps input from instantly repeating without delaying the entire program.
  if(currentTime - previousTime > interval){

    previousTime = currentTime;
    
    if(digitalRead(B_UP) == HIGH && rBright < 10){
      rBright++;
      delay(200);
    }else if(digitalRead(B_DOWN) == HIGH && rBright > 0){
      rBright--;
      delay(200);
    }
  }
  
  if(rBright >= 0 && rBright < 11){ //reverses the brightness input so that setting a high brightness lowers the devision of the color values. Also sets the brightness to five if its out of the range of 0-10.
    bright = 10 - rBright;
  }else{
    bright = 5;
  }

  
  disTime(curHour,hourR,hourG,hourB,curMin,minR,minG,minB);//calls the disTime (display time) function.
}

void disTime(int hr,int hrr,int hrg,int hrb,float mn,int mnr,int mng,int mnb){//This function takes in color and time values then
  
  for (int i = 0; i < LED_COUNT; i++){ //<-- set all lights to off so that leds don't stay on after time chage.
      leds[i] = CRGB(0,0,0);
  }
  
  byte mnt = (((byte)mn / 10) % 10);//<-- math for converting current minute to current led.
  mn = mn - (mnt * 10);
  if(mn < 2.5){
    mnl = 12;
  }else if(mn >= 2.5 && mn < 5){
    mnl = 13;
  }else if(mn >= 5 && mn < 7.5){
    mnl = 14;  
  }else if(mn >= 7.5 && mn < 10){
    mnl = 15;  
  }
  mnl = mnl + (4 * mnt);
  leds[mnl] = CRGB(mnr / bright,mng / bright,mnb / bright);

  if(hr < 12){ //<-- Makes it so that if its 12oclck the first LED representing 12 will be lit instead of the 12th LED. Also sets it to 12oclock if somehow the time is over 12oclock (possible military time, or just error).
    leds[hr] = CRGB(hrr / bright,hrg / bright,hrb / bright);
  }else{
    leds[0] = CRGB(hrr / bright,hrg / bright,hrb / bright);
  }
  
  FastLED.show(); //Tells the arduino to send the LED data to the LED's.

}

//I'm here to waste memory!
