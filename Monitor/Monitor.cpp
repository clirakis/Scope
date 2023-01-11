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

// Root includes for logging the data. 
#include <TROOT.h>
#include <TNtupleD.h>
#include <TFile.h>
#include <TObjString.h>
#include <TEnv.h>

// Local Includes.
#include "debug.h"
#include "CLogger.hh"
#include "Monitor.hh"
#include "DSA602.hh"
#include "Trace.hh"
#include "DefTrace.hh"
#include "filename.hh"

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
static struct t_Station AMStations[10] = {
    {"WMCA 570" , 38.2,  570.0, { 5000, 5000}, 64270.0},    // 0
    {"WFAN 660" , 30.4,  660.0, {50000,50000}, 50390.0},    // 1
    {"WOR 710"  , 35.1,  710.0, {50000,50000}, 59000.0},    // 2
    {"WABC 770" , 29.3,  770.0, {50000,50000}, 49720.0},    // 3
    {"WNBC 820" , 38.2,  820.0, {50000,50000}, 64270.0}, // ????
    {"WCBS 880" , 30.4,  880.0, {50000,50000}, 50510.0},    // 5
    {"WGNY 1220", 19.5, 1220.0, {10000.0, 180.0}, 30780.0},   // 6
    {"WRVP 1310", 12.5, 1310.0, {5000.0, 33.0},   25300.0},   // 7
    {"WLNA 1420",  1.4, 1420.0, {5000.0,1000.0} ,  2000.0},   // 8
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
    char comments[256];

    fMonitor = this;
    /*
     * Finally create scope object
     * Pass in GPIB address. 
     */
    fScope = new DSA602( Scope_GPIB_A);

    /*
     * Create a filename class and get a unique filename. 
     */
    const struct timespec OneDay = {24*3600L, 0L};
    fName = new FileName("AMonitor", "root", OneDay);

    /* setup a root preferences file. */
    ReadConfiguration();

    /*
     * Setup user logging in a root ntuple. 
     *
     *
     * Initialize Root package.
     * We don't really need to track the return pointer. 
     * We just need to initialize it. 
     */
    ::new TROOT("Statons","AM station Data analysis");
    /*
     * make some notes in the logger before we actually run. 
     */

    sprintf( comments,"Sample rate: %d, Timeout: %d, AM index %d", 
	     fSeconds, fSampleTimeout, fAM_Index);
    fComments = new TObjString(comments);
    CLogger::GetThis()->Log("# %s\n", comments);

    SetupRoot();


    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : Monitor destructor
 *
 * Description : clean up the resources we allocated
 *
 * Inputs : NONE
 *
 * Returns : NONE
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 07-Jan-23
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
Monitor::~Monitor (void)
{
    SET_DEBUG_STACK;
    WriteConfiguration();
    delete fEnv;
    delete fScope;
    CloseRoot();
    delete fName;
    delete fNtuple;
}

/**
 ******************************************************************
 *
 * Function Name : SetupRoot
 *
 * Description : Creat all the root elements. 
 *
 * Inputs : NONE
 *
 * Returns : NONE
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 07-Jan-23
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
bool Monitor::SetupRoot (void)
{
    SET_DEBUG_STACK;

    /* Connect to a file. */
    char *name    = (char *) fName->GetUniqueName();
    fName->NewUpdateTime();

    CLogger::GetThis()->Log("# root Filename: %s\n", name); 

    /* Open a TFile. */
    fTFile  = new TFile( name, "RECREATE", "AM Station data analysis");

    /*
     * Setup NTUPLE
     * These are the names of the variables that we want to monitor.
     * indexes
     * WFAN - 1
     * WCBS - 5
     * WRVP - 7
     * WLNA - 8
     */
    char *raw_names = (char *)"Index:Time:nTime:FMax:dBMax:dBArea:WFAN:WCBS:WRVP:WLNA";
    /*
     * Create Ntuple.
     */
    fNtuple = new TNtupleD("Stations","AM Stations ntuple",raw_names);
    SET_DEBUG_STACK;
    return true;
}
/**
 ******************************************************************
 *
 * Function Name : CloseRoot
 *
 * Description : close out the current TFile 
 *
 * Inputs : NONE
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
void Monitor::CloseRoot (void)
{
    SET_DEBUG_STACK;
    DSA602*      scope  = DSA602::GetThis();
    Trace *      pTrace = scope->GetTrace();
    DSAFFT*      pFFT   = scope->GetFFT();
    char         Response[1024];  // this needs to be big. 


    /*
     * Get the index into the trace array.
     */
    uint8_t Number = pTrace->GetSelectedTrace();
    DefTrace* pDefT   = pTrace->GetDef(Number);

    // Save all objects in this file
    fComments->Write("Comments");

    string tmp(scope->GetWFMPRE()->Text());

    /*
     * tmp describes the waveform. 
     * lets also get all the data about the mainframe configuration. 
     */
    if (scope->Command("CH?", Response, sizeof(Response)))
    {
	// This should work and have all the mainframe config in it.
	TObjString Channel(Response);
	Channel.Write("Channel");
    }
    /*
     * Save the Waveform preamble with this. 
     */
    TObjString WFM(tmp.c_str());
    WFM.Write("WFMPRE");
    
    /*
     * And the same for the FFT data. 
     */
    pFFT->Update();
    TObjString FFT(pFFT->Text().c_str());
    FFT.Write("FFT");

    fTFile->Write();
    fTFile->Close();
    delete fTFile;

    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : GetPeakValue
 *
 * Description : Get the peak amplitude in dBm for the FFT at the 
 *               or around the specified frequency. 
 *
 * Inputs : F - frequency to look around. 
 *
 * Returns : the maxiumum value
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 06-Jan-23
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
double Monitor::GetPeakValue(void)
{
    SET_DEBUG_STACK;
    //const double  XPTS  = 2048.0;
    //const double  XINCR = 9.765625E+2;
    const int32_t window = 10;
    int32_t   i;
    double    MaxdB = -96.0;
    double    dBm;
    /*
     * look in a specified window. 
     */
    fMaxIndex = 0;
    for (i=fIndex-window;i< fIndex+window;i++)
    {
	if (i>-1)
	{
	    dBm = fY[i];
	    if (dBm > MaxdB)
	    {
		fMaxIndex  = i;
		MaxdB = dBm;
	    }
	}
    }
    SET_DEBUG_STACK;
    return MaxdB;
}
/**
 ******************************************************************
 *
 * Function Name : GetPeakArea
 *
 * Description : Get the peak normalized Window Area
 *               around the specified frequency. 
 *
 * Inputs : F - frequency to look around. 
 *
 * Returns : the maxiumum value
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 07-Jan-23
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
double Monitor::GetPeakArea(void)
{
    SET_DEBUG_STACK;
    const int32_t window = 5; // kind of +/- 5kHz
    int32_t   i;
    double    TotaldB = 0.0;

    /*
     * look in a specified window. 
     */
    for (i=fIndex-window;i< fIndex+window;i++)
    {
	if (i>-1)
	{
	    TotaldB += fY[i];
//	    cout << "index: " << i << " value: " << fY[i] << endl;
	}
    }
    
    TotaldB = TotaldB/ (2.0*(double)window-1);
    SET_DEBUG_STACK;
    return TotaldB;
}
/**
 ******************************************************************
 *
 * Function Name : FindFrequencyIndex
 *
 * Description : What is the index into the fX array that closely 
 *               matches what frequency we desire?
 *
 * Inputs : F - desired frequency in Hz. 
 *
 * Returns : none
 *
 * Error Conditions : none
 * 
 * Unit Tested on: 06-Jan-23
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void Monitor::FindFrequencyIndex(double F)
{
    SET_DEBUG_STACK;
    double dx = 0.0;
    /*
     * Get a pointer into the frequency vector. 
     * This won't change often. Like only when the program parameters 
     * are changed.  
     *
     * Look for when the difference goes negative. 
     */
    fIndex = 0;
    do 
    {
	dx = F - fX[fIndex];
	fIndex++;
    } while((fIndex<fN) && (dx>0.0));
    fIndex--;   // we went one too far. 

    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : Do
 *
 * Description : perform a loop to acquire and log data regarding
 *               the local AM radio stations. Samples on kSeconds
 *               and finishes after kSampleTimeout seconds have elapsed
 *
 * Inputs : NONE
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
void Monitor::Do(void)
{
    SET_DEBUG_STACK;
    CLogger*        log     = CLogger::GetThis();
    Trace *         pTrace  = fScope->GetTrace();
    double          maxval, Area;
    struct timespec now;
    double          sec, nsec;
    double          TIndex = 0.0;
    uint32_t        SampleTimeSeconds = 0;
    Double_t        Vec[10];

    /* If we are prompted for a file name change. do it. */
    if (fName->ChangeNames())
    {
	log->LogTime("Change names.\n");
	CloseRoot();
	SetupRoot();
    }

    pTrace->SetSelectedTrace(1);  // FFT should be trace 2. 

    /*
     * Get a pointer into the frequency vector. 
     * This won't change often. Like only when the program parameters 
     * are changed.  
     */
    fN     = fScope->Curve(&fX, &fY);

    FindFrequencyIndex(AMStations[fAM_Index].Freq*1.0e3);
    CLogger::GetThis()->Log("# Frequency bin chosen: %f\n", fX[fIndex]);

    /* Loop for a long time. */
    fRun = true;
    while(fRun)
    {
	clock_gettime(CLOCK_REALTIME, &now);
	nsec = 1.0e-9*(double)now.tv_nsec;
	sec  = (double) now.tv_sec;

	// Get the curve value. 
	fN = fScope->Curve(&fX, &fY);
	if (fN>0)
	{
	    /*
	     * Find the index to look for the maximum power received. 
	     */
	    FindFrequencyIndex(AMStations[fAM_Index].Freq*1.0e3);
	    maxval = GetPeakValue();
	    Area   = GetPeakArea();
	    Vec[0] = TIndex;
	    Vec[1] = sec;
	    Vec[2] = nsec;
	    Vec[3] = fX[fMaxIndex];
	    Vec[4] = maxval;
	    Vec[5] = Area;

	    FindFrequencyIndex(AMStations[1].Freq*1.0e3);
	    Vec[6] = GetPeakValue();

	    FindFrequencyIndex(AMStations[5].Freq*1.0e3);
	    Vec[7] = GetPeakValue();

	    FindFrequencyIndex(AMStations[7].Freq*1.0e3);
	    Vec[8] = GetPeakValue();

	    FindFrequencyIndex(AMStations[8].Freq*1.0e3);
	    Vec[9] = GetPeakValue();

	    fNtuple->Fill(Vec);
	    TIndex = TIndex + 1.0;
	}
	 
	free(fY);
	free(fX);
	/*
	 * How much time has elapsed, roughtly? 
	 * if SampleTimeout <0 then indefinite.
	 */
	SampleTimeSeconds = SampleTimeSeconds + fSeconds;
	if ((SampleTimeSeconds > fSampleTimeout) && (fSampleTimeout>0))
	{
	    log->LogTime("Done sampling.\n");
	    fRun = false;
	}
	// Set the sleep time to the sample rate. 
	sleep(fSeconds); //seconds);
    }
    SET_DEBUG_STACK;
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
 * Unit Tested on:  07-Jan-23
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
bool Monitor::ReadConfiguration(void)
{
    SET_DEBUG_STACK;
    CLogger *log = CLogger::GetThis();

    fEnv = new TEnv(".AMonitor");
    log->Log("# AMonitor Loading prefs: %s \n", fEnv->GetRcName());

    int32_t v = fEnv->GetValue("AMonitor.Verbose", 0);
    log->SetVerbose(v);

    // Sample rate, once every 60 seconds 
    fSeconds = fEnv->GetValue("AMonitor.SampleRate", 60);
    // Stop program after one day of seconds. 
    fSampleTimeout = fEnv->GetValue("AMonitor.Timeout", 86400);
    // Monitor station index 5 (AM 880)
    fAM_Index = fEnv->GetValue("AMonitor.AMStation", 5);

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
 * Unit Tested on:  07-Jan-23
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
bool Monitor::WriteConfiguration(void)
{
    SET_DEBUG_STACK;
    CLogger *log = CLogger::GetThis();

    log->Log("# AMonitor: Write Configuration.\n");
    fEnv->SetValue("AMonitor.Verbose"   , (Int_t) log->GetVerbose());
    fEnv->SetValue("AMonitor.SampleRate", (Int_t) fSeconds);
    fEnv->SetValue("AMonitor.Timeout"   , (Int_t) fSampleTimeout);
    fEnv->SetValue("AMonitor.AMStation" , (Int_t) fAM_Index);
    fEnv->SaveLevel(kEnvUser);

    SET_DEBUG_STACK;
    return true;
}
