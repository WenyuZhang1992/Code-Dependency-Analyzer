/////////////////////////////////////////////////////////////////////
// Display.cpp - Generally useful helper functions                 //
//                                                                 //
// Application: CSE687 - Object Oriented Design Project #2         //
// Language:    C++, Visual Studio 2015                            //
// Platform:    Win 8.1, Visual Studio 2015                        //
// Author:      WENYU ZHANG, 233508014                             //
//              wzhan102@syr.edu                                   //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University        //
//              jfawcett@twcny.rr.com, (315) 443-3948              //
/////////////////////////////////////////////////////////////////////

#include "Display.h"
#include <iostream>
#include <string>

#ifdef TEST_DISPLAY

using namespace CodeAnalysis;

int main()
{
	makeTitle("Test makeTitle()");
	makeSubTitle("Test makeSubTitle()");
	makeComment("Test makeComment()");
}

#endif // TEST_DISPLAY