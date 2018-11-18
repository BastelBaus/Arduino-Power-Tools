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

#include <Arduino.h>
#include <Encoder.h>      // https://github.com/PaulStoffregen/Encoder
#include "APT_input.h"


APT_Encoder::APT_Encoder(uint8_t pin1,uint8_t pin2, uint8_t parameters) 
  : Encoder(pin1,pin2) {
  this->parameters = parameters;
}; // APT_Encoder::APT_Encoder


void APT_Encoder::setParameters(uint8_t parameters) {
  this->parameters = parameters;
}


// ToDo:
// if ticks happen in between, they would be discarded.
// Would need to change Encoder  lib to add function "add ticks"
bool APT_Encoder::consumeTicks(int16_t amountOfTicks) {
  long encoderTicks = read();
  if(amountOfTicks>0) {
    if ( encoderTicks >= amountOfTicks ) {
      write(encoderTicks-amountOfTicks);
      return true;
    }
  } else if(amountOfTicks<0) {               
    if ( encoderTicks <= amountOfTicks ) {
      write(encoderTicks-amountOfTicks);
      return true;
    }
  }
  return false;
}
int32_t APT_Encoder::consumeTicks(void) {
  int32_t encoderTicks = read();
  write(0);
  return encoderTicks;
}
    

void APT_Encoder::attachHandler(encoderHandlerType encoderHandler, uint16_t callAtAmountOfTicks) {
  this->encoderHandler = encoderHandler;
  this->callAtAmountOfTicks = callAtAmountOfTicks;
}

void APT_Encoder::loop(void) {
  if(encoderHandler != NULL) {
    if       (callAtAmountOfTicks == 0)           (*encoderHandler)(this);
    else if  (callAtAmountOfTicks <= abs(read())) (*encoderHandler)(this);
  }
} // void APT_Encoder::loop(void) {


APT_Button::APT_Button(void) {  
}

APT_Button::APT_Button(uint8_t pin, uint8_t properties) {
  this->setup(pin, properties);
}

void APT_Button::setup(uint8_t pin, uint8_t properties) {
  this->pin=pin;
  this->properties = properties;
  if( (properties & APT_BUTTON_PROP_PULLUP) == APT_BUTTON_PROP_PULLUP) 
        pinMode(pin       , INPUT_PULLUP);
  else  pinMode(pin       , INPUT);
  state = 0;  
};

void APT_Button::attachHandler(handlerFunctionType handlerFunction) {
    this->handlerFunction = handlerFunction;  
    state = 0;
}
  
void APT_Button::setDebounceTime(uint16_t debounceTime_ms) {
  this->debounceTime_ms = debounceTime_ms;
}
void APT_Button::setLongPressTime(uint16_t longPressTime_ms) {
  this->longPressTime_ms = longPressTime_ms;
}
void APT_Button::setDoubleClickTime(uint16_t doubleClickTime_ms) {
  this->doubleClickTime_ms = doubleClickTime_ms;
}

void APT_Button::loop(void) {
 if( pin>-1 && (handlerFunction != NULL)) {
  if((properties & APT_BUTTON_PROP_INVERT) == APT_BUTTON_PROP_INVERT) 
         advanceStateMachine( ! digitalRead(pin) );
  else   advanceStateMachine(   digitalRead(pin) );
 } //  if( pin>-1) {
}

bool APT_Button::isPushed(void) {
 if( pin>-1) {
  if((properties & APT_BUTTON_PROP_INVERT) == APT_BUTTON_PROP_INVERT) 
         return ! digitalRead(pin) ;
  else   return   digitalRead(pin);
 } else return false;
}


#define APT_BUTTON_STATE0_NORMAL               0
#define APT_BUTTON_STATE1_PUSHED               1
#define APT_BUTTON_STATE2_PUSHED_DEBOUNCED     2
#define APT_BUTTON_STATE3_RELEASE              3
#define APT_BUTTON_STATE4_RELEASE_DEBOUNCED    4


void APT_Button::advanceStateMachine(bool buttonPushed) {
  unsigned long timeStamp = millis();
  switch (state) {
    case 0:   // default state nothing happens
              if(buttonPushed) { 
                state = 1;
                timeMemory1 = timeStamp;
              }
              break;
    case 1:   // button pushed, wait for debouncing
              if(!buttonPushed) {
                state = 0;  
              } else if( (timeStamp - timeMemory1) > debounceTime_ms ) {
                state = 2;
                (*handlerFunction)(APT_BUTTON_EVENT_PRESSED);
              }
              break;
    case 2:   // button pressed, wait for long press or release
              if(!buttonPushed) {  
                state = 3;
                timeMemory2 = timeStamp;
              } else if( (timeStamp - timeMemory1) > longPressTime_ms ) { // toDO 1 ort 2 
                state = 10;
                (*handlerFunction)(APT_BUTTON_EVENT_LONGPRESSED);
              } // if(!buttonPushed) { ... } else {
              break;
    case 3:   // button released to first time 
              if(buttonPushed) {  
                state = 2; // pushed again
              } else if ( (timeStamp - timeMemory2) > debounceTime_ms ) { // and not just bouncing
                  state = 4;
                  timeMemory2 = timeStamp;
                  (*handlerFunction)(APT_BUTTON_EVENT_RELEASED);
              }
              break;
    case 4:   if(buttonPushed) {  // pushed again
                  state = 5;
                  timeMemory2 = timeStamp;
              } else if ( (timeStamp - timeMemory2) > doubleClickTime_ms  ) { 
                (*handlerFunction)(APT_BUTTON_EVENT_CLICKED);
                 state = 0;
              }
              break;
    case 5:   if(!buttonPushed) {  // pushed again
                  state = 4;
              } else if ( (timeStamp - timeMemory1) >  debounceTime_ms  ) { 
                (*handlerFunction)(APT_BUTTON_EVENT_PRESSED);
                state = 6;
              }
              break;   
    case 6:   if(!buttonPushed) {  // pushed again
                  state = 7;
                  timeMemory2 = timeStamp;
              } else if ( (timeStamp - timeMemory1) >  longPressTime_ms   ) { 
                (*handlerFunction)(APT_BUTTON_EVENT_DOUBLELONGPRESSED);
                state = 8;
              }
              break;                   
    case 7:   if(buttonPushed) {  // pushed again
                state = 6;
              } else if ( (timeStamp - timeMemory2) >  debounceTime_ms  ) { 
                (*handlerFunction)(APT_BUTTON_EVENT_DOUBLECLICKED);
                (*handlerFunction)(APT_BUTTON_EVENT_RELEASED);
                state = 0;
              }
              break;            
    case 8:   if(!buttonPushed) {  // pushed again
                  state = 9;
                  timeMemory2 = timeStamp;
              } 
              break;                            
    case 9:   if(buttonPushed) {  // pushed again
                  state = 8;
              } else if ( (timeStamp - timeMemory2) >  debounceTime_ms  ) { 
                (*handlerFunction)(APT_BUTTON_EVENT_DOUBLELONGCLICKED);
                (*handlerFunction)(APT_BUTTON_EVENT_RELEASED);
                state = 0;
              }
              break;                            
    case 10:  if(!buttonPushed) {  // pushed again
                  state = 11;
                  timeMemory1 = timeStamp;
              }
              break;    
    case 11:  if(buttonPushed) {  // pushed again
                  state = 10;
              } else if ( (timeStamp - timeMemory1) >  debounceTime_ms  ) { 
                (*handlerFunction)(APT_BUTTON_EVENT_RELEASED);
                (*handlerFunction)(APT_BUTTON_EVENT_LONGCLICKED);
                state = 0;
              }
              break;                            
    default:  // should never come here, ToDo: Exception Handling ?
              state = 0;
  } //switch (state) {
}



APT_ButtonEncoder::APT_ButtonEncoder(uint8_t encPin1,uint8_t encPin2,uint8_t buttonPin, uint8_t properties = 0 ) 
: APT_Button(buttonPin,properties), APT_Encoder(encPin1, encPin2)  {}

void APT_ButtonEncoder::loop() {
  APT_Button::loop();
  APT_Encoder::loop();
}
