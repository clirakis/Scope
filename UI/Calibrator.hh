/**
 ******************************************************************
 *
 * Module Name : Calibrator.hh
 *
 * Author/Date : C.B. Lirakis / 26-Jan-14
 *
 * Description : Calibrator, stores data on a per channel basis
 * Data for various plug ins:
 *        Sensitivity Range  Offset Range
 *        -----------------  ------------
 * 11A32    1e-3:99.5e-3        -1:1
 * 11A34  100e-3:995e-3        -10:10 
 * 11A52       1:10           -100:100 
 *
 * 11A71 single ended amplifier: range = -10*(SEN):10*(SEN)
 * 11A72 single ended amplifier: range = -25*(SEN):25*(SEN)
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
#ifndef __CHANNEL_h_
#define __CHANNEL_h_
#  include "DSA602_Types.hh"


/*
 * Typical channel query data. issued the command CH? to get this data.
 * 
 * CHL1 MNSCOUPLING:DC,PLSCOUPLING:DC,PROTECT:OFF,OFFSET:0.0E+0,AMPOFFSET:0.0E+0,BW:2.0E+7,IMPEDANCE:5.0E+1,MNSOFFSET:0.0E+0,MNSPROBE:"NONE",PLSOFFSET:0.0E+0,PLSPROBE:"NONE",SENSITIVITY:1.0E+0,UNITS:"VOLTS",VCOFFSET:0.0E+0;CHC1 MNSCOUPLING:DC,PLSCOUPLING:DC,PROTECT:ON,OFFSET:0.0E+0,AMPOFFSET:0.0E+0,BW:1.5E+8,IMPEDANCE:1.0E+6,MNSOFFSET:0.0E+0,MNSPROBE:"NONE",PLSOFFSET:0.0E+0,PLSPROBE:"NONE",SENSITIVITY:1.0E+0,UNITS:"VOLTS",VCOFFSET:0.0E+0;CHR1 COUPLING:DC,OFFSET:2.35E-1,BW:4.0E+8,IMPEDANCE:1.0E+6,PROBE:"NONE",SENSITIVITY:1.0E-1,UNITS:"VOLTS";CHR2 COUPLING:DC,OFFSET:0.0E+0,BW:4.0E+8,IMPEDANCE:1.0E+6,PROBE:"NONE",SENSITIVITY:1.0E+0,UNITS:"VOLTS
 * 
 * looks like channels are seperated by ;
 * data seperated by commas
 * data label and value seperated by :
 * 
 * There are 3 possible channel slots (P)osition: Left, Center and Right 
 * then (N)umber 
 * CH(P)(N)
 */

class Calibrator {
public:
    enum COMMANDs { AMPOFFSET, BW, BWHI, BWLO, CCOUPLING, CIMPEDANCE, 
		    MNSCOUPLING, MNSOFFSET, MNSPROBE, OFFSET, PLSCOUPLING, 
		    PLSOFFSET, PLSPROBE, PROBE, PROTECT, 
		    SENSITIVITY, UNITS, VCOFFSET};

    Calibrator(const char *val);
    ~Calibrator();

    friend ostream& operator<<(ostream& output, const Calibrator &n);
 
    static struct t_Commands CalibratorCommands[18];

    void DecodeString(const char *val);

    // Data access functions;
    inline double AMPLitude()         const {return fAMPLitude;};
    inline double FREquency()         const {return fFREq;};
    inline double IMPedance()         const {return fIMPdeance;};

private:


protected:
    // Page 71

    double    fAMPLitude;
    double    fFREq;
    double    fIMPedance;


};

#endif
