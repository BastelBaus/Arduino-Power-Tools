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


#ifndef __APT_DEBUG_H__
#define __APT_DEBUG_H__




//#define APT_DEBUG

#ifdef APT_DEBUG
	
	#define __ASSERT_USE_STDERR
	#include <assert.h>

	#ifndef APT_DEBUG_SERIAL
		#include <Arduino.h>
		#define APT_DEBUG_SERIAL Serial
	#endif
		
	#define DEBUG_MSG_STR(x)  		{ APT_DEBUG_SERIAL.println(x); APT_DEBUG_SERIAL.flush();}
	#define DEBUG_MSG_PTR(x)  		{ APT_DEBUG_SERIAL.println(x); APT_DEBUG_SERIAL.flush();}
	#define DEBUG_MSG_STRPTR(x,y)   { APT_DEBUG_SERIAL.print(x); APT_DEBUG_SERIAL.println((uint16_t) y,HEX); APT_DEBUG_SERIAL.flush();}
	#define DEBUG_MSG_STRVAL(x,y)   { APT_DEBUG_SERIAL.print(x); APT_DEBUG_SERIAL.println(y); APT_DEBUG_SERIAL.flush(); }

	// abort if assertion failes
/*	#include <Arduino.h>
	void __assert (const char *func, const char *file, int line, const char *failedexpr) { 
		DEBUG_MSG_STR("Assertion failed:"); 
		if (func == NULL) DEBUG_MSG_STR(func); 
		DEBUG_MSG_STR(file); 
		DEBUG_MSG_STR(line); 
		DEBUG_MSG_STR(failedexpr); 
	}
*/

#else
//#ifdef TESTITNOT

	#define DEBUG_MSG_STR(x)  			{}
	#define DEBUG_MSG_PTR(x)  			{}
	#define DEBUG_MSG_STRPTR(x,y) 		{}
	#define DEBUG_MSG_STRVAL(x,y)		{}
	
	// abort if assertion failes
	//void __assert (const char *func, const char *file, int line, const char *failedexpr) { abort(); }

	
#endif


#endif
