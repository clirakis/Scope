/**
 ******************************************************************
 *
 * Module Name : SineDrive.cpp
 *
 * Author/Date : C.B. Lirakis / 19-oct-06
 *
 * Description : Sine driven harmonic oscillator
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

#ifndef lint
/// RCS Information
static char rcsid[]="$Header$";
#endif

// System includes.
#include <iostream>
using namespace std;
#include <string>
#include <cmath>
#include <csignal>
#include <unistd.h>

/// Local Includes.
#include "debug.h"
#include "SineDrive.hh"

/**
 ******************************************************************
 *
 * Function Name :
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
SineDrive::SineDrive(double A, double O, double B, double FA, double OM)
{
    double term1, term2;

    Amplitude = A;
    omega_0   = O;
    gamma     = B;
    F         = FA;
    OmegaF    = OM;

    term1 = omega_0*omega_0 + OmegaF*OmegaF;
    term2 = 4.0* gamma*gamma *OmegaF*OmegaF + term1*term1;

    omega = sqrt( omega_0*omega_0 + OmegaF*OmegaF);

    c1 = -2.0 * F * gamma * OmegaF/term2;
    c2 = -F * OmegaF/omega * (term1 + 2.0*gamma*gamma)/term2;

    phi1 = atan2(c1,c2);
    phi2 = atan2( 2.0*gamma*OmegaF, term1);

    A1 = sqrt(c1*c1 + c2*c2);
    A2 = F/sqrt(term2);
}

/**
 ******************************************************************
 *
 * Function Name :
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
SineDrive::~SineDrive()
{
}
/**
 ******************************************************************
 *
 * Function Name :
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
double SineDrive::Evaluate(double t)
{
    double ans;
    ans = A1 * exp(-gamma*t)* sin(omega*t+phi1) + A2 * sin(OmegaF*t + phi2);
    return ans;
}
