/**
 ******************************************************************
 *
 * Module Name : SHO.hh
 *
 * Author/Date : C.B. Lirakis / 15-Nov-06
 *
 * Description : Simple harmonic oscillator class for studies. 
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
 *
 *******************************************************************
 */
#ifndef __SHO_hh_
#define __SHO_hh_
#include <complex>

class SimpleHarmonicOscillator 
{
public:
    /// Base constructor
    SimpleHarmonicOscillator(double Amplitude = 1.0,
			     double Omega=1.0, 
			     double Beta=0.0);
    ~SimpleHarmonicOscillator();

    complex <double> Evaluate(double t);

private:
    /// Basic constants associated with harmonic oscillator
    double Amplitude;
    double gamma;     // Damping
    double omega;     // Oscillation frequency. 
};

#endif
