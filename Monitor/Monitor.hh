/**
 ******************************************************************
 *
 * Module Name : Monitor.h
 *
 * Author/Date : C.B. Lirakis / 23-May-21
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
#ifndef __MONITOR_hh_
#define __MONITOR_hh_

class DSA602;

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

    bool OpenLogFile(void);

    inline void Stop(void) {fRun = false;};

private:

    bool ReadConfiguration(void);
    bool WriteConfiguration(void);
    double GetPeakValue(double F, int N, const double *X, const double *Y);
    bool    fRun;
    DSA602* fScope;


    static Monitor* fMonitor;
};
#endif
