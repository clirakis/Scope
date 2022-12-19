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
 *******************************************************************
 */
// System includes.
#include <iostream>
using namespace std;
#include <string>
#include <cmath>
#include <stdio.h>
#include <csignal>
#include <unistd.h>
#include <errno.h>
#include <ctime>
#include <sys/time.h>

/// Local Includes.
#include "debug.h"

static bool verbose;
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
        _exit(-1);
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
    cout << "********************************************" << endl;
    cout << "* Test file for text Logging.              *" << endl;
    cout << "* Built on "<< __DATE__ << " " << __TIME__ << "*" << endl;
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
#if 0
    int             count;
	
    for (count = 1; count < argc; count++)
    {
	if (!strcmp(argv[count], "-h"))
	{
	    Help();
	    Terminate(0);
	}
    }
#else
    int option;
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

#endif
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

    signal (SIGHUP , Terminate);  // Hangup.
    signal (SIGINT , Terminate);  // CTRL+C signal 
    signal (SIGKILL, Terminate);  // 
    signal (SIGQUIT, Terminate);  // 
    signal (SIGILL , Terminate);  // Illegal instruction 
    signal (SIGABRT, Terminate);  // Abnormal termination 
    signal (SIGIOT , Terminate);  // 
    signal (SIGBUS , Terminate);  // 
    signal (SIGFPE , Terminate);  // 
    signal (SIGSEGV, Terminate);  // Illegal storage access 
    signal (SIGTERM, Terminate);  // Termination request 
    signal (SIGSTKFLT, Terminate); // 
    signal (SIGSTOP, Terminate); // 
    signal (SIGPWR, Terminate); // 
    signal (SIGSYS, Terminate); // 
    // User initialization goes here. 

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
    //int rc;
    //char *cmd[] = {"cp", "temp.c", "final.c", (char *) 0};
    time_t now;
    struct timeval  tv;
    struct timezone tz;
    struct tm       *TheTime;
    clock_t  nowTime, lastTime;
    struct timespec sleeptime = {1L, 0L};

    ProcessCommandLineArgs(argc, argv);

    if (Initialize())
    {
#if 0
        cout << __GNUC__ << endl;
        rc = execve( "/bin/cp", cmd, NULL);
        if (rc != 0)
        {
            cout << " Failed. " << rc << endl;
            perror("Failed:");
        }
#endif
        time(&now);
        TheTime = localtime(&now);
        cout << "Now: " << now 
             << " dst:" << TheTime->tm_isdst
             << " zone:" << TheTime->tm_zone
             << " gmtoff:" << TheTime->tm_gmtoff
             << endl; 
        gettimeofday(&tv,&tz);
        cout << "Get time of day." << endl;
        cout << "Minutes west of Greenwich:" << tz.tz_minuteswest
             << " DSTTIME:" << tz.tz_dsttime 
             << endl;
        lastTime = clock();
	for (int j=0;j<10;j++)
        {
            //sleep(1);
            nanosleep(&sleeptime, NULL);
            nowTime = clock();
           //cout << ((long double) (nowTime-lastTime))/CLOCKS_PER_SEC
           cout << ((long double) (nowTime-lastTime))/55.0
                 << endl;
            lastTime = nowTime;
        }
        
    }
    Terminate(0);
}
