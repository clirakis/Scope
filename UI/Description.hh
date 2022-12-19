/**
 ******************************************************************
 *
 * Module Name : Description.hh
 *
 * Author/Date : C.B. Lirakis / 04-Jan-15
 *
 * Description : Description of how trace functions. 
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References : DSA602A Programming Reference Manual
 *
 * 
 *******************************************************************
 */
#ifndef __DESCRIPTION_h_
#define __DESCRIPTION_h_
#  include "DSA602_Types.hh"
#  include "Units.hh"
#  include "Input.hh"

/*
 * Typical channel query data. page 272
 * 'TRACE1 DESCRIPTION:"R1 ON MAIN"
 * Page 294
 * 
 * Later add stored waveform, for the moment only the hardware.
 */

class Description {
public:
    enum TraceWindows   {MAIN_WINDOW=0, WINDOW_1, WINDOW_2, WINDOW_NONE};

    Description(const char *val=NULL);
    ~Description();

    friend ostream& operator<<(ostream& output, const Description &n);

    void DecodeString(const char *val);

    inline bool   IsTimeTrace(void) const {return fTime;};

    const char*   sInput(void) const;
    const char*   sWindow(void) const;
    const char*   sString(void) const;

    static const char* WString[];
private:
    void DecodeTimeWindow(const char *s);

    bool           fTime;     // true - Y vs Time "L1 ON MAIN", false Y VS X
    TraceWindows   fWindow;
    Input*         fX;
    Input*         fY;
};
#endif
