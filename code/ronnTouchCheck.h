/*
 * COUNT TOUCH & CHECK HOLD 
 * ***********************************************************************************
 * Code by : fahroni|ganteng
 * contact me : fahroniganteng@gmail.com
 * Date : jan 2021
 * License :  MIT
 */

#include "Arduino.h"
class ronnTouchCheck{
  //PRIVATE CLASS ///////////////////////////////////////////////////////////////
  private:
    // FOR DEBUGING ONLY
    #define DEBUG 0   // set 1 to enable serial debuging
    #if DEBUG
      #define PRINT(s)    { Serial.print(s); }
      #define PRINTLN(s)  { Serial.println(s); }
    #else
      #define PRINT(s)
      #define PRINTLN(s)
    #endif

    // buffer variable for touch
    int checkCount = 0; // count loop (fn check()) if detect touch
    int inTouch = 0;    // count if touch and hold
    int countTouch = 0; // count touch if continues ==> touch - untouch - touch - untouch ... 
    int countHold  = 0; // count after intouch > 500 ms ==> after 500ms = RUN CODE AFTER TOUCH + HOLD



  //PUBLIC CLASS ///////////////////////////////////////////////////////////////
  public:
    bool hold = false;
    int touch = 0;

    bool check(bool touched){
      bool ret = false;
      if(inTouch > 50){
        /*
         * HOLD
         * *****************************************
         * if check every 10ms ==> code like : tmr.every(10, checkTouch);
         * HOLD > 50*10ms (500ms) ==> meaning : if touch > 500ms = HOLD
         */
        if(countTouch>0){
          // RUN CODE AFTER TOUCH + HOLD ==> or return true
          hold =  true;
          touch = countTouch;
          ret = true;
        }
        countHold++;
        
        // FOR DEBUG
        PRINT("RUN FN TOUCH + HOLD : ");
        PRINT(countTouch);
        PRINT(" + ");
        PRINTLN(countHold);
      }
      
      else if(inTouch==0 && touched){
        // TOUCH COUNT
        countHold = 0;
        checkCount = 0;
        countTouch++;
        
        // FOR DEBUG
        PRINT("TOUCH : ");
        PRINTLN(countTouch);
      }
      
      else if(inTouch==0 && checkCount > 40){
        /*
         * TOUCH
         * *****************************************
         * if check every 10ms
         * after touch and 40*10ms (400ms) not detect touch again = END TOUCH
         * so, distance between touches is 10-400ms
         */
        if(countHold>0)
          countHold = 0; // set 0 after run FN TOUCH + HOLD (after hold touch reset in here)
        else if(countTouch>0){
          // RUN CODE AFTER END TOUCH ==> or return true
          hold =  false;
          touch = countTouch;
          ret = true;
          
          // FOR DEBUG
          PRINT("RUN FN TOUCH : ");
          PRINTLN(countTouch);
        }
        countTouch = 0;
        checkCount = 0;
      }
      
      inTouch = touched?inTouch+1:0;
      checkCount = countTouch>0?checkCount+1:0;
      return ret;
    }
  
};
