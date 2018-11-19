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

#include "APT_Timer.h"
#include <Arduino.h>

#define TIMER_STOP_VALUE 4294967295
//#define TIMER_HOLD_VALUE 4294967294


APT_Timer::APT_Timer(void) {
  stop();
}

APT_Timer::APT_Timer(unsigned long duration_ms) {
  setDuration(duration_ms);
}

void APT_Timer::setDuration(unsigned long duration_ms) {
  stop();
  this->timerDuration_ms = duration_ms;
}

unsigned long APT_Timer::getDuration() {
 return timerDuration_ms;
}
   
void APT_Timer::start(uint16_t repititions) {
  this->repititions = repititions;
  startTime_ms = millis();  
}

void APT_Timer::stop(void) {
  
}

bool APT_Timer::isOver() {
  if(startTime_ms == TIMER_STOP_VALUE) 				return false;  
  else if(repititions == APT_TIMER_REPEAT_FOREVER) 	return false; // #define APT_TIMER_REPEAT_FOREVER  0   
  else if(repititions > 1) 						   	return false;  
  else if ( (millis() - startTime_ms) > timerDuration_ms ) {
   	stop(); 
    return true;
  } else return false;
}

void APT_Timer::loop(void) {
  if(startTime_ms != TIMER_STOP_VALUE) {
	uint32_t timeStamp = millis();
	if ( ( timeStamp - startTime_ms) > timerDuration_ms ) {
		if (timerCallbackFunction != NULL) (*timerCallbackFunction)();
		if (repititions == 1)  startTime_ms = TIMER_STOP_VALUE; // stop the timer
		else {
			startTime_ms = timeStamp - ((timeStamp - startTime_ms ) - timerDuration_ms);
			if(repititions>1)  repititions--;
			// else // (repititions == APT_TIMER_REPEAT_FOREVER) ; // #define APT_TIMER_REPEAT_FOREVER  0   
		}
    }  
  }
} // void APT_Timer::loop(void) {

bool APT_Timer::isStopped() {
  if(startTime_ms == TIMER_STOP_VALUE) return true;
  else if ( (millis() - startTime_ms) > timerDuration_ms ) {
    stop(); 
    return true;
  } else return false;
}

void APT_Timer::setCallback(timerCallbackFunctionType timerCallbackFunction) {
  this->timerCallbackFunction = timerCallbackFunction;
}


unsigned long APT_Timer::getRemainingTime() {
  if( startTime_ms == TIMER_STOP_VALUE) return timerDuration_ms;
  unsigned long timePassed_ms = (millis() - startTime_ms);
  if( timePassed_ms > timerDuration_ms ) return 0;
  return timerDuration_ms - timePassed_ms;
}

uint16_t  APT_Timer::getRemainingRepititions(void) {
  return repititions;
}