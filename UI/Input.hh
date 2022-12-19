/**
 ******************************************************************
 *
 * Module Name : Input.hh
 *
 * Author/Date : C.B. Lirakis / 04-Jan-15
 *
 * Input : Input of how trace functions. 
 *
 * Restrictions/Limitations :
 *
 * Change Inputs :
 *
 * Classification : Unclassified
 *
 * References : DSA602A Programming Reference Manual
 *
 * 
 *******************************************************************
 */
#ifndef __INPUT_h_
#define __INPUT_h_
#  include "DSA602_Types.hh"
#  include "Units.hh"
#  include "Channel.hh"

/*
 * Typical channel query data. page 272
 * 'TRACE1 DESCRIPTION:"R1 ON MAIN"
 * Page 294
 * 
 * Later add stored waveform, for the moment only the hardware.
 */

class Input {
public:
    enum TraceFunctions {kABS= 0 , kAVG, kCONVOLUTION, 
			 kCORRELATION, kDEJITTER,
			 kDELAY, kDIFF, kENV, kEXP, kFFTIMAG, kFFTMAG, 
			 kFFTPHASE,
			 kFFTREAL, kFILTER, kIFFT, kINTG, kINTP, kLN, kLOG, 
			 kPIADD, kPISUB, kPULSE, kSIGNUM, kSMOOTH, kSQRT,
			 kFUNCTION_NONE};

    enum TraceOperators { kPLUS=0, kMINUS, kMULTIPLY, kDIVIDE, 
			  kOPERATOR_NONE };

    Input(const char *val=NULL);
    ~Input();

    friend ostream& operator<<(ostream& output, const Input &n);

    void DecodeString(const char *val);

    // FIXME
    //const char*   sInput(void) const {return fModule->ChannelString();};
    const char*   sFunction(bool Y=false) const;
    const char*   sOperator(void) const;
    const char*   sString(void) const;

    // Strings defining inputs etc
    static const char* FString[];
    static const char* OString[];
    static const char  cop[4];

private:
    void DecodeFunction(const char* str);
    void DecodeOperator(const char* str);
    // FIXME
    //ModuleChannel* fModule;   // Module and channel Ln,Cn,Rn
    TraceFunctions fFunction;
    TraceOperators fOperator;
};
#endif
