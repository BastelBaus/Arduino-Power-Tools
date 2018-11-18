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


#ifndef __APT_INPUT_H__
#define __APT_INPUT_H__

// Credits to http://www.mathertel.de/Arduino/OneButtonLibrary.aspx
// which offers also a very nice button library
// PaulStoffregen
// https://github.com/PaulStoffregen/Encoder

// For each button class you can define a handler. The 
// handler is called on all possible events. The event type 
// will be passed to the handler 
typedef void (*handlerFunctionType)(uint8_t eventType);

// The following events will trigger a callback:
#define APT_BUTTON_EVENT_PRESSED                  0
#define APT_BUTTON_EVENT_RELEASED                 1  
#define APT_BUTTON_EVENT_CLICKED                  2
#define APT_BUTTON_EVENT_LONGCLICKED              3
#define APT_BUTTON_EVENT_LONGPRESSED              4
#define APT_BUTTON_EVENT_DOUBLELONGPRESSED        (128 | APT_BUTTON_EVENT_LONGPRESSED)
#define APT_BUTTON_EVENT_DOUBLECLICKED            (128 | APT_BUTTON_EVENT_CLICKED)
#define APT_BUTTON_EVENT_DOUBLELONGCLICKED        (128 | APT_BUTTON_EVENT_DOUBLECLICKED)

// Use the following macro if you are not interrested
// in double click differentiation. It will converts 
// DOUBLECLICKED to CLICKED and DOUBLELONGPRESSED to 
// LONGPRESSED
#define APT_BUTTON_GET_SINGLE_EVENT(x)             (x && 127)     

class APT_Button {

  public:
  
    #define APT_BUTTON_PROP_INVERT    1
    #define APT_BUTTON_PROP_PULLUP    2

    // Initializes an APT_Button class withou a pin associated.
    APT_Button();
    
    // Initializes an APT_Button and directly calls the setup function;
    // pin:         pin numbr for pineMode() function.
    // properties:  Set properties of this button (APT_BUTTON_PROP_INVERT and/or 
    //              APT_BUTTON_PROP_PULLUP)
    APT_Button(uint8_t pin, uint8_t properties = 0);

    // (Re-) Setup the button.
    // pin:         pin numbr for pineMode() function.
    // properties:  Set properties of this button (APT_BUTTON_PROP_INVERT and/or 
    //              APT_BUTTON_PROP_PULLUP)
    // Remark: if the handler function is the, the button state is re-initialized. 
    void setup(uint8_t pin, uint8_t properties = 0);

    
    // Attach a handler function to this button. The handler 
    // function is called on each event. Set it to NULL to deactive it.
    // Remark: if the handler function is the, the button state is re-initialized. 
    void attachHandler(handlerFunctionType handlerFunction = NULL);
  
    // Set the different timings of the button.
    void setDebounceTime    (uint16_t debounceTime_ms);
    void setDoubleClickTime (uint16_t doubleClickTime_ms);
    void setLongPressTime   (uint16_t longPressTime_ms);

    // Returns true if button is currently pushed, else false.
    // Returns false if not initialized.
    bool isPushed(void);
    
    // Must be called in each loop to update button status.
    void loop(void);
    
   private:
    uint8_t       state       =  0;
     int8_t       pin         = -1;
    uint8_t       properties  =  0;
    unsigned long timeMemory1 =  0;
    unsigned long timeMemory2 =  0;

    uint16_t      debounceTime_ms     =    20;
    uint16_t      doubleClickTime_ms  =   200;
    uint16_t      longPressTime_ms     =  800;

    handlerFunctionType handlerFunction = NULL;
    
    void advanceStateMachine(bool buttonPushed);

};

#include "Encoder.h" // https://github.com/PaulStoffregen/Encoder
class APT_Encoder;

typedef void (*encoderHandlerType)(APT_Encoder* encoder);

#define APT_ENCODER_INVERT    1
#define ENCODER_USE_INTERRUPTS

class APT_Encoder : public Encoder {
  public:
    APT_Encoder(uint8_t pint1,uint8_t pint2, uint8_t parameters = 0);

    void setParameters(uint8_t parameters); 

    // fast increment when within short time 
    // the encoder incemennts more than, than it sis scalled
    // 
    
    // handle change of increments
    // ToDo add callback types
    // 0: call always
    // +/-n: call after encoder has equal of more ticks in this direction
    void attachHandler(encoderHandlerType encoderHandler, uint16_t callAtAmountOfTicks);
          
    // If the encoder as at least amountofTicks (sign 
    // matters) function returns true and reduces 
    // ticks by amountOfTicks to come closer to zero
    bool consumeTicks(int16_t amountOfTicks);
    // Returns the amount of ticks and sets it to 
    // zero afterwards
    int32_t consumeTicks();
    
    void loop();
    
  private: 
    encoderHandlerType encoderHandler = NULL;
    uint16_t callAtAmountOfTicks = 0;
    uint8_t parameters = 0;
  
};

class APT_ButtonEncoder : public APT_Button, public APT_Encoder {
   public:
    APT_ButtonEncoder(uint8_t encPin1,uint8_t encPin2,uint8_t buttonPin, uint8_t properties = 0 );

    void loop();
    
  private:     
}; // class APT_ButtonEncoder : public APT_Button, APT_Encoder {

#endif  // #define __APT_INPUT_H__
