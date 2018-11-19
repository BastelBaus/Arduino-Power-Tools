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

#ifndef __APT_MENU_H__
#define __APT_MENU_H__


// APT_MENUITEM_ADD(id,parent,content) 
// add menu item, 0 is root, start ID with 1 and use consequitive number up to 255
// 

// docu:
// uses millis for timing operations
// cursor and scroll get away from 20 length

// todo: 
// Prio 1
// - functionality hide 
// - implement wrap around unequal to jump around :-)
// - scroll to end of menu, call function or give info to allow beep or other
// - set default confguration and document
// Prio 2:
// - null pointers if setup is not called ! // check all pointer access with ASERTS
// - use ASERT Exception Handling
// - change getChild to getFirstChild()
// - check for buffer 20 (19 elemens ?)
// - REPEAT_ADDCHILD_8 extend for 255 entries :-) --> document noo limit :-)
// - screen saver
// - default Back item :-)
// - make functions static inside class

// document if more than one line needs update, alwways the full display is updated

#include <Arduino.h>

 
#define REPEAT_ADDCHILD_11  REPEAT_ADDCHILD_10  APT_MENU_ADDCHILD_11_func(); 
#define REPEAT_ADDCHILD_10  REPEAT_ADDCHILD_9   APT_MENU_ADDCHILD_10_func(); 
#define REPEAT_ADDCHILD_9   REPEAT_ADDCHILD_8   APT_MENU_ADDCHILD_9_func(); 
#define REPEAT_ADDCHILD_8   REPEAT_ADDCHILD_7   APT_MENU_ADDCHILD_8_func(); 
#define REPEAT_ADDCHILD_7   REPEAT_ADDCHILD_6   APT_MENU_ADDCHILD_7_func(); 
#define REPEAT_ADDCHILD_6   REPEAT_ADDCHILD_5   APT_MENU_ADDCHILD_6_func(); 
#define REPEAT_ADDCHILD_5   REPEAT_ADDCHILD_4   APT_MENU_ADDCHILD_5_func(); 
#define REPEAT_ADDCHILD_4   REPEAT_ADDCHILD_3   APT_MENU_ADDCHILD_4_func(); 
#define REPEAT_ADDCHILD_3   REPEAT_ADDCHILD_2   APT_MENU_ADDCHILD_3_func(); 
#define REPEAT_ADDCHILD_2   REPEAT_ADDCHILD_1   APT_MENU_ADDCHILD_2_func(); 
#define REPEAT_ADDCHILD_1   APT_MENU_ADDCHILD_1_func();

#define APT_MENU_REPEAT_ADDCHILD(N) REPEAT_ADDCHILD_##N


#define APT_GET_DEFAULT_CURSOR(isCursor,isActive) (isCursor?(isActive?'*':'>'):' ')


class APT_Menu;     // the class holding all menu functions 
class APT_MenuItem; // the class for each menu item 


#ifdef APT_DEFAULT_SERIEL_MENU_HANDLING_ROUTINES

// ToDo Docu
extern char* scrollbarSerialCharacters[];

  // as clear just outputs a new header
  void APT_DefaultSerialMenu_clearLine(uint8_t line);
  void APT_DefaultSerialMenu_clearMenu();
  
  // prints the menu entry name
  // callbackType:    uses the same encoding as in the main 
  //                  function to encode if cursor is on line 
  //                  or item is selected
  void APT_DefaultSerialMenu_showMenuItem(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line);

  
#endif

#ifdef APT_DEFAULT_LCD_MENU_HANDLING_ROUTINES

  // initialization
  void APT_DefaultLCDMenu_init();
  // as clear just outputs a new header
  void APT_DefaultLCDMenu_clear();
  // prints the menu entry name
  void APT_DefaultLCDMenu_showMenuItem(APT_Menu* menu,APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line);
  
#endif



// Setup function of menu.
// menuVariable:   the variable which created teh menu
// menuItemCount:  count of MenuItem elements
#define APT_MENU_SETUP(menuVariable,menuItemCount) { menuVariable.setup(&APT_menuItem_1_class); APT_MENU_REPEAT_ADDCHILD(menuItemCount); }

typedef void (*clearMenuFunctionType)(void);
typedef void (*clearLineFunctionType)(const uint8_t line);
typedef void (*defaultShowEntryFunctionType)(APT_Menu* menu, APT_MenuItem* entry,const uint8_t callbackType, const uint8_t line) ;

class APT_Menu {
  public:

    //
    // displayLines:              number of lines which can be displayed at once
    // displayColumns:            number of columns including pointer and scrollbar if selected
    // Important: This should not be used directly. The Macro "APT_MENU_SETUP(menuName,elementCount)" 
    //   must be used to initialize all variables.
    APT_Menu(uint8_t displayLines);

    // install the default handler function for menu entries
    // entry:     pointer to MenuEntry class which should be displayed
    // line:      number of line for this entry starting from 0
    // cursor:    true if the curser is at this line 
    void setDefaultShowEntryFunction(defaultShowEntryFunctionType defaultShowEntryFunction);
    //
    void callDefaultShowEntryFunction( APT_MenuItem* entry,const uint8_t callbackType, const uint8_t line);
    
    // Pointer to the functions which are called if menu should be cleared 
    // Seperate functions for full menu or a line of the menu can be specified.
    // If a function is not specified (i.e. NULL), it is not used.
    void setClearFunction( clearMenuFunctionType clearMenuFunction, clearLineFunctionType clearLineFunction = NULL);
    void setLineClearFunction( clearLineFunctionType clearLineFunction);
    
    // use these functions to navigate 
    // through your menu items. If one is within a MenuItem, 
    // the goUp/goDown will not change, the only way  to leaf 
    // is the goBack() function
    void goUp();
    void goDown();
    void goInto();
    void goBack();
    void gotoRoot();
    void gotoID(uint8_t ID);
    void gotoMenuItem(APT_MenuItem *item);
    bool isAMenuItemActivated(void);


    // Turn off or on the menu. By turing off, you can choose
    // if the screen is cleared (by default on). Turing on will
    // clear and fully re-write the menu. At initialization, the 
    // menu is on.
    void turnOff(const bool doClear = true);
    void turnOn();

    // Configure the menu scroll overflow behavior. 
    // wrap arround: After the last MenuItem the first 
    //               one comes again and wise versa 
    //               at the beginning (default behavior).
    // jump back:    After the first MenuItem, the cursor 
    //               jumps back to the first item in the 
    //               first display line and wise versa 
    //               at the beginning.
    // None:         Menu end scroll at the beginning and end
    //               of the menu entires. 
    void setWrapAround(const bool continuously=false);
    void setNoWrapAround(void);
	
    //uint8_t getCurrentID();
    //APT_MenuItem* getCurrentMenuItem();

    // Returns pointer to current MenuItem (where cursor 
    // is located
    APT_MenuItem* getCurrentMenuItem(void);
    // Returns pointer to active MenuItem. If no 
    // Menu Item is active, NULL is returned
    APT_MenuItem* getActiveMenuItem(void);
    
    // Force an update of the current MenuItem 
    // or the complete menu
    void forceUpdate(const bool onlyCurrentLine = false);
    // force an update of the given MenuItem
    void forceUpdate(APT_MenuItem* item);

    uint8_t getScrollbarItem(const uint8_t atLine);

    // will check for updates and display the menu
    // call this in your main loop
    void loop();
    
    // call this once at startup to setup the menu
    void setup(APT_MenuItem* firstEntry);

    private:
      uint8_t cursorPosition = 0;  // postition of cusor in current menu
      uint8_t scrollPosition = 0;  // current scroll position
      
      uint8_t displayLines   = 0;  // number of lines in menu
      
      APT_MenuItem* firstEntry = NULL;          // this is basically root
      APT_MenuItem* entryOfActiveLayer = NULL;  // first entry of current layer

      // The three function pointers which could be used to customize 
      // the menu behavior
      clearMenuFunctionType         clearMenuFunction = NULL;
      clearLineFunctionType         clearLineFunction = NULL;
      defaultShowEntryFunctionType  defaultShowEntryFunction = NULL;

      uint8_t configuration;
      #define APT_MENU_CONFIG_BIT_SCROLLBAR            0    // 
      #define APT_MENU_CONFIG_BIT_MENU_OFF             1    // If activated, turn menu off.
      #define APT_MENU_CONFIG_BIT_SCROLL_WRAPAROUND    2	// If activated, menu will start from 
	                                                        // first entry in first menu line if 
															// goDown() is called.
      #define APT_MENU_CONFIG_BIT_SCROLL_CONTINUOUS    3	// Only when APT_MENU_CONFIG_BIT_SCROLL_WRAPAROUND
															// is activated: cursor will not jump at 
                                                            // position 0 but menu will continuously be 
															// appended at the end.
      #define APT_MENU_CONFIG_BIT_AUTOBACKITEM         4    // A
      
      uint8_t status;
      #define APT_MENU_STATUS_BIT_NEEDFULLUPDATE       0
      #define APT_MENU_STATUS_BIT_NEEDLINEUPDATE       1
      #define APT_MENU_STATUS_BIT_NEEDCURSORUPDATE     2
      #define APT_MENU_STATUS_BIT_MENUITEMSELECTED     3
      
      uint8_t selectedLine;

      // Adds a line update if no full update is scheduled. 
      // Since only one line can be updated at the time, 
      // if the second line would need update, full display. 
      // If already a full update is scheduled, no extra line update is needed
      #define APT_MENU_ADD_LINE_UPDATE(line)    { if( bitRead(status, APT_MENU_STATUS_BIT_NEEDFULLUPDATE ) )  {} \
                                                else if( bitRead(status,APT_MENU_STATUS_BIT_NEEDLINEUPDATE) ) \
                                                { bitSet(status, APT_MENU_STATUS_BIT_NEEDFULLUPDATE ); bitClear(status,APT_MENU_STATUS_BIT_NEEDLINEUPDATE); }  \
                                                else { bitSet(status,APT_MENU_STATUS_BIT_NEEDLINEUPDATE); selectedLine = line; } }
                                                
      // Adds a full menu update and clears a possible scheduled line update
      #define APT_MENU_ADD_FULL_UPDATE()        { bitSet(status, APT_MENU_STATUS_BIT_NEEDFULLUPDATE ); bitClear(status,APT_MENU_STATUS_BIT_NEEDLINEUPDATE);  }

      // ToDO handle CUrsor Update
      #define APT_MENU_ANY_UPDATE_NEEDED()      ( bitRead(status, APT_MENU_STATUS_BIT_NEEDFULLUPDATE ) || bitRead(status, APT_MENU_STATUS_BIT_NEEDLINEUPDATE ) || bitRead(status, APT_MENU_STATUS_BIT_NEEDCURSORUPDATE ) )
      #define APT_MENU_ADD_CURSOR_UPDATE()      bitSet(status, APT_MENU_STATUS_BIT_NEEDCURSORUPDATE ) 
      
      // Shows the updated menu
      void updateMenu();
      
      void clearMenu(void);
      void clearLine(uint8_t line);

  private:
      
      // count the number of entries in current layer
      uint8_t       getNumberOfEntriesOfCurrentLayer(void);
      // returns pointer to current entry
      APT_MenuItem* getCurrentEntry();
      // returns the n-th sibling of current layer
      APT_MenuItem* getNthSiblingOfCurrentLayer(const uint8_t n);
      // find the first sibling and return pointer to it
      APT_MenuItem* getFirstOfSiblings(APT_MenuItem* sib);
      // return n where entry is the n-th sibling starting at 0 for the first sibling.
      uint8_t getCountOfSibling(APT_MenuItem* entry);
    
      // checks if a full or line update is needed
      bool needDisplayUpdate(void);
      
      // Checks if any shown line needs an update and sets the internal 
      // flags accordingly. Optimized if already a display update is needed, 
      // no further search is performed
      void checkForNeededLineUpdates();
      // check if i-th shown ToDO
      bool doShownMenuItemsNeedUpdate(uint8_t element);
      
        
};



        
#define APT_menuItem_0_class  APT_menuItem_1_class      // just to avoid compiler errors, will never be called

#define APT_MENUITEM_ADDLEAF(id,parent,content,clbFunction) \
        const char APT_MENUITEM_ENRY ## id [] PROGMEM = { content }; \
        APT_MenuItem APT_menuItem_ ## id ## _class(id, (char* )(&APT_MENUITEM_ENRY ## id), clbFunction); \
        inline void APT_MENU_ADDCHILD_ ## id ## _func() { if (id!=1) { (parent==0 ? \
                APT_menuItem_1_class.addSibling(& APT_menuItem_ ## id ## _class ) : \
                APT_menuItem_ ## parent ## _class.addChild(& APT_menuItem_ ## id ## _class ) ); } }; 

#define APT_MENUITEM_ADD(id,parent,content) \
        const char APT_MENUITEM_ENRY ## id [] PROGMEM = { content }; \
        APT_MenuItem APT_menuItem_ ## id ## _class(id, (char* )(&APT_MENUITEM_ENRY ## id)); \
        inline void APT_MENU_ADDCHILD_ ## id ## _func() { if (id!=1) { (parent==0 ? \
                APT_menuItem_1_class.addSibling(& APT_menuItem_ ## id ## _class ) : \
                APT_menuItem_ ## parent ## _class.addChild(& APT_menuItem_ ## id ## _class ) ); } }; 



// Each MenuItem can have siblings or childrens. A MenuItem without children
// is called a leaf MenuItem. There can be two different leaf MenuItem types. 
// A line MenuItem is shown only in the actual MenuItem line if entered. 
// A full MenuItem is shown in the complete menu area (so no other menu 
// items are shown).    

// clbFunctionType is the function pointer definition for the callback 
// for each menu item. Function parameters:
// entry:             A pointer to this MenuItem
// callbackType:      lower 6 bits (0..63) decode one of the types #defines below. 
//                    MSB decodes if cursor is set on this MenuItem and MSB-1
//                    will give the info if MenuItem is selected. You should also use
//                    the define below to decode
// line:              Will give the current line in which the MenuItem is shown
//                   (Starting form 0)
// returns:          Meaning is given in below defires. If no meaning
//                   specified should always return false to be future proove
typedef bool (*clbFunctionType)(APT_Menu* 		menu, 
								APT_MenuItem* 	entry,
								const uint8_t 	callbackType, 
								uint8_t 		line);

#define APT_CLB_ISFULLMENUITEM     0  // should returns true if full MenuItem
                                      // and false if line MenuItem 
#define APT_CLB_CHECKIFHIDDEN      1  // return true if the MenuItem should be hidden
#define APT_CLB_DONEEDUPDATE       2  // return true if a update is needed, else false
#define APT_CLB_ONENTER            3  // enter from lower level, if called on a
                                      // a leaf element, return true if it can be
                                      // activated, else false
#define APT_CLB_ONCOMEBACK         4  // come back from a higher level
#define APT_CLB_ONEXIT             5  // if an active menu item goes back or 
                                      // if returned from MenuItem
#define APT_CLB_ONGOHIGHER         6  // enter one higher lever 
#define APT_CLB_ONTIMER            7  
#define APT_CLB_ONLOOP             8

// When the menu needs display updates, one of the following
// messages is sent. A line which needs a definitve update 
// receives APT_CLB_ONUPDATE_LINE if it is activated. Lines 
// which would not necessarily need an update receives 
// APT_CLB_ONUPDATE_OTHERLINE (can be used if no single
// line updates are possible and always the full menu needs to 
// be updated). APT_CLB_ONCURSORUPDATE is received if only the 
// scrollbar and cursor would need an update
#define APT_CLB_ONUPDATE_LINE         10  // content, cursor and scrollbar
                                          // of this line needs update
#define APT_CLB_ONUPDATE_OTHERLINE    11  // content, cursor and scrollbar 
                                          // of another entry is updated
#define APT_CLB_ONCURSORUPDATE        12  // only update the cursor and scrollbar

// use these functions to decode only the reason of the callback
#define APT_CLB_GETCONDITION(x)        (0x3F & x)
#define APT_CLB_CHECKCONDITION(x,cond) ( (0x3F & x) == cond )
// Get info if cursor is on MenuItem 
#define APT_CLB_ISCURSORON(x)          ( (0x80 & x) != 0 )
#define APT_CLB_SETCURSORON            0x80
// Get info if cursor is on MenuItem 
#define APT_CLB_ISACTIVEATED(x)        ( (0x40 & x) != 0 )
#define APT_CLB_SETACTIVEATED          0x40


class APT_MenuItem {
  public:
    // Constructor of the MenuItem. 
    APT_MenuItem(uint8_t id, const char * contentPointer );
    APT_MenuItem(uint8_t id, const char * contentPointer, clbFunctionType clbFunction );

    // returns the unique ID of the MenuItem
    inline uint8_t getID() { return id; };
    // Copies the content to be shown in the MenuItem to the contentBuffer.
    // ContentBuffer must have sufficient memory allocated to hold the full 
    // content string.
    void           getContent(char* contentBuffer);
    // Returns true, if this item is a full MenuItem (in contrast to 
    // a line MenuItem). 
    // Note: if no callback is given, it is always a line menu item.
    // Note: If you pass the MenuItem pointer, it will be passed forward to the 
    // callback function of this menu item
    bool           isFullMenuItem(APT_Menu* menu = NULL);
    
    // functions to set siblings, child and parents
    void                  addChild(APT_MenuItem* child);
    inline APT_MenuItem*  getChild() { return child; };
    inline bool           hasChild() { return child != NULL; };

    void                  addSibling(APT_MenuItem* sibling);
    inline bool           hasSibling() { return sibling != NULL; }; 
    inline APT_MenuItem*  getSibling() { return sibling; };
    void                  setSibling(APT_MenuItem* sibling);
    
    // count the number of siblings including this.
    // ToDo: in the moment this only starts form the current sibling, not from the first :-/
    uint8_t        getSibingsCount(void);    
    // get the n-th sibling, returns NULL if it does not exist.
    APT_MenuItem*  getNthSibling(const uint8_t n);

    // Sets and returns parent of an element. (Note: If it is the root 
    // menu, NULL is returned.
    inline APT_MenuItem*  getParent() { return parent; };
    inline void           setParent(APT_MenuItem* parent) { this->parent = parent; };
    
    // Stores the callback of this MenuItem . If no callback is stored 
    // (i.e. callback==NULL) nothing will happenn.
    void        setCallback(  clbFunctionType clbFunction );
    inline bool hasCallback(void) { return clbFunction != NULL; };
    // will call the callbackback function with given parameters
    bool        doCallback( APT_Menu* menu,
                            const uint8_t callbackType, 
                            const uint8_t line);  
    
  private:
    // Storing the three structure.
    APT_MenuItem *parent  = NULL;           // NULL if root menu layer
    APT_MenuItem *child   = NULL;           // first child, NULL if none
    APT_MenuItem *sibling = NULL;           // next sibling, null if no sibling
   
    uint8_t       id;                       // unique ID of menu element
    const char *  contentPointer;           // pointer to menu entry to be shown
   
    // stores the callback function
    clbFunctionType clbFunction = NULL;  
   
}; // class APT_MenuItem 

#endif
