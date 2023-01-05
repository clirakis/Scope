/********************************************************************
 *
 * Module Name : Monitor.cpp
 *
 * Author/Date : C.B. Lirakis / 23-May-21
 *
 * Description : Generic Monitor
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References :
 *
 ********************************************************************/
// System includes.

#include <iostream>
using namespace std;
#include <string>
#include <cmath>
#include <unistd.h>
#include <stdint.h>

// Local Includes.
#include "debug.h"
#include "CLogger.hh"
#include "Monitor.hh"
#include "DSA602.hh"
#include "Trace.hh"
#include "DefTrace.hh"

Monitor* Monitor::fMonitor = NULL;
/*
 * GPIB address of scope. (FIXME - move to pref file.
 */
const int Scope_GPIB_A = 5;

struct t_Station {
    const char* text;
    double    Distance;        // miles
    double    Freq;            // kHz
    double    SourcePower[2];  // Watts - Day/night
    double    Distance2Source; // meters
};
static struct t_Station AMStations[9] = {
    {"WMCA 570" , 38.2,  570.0, { 5000, 5000}, 64270.0},    // 0
    {"WFAN 660" , 30.4,  660.0, {50000,50000}, 50390.0},    // 1
    {"WOR 710"  , 35.1,  710.0, {50000,50000}, 59000.0},    // 2
    {"WABC 770" , 29.3,  770.0, {50000,50000}, 49720.0},    // 3
    {"WNBC 820" , 38.2,  820.0, {50000,50000}, 64270.0}, // ????
    {"WCBS 880" , 30.4,  880.0, {50000,50000}, 50510.0},    // 5
    {"WGNY 1220", 19.5, 1220.0, {10000.0, 180.0}, 30780.0},   // 6
    {"WLNA 1420",  1.4, 1420.0, {5000.0,1000.0} ,  2000.0},   // 7
    {NULL, 0.0, 0.0}};


/**
 ******************************************************************
 *
 * Function Name : Monitor constructor
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
Monitor::Monitor (void)
{
    SET_DEBUG_STACK;
    fMonitor = this;
    /*
     * Finally create scope object
     * Pass in GPIB address. 
     */
    fScope = new DSA602( Scope_GPIB_A);
    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : Monitor destructor
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
Monitor::~Monitor (void)
{
    SET_DEBUG_STACK;
    delete fScope;
}


/**
 ******************************************************************
 *
 * Function Name : Monitor function
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
double Monitor::GetPeakValue(double F, int N, const double *X, const double *Y)
{
    //const double  XPTS  = 2048.0;
    //const double  XINCR = 9.765625E+2;
    const int32_t window = 10;
    double dx;

    int32_t index=0;
    do 
    {
	dx = F - X[index];
	index++;
    } while((index<N) && (dx>0.0));

    int32_t   i;
    double    MaxdB = -96.0;
    uint32_t  IMax = 0;
    //double    Fpt;
    double dBm;
    for (i=index-window;i< index+window;i++)
    {
	//Fpt = X[i];
	dBm = Y[i];
	if (dBm > MaxdB)
	{
	    IMax  = i;
	    MaxdB = dBm;
	}
    }
    cout << "Imax: " << IMax
	 << " F: " << X[IMax]
	 << " MaxdB: " << MaxdB
	 << endl;
    return MaxdB;
}
#if 0
void* Monitor::function(const char *Name)
{
}
#endif
/**
 ******************************************************************
 *
 * Function Name : Do
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
void Monitor::Do(void)
{
    SET_DEBUG_STACK;
    //Trace *   pTrace  = fScope->GetTrace();
    uint8_t   Number  = 2; //fScope->GetSelectedTrace();
    //DefTrace* pDefT   = pTrace->GetDef(Number+1);
    double    *Y, *X;
    uint32_t  n;
    fRun = true;
    double    maxval;

    uint8_t index = 4;

    /*
     * While we are running, get a scope trace every so often. 
     */
    uint32_t seconds = 10 * 60; // sample every 10 minutes. 
    while(fRun)
    {
	n = fScope->Curve(Number, &X, &Y);
	if (n>0)
	{
	    maxval = GetPeakValue(AMStations[index].Freq, n, X, Y);  
	    cout << "Got points: " << n 
		 << " peak = " << maxval
		 << endl;
	}
	free(Y);
	free(X);
	// Set the sleep time to the sample rate. 
	sleep(1); //seconds);
    }
    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : OpenLogFile
 *
 * Description : Open and manage the HDF5 log file
 *
 * Inputs : none
 *
 * Returns : NONE
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on:  
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
bool Monitor::OpenLogFile(void)
{
    SET_DEBUG_STACK;
#if 0
    // USER TO FILL IN.
    const char *Names = "Time:Lat:Lon:Z:NSV:PDOP:HDOP:VDOP:TDOP:VE:VN:VZ";
    CLogger *pLogger = CLogger::GetThis();
    /* Give me a file name.  */
    const char* name = fn->GetUniqueName();
    fn->NewUpdateTime();
    SET_DEBUG_STACK;

    f5Logger = new H5Logger(name,"Main Logger Dataset", NVar, false);
    if (f5Logger->CheckError())
    {
	pLogger->Log("# Failed to open H5 log file: %s\n", name);
	delete f5Logger;
	f5Logger = NULL;
	return false;
    }
    f5Logger->WriteDataTags(Names);

    /* Log that this was done in the local text log file. */
    time_t now;
    char   msg[64];
    SET_DEBUG_STACK;
    time(&now);
    strftime (msg, sizeof(msg), "%m-%d-%y %H:%M:%S", gmtime(&now));
    pLogger->Log("# changed file name %s at %s\n", name, msg);

    /*
     * If the IPC is realized, put the current filename into it.
     */ 
    if (fIPC)
    {
	fIPC->UpdateFilename(name);
    }
    
    fChangeFile = false;
#endif
    return true;
}
/**
 ******************************************************************
 *
 * Function Name : ReadConfiguration
 *
 * Description : Open read the configuration file. 
 *
 * Inputs : none
 *
 * Returns : NONE
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on:  
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
bool Monitor::ReadConfiguration(void)
{
    SET_DEBUG_STACK;
#if 0
    CLogger *Logger = CLogger::GetThis();
    ClearError(__LINE__);
    Config *pCFG = new Config();

    /*
     * Open the configuragtion file. 
     */
    try{
	pCFG->readFile(fConfigFileName);
    }
    catch( const FileIOException &fioex)
    {
	Logger->LogError(__FILE__,__LINE__,'F',
			 "I/O error while reading configuration file.\n");
	return false;
    }
    catch (const ParseException &pex)
    {
	Logger->Log("# Parse error at: %s : %d - %s\n",
		    pex.getFile(), pex.getLine(), pex.getError());
	return false;
    }


    /*
     * Start at the top. 
     */
    const Setting& root = pCFG->getRoot();

    // USER TO FILL IN
    // Output a list of all books in the inventory.
    try
    {
	int    Debug;
	/*
	 * index into group MainModule
	 */
	const Setting &MM = root["MainModule"];
	MM.lookupValue("Logging",   fLogging);
	MM.lookupValue("Debug",     Debug);
	SetDebug(Debug);
    }
    catch(const SettingNotFoundException &nfex)
    {
	// Ignore.
    }

    delete pCFG;
    pCFG = 0;
#endif
    SET_DEBUG_STACK;
    return true;
}

/**
 ******************************************************************
 *
 * Function Name : WriteConfigurationFile
 *
 * Description : Write out final configuration
 *
 * Inputs : none
 *
 * Returns : NONE
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on:  
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
bool Monitor::WriteConfiguration(void)
{
    SET_DEBUG_STACK;
#if 0
    CLogger *Logger = CLogger::GetThis();
    ClearError(__LINE__);
    Config *pCFG = new Config();

    Setting &root = pCFG->getRoot();

    // USER TO FILL IN
    // Add some settings to the configuration.
    Setting &MM = root.add("MainModule", Setting::TypeGroup);
    MM.add("Debug",     Setting::TypeInt)     = 0;
    MM.add("Logging",   Setting::TypeBoolean)     = true;

    // Write out the new configuration.
    try
    {
	pCFG->writeFile(fConfigFileName);
	Logger->Log("# New configuration successfully written to: %s\n",
		    fConfigFileName);

    }
    catch(const FileIOException &fioex)
    {
	Logger->Log("# I/O error while writing file: %s \n",
		    fConfigFileName);
	delete pCFG;
	return(false);
    }
    delete pCFG;
#endif
    SET_DEBUG_STACK;
    return true;
}
