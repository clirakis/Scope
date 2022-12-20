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
// #include <cstring>
// #include <cmath>
// #include <csignal>
// #include <unistd.h>
// #include <time.h>
// #include <fstream>
// #include <ostream>
// #include <sys/time.h>
// #include <sys/resource.h>

/// Root includes http://root.cern.ch
#include <TROOT.h>
#include <TRint.h>
#include <TStyle.h>
#include <TEnv.h>  

/// Local Includes.
#include "debug.h"
#include "CLogger.hh"
#include "UserSignals.hh"
#include "DSA602.hh"
#include "SPlot.hh"
#include "Version.hh"

static TApplication* theApp;
static SPlot*        plotWindow;
static CLogger*      logger;
static Int_t         VerboseLevel;

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
    cout << "*     -v verbose level                     *" << endl;
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
        option = getopt( argc, argv, "hHnv:V:");
        switch(option)
        {
        case 'h':
        case 'H':
            Help();
        Terminate(0);
        break;
        case 'v':
        case 'V':
            VerboseLevel = atoi(optarg);
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
    char msg[64];
    SET_DEBUG_STACK;
    SetSignals();

    // User initialization goes here. 
    // ========================================================
    sprintf(msg, "%d.%d",MAJOR_VERSION, MINOR_VERSION);
    double version = atof( msg);
    logger = new CLogger("oncore.log", "oncore", version);
    logger->SetVerbose(VerboseLevel);

    // User initialization goes here.
    plotWindow = new SPlot(gClient->GetRoot(), 800, 200);
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
    }
    Terminate(0);
}
