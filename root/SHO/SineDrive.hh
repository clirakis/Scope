/**
 ******************************************************************
 *
 * Module Name : SineDrive.hh
 *
 * Author/Date : C.B. Lirakis / 19-Oct-07
 *
 * Description : Sine driven HO
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
#ifndef __SINEDRIVE_hh_
#define __SINEDRIVE_hh_

class SineDrive
{
public:
    /// Base constructor
    SineDrive(double Amplitude = 1.0, double omega=1.0, double Beta=0.0, 
	      double F=1.0, double Omega = 1.0);
    ~SineDrive();

    double Evaluate(double t);

private:
    /// Basic constants associated with harmonic oscillator
    double Amplitude;
    double gamma;     // Damping
    double omega_0;   // Oscillation frequency. 
    double OmegaF;    // Forcing frequency
    double F;         // Forcing amplitude

    // Intermediate results
    double c1, c2, phi1, phi2, omega;
    double A1, A2;
};

#endif
