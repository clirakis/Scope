/********************************************************************
 *
 * Module Name : filter.cpp
 *
 * Author/Date : C.B. Lirakis / 23-May-21
 *
 * Description : Generic filter
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

// Local Includes.
#include "debug.h"
#include "filter.hh"

/**
 ******************************************************************
 *
 * Function Name : filter constructor
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
Filter::Filter (const Filt &p) : Filt(p)
{
    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : filter
 *
 * Description : Apply the results
 *
 * Inputs : data vector to process. 
 *
 * Returns : resulting filtered data
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 22-Jan-23 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
vector<double> Filter::Do(vector<double>x)
{
    uint32_t len_x = x.size();
    uint32_t len_b = fNumCoeffs.size();
    uint32_t len_a = fDenomCoeffs.size();

    // working variables. 
    vector<double> zi(len_b);
    vector<double> filter_x(len_x);

    // Vastly different
    if (len_a == 1)
    {
	for (uint32_t m = 0; m<len_x; m++)
	{
	    filter_x[m] = fNumCoeffs[0] * x[m] + zi[0];
	    for (uint32_t i = 1; i<len_b; i++)
	    {
		zi[i-1] = fNumCoeffs[i] * x[m] + zi[i];//-fDenomCoeffs[i]*filter_x[m];
	    }
	}
    }
    else
    {
	for (uint32_t m = 0; m<len_x; m++)
	{
	    filter_x[m] = fNumCoeffs[0] * x[m] + zi[0];
	    for (uint32_t i = 1; i<len_b; i++)
	    {
		zi[i-1] = fNumCoeffs[i] * x[m] + zi[i] - fDenomCoeffs[i] * filter_x[m];
	    }
	}
    }
    return filter_x;
}
