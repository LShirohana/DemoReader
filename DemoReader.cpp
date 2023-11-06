// DemoReader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "CDemoFile.h"



/* How the error happens:

if ( !m_DemoFile.Open( filename, true )  )
{
	cl.demonum = -1;		// stop demo loop
	return false;
}

// Read in the m_DemoHeader
demoheader_t *dh = m_DemoFile.ReadDemoHeader();

if ( !dh )
{
	ConMsg( "Failed to read demo header.\n" );
	m_DemoFile.Close();
	cl.demonum = -1;
	return false;
}
*/





int main()
{
    std::cout << "hi i read demo thx!\n";

	CDemoFile demoIn;

	//if (!demoIn.Open("original.dem", true))
	{
		// Even if its already open, im just gonna keep going, to read the header.
		//cl.demonum = -1;		// stop demo loop
		//return false;
	}

	// Read in the m_DemoHeader
	demoheader_t* dh = demoIn.ReadDemoHeader();

	if (!dh)
	{
		//ConMsg("Failed to read demo header.\n");
		//m_DemoFile.Close();
		//cl.demonum = -1;
		return false;
	}
}
