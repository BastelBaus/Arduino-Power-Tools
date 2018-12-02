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

#include <assert.h>
#include "APT_menu.h"

#define APT_DEBUG 1
#include "APT_Debug.h"

/***************************************************************************/
/* DefaultSerialMenu Functions                                             */
/***************************************************************************/

#ifdef APT_DEFAULT_SERIEL_MENU_HANDLING_ROUTINES
  
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
void APT_DefaultSerialMenu_clearLine(uint8_t line) {
  APT_DefaultSerialMenu_clearMenu();
}
#pragma GCC diagnostic pop


void APT_DefaultSerialMenu_clearMenu() {
  for(uint8_t i=0;i<50;i++) Serial.println("");
  Serial.println("------- MenuM -------");
}

const char * scrollbarSerialCharacters[] = {" ", "║","╗","╣","╝"};


void APT_DefaultSerialMenu_showMenuItem(APT_Menu* menu,APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line) {
 
  // just ignores the callback type and reprint the full line
  char buffer[20]; entry->getContent(buffer);
  char printLine[42];
  sprintf(printLine,"%c%-18s%s", 
                  APT_GET_DEFAULT_CURSOR( APT_CLB_ISCURSORON(callbackType), APT_CLB_ISACTIVEATED(callbackType) ),
                  buffer, scrollbarSerialCharacters[ menu->getScrollbarItem(line)]);
  Serial.print(printLine);  

  if (1 == 2) {
    Serial.print("[");
    Serial.print(line);
    Serial.print(":");
    Serial.print((uint16_t)entry, HEX);
    Serial.print("(");
    Serial.print(entry->getID());
    Serial.print(") s: ");
    Serial.print((uint16_t) entry->getYoungerSibling(), HEX );
    if (entry->getYoungerSibling() != NULL) {
      Serial.print("(");
      Serial.print(entry->getYoungerSibling()->getID());
      Serial.print(")");
    };
    Serial.print(" c: ");
    Serial.print((uint16_t) entry->getChild(), HEX );
    if (entry->getChild() != NULL) {
      Serial.print("(");
      Serial.print(entry->getChild()->getID());
      Serial.print(")");
    };
    Serial.print(" p: ");
    Serial.print((uint16_t) entry->getParent(), HEX );
    if (entry->getParent() != NULL) {
      Serial.print("(");
      Serial.print(entry->getParent()->getID());
      Serial.print(")");
    };
    Serial.print("]");
  }

  Serial.println("");
}

#endif



/***************************************************************************/
/* DefaultLCDMenu Functions                                                */
/***************************************************************************/
//#ifdef APT_DEFAULT_LCD_MENU_HANDLING_ROUTINES


  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wuninitialized"
  #include <LiquidCrystal_I2C.h>       	// Library for I2C LCD
  #pragma GCC diagnostic pop
  extern LiquidCrystal_I2C   lcd;		// lcd class must be defined in main program.


#define PUT_TO_PROGMEM
#ifdef  PUT_TO_PROGMEM 

const char CHAR_NONE[8]  PROGMEM = {B10000, B10000, B10000, B10000, B10000, B10000, B10000, B10000}; // none
const char CHAR_UPPER[8] PROGMEM = {B11111, B11111, B10000, B10000, B10000, B10000, B10000, B10000}; // upper
const char CHAR_MID[8]   PROGMEM = {B10000, B10000, B10000, B11111, B11111, B10000, B10000, B10000}; // mid
const char CHAR_LOWER[8] PROGMEM = {B10000, B10000, B10000, B10000, B10000, B10000, B11111, B11111}; // lower

const char* const scrollbarLCDCharacters[4] PROGMEM = { CHAR_NONE, CHAR_UPPER, CHAR_MID, CHAR_LOWER };


void APT_DefaultLCDMenu_init() {
  lcd.init();
  lcd.clear();
  lcd.backlight();
  
  for(uint8_t i=0; i<4;i++) {
    char buffer[8];
	memcpy_P(buffer, (char*)pgm_read_word(&(scrollbarLCDCharacters[i])), 8); 
	lcd.createChar(i, (uint8_t*) buffer);
  }
  
  //lcd.createChar(0, (uint8_t*)scrollbarLCDCharacters[0]);
  //lcd.createChar(1, (uint8_t*)scrollbarLCDCharacters[1]);
  //lcd.createChar(2, (uint8_t*)scrollbarLCDCharacters[2]);
  //lcd.createChar(3, (uint8_t*)scrollbarLCDCharacters[3]);
}

#else

char scrollbarLCDCharacters[4][8] = {
    {B10000, B10000, B10000, B10000, B10000, B10000, B10000, B10000}, // none
    {B11111, B11111, B10000, B10000, B10000, B10000, B10000, B10000}, // upper
    {B10000, B10000, B10000, B11111, B11111, B10000, B10000, B10000}, // mid
    {B10000, B10000, B10000, B10000, B10000, B10000, B11111, B11111}  // lower
};
void APT_DefaultLCDMenu_init() {
  lcd.init();
  lcd.clear();
  lcd.backlight();
    
  for(uint8_t i=0; i<4;i++)
     lcd.createChar(i, (uint8_t*)scrollbarLCDCharacters[i]);
  
  //lcd.createChar(0, (uint8_t*)scrollbarLCDCharacters[0]);
  //lcd.createChar(1, (uint8_t*)scrollbarLCDCharacters[1]);
  //lcd.createChar(2, (uint8_t*)scrollbarLCDCharacters[2]);
  //lcd.createChar(3, (uint8_t*)scrollbarLCDCharacters[3]);
}

#endif




void APT_DefaultLCDMenu_clear() {
  lcd.clear();
}

bool APT_DefaultLCDMenu_showMenuItem(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line) {

	switch( APT_CLB_GETCONDITION(callbackType) ) {
		case APT_CLB_ONUPDATE_LINE:
		case APT_CLB_ONCURSORUPDATE:  {
										// print cursor
										lcd.setCursor(0, line);
										lcd.print( APT_GET_DEFAULT_CURSOR( APT_CLB_ISCURSORON(callbackType), APT_CLB_ISACTIVEATED(callbackType) ) );
  
										// print content
										char buffer[22]; entry->getContent(buffer);
										lcd.print(buffer);

										// print scrollbar
										uint8_t scrollbarItem = menu->getScrollbarItem(line);
										lcd.setCursor(19,line); 
										if(scrollbarItem!=0) lcd.write(scrollbarItem-1) ;
									  } return false;
		default: return false;
	}
  return false; // should never reach here
}

//#endif

/***************************************************************************/
/* APT_Menu class                                                          */
/***************************************************************************/

bool APT_Menu::needDisplayUpdate(void) {
  return bitRead(status, APT_MENU_STATUS_BIT_NEEDFULLUPDATE) ||
         bitRead(status, APT_MENU_STATUS_BIT_NEEDLINEUPDATE); // toDo add cursor update
}

void APT_Menu::setDefaultShowEntryFunction( clbFunctionType defaultShowEntryFunction) {
  this->defaultShowEntryFunction  = defaultShowEntryFunction;
}
bool  APT_Menu::callDefaultShowEntryFunction( APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line) {
  return (* (this->defaultShowEntryFunction) )(this, entry, callbackType, line);
}

void APT_Menu::setClearFunction( clearMenuFunctionType clearMenuFunction, clearLineFunctionType clearLineFunction) {
  this->clearMenuFunction = clearMenuFunction;
  if(clearLineFunction!=NULL) this->clearLineFunction = clearLineFunction;
}

void APT_Menu::setLineClearFunction(  clearLineFunctionType clearLineFunction ) {
  this->clearLineFunction = clearLineFunction;
}


APT_Menu::APT_Menu(uint8_t displayLines) {
  Serial.begin(115200);
  this->displayLines              = displayLines;
  APT_MENU_ADD_FULL_UPDATE();  
}

void APT_Menu::setup(APT_MenuItem* firstEntry) {
  this->firstEntry = firstEntry;
  this->entryOfActiveLayer = firstEntry;
}

void APT_Menu::checkForNeededLineUpdates()  {

  // 1. skip if already update needed
  if( bitRead(status,APT_MENU_STATUS_BIT_NEEDFULLUPDATE) ) return;

  // 2. if menu item selected and it is a full menu item, check only this 
  if ( bitRead(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED) ) {
    APT_MenuItem* selectedItem = getNthSiblingOfCurrentLayer( scrollPosition + cursorPosition);
    assert( selectedItem != NULL ); // should never happen
    if( selectedItem->isFullMenuItem(this) ) {
      if( selectedItem->doCallback(this, APT_CLB_DONEEDUPDATE, cursorPosition) ) APT_MENU_ADD_LINE_UPDATE(cursorPosition);
      return;
    }
  } 

  //3. check all menu items
  APT_MenuItem* entry = entryOfActiveLayer;

  // go to correct scroll position
  for (uint8_t i = 0; i < scrollPosition; i++) {
    entry = entry->getYoungerSibling();
    assert(entry!=NULL); // ToDo: any exception handling (should not happen)
  }

  // check all displayed lines
  for (uint8_t i = 0; i < displayLines; i++) {

    if (entry == NULL) return;
    if ( entry->doCallback(this, APT_CLB_DONEEDUPDATE,i) ) APT_MENU_ADD_LINE_UPDATE(i) ; 

    // exit loop if more than one line update is needed and therfor 
    // we have a display update in any case
    if( bitRead(status,APT_MENU_STATUS_BIT_NEEDFULLUPDATE) ) return;
  
    entry = entry->getYoungerSibling();
  }
} // APT_Menu::checkForNeededLineUpdates() 


void APT_Menu::loop() {

  if ( bitRead(configuration, APT_MENU_CONFIG_BIT_MENU_OFF) ) return;

  checkForNeededLineUpdates();

  // cursor update not yet implemented ;-(
  //if(bitRead(status,APT_MENU_STATUS_BIT_NEEDCURSORUPDATE)) APT_MENU_ADD_FULL_UPDATE();
    
  // toDo Optimize later, to not double search ful line update items
  if ( APT_MENU_ANY_UPDATE_NEEDED() ) {
    this->updateMenu();
    bitClear(status, APT_MENU_STATUS_BIT_NEEDFULLUPDATE);
    bitClear(status, APT_MENU_STATUS_BIT_NEEDLINEUPDATE);
    bitClear(status, APT_MENU_STATUS_BIT_NEEDCURSORUPDATE);
	// ToDo: if within menu update and updte would be made necessary? probably not ?
  } 

  return;
}

void APT_Menu::setWrapAround(const bool continuously) {
  bitSet(configuration, APT_MENU_CONFIG_BIT_SCROLL_WRAPAROUND);
  if( continuously) bitSet(  configuration, APT_MENU_CONFIG_BIT_SCROLL_CONTINUOUS);
  else 				bitClear(configuration, APT_MENU_CONFIG_BIT_SCROLL_CONTINUOUS);
}
  
void APT_Menu::setNoWrapAround(void) {
	bitClear(configuration, APT_MENU_CONFIG_BIT_SCROLL_WRAPAROUND);
	bitClear(configuration, APT_MENU_CONFIG_BIT_SCROLL_CONTINUOUS);
}


void APT_Menu::turnOff(const bool doClear) {
  if ( doClear ) clearMenu(); 
  bitSet(configuration, APT_MENU_CONFIG_BIT_MENU_OFF);
}

void APT_Menu::turnOn() {
  bitClear(configuration, APT_MENU_CONFIG_BIT_MENU_OFF);
  APT_MENU_ADD_FULL_UPDATE();  
}

APT_MenuItem* APT_Menu::getNthSiblingOfCurrentLayer(const uint8_t n) {
  return entryOfActiveLayer->getNthSibling(n);
}

uint8_t APT_Menu::getNumberOfEntriesOfCurrentLayer(void) {
  return entryOfActiveLayer->getNumberOfSibings();
}
 
void APT_Menu::goUp() {
  uint8_t entries = getNumberOfEntriesOfCurrentLayer();

  // no scroll if only one entry
  if (entries == 1) return;
  // no scroll if inside a menu item
  if ( bitRead(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED)) return;

  // first menu item in layer
  if ( (cursorPosition + scrollPosition) == 0 ) {
    if ( bitRead(configuration, APT_MENU_CONFIG_BIT_SCROLL_WRAPAROUND) ) {
      // wrap around
      cursorPosition = min(displayLines, entries) - 1;
      scrollPosition = (entries - cursorPosition) - 1;
      APT_MENU_ADD_FULL_UPDATE();
    } else return; // do nothing and no update of menu is needed

  // first cursor position 
  } else if ( cursorPosition == 0 ) {
    scrollPosition--;
    APT_MENU_ADD_FULL_UPDATE();
  } else {
    cursorPosition--;
    APT_MENU_ADD_CURSOR_UPDATE();
  }

  // update the menu
  
}

void APT_Menu::goDown() {
  uint8_t entries = getNumberOfEntriesOfCurrentLayer();

  // no scroll if only one entry
  if (entries == 1) return;
  // no scroll if inside a menu item
  if ( bitRead(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED)) return;

  // last menu item in layer
  if ( (cursorPosition + scrollPosition) == (entries - 1) ) {
    if ( bitRead(configuration, APT_MENU_CONFIG_BIT_SCROLL_WRAPAROUND) ) {
      // wrap around
      cursorPosition = 0;
      scrollPosition = 0;
      APT_MENU_ADD_FULL_UPDATE();
    } else return; // do nothing and no update of menu is needed

  // last cursor position
  } else if ( cursorPosition == (displayLines - 1) ) {
    scrollPosition++;
    APT_MENU_ADD_FULL_UPDATE();
  } else {
    cursorPosition++;
    APT_MENU_ADD_CURSOR_UPDATE();
  }

}

void APT_Menu::goInto() {

  DEBUG_MSG_STR(F("APT: goInto()"));

  // no enter if item is activated already
  if ( bitRead(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED)) return;

  APT_MenuItem* entry = getNthSiblingOfCurrentLayer(cursorPosition + scrollPosition);
  
  if ( entry != NULL) {
    // menu item with childs, go into
    if ( entry->hasChild() ) { 
  	  DEBUG_MSG_STR(F("APT: goInto() - enter with child"));
	
	  //bitSet(status, APT_MENU_STATUS_BIT_NEEDFULLUPDATE );
  	  //entry->doCallback(this, APT_CLB_ONENTER, cursorPosition);
	  
      // check if at least one is not hidden, 
	  // if all are hidden, you cannot enter
	  APT_MenuItem* childEntries = entry->getChild();
	  while ( childEntries != NULL) {
		if ( ! childEntries->isHidden() ) {
		
		    entryOfActiveLayer = entry->getChild();
			cursorPosition = 0;
			scrollPosition = 0;
			APT_MENU_ADD_FULL_UPDATE();
			entry->doCallback(this,APT_CLB_ONENTER, cursorPosition);
			return;
		}
		childEntries = childEntries->getYoungerSibling();
	  }
	  // when reached here, no unhidden siblings present, so 
	  // one cannot enter the sub menu
  	  DEBUG_MSG_STR(F("APT: goInto() - no unhidden child"));
	  return;

    // leaf menu item
    } else {   
   	  DEBUG_MSG_STR(F("APT: goInto() - activating MenuItem"));

      if( entry->doCallback(this,APT_CLB_ONENTER, cursorPosition) ) {
        bitSet(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED);
      } 

      // full update for full menu, else line update
      if( entry->doCallback(this, APT_CLB_ISFULLMENUITEM , cursorPosition) ) {
             APT_MENU_ADD_FULL_UPDATE();
      } else APT_MENU_ADD_LINE_UPDATE(cursorPosition);

    } // if ( entry->hasChild() ) { } else {
  }
  // else ToDo: possible callback ? or not ? or assert ?
}

void APT_Menu::goBack() {

  // if selected menu item, de-select menu item
  if ( bitRead(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED) ) {
	DEBUG_MSG_STR(F("APT: goBack in active Menu"));

    APT_MenuItem* entry = getActiveMenuItem();
    assert( entry != NULL);    
    bitClear(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED); // do thjis only aftetr get active menu item (or change to getCurrentItem :-)
    
    if( entry->doCallback(this, APT_CLB_ISFULLMENUITEM , cursorPosition) ) {
           APT_MENU_ADD_FULL_UPDATE();
    } else APT_MENU_ADD_LINE_UPDATE(cursorPosition);
    entry->doCallback(this, APT_CLB_ONEXIT, cursorPosition);    

  // else go one level down
  } else {
      APT_MenuItem* parentEntry = entryOfActiveLayer->getParent();	  
	  if (parentEntry != NULL) { // NULL if we are in root, so no back available
		
		// callback before going one level down
		getCurrentMenuItem()->doCallback(this, APT_CLB_ONEXIT, cursorPosition);    
        
		// set the new layer and cursor/scroll positions
		entryOfActiveLayer = parentEntry->getOldestSibling();
		uint8_t n = parentEntry->getCountOfOlderSiblings(); // oldest 0
		Serial.print("APT: N: "); Serial.println(n);
		Serial.print("APT: display: "); Serial.println(displayLines);
        cursorPosition = min(n, displayLines - 1);
        scrollPosition = n - cursorPosition;
        bitSet(status, APT_MENU_STATUS_BIT_NEEDFULLUPDATE );

		// ToDo: could also set last scroll position,
		// current implementation scrolls from beginning 
		// and does not save scroll position.
		// Overhead probably to much since for each layer 
		// would need to store scroll position
		
      } // if (entry != NULL) { } else {} do nothing
  }
  // else we are in root, ToDo possible add callback
}

void APT_MenuItem::hide() {
	bitSet(config,APT_MENUITEM_CONFIG_HIDDEN);
}

void APT_MenuItem::show() {
	bitClear(config,APT_MENUITEM_CONFIG_HIDDEN);
}

bool APT_MenuItem::isHidden() {
    //DEBUG_MSG_STRVAL("MenuItem: ",config);
	return bitRead(config,APT_MENUITEM_CONFIG_HIDDEN);
}
			
void APT_Menu::gotoRoot() {
	APT_MenuItem* entry = getCurrentMenuItem();
	while( entry->hasParent() ) {
		goBack();
		entry = getCurrentMenuItem();
	}
};

void APT_Menu::gotoFirstRoot(){	
	gotoRoot();
	APT_MenuItem* entry = getCurrentMenuItem();
	while( entry->hasOlderSibling() ) {
		goUp();
		entry = getCurrentMenuItem();
	}	
}; 


APT_MenuItem* APT_Menu::getMenuItem(APT_MenuItem* firstEntryOfRow, uint8_t id) {
   while(firstEntryOfRow!= NULL) {
	 if( firstEntryOfRow->getID() == id ) return firstEntryOfRow;
	 if( firstEntryOfRow->hasChild() ) {
		APT_MenuItem* foundMenuItem = getMenuItem(firstEntryOfRow->getChild(), id);
		if( foundMenuItem != NULL) {
		   DEBUG_MSG_STRVAL(F("APT: Did find: "),id);
		   return foundMenuItem;
		}
	 }
	 firstEntryOfRow = firstEntryOfRow->getYoungerSibling();
   }
   DEBUG_MSG_STRVAL(F("APT: id not found"),id);
   return NULL;
}

APT_MenuItem* APT_Menu::getMenuItem(uint8_t id) {
   return getMenuItem(firstEntry, id);;
}


#define APT_DONT_SEARCH_YOUNGER_SIBLING   	0
#define APT_DONT_SEARCH_CHILD             	1
#define APT_DONT_SEARCH_PARENT             	2
#define APT_DONT_SEARCH_OLDER_SIBLING      	3

#define APT_FOUND_ON_YOUNGER_SIBLING   		0
#define APT_FOUND_ON_CHILD             		1
#define APT_FOUND_ON_OLDER_SIBLING      	2
#define APT_FOUND_ON_PARENT			      	3
#define APT_NOT_FOUND				      	4
#define APT_FOUND				      		5

#define APT_WAS_FOUND(x)					( x != APT_NOT_FOUND )
#define IS_BIT_SET(a,x) 					( (a&(1<<x)) == (1<<x) ) 
#define ADD_BIT(a,x)    					( a | (1<<x))
#define SET_BIT(x)       					(1<<x)


uint8_t findID(APT_MenuItem* element, uint8_t ID, uint8_t config = 0) {
	uint8_t found;
	
	// checkif we reached element
	if( element->getID() == ID) {
		DEBUG_MSG_STRVAL(F("APT: found element: "), element->getID() );
		return APT_FOUND;
	}		
	
	// check child
	DEBUG_MSG_STRVAL(F("APT: Check child of "), element->getID() );
	if( !IS_BIT_SET(config,APT_DONT_SEARCH_CHILD) && element->hasChild() ) {
		DEBUG_MSG_STRVAL(F("APT: child is "), element->getChild()->getID() );
		found = findID(element->getChild(), ID, SET_BIT(APT_DONT_SEARCH_PARENT));
		if( APT_WAS_FOUND(found) ) {
			DEBUG_MSG_STRVAL(F("APT: found at child of "), element->getID() );
			return APT_FOUND_ON_CHILD;
		}
	}
	
	// check younger sibling	
	DEBUG_MSG_STRVAL(F("APT: Check younger sibling of "), element->getID() );
	if(  !IS_BIT_SET(config,APT_DONT_SEARCH_YOUNGER_SIBLING) && element->hasYoungerSibling() ) {
		DEBUG_MSG_STRVAL(F("APT: younger sibling is "), element->getYoungerSibling()->getID() );
		found = findID(element->getYoungerSibling(), ID, SET_BIT( APT_DONT_SEARCH_OLDER_SIBLING ) | SET_BIT( APT_DONT_SEARCH_PARENT ));
		if( APT_WAS_FOUND(found) ) {
			DEBUG_MSG_STRVAL(F("APT: found at younger sibling of "), element->getID() );
			return APT_FOUND_ON_YOUNGER_SIBLING;
		}	
	}

	// check older sibling	
	DEBUG_MSG_STRVAL(F("APT: Check older sibling and parent of "), element->getID() );
	if( !IS_BIT_SET(config,APT_DONT_SEARCH_OLDER_SIBLING) && element->hasOlderSibling() ) {
		DEBUG_MSG_STRVAL(F("APT: older sibling is "), element->getOlderSibling()->getID() );
		found = findID(element->getOlderSibling(), ID, SET_BIT( APT_DONT_SEARCH_YOUNGER_SIBLING ) | SET_BIT( APT_DONT_SEARCH_PARENT ));
		if( APT_WAS_FOUND(found) ) {
			DEBUG_MSG_STRVAL(F("APT: found at older sibling of "), element->getID() );
			return APT_FOUND_ON_OLDER_SIBLING;
		}	
	// check predecessor (older sibling or parent)
	} else if( !IS_BIT_SET(config,APT_DONT_SEARCH_PARENT) && element->hasParent() ) {
		DEBUG_MSG_STRVAL(F("APT: parent is "), element->getParent()->getID() );
		found = findID(element->getParent(), ID, SET_BIT( APT_DONT_SEARCH_CHILD ));
		if( APT_WAS_FOUND(found) ) {
			DEBUG_MSG_STRVAL(F("APT: found at parent of "), element->getID() );
			return APT_FOUND_ON_PARENT;
		}
	} // else { } // we are at root and do not continue to search
	
	// not found
	return APT_NOT_FOUND;
}

void APT_Menu::gotoMenuItem(APT_MenuItem *item,bool fastSearch) {
	if(item == NULL) return;
	gotoID(item->getID(),fastSearch);
}
    
	
void APT_Menu::gotoID(uint8_t ID, bool fastSearch) {
  // check if ID is within siblings
  
  DEBUG_MSG_STRVAL("APT: Find ID",ID);
  if(fastSearch) {
	DEBUG_MSG_STRVAL("APT: Fast Search not yet implemented : ",ID);
	return;
  }
  
  while (1==1) {
    //
	APT_MenuItem* iterator = entryOfActiveLayer->getNthSibling(scrollPosition + cursorPosition); 
	assert( iterator != NULL );
	DEBUG_MSG_STR("--------------- ");
	DEBUG_MSG_STRVAL("APT: Search from ",iterator->getID());
	uint8_t dir = findID(iterator,ID);
	DEBUG_MSG_STRVAL("APT: Found ",dir);
  
	switch( dir ) {
		case APT_FOUND_ON_YOUNGER_SIBLING:  goDown(); DEBUG_MSG_STR("goDown()"); break;
		case APT_FOUND_ON_CHILD:			goInto(); DEBUG_MSG_STR("goInto()");  break;        
		case APT_FOUND_ON_OLDER_SIBLING:    goUp();   DEBUG_MSG_STR("goUp()"); break;
		case APT_FOUND_ON_PARENT:			goBack(); DEBUG_MSG_STR("goBack()"); break;
		case APT_FOUND:						DEBUG_MSG_STR("reached element"); return;
		default: DEBUG_MSG_STRVAL("APT: element not found",dir); return;
	} // switch( dir ) {
	
  } //  while (1==1)
  
} // void APT_Menu::gotoID(uint8_t ID) {

APT_MenuItem* APT_MenuItem::getParent(void) {
	APT_MenuItem* entry = this;
	while ( entry->pre != NULL) {
		if ( entry->pre->child == entry ) return entry->pre;
		else entry = entry->pre; // set to older sibling and continue search
	}
	return NULL;
}

bool APT_MenuItem::hasParent(void) {
	APT_MenuItem* entry = this;
	while ( entry->pre != NULL) {
		if ( entry->pre->child == entry ) return true;
		else entry = entry->pre; // set to older sibling and continue search
	}
	return false; // root elements
}


bool APT_MenuItem::isOldestSibling()  { 
	if ( ( pre == NULL) || (pre->child == this)) 	return true; 
	else 											return false; 
}; // bool APT_MenuItem::isOldestSibling()  { 

APT_MenuItem* APT_MenuItem::getOldestSibling() {
	APT_MenuItem* entry = this;
	while( entry->pre != NULL) {
		// loop until predecessors child of itself is itself (oldest sibling)
		if ( (entry->pre->child == entry)) 	return entry; 
		else								entry = entry->pre;

	}	
	// root has no predecessor, so return first element
	return entry;
} // APT_MenuItem* APT_MenuItem::getOldestSibling() { 

uint8_t APT_Menu::getCountOfSibling(APT_MenuItem* entry) {
  uint8_t count = 0;
  APT_MenuItem* iterator = entry->getOldestSibling();
  while ( iterator->hasYoungerSibling() ) {
    iterator = iterator->getYoungerSibling();
    count++;
  }
  return count; 
} // uint8_t APT_Menu::getCountOfSibling(APT_MenuItem* entry) {


void APT_Menu::hide(uint8_t ID) {
  hide( getMenuItem(ID) );
} // void APT_Menu::hide(uint8_t ID) {
void APT_Menu::show(uint8_t ID) {
  show( getMenuItem(ID) );
}
bool APT_Menu::isHidden(uint8_t ID) {
  APT_MenuItem* it = getMenuItem(ID);
  if(it != NULL) return it->isHidden();
  else return false;
}

void APT_Menu::hide(APT_MenuItem* menuItem) {
   DEBUG_MSG_STRPTR("APT: hide ",menuItem);
  
   if(menuItem == NULL) return;
   // check if in current menu row and befor cursor position
   APT_MenuItem *iterator = entryOfActiveLayer;
   uint8_t absCursorPosition = cursorPosition + scrollPosition;
   
   while( iterator->getID() != menuItem->getID() ) {

    // not found before current cursor position, just hide it
	if(absCursorPosition == 0) {
		DEBUG_MSG_STR("element ot hide after this");
		menuItem->hide(); 
		forceUpdate();
		// ToDo: check if in this menu, only then force update
		return;
	} else absCursorPosition--;
	
	// not found in current row and row is over,
	// just hide it
	if( ! iterator->hasYoungerSibling() )  {
		DEBUG_MSG_STR("APT: no younger siblings");
		menuItem->hide(); 
		return;
	} else iterator->getYoungerSibling();
	
   }
   DEBUG_MSG_STRVAL("APT: found ID",menuItem->getID());
   
   // if current ID to be hidden and it is actived
   if ( (absCursorPosition == 0) && bitRead(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED) )  {
      DEBUG_MSG_STR("APT: go out of active menu");
	  goBack();
   }
   
   // menu item to be hidden befor or at 
   // cursor position, special handling needed
   DEBUG_MSG_STR("APT: set cursor correctly");
 
   if(cursorPosition >0) cursorPosition--;
   else if (scrollPosition > 0) scrollPosition--;
   else if (! iterator->hasYoungerSibling() ) {
	if ( iterator->hasParent() ) goBack(); // last element on row hidden, go back
	else return;  // do nothing, last element on 
				  // first row cannot be hidden
	
   }

   // now hide the menu item
   menuItem->hide(); 
   forceUpdate();
   return;
} // void APT_Menu::hide(APT_MenuItem* menuItem) {

void APT_Menu::show(APT_MenuItem* menuItem) {
  if(menuItem != NULL && menuItem->isHidden()) {
	menuItem->show();
	forceUpdate();
  } 
  return;
}
bool APT_Menu::isHidden(APT_MenuItem* menuItem) {
  if(menuItem == NULL) return false;
  else 				   return menuItem->isHidden();
}	
	

/********************************************************************/
/**                                                                **/
/********************************************************************/
void APT_Menu::clearMenu() {
 if (clearMenuFunction !=  NULL) (*this->clearMenuFunction)();
}
void APT_Menu::clearLine(uint8_t line) {
   if (this->clearLineFunction != NULL) (*this->clearLineFunction)(line);
}


void APT_Menu::updateMenu() {
  APT_MenuItem* entry = entryOfActiveLayer;

  // full screen menu item selected, show only this
  if ( bitRead(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED) ) {
    APT_MenuItem* selectedItem = getNthSiblingOfCurrentLayer( scrollPosition + cursorPosition);
    assert( selectedItem != NULL ); // should never happen
    if ( selectedItem->isFullMenuItem(this) ) {
      if (this->clearMenuFunction != NULL) (*this->clearMenuFunction)();
      selectedItem->doCallback(this, APT_CLB_ONUPDATE_LINE | APT_CLB_SETACTIVEATED | APT_CLB_SETCURSORON,
                                cursorPosition);
      return;
    } // clears the screen only for full menu items, else
  }
  
 
  // clear full display
  if (  bitRead(status,APT_MENU_STATUS_BIT_NEEDFULLUPDATE ) && 
        this->clearMenuFunction != NULL) (*this->clearMenuFunction)();
  if ( bitRead(status,APT_MENU_STATUS_BIT_NEEDLINEUPDATE)  &&
        this->clearLineFunction != NULL) (*this->clearLineFunction)(selectedLine);
  if ( bitRead(status,APT_MENU_STATUS_BIT_NEEDCURSORUPDATE)  && // toDo: use here dedicated function !!
        this->clearLineFunction != NULL) (*this->clearLineFunction)(selectedLine);


  uint8_t lineCount = 0;
  while (lineCount < scrollPosition) {
    entry = entry->getYoungerSibling();
	if( ! entry->isHidden() ) lineCount++;
    assert( entry!=NULL ); // ToDo: any exception handling (should not happen)
  }

  // display all lines
  lineCount = 0;
  while( lineCount < displayLines) {
    
	// no more entries, exit
	if (entry == NULL) break;
	
	// just skip elements which are not shown
	if( ! entry->isHidden() ) { 
		bool    updateThisLine  = bitRead(status,APT_MENU_STATUS_BIT_NEEDFULLUPDATE) ||
								 ( bitRead(status,APT_MENU_STATUS_BIT_NEEDLINEUPDATE) && (lineCount == selectedLine) );
		uint8_t callbackType    = ( bitRead(status,APT_MENU_STATUS_BIT_NEEDCURSORUPDATE) ? APT_CLB_ONCURSORUPDATE : 
								(updateThisLine ? APT_CLB_ONUPDATE_LINE : APT_CLB_ONUPDATE_OTHERLINE ) );
								
		// show content with menu item function
		if ( entry->hasCallback() ) {

			if (lineCount == cursorPosition) {
				callbackType |= APT_CLB_SETCURSORON;
				if (bitRead(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED) ) callbackType |= APT_CLB_SETACTIVEATED;
			}
			entry->doCallback(this,callbackType, lineCount);
			
		// if MenuItem does not have a callback, used default show function
		} else if (this->defaultShowEntryFunction != NULL) {
			if (lineCount == cursorPosition) {
				callbackType |= APT_CLB_SETCURSORON;
				if (bitRead(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED) ) callbackType |= APT_CLB_SETACTIVEATED;
			}
			(*this->defaultShowEntryFunction) (this, entry, callbackType, lineCount);
		} else {
			assert(false); // there should be at least one of both possibilities
		}
		
		lineCount++;
		
    } // if( ! entry->isHidden() ) { 
	
    // next entry
    entry = entry->getYoungerSibling();
    
  } // for(uint8_t i=0; i<displayLines;i++) {

}

APT_MenuItem* APT_Menu::getCurrentMenuItem(void) {
    return getNthSiblingOfCurrentLayer( scrollPosition + cursorPosition );
}

uint8_t APT_Menu::getCurrentMenuItemID(void) {
		return getCurrentMenuItem()->getID();
}
	
    
	
APT_MenuItem* APT_Menu::getActiveMenuItem(void) {
  if (bitRead(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED) )  {
    return getCurrentMenuItem();
  } else return NULL;
}

void APT_Menu::forceUpdate(const bool onlyCurrentLine) {
  if(onlyCurrentLine) {
    APT_MENU_ADD_LINE_UPDATE(cursorPosition);
  } else {               APT_MENU_ADD_FULL_UPDATE();  }
}

void APT_Menu::forceUpdate(APT_MenuItem* item) {
  if(item==NULL) return;

  APT_MenuItem* entry = entryOfActiveLayer;

  // go to correct scroll position
  for (uint8_t i = 0; i < scrollPosition; i++) {
    entry = entry->getYoungerSibling();
    assert(entry!=NULL); // ToDo: any exception handling (should not happen)
  }

  // check all displayed lines
  for (uint8_t i = 0; i < displayLines; i++) {

    if (entry == NULL) return;
    if (item==entry ) { APT_MENU_ADD_LINE_UPDATE(i);  return;} 
  
    entry = entry->getYoungerSibling();
  }
}; // APT_Menu::forceUpdate(APT_MenuItem* item)


APT_MenuItem* 	APT_Menu::getMenuItemInLine(uint8_t line) {
	// ToDo: if line is after last elements !! 
	return getNthSiblingOfCurrentLayer( scrollPosition + line );
}

uint8_t APT_Menu::getMenuItemIDInLine(uint8_t line){
	return getMenuItemInLine(line)->getID();
}



// returns 0 if no scrollbar character should be shown
// returns 1 if scrollbar character without the arrow
// returns 2..(2+scrollbarSymbolCount) for the scrolbar item to be shown

// ToDO:
// returns 0..scrollbarSymbolCount for the scrollbar item to be shown
// returns scrollbarSymbolCount+1  if scrollbar character without the arrow
// returns 255 if no scrollbar character should be shown

uint8_t APT_Menu::getScrollbarItem(const uint8_t atLine) {
    uint8_t entries = getNumberOfEntriesOfCurrentLayer();
    
    // no scrolbar needed
    if(entries <= displayLines) return 0;     // ToDo return ID of empty fild, no ASIC Code !!
    
    uint8_t scrollbarSymbolCount = 3;

    // todo later to get a more homogenious distribnutibn 
    // so many possibilities are there displayLines * scrollbarSymbolCount
    // float scrollSignPosition = (float) absolutePosition * (displayLines-1) / (entries-1);


    // entries >=0 and therefor here displayLines > 0
    uint8_t absolutePosition = scrollPosition + cursorPosition ;
    float scrollSignPosition = (float) absolutePosition * (displayLines-1) / (entries-1);
    uint8_t lineWithMarker = (uint8_t) round( scrollSignPosition ) ; 
    uint8_t markerHeight =  uint8_t round(  ( -1. * lineWithMarker  + scrollSignPosition + 0.5) * (scrollbarSymbolCount-1)) + 2;

    if(1==2) {
      Serial.print(atLine);Serial.print('|');
      Serial.print(entries);Serial.print('|');
      Serial.print(absolutePosition);Serial.print('|');
      Serial.print(scrollSignPosition);Serial.print('|');
      Serial.print(lineWithMarker);Serial.print('|');
      Serial.print(markerHeight);Serial.print('|');
    }
    
    // checks, should never happen according to above formulas
    //assert(lineWithMarker<displayLines);
    //assert(markerHeight<scrollbarSymbolCount);
    
    if (lineWithMarker == atLine) {
      if ( absolutePosition == (entries-1))  return scrollbarSymbolCount+1;
      else if ( absolutePosition == 0)       return 2;
      else                                   return markerHeight;
    } else                                   return 1; 
  
}
//        uint8_t n_max             = (LCDML.MENU_getChilds() >= _LCDML_DISP_rows) ? _LCDML_DISP_rows : (LCDML.MENU_getChilds());
//    uint8_t scrollbar_min     = 0;
//    uint8_t scrollbar_max     = LCDML.MENU_getChilds();
//    uint8_t scrollbar_cur_pos = LCDML.MENU_getCursorPosAbs();
//    uint8_t scroll_pos        = ((1.*n_max * _LCDML_DISP_rows) / (scrollbar_max - 1) * scrollbar_cur_pos);
//         
//        //set scroll position
//        if (scrollbar_cur_pos == scrollbar_min) {
//          // min pos
//          lcd.setCursor((_LCDML_DISP_cols - 1), 0);
//          lcd.write((uint8_t)1);
//        } else if (scrollbar_cur_pos == (scrollbar_max - 1)) {
//          // max pos
//          lcd.setCursor((_LCDML_DISP_cols - 1), (n_max - 1));
//          lcd.write((uint8_t)4);
//        } else {
//          // between
//          lcd.setCursor((_LCDML_DISP_cols - 1), scroll_pos / n_max);
//          lcd.write((uint8_t)(scroll_pos % n_max) + 1);

bool APT_Menu::isAMenuItemActivated(void) { 
  return bitRead(status,APT_MENU_STATUS_BIT_MENUITEMSELECTED ); 
};
        
/***************************************************************************/
/* APT_MenuItem class                                                      */
/***************************************************************************/

APT_MenuItem::APT_MenuItem()
 : id(0), contentPointer(NULL) {
}

APT_MenuItem::APT_MenuItem(const uint8_t id, const char * const contentPointer) 
 : id(id), contentPointer(contentPointer) {
  //Serial.begin(115200);
  //APT_DEBUG_STRVAL(F("APT: creating ID:"),id);  
};

APT_MenuItem::APT_MenuItem( const uint8_t id,
                            const char * const contentPointer,
                            clbFunctionType clbFunction )
  : APT_MenuItem(id, contentPointer) {
  setCallback(clbFunction);
}

void APT_MenuItem::getContent(char* contentBuffer) {
  // get label from flash
  strcpy_P(contentBuffer, contentPointer );
}

void APT_MenuItem::setSibling(APT_MenuItem* sibling) {
  this->sibling = sibling;
};

bool APT_MenuItem::hasYoungerSibling() { 
	return sibling != NULL; 
}; 
APT_MenuItem*  APT_MenuItem::getYoungerSibling() { 
	return sibling; 
};

bool APT_MenuItem::hasOlderSibling() { 
	if( pre == NULL) return false; // root element
	if( pre->getChild() == this ) return false; // oldest sibling
	return true;
}; 

APT_MenuItem* APT_MenuItem::getOlderSibling() { 
	if( pre == NULL) return NULL; // root element
	if( pre->getChild() == this ) return NULL; // oldest sibling
	return pre;
};

void APT_MenuItem::addSibling(APT_MenuItem* sibling) {
  // onself cannot be a sibling :-)
  // NULL cannot be added
  if ( this == sibling ||sibling == NULL) return;
  
  // look for the next free sibling space
  APT_MenuItem* iterator = this;
  while ( iterator->sibling != NULL) {
	iterator = iterator->sibling;
  }
  // now add sibling
  iterator->sibling = sibling;
  sibling->pre = this;
} // void APT_MenuItem::addSibling(APT_MenuItem* sibling) {

uint8_t APT_MenuItem::getCountOfOlderSiblings(void) {
  uint8_t siblingsCount = 0;
  
  // count younger siblings
  APT_MenuItem* entry = this;
  while ( entry->hasOlderSibling() ) {
    entry = entry->getOlderSibling();
    siblingsCount++;
  }
  
  return siblingsCount;
} // uint8_t APT_MenuItem::getCountOfOlderSiblings(void) {   
    
	
uint8_t APT_MenuItem::getNumberOfSibings(void) {
  uint8_t siblingsCount = 0;
  if( ! this->isHidden() ) siblingsCount++;
  
  // count younger siblings
  APT_MenuItem* entry = this;
  while ( entry->hasYoungerSibling() ) {
    entry = entry->getYoungerSibling();
    if( ! entry->isHidden() ) siblingsCount++;
  }
  // count older siblings
  entry = this;
  while ( entry->hasOlderSibling() ) {
    entry = entry->getOlderSibling();
    if( ! entry->isHidden() ) siblingsCount++;
  }
  
  return siblingsCount;
}

APT_MenuItem* APT_MenuItem::getNthSibling(const uint8_t n) {
  APT_MenuItem* entry = getOldestSibling();
  uint8_t countMenuEntry = 0;
  while (countMenuEntry < n || entry->isHidden() ) {
    if ( entry == NULL ) break;
	if( ! entry->isHidden() ) countMenuEntry++;
    entry = entry->getYoungerSibling();
  }
  return entry;
}

void APT_MenuItem::addChild(APT_MenuItem* child) {

  if ( this == child || child == NULL)  return;
    // onself cannot be a child :-)
	// NULL cannot be added as child
	
  if (this->child == NULL) {
    this->child = child;
    child->pre = this;
  } else {
    this->child->addSibling(child);
  }
};

void APT_MenuItem::setCallback(  clbFunctionType clbFunction ) {
  this->clbFunction = clbFunction;
}

bool APT_MenuItem::doCallback(APT_Menu* menu, const uint8_t callbackType,
                               const uint8_t line) {
  if (clbFunction != NULL) return (*clbFunction)(menu, this, callbackType, line);
  else return false;
};

bool APT_MenuItem::isFullMenuItem(APT_Menu* menu) {
  if (this->clbFunction == NULL) return false;
  else return (*clbFunction)(menu, this, APT_CLB_ISFULLMENUITEM, 0);
};


/***************************************************************************/
/* end                                                                     */
/***************************************************************************/
