/**
 ******************************************************************
 *
 * Module Name : main.cpp
 *
 * Author/Date : C.B. Lirakis / 23-Apr-08
 *
 * Description : Standalone spiceplot
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
 * $RCSfile: main.cpp,v $
 * $Author: clirakis $
 * $Date: 2008/04/25 11:24:37 $
 * $Locker:  $
 * $Name:  $
 * $Revision: 1.1.1.1 $
 *
 * $Log: main.cpp,v $
 * Revision 1.1.1.1  2008/04/25 11:24:37  clirakis
 * Imported using TkCVS
 *
 * Revision 1.1  2004/10/24 17:23:13  clirakis
 * Initial revision
 *
 *
 *
 *******************************************************************
 */

#ifndef lint
/// RCS Information
static char rcsid[]="$Header: /home/clirakis/cvs/spiceplot/main.cpp,v 1.1.1.1 2008/04/25 11:24:37 clirakis Exp $";
#endif

// System includes.
#include <iostream>
using namespace std;
#include <string>
#include <cmath>
#include <csignal>

/// Root includes http://root.cern.ch
#include <TROOT.h>
#include <TRint.h>
#include <TGClient.h>
#include <TStyle.h>
#include <TEnv.h>  
#include <KeySymbols.h>
#include <pthread.h>

/// Local Includes.
#include "debug.h"
#include "gpsPlot.hh"

Bool_t rootint = kFALSE;
static TApplication *theApp;


extern char         *optarg;
Int_t               verbose = 0;
static char         *FileName = NULL;
static gpsPlot    *plotWindow;

/**
******************************************************************
*
* Function Name : Terminate
*
* Description : Deal with errors in a clean way!
*               ALL, and I mean ALL exits are brought through here!
*
* Inputs : Signal causing termination.
*
* Returns : none
*
* Error Conditions : Well, we got an error to get here
*
*******************************************************************
*/
void Terminate(int sig)
{
    static int i = 0;

    // prevent this from getting into a infinite loop of deaths.
    i++;
    if (i > 1)
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
    case SIGQUIT:              //QUIT
        cout << " SIGQUIT " << endl;
        break;
    case SIGILL:               // Illegal instruction
        cout << " SIGILL " << endl;
        break;
    case SIGABRT:              // Abnormal termination
        cout << " SIGABRT " << endl;
        break;
    case SIGBUS:               // Bus Error!
        cout << " SIGBUS " << endl;
        break;
    case SIGFPE:               // Floating-point error
        cout << " SIGFPE " << endl;
        break;
    case SIGKILL:              // Kill!!!!
        cout << " SIGKILL" << endl;
        break;
    case SIGSEGV:              // Illegal storage access
        cout << " SIGSEGV " << endl;
        break;
    case SIGTERM:              // Termination request
        cout << " SIGTERM " << endl;
        break;
    case SIGSTKFLT:            // Stack fault
        cout << " SIGSTKFLT " << endl;
        break;
    case SIGTSTP:              //
        cout << " SIGTSTP" << endl;
        break;
    case SIGXCPU:              //
        cout << " SIGXCPU" << endl;
        break;
    case SIGXFSZ:              //
        cout << " SIGXFSZ" << endl;
        break;
    case SIGSTOP:              //
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
    free(FileName);

    delete theApp;

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
    cout << "*     -h Help                              *" << endl;
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
static void ProcessCommandLineArgs(int argc, char **argv)
{
    int option;
    SET_DEBUG_STACK;
    do
    {
        option = getopt(argc, argv, "hHv:V:");
        switch(option)
        {
        case 'h':
        case 'H':
            Help();
            Terminate(0);
            break;
        case 'v':
        case 'V':
            verbose = atoi(optarg);
	    cout << "Verbose set to: " << verbose << endl;
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

    signal(SIGHUP , Terminate);   // Hangup.
    signal(SIGINT , Terminate);   // CTRL+C signal
    signal(SIGKILL, Terminate);   //
    signal(SIGQUIT, Terminate);   //
    signal(SIGILL , Terminate);   // Illegal instruction
    signal(SIGABRT, Terminate);   // Abnormal termination
    signal(SIGIOT , Terminate);   //
    signal(SIGBUS , Terminate);   //
    signal(SIGFPE , Terminate);   //
    signal(SIGSEGV, Terminate);   // Illegal storage access
    signal(SIGTERM, Terminate);   // Termination request
    signal(SIGSTKFLT, Terminate); //
    signal(SIGSTOP, Terminate);   //
    signal(SIGPWR, Terminate);    //
    signal(SIGSYS, Terminate);    //

    // User initialization goes here.
    plotWindow = new gpsPlot(gClient->GetRoot(), 800, 400, verbose);

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
    // Start up root.
    if (rootint)
    {
       theApp = new TRint("App", &argc, argv);
    }
    else
    {
        theApp = new TApplication("App", &argc, argv);
    }
    gStyle->SetOptStat(1111);
    gStyle->SetOptFit(1111);
    gStyle->SetStatFont(42);
    if (Initialize())
    {
        theApp->Run();
    }
    Terminate(0);
}
