/**
 ******************************************************************
 *
 * Module Name : hytek
 *
 * Author/Date : C.B. Lirakis / 10-Oct-08
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

/// Root includes
#include <TROOT.h>
#include <TFile.h>
#include <TNtuple.h>

/// Local Includes.
#include "debug.h"
#include "IIR.hh"

static bool verbose;

static TNtuple *fTemperature;
static TFile   *fFile;
static IIR     *filter;
static Char_t  *Filename;
static Char_t  *FilterFile;

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
    if (fFile)
    {
	fFile->Write();
	
	// Close the file. Note that this is automatically done when you leave
	// the application.
	fFile->Close();
    }
    delete filter;
    delete fTemperature;

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
        option = getopt( argc, argv, "f:hHm:v");
        switch(option)
        {
        case 'f':
	    Filename = strdup(optarg);
            break;
        case 'h':
        case 'H':
            Help();
            Terminate(0);
            break;
	case 'm':
	    FilterFile = strdup(optarg);
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
    Char_t   *names="N:T:CH0:CH1:CH2:CH3:CH4:CH5:CH6:CH7:FILTER";

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
    fFile = new TFile("temp.root","RECREATE","iUSBDAQ data analysis");
    /*
     * Create Ntuple.
     */
    fTemperature = new TNtuple("DEGF","iUSBDAQ ntuple",names);

    if (FilterFile)
    {
	filter = new IIR(FilterFile);
    }
    else
    {
	cout << "Using default Filter" << endl;
	filter = new IIR(35);
    }
    filter->Print();
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
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
static bool ReadTempFile(const char *filename)
{
    SET_DEBUG_STACK;
    unsigned long t, t0;
    Int_t         Count;
    Float_t       data[15], x;
    Double_t      y;
    Char_t        line[256];
    if (filename == NULL)
    {
	cout << "No file name provided." << endl;
	SET_DEBUG_STACK;
	return false;
    }
    ifstream in(filename);
    Count = 0;
    t0 = 0;
    if (!in.fail())
    {
	while (!in.eof())
	{
	    in.getline(line, sizeof(line));	
	    sscanf( line, "%ld %f %f %f %f %f %f %f \n",
		    &t, &x, &data[3], &data[4], &data[5], &data[6],
		    &data[7], &data[8]);

	    if (t0 == 0)
	    {
		t0 = t;
	    }
	    data[1] = t;
	    y = filter->AddSample((Double_t)x);
	    data[2]  = x;
	    data[10] = y;
	    //printf("%f \n",data[1]);
	    data[0] = Count;
  	    fTemperature->Fill(data);
	    Count++;
	}
	in.close();
	cout << "Processed: " << Count << " records." << endl; 
    }
    SET_DEBUG_STACK;
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
    if (Initialize())
    {
	ReadTempFile(Filename); 
    }
    Terminate(0);
}
