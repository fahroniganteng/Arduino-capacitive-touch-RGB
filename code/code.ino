/*
 * ARDUINO - CAPACITIVE TOUCH RGB LED  
 * ***********************************************************************************
 * Code by : fahroni|ganteng
 * contact me : fahroniganteng@gmail.com
 * Date : jan 2021
 * License :  MIT
 * 
 */

#include "Timer.h"
#include <EEPROM.h>
#include "capacitive.h"
#include "ronnTouchCheck.h"

// FOR DEBUGING ONLY
#define DEBUG 0  // set 1 to enable serial debuging
#if DEBUG
  #define PRINT(s)    { Serial.print(s); }
  #define PRINTLN(s)  { Serial.println(s); }
#else
  #define PRINT(s)
  #define PRINTLN(s)
#endif
// fn get size of int array
#define arrSize(x)   (sizeof(x) / sizeof(x[0]))


// RGB TYPE
// ------------------------------------------------------
// Note:
// Led strip  like 5050 12v is common anode, 
// but requires N channel mosfet for convert voltage from 5v pin arduino to 12v, 
// so set COMMON_ANODE to false
#define COMMON_ANODE true // if using RGB common anode (+) set 'true'; if using common cathode (-) set 'false'

// PIN Mapping
#define PIN_CP 10 // Capacitive Pin ==> can use analog pins
const int PIN_RGB[3] = {3,5,6}; // for RGB PIN ==> only support PWM pin 

// Set sensitify of capacitive
// Note : Sensitivity may vary depending on touch area and pressure.
#define SENSITIFY 4 // if pin read > SENSITIFY = touch; ==> range 0-17 (check on file capacitive.c)

/*
 * EEPROM Mapping
 * ****************************************************************************
 */
#define MODE 0
#define BRIG 1
#define R 10
#define G 11
#define B 12

int E_MODE  = EEPROM[MODE]; // Mode
int E_BRIG  = EEPROM[BRIG]; // Brightness
// RGB Color
int E_RGB[3] = {EEPROM[R],EEPROM[G],EEPROM[B]};

//buffer setting
bool inSettingMenu = false;
bool rgb_on = false;
int red[3]    = {255,0,0};
int green[3]  = {0,255,0};
int blue[3]   = {0,0,255};
int yellow[3] = {255,255,0};
//int cyan[3]   = {0,255,255};
//int pink[3]   = {255,0,255};
//int white[3]  = {255,255,255};
//int warm[3]   = {255,150,50};
int chColor = 0;
int step_chColor = 0;
long readCap = 0; //read capacitive

Timer tmr;
Timer tmrRGB;
ronnTouchCheck r;


/*
 * FUNCTION
 * ****************************************************************************
 */
#include "ronnRGB.h" // Move to another file to easier read & tracking 

void checkTouch(){
  /*
   * FUNCTION FOR MAPPING TOUCH
   * ******************************************************************
     start
     |
     +-- any touch ==> turn on
     |
     +-- 2x touch ==> change mode (mode 1 to 10) ==> list mode in ronnRGB.h
     |
     +-- 3x touch ==> turn off
     |
     +-- 4x touch ==> setting mode (3x blink blue)
     |   |
     |   +-- 1x touch ==> change color
     |   |
     |   +-- 1x touch + hold ==> step change color
     |   |
     |   +-- 2x touch ==> increase brightness +50 (blink red when max)
     |   |
     |   +-- 2x touch + hold ==> increase brightness (blink red when max)
     |   |
     |   +-- 3x touch ==> decrease brightness -50 (blink red when minimum)
     |   |
     |   +-- 3x touch + hold ==> decrease brightness (blink red when mininmum)
     |   |
     |   +-- 4x touch ==> exit setting mode (3x blink green)
     |
     +-- 10x touch ==> reset default (blink yellow)

     
   * *****************************************************************
   */
   
  
  //int cap = readCapacitivePin(PIN_CP); //  ==> enable this and DEBUG for check SENSITIFY
  //if(cap>1)PRINTLN(cap);
  
  bool touched = readCapacitivePin(PIN_CP)>SENSITIFY;
  if(r.check(touched)){
    // Start, any touch = turn on ----------------------
    if(!rgb_on){
      rgb_start();
      rgb_on = true;
    }

    // TOUCH + HOLD ------------------------------------
    else if(r.hold){
      if(inSettingMenu){
        switch(r.touch){
          case 1: rgb_step_changeColor(); break; // 1x touch + hold = change color
          case 2: rgb_step_increaseBrightness(); break; // 2x touch + hold = increaseBrightness
          case 3: rgb_step_decreaseBrightness(); break; // decrease brightness
        }
      }
      else{
        // Yeah, no process here... :P
      }
    }

    // TOUCH ONLY --------------------------------------
    else{
      if(inSettingMenu){
        switch(r.touch){
          case 1: rgb_changeColor(); break; // step by step change color
          case 2: rgb_increaseBrightness(); break; // step by step increaseBrightness
          case 3: rgb_decreaseBrightness(); break; // step by step decrease brightness
          case 4: // exit setting
            inSettingMenu = false;
            
            //after setting always change to mode 1
            //EEPROM[MODE] = 1;
            //E_MODE = 1;
            
            //if change brightness or color, save after exit from setting
            EEPROM[BRIG] = E_BRIG; 
            EEPROM[R] = E_RGB[0];
            EEPROM[G] = E_RGB[1];
            EEPROM[B] = E_RGB[2];
            rgb_blinkInfo(3,green);
            rgb_start();

            // FOR DEBUG
            PRINT("MODE : ");
            PRINTLN(E_MODE);
            PRINT("BRIGHTNESS : ");
            PRINTLN(E_BRIG);
            PRINT("RGB : ");
            PRINTLN(String(E_RGB[0]) + " " + String(E_RGB[1]) + " " + String(E_RGB[2]));
            break;
        }
      }
      else{
        switch(r.touch){
          case 2: // change mode
            rgb_stop();
            E_MODE = E_MODE>=10?1:E_MODE+1;
            EEPROM[MODE] = E_MODE;
            rgb_start();
            break;
          case 3: // turn off
            rgb_stop();
            rgb_on = false;
            break;
          case 4: // start setting mode
            rgb_stop();
            inSettingMenu = true;
            rgb_blinkInfo(3,blue);
            rgb_viewColor();
            break;
          case 10: // reset default
            rgb_stop();
            EEPROM[MODE] = 1;
            EEPROM[BRIG] = 255; 
            EEPROM[R] = 255;
            EEPROM[G] = 255;
            EEPROM[B] = 255;
            E_MODE = 1;
            E_BRIG = 255; 
            E_RGB[0] = 255;
            E_RGB[1] = 255;
            E_RGB[2] = 255;
            rgb_blinkInfo(5,yellow);
            rgb_start();
            break;
        }
      }
    }
  }
  
}












void setup() {
  // For debug
  Serial.begin(115200);

  //Verify EEPROM
  E_MODE = (E_MODE<0 || E_MODE>10)?1:E_MODE;
  E_BRIG = (E_BRIG<1 || E_BRIG>255)?255:E_BRIG;
  E_RGB[1] = (E_RGB[0]==0 && E_RGB[0]==0 && E_RGB[2]==0)?255:E_RGB[1]; //default green

  // FOR DEBUG
  PRINT("MODE : ");
  PRINTLN(E_MODE);
  PRINT("BRIGHTNESS : ");
  PRINTLN(E_BRIG);
  PRINT("RGB : ");
  PRINTLN(String(E_RGB[0]) + " " + String(E_RGB[1]) + " " + String(E_RGB[2]));
  
  // Set pin mode
  pinMode(PIN_CP, INPUT);
  for(int i=0;i<arrSize(PIN_RGB);i++){
    pinMode(PIN_RGB[i], OUTPUT); // Set pin RGB to output
    digitalWrite(PIN_RGB[i],COMMON_ANODE?HIGH:LOW); // turn OFF
  }

  // start RGB ==> fade green
  rgb_starting();
  
  // check touch every 10ms
  tmr.every(10, checkTouch);

 
}

void loop() {
  tmr.update();
  tmrRGB.update();
}
