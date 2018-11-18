/***************************************************************************/
/* Arduino Power Tools (APT) - Library by BastelBaus                       */
/* - example how to use the APT_menu library                               */
/***************************************************************************/


// This example can be configured to a serial menu 
// or an LCD menu. Uncomment one of the following to 
// choose
//#define   APT_DEFAULT_SERIEL_MENU_HANDLING_ROUTINES
#define   APT_DEFAULT_LCD_MENU_HANDLING_ROUTINES


/***************************************************************************/
/* Including libraries                                                     */
/***************************************************************************/
#include "APT_Menu.h"
#include "APT_Input.h"
#include "APT_Timer.h"

#ifdef APT_DEFAULT_LCD_MENU_HANDLING_ROUTINES
  #include "APT_menu_example_functions_LCD.h"
#elif  APT_DEFAULT_SERIEL_MENU_HANDLING_ROUTINES
  #include "APT_menu_example_functions_Serial.cpp"
#endif

/***************************************************************************/
/* General settings                                                        */
/***************************************************************************/


#define  PIN_BUTTON               A1 
#define  PIN_ENC_BUTTON           4 
#define  PIN_ENC_A                2 
#define  PIN_ENC_B                3 

#define  SCREEN_SAVER_TIME_MS  15000
#define  ROW_COUNT             4
#define  REBOOT()  { asm volatile ("  jmp 0"); }

/***************************************************************************/
/* Global variables for the menu items                                     */
/***************************************************************************/

bool      menu4_value = false;
uint8_t   menu5_value = 0;
uint8_t   menu6_value = 0;
APT_Timer screenSaver(SCREEN_SAVER_TIME_MS);
#define   SCREENSAVER_KEEPAWAKE() screenSaver.start() 


bool MENU3_callback(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line);
bool MENU4_callback(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line);
bool MENU5_callback(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line);
bool MENU6_callback(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line);

//***********************************************************//
//* Initialize the menu                                     *//
//***********************************************************//

APT_Menu menu(ROW_COUNT);                               // define a global menu variable
APT_MENUITEM_ADD     ( 1,  0,"Menu 1 (empty)");                       //
APT_MENUITEM_ADD     ( 2,  0,"Menu 2 (sub)");
APT_MENUITEM_ADD     ( 3,  2,"Menu 2.1");
APT_MENUITEM_ADD     ( 4,  3,"Menu 2.1.1");
APT_MENUITEM_ADD     ( 5,  4,"Menu 2.1.1.1");
APT_MENUITEM_ADD     ( 6,  2,"Menu 2.2");
APT_MENUITEM_ADDLEAF ( 7,  0,"Menu 3 (full)",   MENU3_callback);      
APT_MENUITEM_ADDLEAF ( 8,  0,"Menu 4 (bool):",  MENU4_callback);
APT_MENUITEM_ADDLEAF ( 9,  0,"Menu 5 (value):", MENU5_callback);
APT_MENUITEM_ADDLEAF (10,  0,"Menu 6 (auto):",  MENU6_callback);
#define APT_MENU_COUNT 10

//***********************************************************//
//* Initialize the buttons and encoders                     *//
//***********************************************************//

void functionEncoderDefault(APT_Encoder* encoder);
void functionButtonDefault(uint8_t eventType);

APT_Button  button1(PIN_BUTTON, APT_BUTTON_PROP_PULLUP | APT_BUTTON_PROP_INVERT );
APT_Encoder encoder(PIN_ENC_A,PIN_ENC_B);
APT_Button  encButton(PIN_ENC_BUTTON,  APT_BUTTON_PROP_INVERT );

APT_ButtonEncoder buttonEncoder(PIN_ENC_A,PIN_ENC_B, PIN_ENC_BUTTON,  APT_BUTTON_PROP_INVERT );

//void button1Function(uint8_t eventType) {
//  switch ( eventType ) {
//    case APT_BUTTON_EVENT_PRESSED           : Serial.println(F("pressed")); break; 
//    case APT_BUTTON_EVENT_RELEASED          : Serial.println(F("released")); break; 
//    case APT_BUTTON_EVENT_CLICKED           : Serial.println(F("clicked")); break; 
//    case APT_BUTTON_EVENT_LONGPRESSED       : Serial.println(F("long pressed")); break; 
//    case APT_BUTTON_EVENT_DOUBLELONGPRESSED : Serial.println(F("double long pressed")); break; 
//    case APT_BUTTON_EVENT_DOUBLECLICKED     : Serial.println(F("double click")); break; 
//    default                                 : Serial.println(F("ERROR unkown event type")); break; 
//  }  
//} // void attachHandler(button1Function(uint8_t eventType) {
//


//***********************************************************//
//* Main setup function                                     *//
//***********************************************************//

void setup() {

  Serial.begin(115200);

  
  APT_MENU_SETUP(menu, APT_MENU_COUNT );
  menu.setWrapAround(true);

#ifdef APT_DEFAULT_LCD_MENU_HANDLING_ROUTINES
  APT_DefaultLCDMenu_init();
  menu.setDefaultShowEntryFunction( APT_DefaultLCDMenu_showMenuItem );
  menu.setClearFunction( APT_DefaultLCDMenu_clear );
  buttonEncoder.APT_Button::attachHandler(functionButtonDefault);
  buttonEncoder.APT_Encoder::attachHandler(functionEncoderDefault,4); // each 4 ticks a "Raster"
 
#elif  APT_DEFAULT_SERIEL_MENU_HANDLING_ROUTINES
  menu.setDefaultShowEntryFunction( APT_DefaultSerialMenu_showMenuItem );
  menu.setClearFunction( APT_DefaultSerialMenu_clearMenu, APT_DefaultSerialMenu_clearLine );

#endif

  screenSaver.start();
}



#include <LiquidCrystal_I2C.h>       // Library for I2C LCD
extern LiquidCrystal_I2C  lcd;

uint8_t key = ' ';

//***********************************************************//
//* Main loop                                               *//
//***********************************************************//

      
void loop() {

  // Constantly increase variable of menu 5
  menu6_value++; 

  // Call loop function of all classes
  buttonEncoder.loop();
  menu.loop();


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


      
  #if  APT_DEFAULT_SERIEL_MENU_HANDLING_ROUTINES
    if (Serial.available() > 0) {
      uint8_t key = Serial.read();
      APT_MenuItem* activeMenuItem = menu.getActiveMenuItem();
      if( activeMenuItem != NULL ) {      
        switch( activeMenuItem->getID() ) {
          case 9:   // value MenuItem
                    switch(key) {
                      case 's' : menu5_value++;
                               menu.forceUpdate(activeMenuItem);
                               break;
                    case 'w' : menu5_value--;
                               menu.forceUpdate(activeMenuItem);
                               break;
                    default: ;
                  };
                  break;
        default:  ;
      }
      }
      // no else, so process menu key's in any case
      switch(key) {
        case 'w': menu.goUp();  screenSaver.start();break;
        case 's': menu.goDown(); screenSaver.start();break;
        case 'e': menu.goInto(); screenSaver.start();break;
        case 'q': menu.goBack(); screenSaver.start();break;
        case 'p': REBOOT(); break; // SW reset      
        case 10 : case 13: break;
        default: Serial.print("Unknown command: "); Serial.println((char) key);
      }
   }
  #endif
  
} // void loop();
