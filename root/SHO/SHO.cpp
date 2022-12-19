/**
 ******************************************************************
 *
 * Module Name : SHO.cpp
 *
 * Author/Date : C.B. Lirakis / 15-Nov-06
 *
 * Description : Simple Harmonic Oscillator class. 
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
#include "SHO.hh"

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
SimpleHarmonicOscillator::SimpleHarmonicOscillator(double A,
						   double O, double B)
{
    Amplitude = A;
    omega     = O;
    gamma     = B;
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
SimpleHarmonicOscillator::~SimpleHarmonicOscillator()
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
complex <double> SimpleHarmonicOscillator::Evaluate(double t)
{
    complex <double> ans;
    complex < double> omi(0.0, omega);
    ans = Amplitude * exp( (omi + gamma)*t);
    return ans;
}
