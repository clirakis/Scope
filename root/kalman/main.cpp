/**
 ******************************************************************
 *
 * Module Name : 
 *
 * Author/Date : C.B. Lirakis / 01-Nov-05
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
 * Copyright (C) BBNT 2005
 *******************************************************************
 */

#ifndef lint
/// RCS Information
static char rcsid[]="$Header$";
#endif

// System includes.
#include <iostream>
using namespace std;
#include <string>
#include <cmath>
#include <csignal>
#include <unistd.h>
#include <fstream>

/// Local Includes.
#include "debug.h"
#include "Kalman.hh"

// Root include
#include "TRandom.h"
#include "TEnv.h"

static bool  verbose;
static TEnv *Parameters;
static Double_t Qx, Qv, Qa, Rx, Rv, Ra;

static bool LoadPrefs()
{

    Parameters = new TEnv("Kinit.rc");

    Qx = Parameters->GetValue( "Rx", 1.0);
    Qv = Parameters->GetValue( "Rv", 1.0);
    Qa = Parameters->GetValue( "Ra", 1.0);

    Rx = Parameters->GetValue( "Qx", 100.0);
    Rv = Parameters->GetValue( "Qv", 5.0);
    Ra = Parameters->GetValue( "Qa", 0.1);

    return kTRUE;
}
static bool SavePrefs()
{
    Parameters->SetValue("Qx", Qx);
    Parameters->SetValue("Qv", Qv);
    Parameters->SetValue("Qa", Qa);

    Parameters->SetValue("Rx", Rx);
    Parameters->SetValue("Rv", Rv);
    Parameters->SetValue("Ra", Ra);
    Parameters->SaveLevel(kEnvLocal);

    return kTRUE;
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
   static int i=0;
   // prevent this from getting into a infinite loop of 
   // deaths. 
    i++;
    if (i>1)
    { 
        exit(-1);
    }
    // Don't tell the user something is wrong when we have a signal of 0.
    if (sig != 0)
    {
	cout << "Death occured at: " << LastFile << " " << LastLine << endl;
    }

    // Add the apprpriate message. 
    switch (sig)
    {
    case -1: 
      cout << "User abnormal termination" << endl;
      break;
    case 0:                    // Normal termination
        cout << "Normal program termination." << endl;
        break;
    case SIGHUP:
        cout << " Hangup" << endl;
        break;
    case SIGINT:               // CTRL+C signal 
        cout << " SIGINT " << endl;
        break;
    case SIGQUIT:               //QUIT 
        cout << " SIGQUIT " << endl;
        break;
    case SIGILL:               // Illegal instruction 
        cout << " SIGILL " << endl;
        break;
    case SIGABRT:              // Abnormal termination 
        cout << " SIGABRT " << endl;
        break;
    case SIGBUS:               //Bus Error! 
        cout << " SIGBUS " << endl;
        break;
    case SIGFPE:               // Floating-point error 
        cout << " SIGFPE " << endl;
        break;
    case SIGKILL:               // Kill!!!! 
        cout << " SIGKILL" << endl;
        break;
    case SIGSEGV:              // Illegal storage access 
        cout << " SIGSEGV " << endl;
        break;
    case SIGTERM:              // Termination request 
        cout << " SIGTERM " << endl;
        break;
    case SIGSTKFLT:               // Stack fault
        cout << " SIGSTKFLT " << endl;
        break;
    case SIGTSTP:               // 
        cout << " SIGTSTP" << endl;
        break;
    case SIGXCPU:               // 
        cout << " SIGXCPU" << endl;
        break;
    case SIGXFSZ:               // 
        cout << " SIGXFSZ" << endl;
        break;
    case SIGSTOP:               // 
        cout << " SIGSTOP " << endl;
        break;
    case SIGPWR:               // 
        cout << " SIGPWR " << endl;
        break;
    case SIGSYS:               // 
        cout << " SIGSYS " << endl;
        break;
    default:
        cout << " Uknown signal type: "  << sig <<  endl;
        break;
    }

    // Put any user termination routines here. 
    SavePrefs();

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
    SET_DEBUG_STACK;
    cout << "********************************************" << endl;
    cout << "* Test file for text Logging.              *" << endl;
    cout << "* Built on "<< __DATE__ << " " << __TIME__ << "*" << endl;
    cout << "* RCSinfo = " << rcsid << endl;
    cout << "* Available options are :                  *" << endl;
    cout << "*                                          *" << endl;
    cout << "********************************************" << endl;
}
/**
 ******************************************************************
 *
 * Function Name :  ProcessCommandLineArgs
 *
 * Description : Loop over all command line arguments
 *               and parse them into useful data.
 *
 * Inputs : command line arguments. 
 *
 * Returns : none
 *
 * Error Conditions : none
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
static void
ProcessCommandLineArgs(int argc, char **argv)
{
    int option;
    SET_DEBUG_STACK;
    do
    {
        option = getopt( argc, argv, "f:hHnv");
        switch(option)
        {
        case 'f':
            break;
        case 'h':
        case 'H':
            Help();
            Terminate(0);
            break;
        case 'v':
            verbose = true;
            break;
	}
    } while(option != -1);
}
/**
 ******************************************************************
 *
 * Function Name : Initialize
 *
 * Description : Initialze the process
 *               - Setup traceback utility
 *               - Connect all signals to route through the terminate 
 *                 method
 *               - Perform any user initialization
 *
 * Inputs : none
 *
 * Returns : true on success. 
 *
 * Error Conditions : depends mostly on user code
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
static bool Initialize(void)
{

    LastFile = __FILE__;
    LastLine = __LINE__;
    signal (SIGHUP , Terminate);   // Hangup.
    signal (SIGINT , Terminate);   // CTRL+C signal 
    signal (SIGKILL, Terminate);   // 
    signal (SIGQUIT, Terminate);   // 
    signal (SIGILL , Terminate);   // Illegal instruction 
    signal (SIGABRT, Terminate);   // Abnormal termination 
    signal (SIGIOT , Terminate);   // 
    signal (SIGBUS , Terminate);   // 
    signal (SIGFPE , Terminate);   // 
    signal (SIGSEGV, Terminate);   // Illegal storage access 
    signal (SIGTERM, Terminate);   // Termination request 
    signal (SIGSTKFLT, Terminate); // 
    signal (SIGSTOP, Terminate);   // 
    signal (SIGPWR, Terminate);    // 
    signal (SIGSYS, Terminate);    // 
    // User initialization goes here. 
    LoadPrefs();

    return true;
}

/**
 ******************************************************************
 *
 * Function Name : main
 *
 * Description : It all starts here:
 *               - Process any command line arguments
 *               - Do any necessary initialization as a result of that
 *               - Do the operations
 *               - Terminate and cleanup
 *
 * Inputs : command line arguments
 *
 * Returns : exit code
 *
 * Error Conditions :
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
int main(int argc, char **argv)
{
    const Int_t NSteps = 100;
    Kalman      *k;
    TVector3    X,V,A, VP;
    Int_t       i;

    ofstream myout("test.dat");

    ProcessCommandLineArgs(argc, argv);
    if (Initialize())
    {
	X(0) = 1.0;
	X(1) = 0.0;
	V(0) = 0.5;
	V(1) = 0.0;
	TRandom *myran = new TRandom();
	k = new Kalman();
	k->SetMeasurementError( Rx, Rv, Ra);
	k->SetProcessNoise( Qx, Qv, Qa);

	A.Print();
	k->SetMeasurement( &X, &V, &A);
	k->Reset();
	for (i=0;i<NSteps;i++)
	{
	    k->ProcessMeasurement();
	    VP = k->GetV();
	    myout << i << " " << VP(0) << endl;
	    V(0) = myran->Gaus(0.5, 0.05);
	    //k->SetMeasurement( NULL, &V, NULL);
	}
	cout << " NSteps = " << NSteps << endl;
	k->Print();
	myout.close();
    }
    Terminate(0);
}
