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
/* portions of the Software.                               */
/*                                                                         */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,         */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF      */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY    */ 
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,    */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  */
/***************************************************************************/

#ifdef APT_DEFAULT_LCD_MENU_HANDLING_ROUTINES

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
#include <LiquidCrystal_I2C.h>       // Library for I2C LCD
#pragma GCC diagnostic pop
extern LiquidCrystal_I2C   lcd;

#include <APT_menu.h>
#include <APT_Timer.h>
#include <APT_Input.h>
#include "APT_menu_example_header.h"

#define APT_EXAMPLE_LCD_COLUMNS    20

extern bool              menu4_value;
extern uint8_t           menu5_value;
extern uint8_t           menu6_value;
extern APT_Menu          menu;
extern APT_Timer         screenSaver;

extern APT_ButtonEncoder inputHandler;


void functionButtonDefault(uint8_t eventType) {
  SCREENSAVER_KEEPAWAKE(); 
  switch (eventType) {
    case APT_BUTTON_EVENT_CLICKED:            menu.goInto(); break;
    case APT_BUTTON_EVENT_LONGCLICKED:        menu.goBack(); break;
    case APT_BUTTON_EVENT_DOUBLELONGPRESSED:  REBOOT(); break;
  }
}

void functionButtonMenu3(uint8_t eventType) {
  SCREENSAVER_KEEPAWAKE();
  switch (eventType) {
    case APT_BUTTON_EVENT_DOUBLECLICKED:      menu.goBack(); break;
    case APT_BUTTON_EVENT_DOUBLELONGPRESSED:  REBOOT(); break;
  }
}

void functionButtonMenu5(uint8_t eventType) {
  SCREENSAVER_KEEPAWAKE();
  switch (eventType) {
    case APT_BUTTON_EVENT_CLICKED:            menu.goBack(); break;
    case APT_BUTTON_EVENT_DOUBLELONGPRESSED:  REBOOT(); break;
  }
}

void functionEncoderDefault(APT_Encoder* encoder) {
  // SCREENSAVER_KEEPAWAKE(); 
  if      (encoder->consumeTicks(-4)) { SCREENSAVER_KEEPAWAKE(); menu.goDown(); }
  else if (encoder->consumeTicks(+4)) { SCREENSAVER_KEEPAWAKE(); menu.goUp(); }
}

void functionEncoderMenu5(APT_Encoder* encoder) {
  int32_t ticks = encoder->consumeTicks() >> 2;
  if(ticks !=0) {
     SCREENSAVER_KEEPAWAKE();
     menu5_value -= ticks;
     menu.forceUpdate(true); // update at least the current menu line
  }
}


// example for a timer MenuItem
APT_Timer Menu6RefreshTimer(100);
unsigned long lastUpdate = 0;
bool MENU6_callback(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line) {
 switch ( APT_CLB_GETCONDITION(callbackType) ) {
  case APT_CLB_ISFULLMENUITEM:  return false; // line menu item
  case APT_CLB_DONEEDUPDATE:    if( Menu6RefreshTimer.isStopped() ) {
                                  Menu6RefreshTimer.start();
                                  return true; 
                                } else return false;  
  case APT_CLB_ONENTER:         return false; // false cannot be entered
  case APT_CLB_ONCURSORUPDATE:
  case APT_CLB_ONUPDATE_OTHERLINE: // must be updated in both cases
  case APT_CLB_ONUPDATE_LINE:   { char printLine[23];
                                  char cursorItem = APT_GET_DEFAULT_CURSOR( APT_CLB_ISCURSORON(callbackType), APT_CLB_ISACTIVEATED(callbackType) );
                                  char content[APT_EXAMPLE_LCD_COLUMNS+1]; entry->getContent(content);
                                  char scrollbarItem = menu->getScrollbarItem(line);
                                  if(scrollbarItem == 0) scrollbarItem = ' '; else scrollbarItem -= 1;
                                  sprintf(printLine,"%c%-15s%3d%c",cursorItem,content,menu6_value,scrollbarItem);
                                  lcd.setCursor(0,line);
                                  lcd.print(printLine);
                                } break;
  default:                      {} // unhandled command
 };
 return false;
}


// example for a value MenuItem
bool MENU5_callback(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line) {
 switch ( APT_CLB_GETCONDITION(callbackType) ) {
  case APT_CLB_ISFULLMENUITEM:  return false;
  case APT_CLB_ONEXIT:          inputHandler.APT_Encoder::attachHandler( functionEncoderDefault, 4 );
                                inputHandler.APT_Button::attachHandler(  functionButtonDefault );
                                return true; 
  case APT_CLB_ONENTER:         inputHandler.APT_Encoder::attachHandler( functionEncoderMenu5, 4 );
                                inputHandler.APT_Button::attachHandler(  functionButtonMenu5 );
                                return true; // true, since can be activated
  case APT_CLB_ONCURSORUPDATE:  
  case APT_CLB_ONUPDATE_LINE:   { lcd.setCursor(0,line);
                                  lcd.print( APT_GET_DEFAULT_CURSOR( APT_CLB_ISCURSORON(callbackType), APT_CLB_ISACTIVEATED(callbackType) ) );
                                  char buffer[20]; entry->getContent(buffer);
                                  char printLine[22];
                                  sprintf(printLine,"%s%3d",buffer,menu5_value);
                                  lcd.print(printLine);
                                  lcd.setCursor(19,line); 
                                  uint8_t scrollbarItem = menu->getScrollbarItem(line);
                                  if(scrollbarItem!=0) lcd.write(scrollbarItem-1) ;
                                } break;
  default:                      {}// unhandled command
 };
 return false;
}

// example for a toggle line MenuItem
bool MENU4_callback(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line) {
 switch ( APT_CLB_GETCONDITION(callbackType) ) {
  case APT_CLB_ISFULLMENUITEM:  return false;
                                break;
  case APT_CLB_ONENTER:         menu4_value = !menu4_value; // toggle value
                                return false; // false, since cannot be activated
                                break;
 case APT_CLB_ONCURSORUPDATE:
 case APT_CLB_ONUPDATE_LINE:    { lcd.setCursor(0,line);
                                  lcd.print( APT_GET_DEFAULT_CURSOR( APT_CLB_ISCURSORON(callbackType), APT_CLB_ISACTIVEATED(callbackType) ) );
                                  char buffer[20]; entry->getContent(buffer);
                                  lcd.print(buffer);
                                  lcd.print(menu4_value);
                                  lcd.setCursor(19,line); 
                                  uint8_t scrollbarItem = menu->getScrollbarItem(line);
                                  if(scrollbarItem!=0) lcd.write(scrollbarItem-1) ;
                                } break;
  default:                      {}; // unhandled command
 };
 return false;
}

// example for a full MenuItem
bool MENU3_callback(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line) {
 switch ( APT_CLB_GETCONDITION(callbackType) ) {
  case APT_CLB_ISFULLMENUITEM:  return true;
  case APT_CLB_ONEXIT:          inputHandler.APT_Button::attachHandler(functionButtonDefault); 
                                break;                                              
  case APT_CLB_ONENTER:         inputHandler.APT_Button::attachHandler(functionButtonMenu3); 
                                return true; // yes we can go inside the function ToDo: do we need it ?
  case APT_CLB_ONCURSORUPDATE:  
  case APT_CLB_ONUPDATE_LINE:   if ( APT_CLB_ISACTIVEATED(callbackType) ) {
                                  lcd.setCursor(0,0); lcd.print(F("This is a menu. "));
                                  lcd.setCursor(0,1); lcd.print(F("Double click"));
                                  lcd.setCursor(0,2); lcd.print(F("to exit."));                                  
                                } else menu->callDefaultShowEntryFunction(entry, callbackType, line);
                                break;                                                               
  default:                      {}; // unhandled command
 };
 return false;
}




#endif // #ifdef APT_DEFAULT_LCD_MENU_HANDLING_ROUTINES
