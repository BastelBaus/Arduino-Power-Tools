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



/***************************************************************************/
/* DefaultSerialMenu Functions                                             */
/***************************************************************************/

  
void APT_DefaultSerialMenu_clearLine(uint8_t line) {
  for(uint8_t i=0;i<50;i++) Serial.println("");
  Serial.println("------- MenuL -------");
}

void APT_DefaultSerialMenu_clearMenu() {
  for(uint8_t i=0;i<50;i++) Serial.println("");
  Serial.println("------- MenuM -------");
}

char* scrollbarSerialCharacters[] = {" ", "║","╗","╣","╝"};


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
    Serial.print((uint16_t) entry->getSibling(), HEX );
    if (entry->getSibling() != NULL) {
      Serial.print("(");
      Serial.print(entry->getSibling()->getID());
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


/***************************************************************************/
/* DefaultLCDMenu Functions                                                */
/***************************************************************************/

#include <LiquidCrystal_I2C.h>       // Library for I2C LCD

LiquidCrystal_I2C   lcd(0x27, 20, 4); // Set address 0x27 (default) 20 x 4 character LCD

char scrollbarLCDCharacters[4][8] = {
    {B10001, B10001, B10001, B10001, B10001, B10001, B10001, B10001}, // none
    {B11111, B11111, B10001, B10001, B10001, B10001, B10001, B10001}, // upper
    {B10001, B10001, B10001, B11111, B11111, B10001, B10001, B10001}, // mid
    {B10001, B10001, B10001, B10001, B10001, B10001, B11111, B11111}  // lower
};


void APT_DefaultLCDMenu_init() {
  lcd.init();
  lcd.clear();
  lcd.backlight();
  
  lcd.createChar(0, (uint8_t*)scrollbarLCDCharacters[0]);
  lcd.createChar(1, (uint8_t*)scrollbarLCDCharacters[1]);
  lcd.createChar(2, (uint8_t*)scrollbarLCDCharacters[2]);
  lcd.createChar(3, (uint8_t*)scrollbarLCDCharacters[3]);
}

void APT_DefaultLCDMenu_clear() {
  lcd.clear();
}

void APT_DefaultLCDMenu_showMenuItem(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line) {
  // print cursor
  lcd.setCursor(0, line);
  lcd.print( APT_GET_DEFAULT_CURSOR( APT_CLB_ISCURSORON(callbackType), APT_CLB_ISACTIVEATED(callbackType) ) );
  
  // print content
  char buffer[20]; entry->getContent(buffer);
  lcd.print(buffer);

  // print scrollbar
  uint8_t scrollbarItem = menu->getScrollbarItem(line);
  lcd.setCursor(19,line); 
  if(scrollbarItem!=0) lcd.write(scrollbarItem-1) ;
}

/***************************************************************************/
/* APT_Menu class                                                          */
/***************************************************************************/

bool APT_Menu::needDisplayUpdate(void) {
  return bitRead(status, APT_MENU_STATUS_BIT_NEEDFULLUPDATE) ||
         bitRead(status, APT_MENU_STATUS_BIT_NEEDLINEUPDATE); // toDo add cursor update
}

void APT_Menu::setDefaultShowEntryFunction( defaultShowEntryFunctionType defaultShowEntryFunction) {
  this->defaultShowEntryFunction  = defaultShowEntryFunction;
}
void APT_Menu::callDefaultShowEntryFunction( APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line) {
  (* (this->defaultShowEntryFunction) )(this, entry, callbackType, line);
}
void APT_Menu::setClearFunction( void (*clearMenuFunction)(void), 
                       void (*clearLineFunction)(uint8_t line)) {
  this->clearLineFunction = clearLineFunction;
  this->clearMenuFunction = clearMenuFunction;
}

void APT_Menu::setLineClearFunction( void (*clearLineFunction)(uint8_t line) ) {
  this->clearLineFunction = clearLineFunction;
}


APT_Menu::APT_Menu(uint8_t displayLines) {
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
    entry = entry->getSibling();
    assert(entry!=NULL); // ToDo: any exception handling (should not happen)
  }

  // check all displayed lines
  for (uint8_t i = 0; i < displayLines; i++) {

    if (entry == NULL) return;
    if ( entry->doCallback(this, APT_CLB_DONEEDUPDATE,i) ) APT_MENU_ADD_LINE_UPDATE(i) ; 

    // exit loop if more than one line update is needed and therfor 
    // we have a display update in any case
    if( bitRead(status,APT_MENU_STATUS_BIT_NEEDFULLUPDATE) ) return;
  
    entry = entry->getSibling();
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
  } 

  return;
}

void APT_Menu::setWrapAround(bool wrapAround) {
  (wrapAround ? bitSet(configuration, APT_MENU_CONFIG_BIT_WRAP_AROUND) : bitClear(configuration, APT_MENU_CONFIG_BIT_WRAP_AROUND));
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
  return entryOfActiveLayer->getSibingsCount();
}
 
void APT_Menu::goUp() {
  uint8_t entries = getNumberOfEntriesOfCurrentLayer();

  // no scroll if only one entry
  if (entries == 1) return;
  // no scroll if inside a menu item
  if ( bitRead(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED)) return;

  // first menu item in layer
  if ( (cursorPosition + scrollPosition) == 0 ) {
    if ( bitRead(configuration, APT_MENU_CONFIG_BIT_WRAP_AROUND) ) {
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
    if ( bitRead(configuration, APT_MENU_CONFIG_BIT_WRAP_AROUND) ) {
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

  // no enter if item is activated already
  if ( bitRead(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED)) return;

  APT_MenuItem* entry = getNthSiblingOfCurrentLayer(cursorPosition + scrollPosition);

  if ( entry != NULL) {
    // menu item with childs, go into
    if ( entry->hasChild() ) { 
      entryOfActiveLayer = entry->getChild();
      cursorPosition = 0;
      scrollPosition = 0;
      bitSet(status, APT_MENU_STATUS_BIT_NEEDFULLUPDATE );

    // leaf menu item
    } else {                  
      if( entry->doCallback(this,APT_CLB_ONENTER, cursorPosition) ) {
        bitSet(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED);
      } 
      // full update for full menu, else line update
      if( entry->doCallback(this, APT_CLB_ISFULLMENUITEM , cursorPosition) ) {
             APT_MENU_ADD_FULL_UPDATE();
      } else APT_MENU_ADD_LINE_UPDATE(cursorPosition);

    }
  }
  // else ToDo: possible callback ? or not ? or assert ?
}

void APT_Menu::goBack() {

  // if selected menu item, de-select menu item
  if ( bitRead(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED) ) {

    APT_MenuItem* entry = getActiveMenuItem();
    assert( entry != NULL);    
    bitClear(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED); // do thjis only aftetr get active menu item (or change to getCurrentItem :-)
    
    if( entry->doCallback(this, APT_CLB_ISFULLMENUITEM , cursorPosition) ) {
           APT_MENU_ADD_FULL_UPDATE();
    } else APT_MENU_ADD_LINE_UPDATE(cursorPosition);
    entry->doCallback(this, APT_CLB_ONEXIT, cursorPosition);    

  // else go one level down
  } else {
      APT_MenuItem* entry = entryOfActiveLayer->getParent();
      if (entry != NULL) { // NULL if we are in root, so no back available
        entryOfActiveLayer = getFirstOfSiblings(entry);
        uint8_t n = getCountOfSibling(entry);
        // ToDo: restore last scroll position
        cursorPosition = min(n, displayLines - 1);
        scrollPosition = n - cursorPosition;
        bitSet(status, APT_MENU_STATUS_BIT_NEEDFULLUPDATE );
        entry->doCallback(this, APT_CLB_ONEXIT, cursorPosition);    
      } // if (entry != NULL) {
  }
  // else we are in root, ToDo possible add callback
}


void APT_Menu::gotoRoot() {
}

uint8_t APT_Menu::getCountOfSibling(APT_MenuItem* entry) {
  APT_MenuItem* iterator = getFirstOfSiblings(entry);
  uint8_t count = 0;
  while ( iterator != entry) {
    iterator = iterator->getSibling();
    count++;
  }
  return count; // will never reach here
  // ToDo: no exceptions like NULL pointer since this function itself cannot fail (hopefully ;-)
}

APT_MenuItem* APT_Menu::getFirstOfSiblings(APT_MenuItem* sib) {
  APT_MenuItem* parent = sib->getParent();
  if ( parent == NULL) return firstEntry;
  else                return parent->getChild();
}

void APT_Menu::clearMenu() {
 if (clearMenuFunction !=  NULL) (*this->clearMenuFunction)();
}
void APT_Menu::clearLine(uint8_t line) {
   if (this->clearLineFunction != NULL) (*this->clearLineFunction)(line);
}


void APT_Menu::updateMenu() {
  APT_MenuItem* entry = entryOfActiveLayer;
  APT_MenuItem* selectedItem = getNthSiblingOfCurrentLayer( scrollPosition + cursorPosition);

  assert( selectedItem != NULL ); // should never happen

  // full screen menu item selected, show only this
  if ( bitRead(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED) ) {
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


  for (uint8_t i = 0; i < scrollPosition; i++) {
    entry = entry->getSibling();
    assert(entry!=NULL); // ToDo: any exception handling (should not happen)
  }

  // display all lines
  for (uint8_t i = 0; i < displayLines; i++) {
    if (entry == NULL) break;
    bool    updateThisLine  = bitRead(status,APT_MENU_STATUS_BIT_NEEDFULLUPDATE) ||
                              ( bitRead(status,APT_MENU_STATUS_BIT_NEEDLINEUPDATE) && (i == selectedLine) );
    uint8_t callbackType    = ( bitRead(status,APT_MENU_STATUS_BIT_NEEDCURSORUPDATE) ? APT_CLB_ONCURSORUPDATE : 
                               (updateThisLine ? APT_CLB_ONUPDATE_LINE : APT_CLB_ONUPDATE_OTHERLINE ) );

    // show content with menu item function
    if ( entry->hasCallback() ) {
      
      if (i == cursorPosition) {
        callbackType |= APT_CLB_SETCURSORON;
        if (bitRead(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED) ) callbackType |= APT_CLB_SETACTIVEATED;
      }
      entry->doCallback(this,callbackType, i);

    // if MenuItem does not have a callback, used default show function
    } else if (this->defaultShowEntryFunction != NULL) {
      if (i == cursorPosition) {
        callbackType |= APT_CLB_SETCURSORON;
        if (bitRead(status, APT_MENU_STATUS_BIT_MENUITEMSELECTED) ) callbackType |= APT_CLB_SETACTIVEATED;
      }
      (*this->defaultShowEntryFunction) (this, entry, callbackType, i);
    } else {
      assert(false); // there should be at least one of both possibilities
    }
    
    // next entry
    entry = entry->getSibling();
    
  } // for(uint8_t i=0; i<displayLines;i++) {

}

APT_MenuItem* APT_Menu::getCurrentMenuItem(void) {
    return getNthSiblingOfCurrentLayer( scrollPosition + cursorPosition );
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
    entry = entry->getSibling();
    assert(entry!=NULL); // ToDo: any exception handling (should not happen)
  }

  // check all displayed lines
  for (uint8_t i = 0; i < displayLines; i++) {

    if (entry == NULL) return;
    if (item==entry ) { APT_MENU_ADD_LINE_UPDATE(i);  return;} 
  
    entry = entry->getSibling();
  }
}; // APT_Menu::forceUpdate(APT_MenuItem* item)





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

APT_MenuItem::APT_MenuItem(uint8_t id, const char * contentPointer ) {
  this->id = id;
  this->contentPointer = contentPointer;
};

APT_MenuItem::APT_MenuItem( uint8_t id,
                            const char * contentPointer,
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

void APT_MenuItem::addSibling(APT_MenuItem* sibling) {
  if ( this == sibling) {
    // ToDo: Handling? --> do nothing, onself cannot be a sibling :-)
  } else if (this->sibling == NULL) {
    this->setSibling(sibling);
  } else {
    // look for the next free sibling space
    APT_MenuItem* tmp = this->sibling;
    do {
      if ( tmp->hasSibling() ) tmp = tmp->getSibling();
      else {
        tmp->setSibling(sibling);
        return;
      }
    } while (1 == 1);
  }
}

uint8_t APT_MenuItem::getSibingsCount(void) {
  uint8_t siblingsCount = 1;
  APT_MenuItem* entry = this;
  while ( entry->hasSibling() ) {
    entry = entry->getSibling();
    siblingsCount++;
  }
  return siblingsCount;
}

APT_MenuItem* APT_MenuItem::getNthSibling(const uint8_t n) {
  APT_MenuItem* entry = this;
  for (uint8_t i = 0; i < n; i++) {
    entry = entry->getSibling();
    if ( entry == NULL) return NULL;
  }
  return entry;
}

void APT_MenuItem::addChild(APT_MenuItem* child) {
  if ( this == child) {
    // ToDo: Handling? --> do nothing, onself cannot be a child :-)
  } else if (this->child == NULL) {
    this->child = child;
    child->setParent(this);
  } else {
    this->child->addSibling(child);
    child->setParent(this);
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
