/**
 ******************************************************************
 *
 * Module Name : HP main program
 *
 * Author/Date : C.B. Lirakis / 27-Nov-14
 *
 * Description :
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 *  17-Jan-21       CBL        Major Update
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
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <time.h>

#include <sys/time.h>
#include <sys/resource.h>

/// Local Includes.
#include "debug.h"
#include "CLogger.hh"
#include "Version.hh"
#include "DSA602.hh"
#include "WFMPRE.hh"
#include "GParse.hh"

/** Pointer to gpib interface to DSA602 digital scope */
static DSA602 *hgpib;

/** Control the verbosity of the program output via the bits shown. */
static unsigned int VerboseLevel = 0;

/** Pointer to the logger structure. */
static CLogger   *logger;

const int kGPIB_Address = 5; 

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
    case SIGSYS:               // 
        sprintf( msg, " SIGSYS ");
        break;
#ifndef MAC
     case SIGPWR:               // 
        sprintf( msg, " SIGPWR ");
        break;
    case SIGSTKFLT:               // Stack fault
        sprintf( msg, " SIGSTKFLT ");
        break;
#endif
   default:
        sprintf( msg, " Uknown signal type: %d", sig);
        break;
    }
    if (sig!=0)
    {
        sprintf ( tmp, " %s %d\n", LastFile, LastLine);
        strncat ( msg, tmp, sizeof(msg));
        //syslog  ( LOG_ERR, msg);
        logger->Log( msg);
    }
    
    // User termination here
    delete hgpib;
    delete logger;

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
    char msg[64];
    sprintf(msg, "%d.%d",MAJOR_VERSION, MINOR_VERSION);

    cout << "********************************************" << endl;
    cout << "* Keithly test                             *" << endl;
    cout << "* Built on "<< __DATE__ << " " << __TIME__ << "*" << endl;
    cout << "* Version: " << msg << "*" << endl;
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
        option = getopt( argc, argv, "hHv:");
        switch(option)
        {
        case 'h':
        case 'H':
            Help();
        Terminate(0);
        break;
        case 'v':
	    VerboseLevel = atoi(optarg);
	    cout << "Setting verbosity to: " << VerboseLevel << endl;
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
    sprintf(msg, "%d.%d",MAJOR_VERSION, MINOR_VERSION);
    double version = atof( msg);
    logger = new CLogger("DSA602.log", "DSA602", version);
    logger->SetVerbose(VerboseLevel);

    // User initialization goes here.
    hgpib = new DSA602( kGPIB_Address);
    if (hgpib->CheckError())
    {
	return false;
    }

    hgpib->SetDebug(VerboseLevel);

    return true;
}
#if 0
static void ParseTest(void)
{
    const char *str = "WFMPRE ACSTATE:ENHANCED,BIT/NR:16,BN.FMT:RI,BYT/NR:2,BYT.OR:LSB,CRVCHK:CHKSM0,ENCDG:BINARY,NR.PT:1024,PT.FMT:Y,WFID:STO1,XINCR:5.0E-7,XMULT:1.5625E-4,XUNIT:SECONDS,XZERO:0.0E+0,YMULT:1.5625E-4,YUNIT:VOLTS,YZERO:0.0E+0,LABEL:\"\",TIME:\"21:37:01.40\",DATE:\" 4-FEB-14\",TSTIME:0.0E+0";

    //cout << "Test string: " << instr << endl;

    //WFMPRE testme(str);
    GParse p(str);
    //cout << p;
    cout << "Value found. " << p.Value("XUNIT") << endl;

}
#endif
#if 0
static void TestOne(void)
{
    System *pSys = hgpib->pSystem();
    hgpib->GetWaveformHeader();
    cout << *hgpib << endl;
    //cout << "Uptime: " << hgpib->pSystem()->UpTime() << endl;
    pSys->CalibratorFrequency(CAL_FREQ_ZERO);
    cout << "CALIBRATOR FREQ" << pSys->CalibratorFrequency() << endl;
    cout << "CALIBRATOR AMPL" << pSys->CalibratorAmplitude() << endl;
    cout << "CALIBRATOR IMP" << pSys->CalibratorImpedence() << endl;
    sleep(1);
#if 0
    pSys->CalibratorAmplitude(1.0);
    cout << "CALIBRATOR AMPL 1.0" << pSys->CalibratorAmplitude() << endl;
    sleep(1);

    hgpib->pSystem()->CalibratorFrequency(CAL_FREQ_1KHZ);
    cout << "CALIBRATOR FREQ" << pSys->CalibratorFrequency() << endl;
    cout << "CALIBRATOR AMPL" << pSys->CalibratorAmplitude() << endl;
    sleep(1);

    hgpib->pSystem()->CalibratorFrequency(CAL_FREQ_1024MHZ);
    cout << "CALIBRATOR FREQ" << pSys->CalibratorFrequency() << endl;
    hgpib->pSystem()->CalibratorAmplitude(5.0);
    cout << "CALIBRATOR AMPL" << pSys->CalibratorAmplitude() << endl;
#endif

    sleep(1);
    pSys->CalibratorFrequency(CAL_FREQ_1KHZ);
    cout << "CALIBRATOR FREQ" << pSys->CalibratorFrequency() << endl;

}
#endif
#if 0
static void TestTwo(void)
{
    System *pSys = hgpib->pSystem();
    //cout << "Set: " << pSys->SetDate() << endl;
    cout << pSys->Date() << endl;
    cout << " Set Time : " << pSys->SetTime() << endl;
    cout << pSys->Time() << endl;
   
}
#endif
#if 0
static void TestThree(void)
{
    System *pSys = hgpib->pSystem();

    cout << pSys->UserID("foo-bar") << endl;
    cout << pSys->UserID() << endl;
}
#endif
#if 0
static void TestFour(void)
{
    //cout << *hgpib << endl;
#if 0
    cout << hgpib->pMeasurement()->Cross(true) << endl;
    cout << hgpib->pMeasurement()->Delay(true) << endl;
    cout << hgpib->pMeasurement()->Duty(true) << endl;
    cout << hgpib->pMeasurement()->Falltime(true) << endl;
    cout << hgpib->pMeasurement()->Frequency(true) << endl;
    cout << hgpib->pMeasurement()->Gain(true) << endl;
    cout << hgpib->pMeasurement()->Max(true) << endl;
    cout << hgpib->pMeasurement()->Mean(true) << endl;
    cout << hgpib->pMeasurement()->Midpoint(true) << endl;
    cout << hgpib->pMeasurement()->Min(true) << endl;
    cout << hgpib->pMeasurement()->Overshoot(true) << endl;
    cout << hgpib->pMeasurement()->PDelay(true) << endl;
    cout << hgpib->pMeasurement()->Period(true) << endl;
    cout << hgpib->pMeasurement()->Phase(true) << endl;
    cout << hgpib->pMeasurement()->PeakToPeak(true) << endl;
    cout << hgpib->pMeasurement()->Risetime(true) << endl;
    cout << hgpib->pMeasurement()->RMS(true) << endl;
    cout << hgpib->pMeasurement()->SpectralFrequency(true) << endl;
    cout << hgpib->pMeasurement()->Skew(true) << endl;
    cout << hgpib->pMeasurement()->SpectralMagnitude(true) << endl;
    cout << hgpib->pMeasurement()->TotalHarmonicDistortion(true) << endl;
    cout << hgpib->pMeasurement()->TimeToTrigger(true) << endl;
    cout << hgpib->pMeasurement()->Undershoot(true) << endl;
    cout << hgpib->pMeasurement()->Width(true) << endl;
    cout << hgpib->pMeasurement()->YTEnergy(true) << endl;
    cout << hgpib->pMeasurement()->YTMNS_Area(true) << endl;
    cout << hgpib->pMeasurement()->YTPLS_Area(true) << endl;
#else
    hgpib->pMeasurement()->Update();
    cout << *hgpib->pMeasurement();
#endif
}
#endif
#if 0
static void TestFive(void)
{
    hgpib->pMeasurement()->Update();
    cout << endl;
    cout << *hgpib->pMeasurement();
}
#endif
#if 0
static void TestSix(void)
{
    cout << "Testing Channel." << endl;
    //cout << "amplifer offset: " << hgpib->AmpOffset(true) << endl;
}
#endif
#if 0
static void GetData(void)
{
    /*
     * for the given trace number, return the X and Y point set. 
     * n contains the number of points in the curve if present. 
     */
    double *X, *Y;

    int32_t TraceNumber = 1;

    cout << __FUNCTION__ << " "
	 << " Get the data for trace: " << TraceNumber << endl;
    int32_t n = hgpib->Curve(TraceNumber, &X, &Y);
    cout << "Number points: " << n << endl;

    for(int32_t i=0; i<n;i++)
    {
	cout << X[i] << "," << Y[i] << endl;
    }
    free(X);
    free(Y);

}
#endif
#if 0
static void TestMeasurement(void)
{
    Measurement *pmeas  = hgpib->pMeasurement();
    char s[32];
    logger->SetVerbose(2);

    cout << "Test Measurement." << endl;

    pmeas->Update();
    pmeas->NList();
    pmeas->ActiveList();
    cout << "And the results are: " << endl << *pmeas;
}
#endif
static void TestReadFile(void)
{
    logger->SetVerbose(2);
    hgpib->ExecuteFile("init.cmds");

    hgpib->SaveSetup("setup.cmds");
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
    if (Initialize())
    {
	cout << "Version data." << hgpib->Version() << endl;
	//TestOne();
	//ParseTest();
	//TestTwo();
	//TestThree();
	//TestFour();
	//TestFive();
	//TestSix();
	//GetData();
	//TestMeasurement();
	TestReadFile();
    }
    Terminate(0);
}
