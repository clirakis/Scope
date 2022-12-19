/**
 ******************************************************************
 *
 * Module Name : main 
 *
 * Author/Date : C.B. Lirakis / 19-Dec-02
 *
 * Description :
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References :
 *
 *
 * RCS header info.
 * ----------------
 * $RCSfile$
 * $Author$
 * $Date$
 * $Locker$
 * $Name$
 * $Revision$
 *
 * $Log: $
 *
 *
 * Copyright (C) BBNT Solutions LLC  2001
 *******************************************************************
 */
#ifndef lint
/** RCS Information */
static char rcsid[]="$Header$";
#endif

// System includes.
#include <iostream>
using namespace std;
#include <string>
#include <cmath>
#include <csignal>
#include <fstream>


/// Local Includes.
#include "logdata.h"

const double pi=4.0*atan(1.0);


/**
 ******************************************************************
 *
 * Function Name :
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 *
 *******************************************************************
 */
static bool Initialize()
{
    InitializeLogData();
    return true;
}

/**
 ******************************************************************
 *
 * Function Name :
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 *
 *******************************************************************
 */
static void Finalize()
{

    FinalizeLogData();
}
/**
 ******************************************************************
 *
 * Function Name :
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 *
 *******************************************************************
 */


/**
 ******************************************************************
 *
 * Function Name :
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 *
 *******************************************************************
 */
bool DoIt()
{
    float data[4];
    LogData(data,0);
    return true;
}

/**
******************************************************************
*
* Function Name : Terminate
*
* Description : Deal with errors in a clean way!
*               ALL, and I mean ALL exits are brought 
*               through here!
*
* Inputs : Signal causing termination. 
*
* Returns : none
*
* Error Conditions : Well, we got an error to get here. 
*
*******************************************************************
*/ 
static void Terminate (int sig) 
{

    switch (sig)
		
    {
    case 0:			// Normal termination
	cerr << "Normal program termination." << endl;
	break;
    case SIGABRT:		// Abnormal termination 
	cerr << " SIGABRT " << endl;
	break;
    case SIGFPE:		// Floating-point error 
	cerr << " SIGFPE " << endl;
	break;
    case SIGILL:		// Illegal instruction 
	cerr << " SIGILL " << endl;
	break;
    case SIGINT:		// CTRL+C signal 
	cerr << " SIGINT " << endl;
	break;
    case SIGSEGV:		// Illegal storage access 
	cerr << " SIGSEGV " << endl;
	break;
    case SIGTERM:		// Termination request 
	cerr << " SIGTERM " << endl;
	break;
    default:
	cerr << " Uknown signal type " << sig << endl;
	break;
    }

    Finalize();
    if (sig == 0)
    {
	exit (0);
    }
    else
    {
	exit (-1);
    }
}

/**
 ******************************************************************
 *
 * Function Name : Help
 *
 * Description : provides user with help if needed.
 *
 * Inputs : none
 *
 * Returns : none
 *
 * Error Conditions : none
 *
 *******************************************************************
 */
static void Help(void)
{
  cout << "********************************************" << endl;
  cout << "* Test file for XXXX.              *" << endl;
  cout << "* Built on "<< __DATE__ << " " << __TIME__ << "*" << endl;
  cout << "* RCSinfo = " << rcsid << endl;
  cout << "* Available options are :                  *" << endl;
  cout << "*                                          *" << endl;
  cout << "********************************************" << endl;
}
/********************************************************************
 *
 * Function Name :
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 *
 ********************************************************************/
static void
ProcessCommandLineArgs(int argc, char **argv)
{
    int             count;
	
    for (count = 1; count < argc; count++)
    {
	if (!strcmp(argv[count], "-h"))
	{
	    Help();
	    Terminate(0);
	}
    }
}

/**
 ******************************************************************
 *
 * Function Name :
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 *
 *******************************************************************
 */
int main(int argc, char **argv)
{
	ProcessCommandLineArgs(argc, argv);
    /*
     * Setup a signal handler.      
     */
    signal (SIGABRT, Terminate); // Abnormal termination 
    signal (SIGFPE , Terminate); // Floating-point error 
    signal (SIGILL , Terminate); // Illegal instruction 
    signal (SIGINT , Terminate); // CTRL+C signal 
    signal (SIGSEGV, Terminate); // Illegal storage access 
    signal (SIGTERM, Terminate); // Termination request 
    // To open the wave device use the call waveInOpen
    Initialize();
    DoIt();
    Terminate(0);
}
