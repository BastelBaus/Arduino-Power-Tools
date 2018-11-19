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
/* portions of the Software.                                               */
/*                                                                         */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,         */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF      */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY    */ 
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,    */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE       */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  */
/***************************************************************************/

#ifndef __APT_SRIALINPUT_H__
#define __APT_SRIALINPUT_H__

// typedef of function which is called when a serial 
// char was received
typedef void (*functionHandlerType)(const char key);

class APT_SerialInput {
  public:
    APT_SerialInput(void);
    APT_SerialInput(functionHandlerType functionHandler);

    // set handle to be called when a char received on serial 
    // interface
    void attachHandler(functionHandlerType functionHandler);

    // call this in every loop
    void loop(void);
    
  private:
    functionHandlerType functionHandler;
  
}; // class SerialInput {

#endif
