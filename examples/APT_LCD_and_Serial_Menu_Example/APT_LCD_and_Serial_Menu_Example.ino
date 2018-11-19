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
/* portions of the Software.                                               */
/*                                                                         */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,         */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF      */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY    */ 
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,    */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE       */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  */
/***************************************************************************/


// This example can be configured to a serial menu or an LCD 
// menu. Uncomment one of the following to choose:
//#define   APT_DEFAULT_SERIEL_MENU_HANDLING_ROUTINES
#define   APT_DEFAULT_LCD_MENU_HANDLING_ROUTINES


/***************************************************************************/
/* Including libraries                                                     */
/***************************************************************************/
#include <APT_Menu.h>
#include <APT_Input.h>
#include <APT_Timer.h>
#include <APT_SerialInput.h>
#include "APT_menu_example_header.h"
#include "APT_menu_example_functions_LCD.cpp"
#include "APT_menu_example_functions_Serial.cpp"


/***************************************************************************/
/* General settings                                                        */
/***************************************************************************/

#define  PIN_ENC_BUTTON           4 
#define  PIN_ENC_A                2 
#define  PIN_ENC_B                3 

#define  SCREEN_SAVER_TIME_MS  15000
#define  ROW_COUNT             4


/***************************************************************************/
/* Global variables for the menu items                                     */
/***************************************************************************/

bool      menu4_value = false;
uint8_t   menu5_value = 0;
uint8_t   menu6_value = 0;
APT_Timer screenSaver(SCREEN_SAVER_TIME_MS);


//***********************************************************//
//* Initialize the menu                                     *//
//***********************************************************//

// prototypes of the differnt callbacks for the menu
bool MENU3_callback(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line);
bool MENU4_callback(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line);
bool MENU5_callback(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line);
bool MENU6_callback(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line);

APT_Menu menu(ROW_COUNT);                                             // define a global menu variable
APT_MENUITEM_ADD     ( 1,  0,"Menu 1 (empty)");                       // A menu entry w/ content
APT_MENUITEM_ADD     ( 2,  0,"Menu 2 (sub)");                         // A menu with sub menus.
APT_MENUITEM_ADD     ( 3,  2,"Menu 2.1");                             // -"-
APT_MENUITEM_ADD     ( 4,  3,"Menu 2.1.1");                           // -"-
APT_MENUITEM_ADD     ( 5,  4,"Menu 2.1.1.1");                         // -"-
APT_MENUITEM_ADD     ( 6,  2,"Menu 2.2");                             // -"-
APT_MENUITEM_ADDLEAF ( 7,  0,"Menu 3 (full)",   MENU3_callback);      // A full menu showing some text.
APT_MENUITEM_ADDLEAF ( 8,  0,"Menu 4 (bool):",  MENU4_callback);      // A menu wich toggles a value on enter.
APT_MENUITEM_ADDLEAF ( 9,  0,"Menu 5 (value):", MENU5_callback);      // A menu with adjustable value.
APT_MENUITEM_ADDLEAF (10,  0,"Menu 6 (auto):",  MENU6_callback);      // A menu with automatic updated value.
#define APT_MENU_COUNT 10                                             // Number of menu entries


#if  defined(APT_DEFAULT_LCD_MENU_HANDLING_ROUTINES)
  //***********************************************************//
  //* Initialize LCD, button and encoder                      *//
  //***********************************************************//

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wuninitialized"
  #include <LiquidCrystal_I2C.h>       // Library for I2C LCD
  #pragma GCC diagnostic pop
  extern LiquidCrystal_I2C  lcd;

  // prototype for the default handlers
  void functionEncoderDefault(APT_Encoder* encoder);
  void functionButtonDefault(uint8_t eventType);

  APT_ButtonEncoder inputHandler(PIN_ENC_A,PIN_ENC_B, PIN_ENC_BUTTON,  APT_BUTTON_PROP_INVERT );

#elif  defined(APT_DEFAULT_SERIEL_MENU_HANDLING_ROUTINES)
  //***********************************************************//
  //* Initialize Serial input handler                         *//
  //***********************************************************//

  // prototype for the default handler
  void functionSerialCommandDefault(const char command);
    
  APT_SerialInput inputHandler;

#endif
    
//***********************************************************//
//* Main setup function                                     *//
//***********************************************************//

void setup() {

  // used for debugging
  Serial.begin(115200);
  
  // Initialization of the menu
  APT_MENU_SETUP(menu, APT_MENU_COUNT );
  // Configure menu, jump back to first line when end is reached
  menu.setWrapAround(false);   
  

#if   defined(APT_DEFAULT_LCD_MENU_HANDLING_ROUTINES)
  // Initialization example for an LCD menu with encoder
  APT_DefaultLCDMenu_init();
  menu.setDefaultShowEntryFunction( APT_DefaultLCDMenu_showMenuItem );
  menu.setClearFunction( APT_DefaultLCDMenu_clear );
  inputHandler.APT_Button::attachHandler(functionButtonDefault);
  inputHandler.APT_Encoder::attachHandler(functionEncoderDefault,4); // each 4 ticks a notch

#elif  defined(APT_DEFAULT_SERIEL_MENU_HANDLING_ROUTINES)
  // Initialization example for a serial interfae menu
  menu.setDefaultShowEntryFunction( APT_DefaultSerialMenu_showMenuItem );
  menu.setClearFunction( APT_DefaultSerialMenu_clearMenu, APT_DefaultSerialMenu_clearLine );
  inputHandler.attachHandler(functionSerialCommandDefault);

#endif // defined(APT_DEFAULT_LCD_MENU_HANDLING_ROUTINES)

  // start the screensaver
  screenSaver.start();
}


//***********************************************************//
//* Main loop                                               *//
//***********************************************************//

      
void loop() {

  // Constantly increase variable of menu 5
  menu6_value++; 

  // Call loop function of all classes
  inputHandler.loop();
  menu.loop();
  

#ifdef APT_DEFAULT_LCD_MENU_HANDLING_ROUTINES

  // Screensaver: If the timer is overrun, turn off 
  // the menu and the backlight. If somebody re-started 
  // the screensaver timer, turn on the screen and menu 
  // again
  static bool screenOff = false;
  if ( screenSaver.isOver() ) {
      menu.turnOff();
      screenOff = true;
      lcd.noBacklight();
  } else if (screenOff && ! screenSaver.isStopped() ) {
      menu.turnOn();
      screenOff = false;
      lcd.backlight();
  }
#elif  defined(APT_DEFAULT_SERIEL_MENU_HANDLING_ROUTINES)
#endif

} // void loop();
