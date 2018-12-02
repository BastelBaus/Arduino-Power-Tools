/***************************************************************************/
/* Arduino Power Tools (APT) - Library by BastelBaus                       */
/*                                                                         */
/* MIT License Copyright (c) 2018 BastelBaus                               */
/* Permission is hereby granted, free of charge, to any person obtaining   */
/* a copy of this software and associated documentation files (the         */
/* "Software"), to deal in the Software without restriction, including     */
/* without limitation the rights to use, copy, modify, merge, publish,     */
/* distribute, sublicense, and/or sell copies of the Software, and to      */
/* permit persons to whom the Software is furnished to do so, subject to   */
/* the following conditions: The above copyright notice and this           */
/* permission notice shall be included in all copies or substantial        */
/* portions of the Software.								               */
/*                                                                         */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,         */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF      */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY    */ 
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,    */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 	   */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  */
/***************************************************************************/

#ifndef __APT_TIMER_H__
#define __APT_TIMER_H__

#include <Arduino.h>

// This timer can be used in two ways
//  1. Polling it manually to check if a given duration 
//     has passed.
//  2. Installing a callback which is called once the 
//     timer passes its duration
//
// Notes:
// - The implementation uses the millis() function.
// - Duration can be [0 4294967295] milliseconds (~49 days)

// type of callback which is called once the 
// Timer is over
typedef void (*timerCallbackFunctionType)(void);

class APT_Timer {
  
  public:
   APT_Timer(void);
   APT_Timer(unsigned long duration_ms);
   
   //set teh callback function
   APT_Timer(unsigned long duration_ms, timerCallbackFunctionType timerCallbackFunction);   
   // starts automatically the timer
   APT_Timer(unsigned long duration_ms, timerCallbackFunctionType timerCallbackFunction, uint16_t repititions);
   
   // Sets the duration of the timer. If timer is running 
   // or paused, timer will be reseted and set to stop. No 
   // callback will be called. 
   void     setDuration(uint32_t duration_ms);
   
   // returns the currently set duration 
   uint32_t  getDuration(void);
  
   
   // Starts the timer time. If it is alread running, 
   // it is started from the beginning w/o calling a callback.
   // Repitions is the amount of repititions the timer will 
   // run. 
   // Set it to 0 (APT_TIMER_REPEAT_FOREVER) to let it 
   // run unlimted repititions. Each time it passes its duration
   // the callback will be called if not zero.
   void start(uint16_t repititions = 1);
   #define APT_TIMER_REPEAT_FOREVER  0

   // Stops the timer, no callback will be called
   void stop(void);
   
   // Checks if the timer is stopped. If it is over, it is stopped before.
   bool isStopped(void);
   
   // Checks if the timer is overrun. If so, it is stopped. A stopped 
   // timer is not overrun. So this fucntion returns only once true 
   // when the timer is overrund and not yet stopped.
   // If the timer runs more than one repitition, it is only over if 
   // the last repition is passed.
   bool isOver(void);
   
   // returns the remaining time in miliseconds
   uint32_t  getRemainingTime();
   

   
   uint16_t  getRemainingRepititions(void);

   // Set the callback which is called once the timer 
   // reaches his end. When the timer is over, it is stopped
   void 	 setCallback(timerCallbackFunctionType timerCallbackFunction);
   
   // This function must be called within the main loop 
   // if the callback functionality is used.
   void loop(void);
   
  private:
    unsigned long 				timerDuration_ms = 0; 
    unsigned long 				startTime_ms;   
    timerCallbackFunctionType 	timerCallbackFunction = NULL;
    uint16_t					repititions;
};


#endif // #ifndef __APT_TIMER_H__
