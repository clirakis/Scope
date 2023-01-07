/**
 ******************************************************************
 *
 * Module Name : Monitor.h
 *
 * Author/Date : C.B. Lirakis / 06-Jan-23
 *
 * Description : Monitor a series of Radio stations and watch for the 
 *               variations as a function of time. 
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
#ifndef __MONITOR_hh_
#define __MONITOR_hh_

class DSA602;
class TFile;
class TNtupleD;
class FileName;
class TObjString;
class TEnv;

/// Monitor documentation here. 
class Monitor {
public:
    /// Default Constructor
    Monitor(void);
    /// Default destructor
    ~Monitor(void);

    inline static Monitor* GetThis(void) {return fMonitor;};
    /// Monitor function
    /*!
     * Description: 
     *   
     *
     * Arguments:
     *   
     *
     * Returns:
     *
     * Errors:
     *
     */
    void Do(void);


    inline void Stop(void) {fRun = false;};

private:

    bool   ReadConfiguration(void);
    bool   WriteConfiguration(void);
    bool   SetupRoot(void);
    void   CloseRoot(void);
    void   FindFrequencyIndex(double F);
    double GetPeakValue(void);
    double GetPeakArea(void); // input is frequency to scan. 

    bool        fRun;       // is the loop running
    DSA602*     fScope;     // handle to DSA602 scope. 

    int32_t     fN;         // Number of samples. 
    double      *fX, *fY;   // Frequency and Amplitude in (dBm) respectively.
    int32_t     fIndex;     // frequency index near where we want to look
    int32_t     fMaxIndex;  // frequency index of found best value in window. 
 
    TFile*      fTFile;      // Root file descriptor
    TNtupleD*   fNtuple;     // pointer to open ntuple for data recording
    TObjString* fComments;   // Comments on run.
    TEnv*       fEnv;        // Environment variables for setup. 

    // Create unique file names. 
    FileName*   fName;

    // Run time parameters
    uint32_t  fSeconds;       // how long between samples in seconds
    uint32_t  fSampleTimeout; // how long should the program run? -1 indefinite
    int32_t   fAM_Index;      // Which station to monitor?

    static Monitor* fMonitor;
};
#endif
