/**
 ******************************************************************
 *
 * Module Name : Scope main program
 *
 * Author/Date : C.B. Lirakis / 19-Jan-14
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
 *******************************************************************
 */
// System includes.
#include <iostream>
using namespace std;
#include <cstring>
#include <cmath>
#include <csignal>
#include <unistd.h>
#include <time.h>
#include <fstream>
#include <ostream>
#include <sys/time.h>
#include <sys/resource.h>

/// Root includes http://root.cern.ch
#include <TROOT.h>
#include <TRint.h>
#include <TStyle.h>
#include <TEnv.h>  
#include <KeySymbols.h>
#include <pthread.h>

/// Local Includes.
#include "debug.h"
#include "DSA602.hh"
#include "SPlot.hh"
#include "Version.hh"

static TApplication* theApp;
static bool          verbose;
static ofstream*     logFile;
static DSA602*       scope;
static SPlot*        plotWindow;

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
    char msg[128], tmp[64];
    time_t now;
    time(&now);
 
    i++;
    if (i>1) 
    {
        _exit(-1);
    }

    *logFile << "# Program Ends: " << ctime(&now);

    switch (sig)
    {
    case -1: 
      sprintf( msg, "User abnormal termination");
      break;
    case 0:                    // Normal termination
        sprintf( msg, "Normal program termination.");
        break;
    case SIGHUP:
        sprintf( msg, " Hangup");
        break;
    case SIGINT:               // CTRL+C signal 
        sprintf( msg, " SIGINT ");
        break;
    case SIGQUIT:               //QUIT 
        sprintf( msg, " SIGQUIT ");
        break;
    case SIGILL:               // Illegal instruction 
        sprintf( msg, " SIGILL ");
        break;
    case SIGABRT:              // Abnormal termination 
        sprintf( msg, " SIGABRT ");
        break;
    case SIGBUS:               //Bus Error! 
        sprintf( msg, " SIGBUS ");
        break;
    case SIGFPE:               // Floating-point error 
        sprintf( msg, " SIGFPE ");
        break;
    case SIGKILL:               // Kill!!!! 
        sprintf( msg, " SIGKILL");
        break;
    case SIGSEGV:              // Illegal storage access 
        sprintf( msg, " SIGSEGV ");
        break;
    case SIGTERM:              // Termination request 
        sprintf( msg, " SIGTERM ");
        break;
    case SIGSTKFLT:               // Stack fault
        sprintf( msg, " SIGSTKFLT ");
        break;
    case SIGTSTP:               // 
        sprintf( msg, " SIGTSTP");
        break;
    case SIGXCPU:               // 
        sprintf( msg, " SIGXCPU");
        break;
    case SIGXFSZ:               // 
        sprintf( msg, " SIGXFSZ");
        break;
    case SIGSTOP:               // 
        sprintf( msg, " SIGSTOP ");
        break;
    case SIGPWR:               // 
        sprintf( msg, " SIGPWR ");
        break;
    case SIGSYS:               // 
        sprintf( msg, " SIGSYS ");
        break;
    default:
        sprintf( msg, " Uknown signal type: %d", sig);
        break;
    }
    sprintf ( tmp, " %s %d", LastFile, LastLine);
    strncat ( msg, tmp, sizeof(msg));

    *logFile << msg << endl;
    
    // User termination here
    delete scope;
    delete theApp;

    logFile->close();
    delete logFile;

    if (sig == 0)
    {
        _exit (0);
    }
    else
    {
        _exit (-1);
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
    cout << "* DSA602 - control program                 *" << endl;
    cout << "* Built on "<< __DATE__ << " " << __TIME__ << "*" << endl;
    cout << "* Version: " << MAJOR_VERSION << "." 
	 << MINOR_VERSION << "*" << endl;
    cout << "* Available options are :                  *" << endl;
    cout << "*     -v verbose                           *" << endl;
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
        option = getopt( argc, argv, "hHnv");
        switch(option)
        {
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
    SET_DEBUG_STACK;
    time_t now;
    char   msg[128];
    time(&now);
    struct tm *tmnow = gmtime(&now);
    strftime (msg, sizeof(msg), "%F %T", tmnow);


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
    // ========================================================
    logFile = new ofstream("DSA602.log");
    *logFile << "# Program DSA602 begins: " << msg << endl;
    // User initialization goes here.
    plotWindow = new SPlot(gClient->GetRoot(), 800, 200, verbose);
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
    ProcessCommandLineArgs(argc, argv);
    //struct timespec sleeptime = {1L,000000000};

    theApp = new TApplication("App", &argc, argv);
    gStyle->SetOptStat(1111);
    gStyle->SetOptFit(1111);
    gStyle->SetStatFont(42);
    
    if (Initialize())
    {
        theApp->Run();
#if 0
	scope->UpTime();
	nanosleep(&sleeptime, NULL);
	GParse A("CALIBRATOR AMPLITUDE:5.0E-1,FREQ:1.024E+6,IMPEDANCE:5.0E+1");
	cout << A;
	rc = A.Value("FREQ");
	cout << "Freq:" << rc << endl;
	cout << "Avg:" << scope->Avg() << endl;
#endif
    }
    Terminate(0);
}
