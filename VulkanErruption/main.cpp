///////////////////////////////////////////////////////////////////////////////
// File: main.cpp
// Date: 21.08.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Entry point of the program
///////////////////////////////////////////////////////////////////////////////


#include "HelloTriangleApplication.h"

#include <iostream>

using namespace std;

int main()
{

	HelloTriangleApplication app;

	try 
	{
		app.run();
	}
	catch (exception const& e)
	{
		cerr << e.what() << endl;
		return 1;
	}

	return 0;
}