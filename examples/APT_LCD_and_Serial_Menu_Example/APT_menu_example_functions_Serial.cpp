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

// example routines for serial interface
#ifdef APT_DEFAULT_SERIEL_MENU_HANDLING_ROUTINES

#include <Arduino.h>
#include <APT_Menu.h>
#include <APT_Input.h>
#include "APT_menu_example_header.h"

extern bool              menu4_value;
extern uint8_t           menu5_value;
extern uint8_t           menu6_value;
extern APT_Menu          menu;
extern APT_Timer         screenSaver;
extern APT_SerialInput   inputHandler;



void functionSerialCommandDefault(const char command) {
  SCREENSAVER_KEEPAWAKE(); 
      switch(command) {
        case 'w': menu.goUp();  screenSaver.start();break;
        case 's': menu.goDown(); screenSaver.start();break;
        case 'e': menu.goInto(); screenSaver.start();break;
        case 'q': menu.goBack(); screenSaver.start();break;
        case 'p': REBOOT(); break; // SW reset      
        case 10 : case 13: break;
        default: Serial.print("Unknown command: "); Serial.println(command);
      } // switch(command) {
}

void functionSerialCommandMenu5(const char command) {
  SCREENSAVER_KEEPAWAKE(); 
  switch(command) {
    case 'w': menu5_value += 1; menu.forceUpdate(true); break; // update at least the current menu line
    case 's': menu5_value -= 1; menu.forceUpdate(true); break; // update at least the current menu line
    case 'q': menu.goBack(); break;
    case 'p': REBOOT(); break; // SW reset      
    default: {};
  } // switch(command) {
}



// example for a timer MenuItem
unsigned long lastUpdate = 0;
bool MENU6_callback(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line) {
 switch ( APT_CLB_GETCONDITION(callbackType) ) {
  case APT_CLB_ISFULLMENUITEM:  return false; // line menu item
  case APT_CLB_DONEEDUPDATE:    if( ( millis() - lastUpdate ) > 1000) { lastUpdate = millis(); return true; }
                                else return false;  // update every second
  case APT_CLB_ONENTER:         return false; // false cannot be entered
  case APT_CLB_ONCURSORUPDATE:
  case APT_CLB_ONUPDATE_OTHERLINE: // all muist be updated since Seriel cannot line update
  case APT_CLB_ONUPDATE_LINE:   char buffer[20]; entry->getContent(buffer);
                                char printLine[100]; 
                                sprintf(printLine,"%c%-15s%3d%s",
                                          APT_GET_DEFAULT_CURSOR( APT_CLB_ISCURSORON(callbackType), APT_CLB_ISACTIVEATED(callbackType) ),
                                          buffer,
                                          menu6_value,
                                          scrollbarSerialCharacters[ menu->getScrollbarItem(line)] );
                                Serial.println(printLine);
                                lastUpdate = millis(); 
                                break;
  default:                      return false; // unhandled command
 };
 return false;
}


// example for a value MenuItem
bool MENU5_callback(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line) {
 switch ( APT_CLB_GETCONDITION(callbackType) ) {
  case APT_CLB_ISFULLMENUITEM:  return false;
  case APT_CLB_ONEXIT:          inputHandler.attachHandler(functionSerialCommandDefault);
                                break;
  case APT_CLB_ONENTER:         inputHandler.attachHandler(functionSerialCommandMenu5);
                                return true; // true, since can be activated
  case APT_CLB_ONCURSORUPDATE:
  case APT_CLB_ONUPDATE_OTHERLINE: // all muist be updated since Seriel cannot line update
  case APT_CLB_ONUPDATE_LINE:   char buffer[20]; entry->getContent(buffer);
                                char printLine[100]; 
                                sprintf(printLine,"%c%-15s%3d%s",
                                          APT_GET_DEFAULT_CURSOR( APT_CLB_ISCURSORON(callbackType), APT_CLB_ISACTIVEATED(callbackType) ),
                                          buffer,
                                          menu5_value,
                                          scrollbarSerialCharacters[ menu->getScrollbarItem(line)] );
                                Serial.println(printLine);
                                break;
  default:                      return false; // unhandled command
 };
 return false;
}

// example for a toggle line MenuItem
bool MENU4_callback(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line) {
 switch ( APT_CLB_GETCONDITION(callbackType) ) {
  case APT_CLB_ISFULLMENUITEM:  return false;
  case APT_CLB_ONENTER:         menu4_value = !menu4_value; // toggle value
                                return false; // false, since cannot be activated
  case APT_CLB_ONCURSORUPDATE:
  case APT_CLB_ONUPDATE_OTHERLINE: // all muist be updated since Serial cannot line update
  case APT_CLB_ONUPDATE_LINE:   char buffer[20]; entry->getContent(buffer);
                                char printLine[100]; 
                                sprintf(printLine,"%c%-17s%1d%s",
                                          APT_GET_DEFAULT_CURSOR( APT_CLB_ISCURSORON(callbackType), APT_CLB_ISACTIVEATED(callbackType) ),
                                          buffer,
                                          menu4_value,
                                          scrollbarSerialCharacters[ menu->getScrollbarItem(line)] );
                                Serial.println(printLine);
                                break;
  default:                      return false; // unhandled command
 };
 return false;
}

// example for a full MenuItem
bool MENU3_callback(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line) {
  switch ( APT_CLB_GETCONDITION(callbackType) ) { // Note: callbackType includes bits for cursor and actived setting                               
  case APT_CLB_ISFULLMENUITEM:      return true; // it is a full menu item
  case APT_CLB_ONENTER:             return true; // can be entered
  case APT_CLB_ONCURSORUPDATE:      // Must be updated on all three events since Serial
  case APT_CLB_ONUPDATE_OTHERLINE:  // interface does not support line updates
  case APT_CLB_ONUPDATE_LINE:       if ( APT_CLB_ISACTIVEATED(callbackType) ) {
                                      Serial.println(F("This is a menu."));
                                      Serial.println(F("Press back"));
                                      Serial.println(F("to exit."));
                                    } else menu->callDefaultShowEntryFunction(entry, callbackType, line);
                                    break; 
  default:                          return false; // unhandled command
 };
 return false;
}

#endif     
