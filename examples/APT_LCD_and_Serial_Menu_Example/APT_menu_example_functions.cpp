
#include <APT_menu.h>
#include <APT_Timer.h>
#include <APT_Input.h>
#define APT_DEBUG 1
#include <APT_Debug.h>
#include "APT_menu_example_header.h"




bool MENU_GOTO_callback(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line) {
 if ( APT_CLB_GETCONDITION(callbackType) == APT_CLB_ONENTER) menu->gotoID(3);
 return menu->callDefaultShowEntryFunction(entry, callbackType, line);
}




bool MENU_HIDE_callback(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line) {
  if ( APT_CLB_GETCONDITION(callbackType) == APT_CLB_ONENTER) {
    switch( entry->getID() ) {
      case 13:  menu->hide(entry); break;
      case 14: if ( APT_CLB_GETCONDITION(callbackType) == APT_CLB_ONENTER) menu->show( APT_MENUITEM_POINTER(13) ); break;
    }
  } // if ( APT_CLB_GETCONDITION(callbackType) == APT_CLB_ONENTER) {
    
 return menu->callDefaultShowEntryFunction(entry, callbackType, line);
}
