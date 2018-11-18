
// example routines for serial interface
#ifdef APT_DEFAULT_SERIEL_MENU_HANDLING_ROUTINES

#include <Arduino.h>

extern bool    menu4_value;
extern uint8_t menu5_value;
extern uint8_t menu6_value;


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
                                break;
  case APT_CLB_ONENTER:         return true; // true, since can be activated
                                break;
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
                                break;
  case APT_CLB_ONENTER:         menu4_value = !menu4_value; // toggle value
                                return false; // false, since cannot be activated
                                break;
  case APT_CLB_ONCURSORUPDATE:
  case APT_CLB_ONUPDATE_OTHERLINE: // all muist be updated since Seriel cannot line update
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
  switch ( APT_CLB_GETCONDITION(callbackType) ) {
  case APT_CLB_ISFULLMENUITEM:  return true;
                                break;
  case APT_CLB_ONCURSORUPDATE:
  case APT_CLB_ONUPDATE_OTHERLINE: // all must be updated since Serial cannot line update
  case APT_CLB_ONUPDATE_LINE:   if ( APT_CLB_ISACTIVEATED(callbackType) ) {
                                  Serial.println(F("This is a menu."));
                                  Serial.println(F("Press back"));
                                  Serial.println(F("to exit."));
                                } else menu->callDefaultShowEntryFunction(entry, callbackType, line);
                                break; // Note: callbackType includes bits for cursor and actived setting                               
  default:                      return false; // unhandled command
 };
 return false;
}

#endif     
