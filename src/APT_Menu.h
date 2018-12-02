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
// - toDO: delete childs ??
// document if more than one line needs update, alwways the full display is updated
// - dynamically added and delteded menu Items
// DoNeed Updte --> Callback "loop"
// Full Menu Item by config and not by callback ?? or alternative: hiffen by callback (no gut idea due to dynamics !)


#include <Arduino.h>

 
#define REPEAT_ADDCHILD_37  REPEAT_ADDCHILD_36  APT_MENU_ADDCHILD_37_func(); 
#define REPEAT_ADDCHILD_36  REPEAT_ADDCHILD_35  APT_MENU_ADDCHILD_36_func(); 
#define REPEAT_ADDCHILD_35  REPEAT_ADDCHILD_34  APT_MENU_ADDCHILD_35_func(); 
#define REPEAT_ADDCHILD_34  REPEAT_ADDCHILD_33  APT_MENU_ADDCHILD_34_func(); 
#define REPEAT_ADDCHILD_33  REPEAT_ADDCHILD_32  APT_MENU_ADDCHILD_33_func(); 
#define REPEAT_ADDCHILD_32  REPEAT_ADDCHILD_31  APT_MENU_ADDCHILD_32_func(); 
#define REPEAT_ADDCHILD_31  REPEAT_ADDCHILD_30  APT_MENU_ADDCHILD_31_func(); 
#define REPEAT_ADDCHILD_30  REPEAT_ADDCHILD_29  APT_MENU_ADDCHILD_30_func(); 
#define REPEAT_ADDCHILD_29  REPEAT_ADDCHILD_28  APT_MENU_ADDCHILD_29_func(); 
#define REPEAT_ADDCHILD_28  REPEAT_ADDCHILD_27  APT_MENU_ADDCHILD_28_func(); 
#define REPEAT_ADDCHILD_27  REPEAT_ADDCHILD_26  APT_MENU_ADDCHILD_27_func(); 
#define REPEAT_ADDCHILD_26  REPEAT_ADDCHILD_25  APT_MENU_ADDCHILD_26_func(); 
#define REPEAT_ADDCHILD_25  REPEAT_ADDCHILD_24  APT_MENU_ADDCHILD_25_func(); 
#define REPEAT_ADDCHILD_24  REPEAT_ADDCHILD_23  APT_MENU_ADDCHILD_24_func(); 
#define REPEAT_ADDCHILD_23  REPEAT_ADDCHILD_22  APT_MENU_ADDCHILD_23_func(); 
#define REPEAT_ADDCHILD_22  REPEAT_ADDCHILD_21  APT_MENU_ADDCHILD_22_func(); 
#define REPEAT_ADDCHILD_21  REPEAT_ADDCHILD_20  APT_MENU_ADDCHILD_21_func(); 
#define REPEAT_ADDCHILD_20  REPEAT_ADDCHILD_19  APT_MENU_ADDCHILD_20_func(); 
#define REPEAT_ADDCHILD_19  REPEAT_ADDCHILD_18  APT_MENU_ADDCHILD_19_func(); 
#define REPEAT_ADDCHILD_18  REPEAT_ADDCHILD_17  APT_MENU_ADDCHILD_18_func(); 
#define REPEAT_ADDCHILD_17  REPEAT_ADDCHILD_16  APT_MENU_ADDCHILD_17_func(); 
#define REPEAT_ADDCHILD_16  REPEAT_ADDCHILD_15  APT_MENU_ADDCHILD_16_func(); 
#define REPEAT_ADDCHILD_15  REPEAT_ADDCHILD_14  APT_MENU_ADDCHILD_15_func(); 
#define REPEAT_ADDCHILD_14  REPEAT_ADDCHILD_13  APT_MENU_ADDCHILD_14_func(); 
#define REPEAT_ADDCHILD_13  REPEAT_ADDCHILD_12  APT_MENU_ADDCHILD_13_func(); 
#define REPEAT_ADDCHILD_12  REPEAT_ADDCHILD_11  APT_MENU_ADDCHILD_12_func(); 

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

  // as clear just outputs a new header
  void APT_DefaultSerialMenu_clearLine(uint8_t line);
  void APT_DefaultSerialMenu_clearMenu();
  
  // prints the menu entry name
  // callbackType:    uses the same encoding as in the main 
  //                  function to encode if cursor is on line 
  //                  or item is selected
  bool APT_DefaultSerialMenu_showMenuItem(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line);

  
#endif

//#ifdef APT_DEFAULT_LCD_MENU_HANDLING_ROUTINES


  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wuninitialized"
  #include <LiquidCrystal_I2C.h>       	// Library for I2C LCD
  #pragma GCC diagnostic pop
  extern LiquidCrystal_I2C   lcd;		// lcd class must be defined in main program.

  // initialization
  void APT_DefaultLCDMenu_init();
  // as clear just outputs a new header
  void APT_DefaultLCDMenu_clear();
  // prints the menu entry name
  bool APT_DefaultLCDMenu_showMenuItem(APT_Menu* menu, APT_MenuItem* entry, const uint8_t callbackType, const uint8_t line);

  //#endif


// Setup function of menu.
// menuVariable:   the variable which created teh menu
// menuItemCount:  count of MenuItem elements
#define APT_MENU_SETUP(menuVariable,menuItemCount) { menuVariable.setup(&APT_menuItem_1_class); APT_MENU_REPEAT_ADDCHILD(menuItemCount); }



/********************************************************************/
/** Callback handling                                              **/
/********************************************************************/

        
#define APT_menuItem_0_class  APT_menuItem_1_class      // just to avoid compiler errors, will never be called

// ToDo, make this shorter and combine it to one

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


//get pointer to ID, does not work with variables
#define	APT_MENUITEM_POINTER(id) (&APT_menuItem_ ## id ## _class)
#define APT_MENU_EXTERNAL(N) 	  APT_MENU_EXTERNAL_##N		

#define APT_MENU_EXTERNAL_18      APT_MENU_EXTERNAL_17; extern APT_MenuItem APT_menuItem_18_class
#define APT_MENU_EXTERNAL_17      APT_MENU_EXTERNAL_16; extern APT_MenuItem APT_menuItem_17_class
#define APT_MENU_EXTERNAL_16      APT_MENU_EXTERNAL_15; extern APT_MenuItem APT_menuItem_16_class
#define APT_MENU_EXTERNAL_15      APT_MENU_EXTERNAL_14; extern APT_MenuItem APT_menuItem_15_class
#define APT_MENU_EXTERNAL_14      APT_MENU_EXTERNAL_13; extern APT_MenuItem APT_menuItem_14_class
#define APT_MENU_EXTERNAL_13      APT_MENU_EXTERNAL_12; extern APT_MenuItem APT_menuItem_13_class
#define APT_MENU_EXTERNAL_12      APT_MENU_EXTERNAL_11; extern APT_MenuItem APT_menuItem_12_class
#define APT_MENU_EXTERNAL_11      APT_MENU_EXTERNAL_10; extern APT_MenuItem APT_menuItem_11_class
#define APT_MENU_EXTERNAL_10      APT_MENU_EXTERNAL_9;  extern APT_MenuItem APT_menuItem_10_class
#define APT_MENU_EXTERNAL_9       APT_MENU_EXTERNAL_8;  extern APT_MenuItem APT_menuItem_9_class
#define APT_MENU_EXTERNAL_8       APT_MENU_EXTERNAL_7;  extern APT_MenuItem APT_menuItem_8_class
#define APT_MENU_EXTERNAL_7       APT_MENU_EXTERNAL_6;  extern APT_MenuItem APT_menuItem_7_class
#define APT_MENU_EXTERNAL_6       APT_MENU_EXTERNAL_5;  extern APT_MenuItem APT_menuItem_6_class
#define APT_MENU_EXTERNAL_5       APT_MENU_EXTERNAL_4;  extern APT_MenuItem APT_menuItem_5_class
#define APT_MENU_EXTERNAL_4       APT_MENU_EXTERNAL_3;  extern APT_MenuItem APT_menuItem_4_class
#define APT_MENU_EXTERNAL_3       APT_MENU_EXTERNAL_2;  extern APT_MenuItem APT_menuItem_3_class
#define APT_MENU_EXTERNAL_2       APT_MENU_EXTERNAL_1;  extern APT_MenuItem APT_menuItem_2_class
#define APT_MENU_EXTERNAL_1       extern APT_MenuItem APT_menuItem_1_class

				
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

#define APT_CLB_CAN_ONCURSORON     9   // ToDo: 

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




// Each MenuItem can have siblings or childrens. A MenuItem without children
// is called a leaf MenuItem. There can be two different leaf MenuItem types. 
// A line MenuItem is shown only in the actual MenuItem line if entered. 
// A full MenuItem is shown in the complete menu area (so no other menu 
// items are shown).    


class APT_MenuItem {

  public:

    /********************************************************************/
	/** Constructur and basic functions                                **/
	/********************************************************************/

    // Constructor of the MenuItem. 
    APT_MenuItem(const uint8_t id, const char * const contentPointer );
    APT_MenuItem(const uint8_t id, const char * const contentPointer, clbFunctionType clbFunction );

    // returns the ID of the MenuItem
    uint8_t 		getID() { return id; };
		
    // Copies the content to be shown in the MenuItem to the contentBuffer.
    // ContentBuffer must have sufficient memory allocated to hold the full 
    // content string.
    void           	getContent(char* contentBuffer);
	
    // Returns true, if this item is a full MenuItem (in contrast to 
    // a line MenuItem). 
    // Note: If no callback is given, it is always a line menu item.
    // Note: If you pass the MenuItem pointer, it will be passed 
	// forward to the callback function of this menu item
    bool           	isFullMenuItem(APT_Menu* menu = NULL);

	/********************************************************************/
	/** creating and walking through the menu structure                **/
	/********************************************************************/
	
    // Sets and returns parent of an element. (Note: If it is the root 
    // menu, NULL is returned.
    bool            hasParent(); // returns true if not oldest generation (root, no parent)
    APT_MenuItem*   getParent(); // 
    void     		setPredecessor(APT_MenuItem* pre) { this->pre = pre; };
	
	
    // functions to set siblings, child and parents
    void            addChild(APT_MenuItem* child);
    APT_MenuItem*   getChild() { return child; };
    bool            hasChild() { return child != NULL; };

	
    // adds a sibling 
	void            addSibling(APT_MenuItem* sibling); // adds a new born sibling (youngest)
	
	// query and return siblings
	bool            hasYoungerSibling(); 			  // returns true if there is a younger sibling    
	APT_MenuItem*   getYoungerSibling();				  // returns next younger sibling 
													  // (if no younger sibing, return NULL)
    bool            hasOlderSibling(); 				  // returns true if there is an older sibling
    APT_MenuItem*   getOlderSibling();				  // returns next older sibling 
													  // (if no older sibing, return NULL)
    
	// returns true if MenuItem is oldestSibling
	bool   			isOldestSibling();
	APT_MenuItem*	getOldestSibling();
	
	// count the total number of siblings
    uint8_t        getNumberOfSibings(void);    
	// count the hwo oldest sibling thie MenuIem is
    uint8_t 	   getCountOfOlderSiblings(void);
	// get the n-th sibling, returns NULL if it does not exist.
    APT_MenuItem*  getNthSibling(const uint8_t n);


    /********************************************************************/
	/** Callback handling                                              **/
	/********************************************************************/
	
    // Stores the callback of this MenuItem . If no callback is stored 
    // (i.e. callback==NULL) nothing will happenn.
    void        setCallback(  clbFunctionType clbFunction );
    inline bool hasCallback(void) { return clbFunction != NULL; };
    // will call the callbackback function with given parameters
    bool        doCallback( APT_Menu* menu,
                            const uint8_t callbackType, 
                            const uint8_t line);  
    
	
  	// configure each menu item to be hidden or shown
	// protected since it can only be accessed from APT_Menu
	// which has to handle the cursor and scroll position
	void hide();
	void show();
	bool isHidden();
	// ToDo, make this private or add link to Menu and start
	
	
  protected:

    /********************************************************************/
	/** Menu tree pointers                                             **/
	/********************************************************************/

	APT_MenuItem();
    
	void setSibling(APT_MenuItem* sibling); // to do, make this private ?
    // Storing the three pointers within the tree.
    APT_MenuItem *pre    = NULL;            // NULL if root menu layer, 
											// if oldest sibling link to parent
											// else link to next older sibling
    APT_MenuItem *child   = NULL;           // oldest child, NULL if none
    APT_MenuItem *sibling = NULL;           // next younger sibling, null if 
											// no younger sibling
   

	
  private:

    /********************************************************************/
	/** private elements                                               **/
	/********************************************************************/

	const uint8_t       id;                     // unique ID of menu element
    const char * const  contentPointer;         // pointer to menu entry to be shown
    clbFunctionType     clbFunction = NULL; 	// stores the callback function
	uint8_t				config;					// configuration parameters of each menu entry
	#define  APT_MENUITEM_CONFIG_HIDDEN       0
   
}; // class APT_MenuItem 

typedef void (*clearMenuFunctionType)(void);
typedef void (*clearLineFunctionType)(const uint8_t line);

								
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
    void setDefaultShowEntryFunction(clbFunctionType defaultShowEntryFunction);
    //
    bool callDefaultShowEntryFunction( APT_MenuItem* entry,const uint8_t callbackType, const uint8_t line);
    
    // Pointer to the functions which are called if menu should be cleared 
    // Seperate functions for full menu or a line of the menu can be specified.
    // If a function is not specified (i.e. NULL), it is not used.
	// clearLineFunctionType clearLineFunction = NULL == NULL not set ! ToDo
    void setClearFunction( clearMenuFunctionType clearMenuFunction, clearLineFunctionType clearLineFunction = NULL);
    void setLineClearFunction( clearLineFunctionType clearLineFunction);
    
    // use these functions to navigate 
    // through your menu items. If one is within a MenuItem, 
    // the goUp/goDown will not change, the only way to leaf 
    // is the goBack() function. goInto will go into the next menu level
	// or enter a MenuItem.
    void goUp();
    void goDown();
    void goInto();
    void goBack();
    
	//
	// If ID cannot be found, menu is unchanged
	// fastSearch:   if true, diretly goes to element 
	//               (w/o traversing the menu structure).
	//				 Can be used to speed up function if 
	//				 no callbacks are installed which should 
	//				 be called
	void 		gotoID(uint8_t ID, bool fastSearch = false);
    void 		gotoMenuItem(APT_MenuItem *item,bool fastSearch = false);
    void 		gotoRoot();      // goto first layer
	void 		gotoFirstRoot(); // goto first and first menu element
	
	// get a pointer to the MenuItem with the given ID. 
	// If this MenuItem does not exists, NULL is returned
	APT_MenuItem*	getMenuItem(uint8_t id);
	
	//
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

    // Returns pointer to current (where cursor 
    // is located) MenuItem or its ID [independent 
	// of menu is actived or not)
    APT_MenuItem* 	getCurrentMenuItem(void);
    uint8_t 		getCurrentMenuItemID(void);
    // Returns pointer to active MenuItem. If no 
    // Menu Item is active, NULL is returned
    APT_MenuItem*   getActiveMenuItem(void);

	// ToDO: Docu
	// If line is not available, returns 
	// last available line.
    APT_MenuItem* 	getMenuItemInLine(uint8_t line);
	uint8_t 		getMenuItemIDInLine(uint8_t line);

	
	// control visbility of menu items
	void hide(uint8_t ID);
	void show(uint8_t ID);
	bool isHidden(uint8_t ID);
	void hide(APT_MenuItem* menuItem);
	void show(APT_MenuItem* menuItem);
	bool isHidden(APT_MenuItem* menuItem);
	

	
    
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

	APT_MenuItem* getMenuItem(APT_MenuItem* firstEntryOfRow, uint8_t id);
	
    // The three function pointers which could be used to customize 
    // the menu behavior
    clearMenuFunctionType         clearMenuFunction = NULL;
    clearLineFunctionType         clearLineFunction = NULL;
    clbFunctionType               defaultShowEntryFunction = NULL;

    uint8_t configuration;
    #define APT_MENU_CONFIG_BIT_SCROLLBAR            0    // 
    #define APT_MENU_CONFIG_BIT_MENU_OFF             1    // If activated, turn menu off.
    #define APT_MENU_CONFIG_BIT_SCROLL_WRAPAROUND    2	  // If activated, menu will start from 
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
      
        
}; // class APT_Menu



#endif
