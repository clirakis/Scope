/********************************************************************
 *
 * Module Name : Butterworth.cpp
 *
 * Author/Date : C.B. Lirakis / 15-Jan-23
 *
 * Description : Generic Butterworth
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References :
 * https://github.com/KBaur/FiltFilt
 * https://github.com/nxsEdson/Butterworth-Filter/blob/master/butterworth.cpp
 * https://github.com/scipy/scipy/tree/main/scipy/signal
 * https://github.com/KBaur/FiltFilt
 * https://github.com/ruohoruotsi/Butterworth-Filter-Design/blob/master/Butterworth.cpp
 * https://github.com/scipy/scipy/blob/dde50595862a4f9cede24b5d1c86935c30f1f88a/scipy/signal/_filter_design.py
 * 
 * https://github.com/scipy/scipy/tree/main/scipy/signal
 *
 ********************************************************************/
// System includes.
#include <iostream>
using namespace std;
#include <string>
#include <cmath>

// Local Includes.
#include "debug.h"
#include "Butterworth.hh"

/**
 ******************************************************************
 *
 * Function Name : Butterworth constructor
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
Butterworth::Butterworth (uint16_t FilterOrder, double Lcutoff, double Ucutoff)
{
    SET_DEBUG_STACK;
    fFilterOrder = FilterOrder;
    fLowerCutoff = Lcutoff;
    fUpperCutoff = Ucutoff;

    ComputeDenCoeffs();
    ComputeNumCoeffs();

    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : Butterworth destructor
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
Butterworth::~Butterworth (void)
{
    SET_DEBUG_STACK;
    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : ComputeDenCoeffs
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
void Butterworth::ComputeDenCoeffs(void)
{
    SET_DEBUG_STACK;
    uint16_t k;                 // loop variable

    vector<double> RCoeffs;     // z^-2 coefficients 
    vector<double> TCoeffs;     // z^-1 coefficients

    double PoleAngle;           // pole angle
    double SinPoleAngle;        // sine of pole angle
    double CosPoleAngle;        // cosine of pole angle
    double a;                   // workspace variable

    double cp    = cos(M_PI * (fUpperCutoff + fLowerCutoff) / 2.0);
    double theta = M_PI * (fUpperCutoff - fLowerCutoff) / 2.0;
    double st    = sin(theta);
    double ct    = cos(theta);
    double s2t   = 2.0*st*ct;        // sine of 2*theta
    double c2t   = 2.0*ct*ct - 1.0;  // cosine of 2*theta

    for (k = 0; k < fFilterOrder; ++k)
    {
	PoleAngle = M_PI * (double)(2*k+1)/((double)(2*fFilterOrder));
	SinPoleAngle = sin(PoleAngle);
	CosPoleAngle = cos(PoleAngle);
	a = 1.0 + s2t*SinPoleAngle;
	RCoeffs.push_back(c2t/a);
	RCoeffs.push_back(s2t*CosPoleAngle/a);
	TCoeffs.push_back(-2.0*cp*(ct + st*SinPoleAngle)/a);
	TCoeffs.push_back(-2.0*cp*st*CosPoleAngle/a);
    }

    fDenomCoeffs = TrinomialMultiply(TCoeffs, RCoeffs);

    fDenomCoeffs[1] = fDenomCoeffs[0];
    fDenomCoeffs[0] = 1.0;
    for (k = 3; k<= 2*fFilterOrder; ++k)
    {
	fDenomCoeffs[k] = fDenomCoeffs[2*k-2];
    }

    // Get rid of the extra space used in the calculation. 
    for (uint16_t i = fDenomCoeffs.size(); i>(2*fFilterOrder+1);i--)
    {
	fDenomCoeffs.pop_back();
    }
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : ComputeNumCoeffs
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void Butterworth::ComputeNumCoeffs(void)
{
    SET_DEBUG_STACK;
    vector<double> TCoeffs;
    //vector<double> NumCoeffs(2*FilterOrder+1);
    // https://en.cppreference.com/w/cpp/numeric/complex
    vector<complex<double>> NormalizedKernel(2*fFilterOrder+1);

    vector<double> Numbers;
    for (uint16_t n = 0; n < fFilterOrder*2+1; n++)
    {
	Numbers.push_back((double)n);
    }

    TCoeffs = ComputeHP();

    for (uint16_t i = 0; i < fFilterOrder; ++i)
    {
	fNumCoeffs.push_back(TCoeffs[i]);
	fNumCoeffs.push_back(0.0);
    }
    fNumCoeffs[2*fFilterOrder] = TCoeffs[fFilterOrder];

    double cp[2];
    cp[0] = 4.0*tan(M_PI_2*fLowerCutoff);
    cp[1] = 4.0*tan(M_PI_2*fUpperCutoff);
    //cout << "Lcutoff: " << M_PI_2*fLowerCutoff << " " << cp[0] << endl;
    //double Bw = cp[1] - cp[0];

    //center frequency
    double Wn = sqrt(cp[0] * cp[1]);
    Wn = 2 * atan2(Wn, 4);

    const std::complex<double> result(-1, 0);

    for (int k = 0; k<fFilterOrder* 2+1; k++)
    {
	NormalizedKernel[k] = exp(-sqrt(result)*Wn*Numbers[k]);
    }

    double b   = 0;
    double den = 0;
    for (uint16_t d = 0; d<fFilterOrder*2+1; d++)
    {
	b   += real(NormalizedKernel[d] * fNumCoeffs[d]);
	den += real(NormalizedKernel[d] * fDenomCoeffs[d]);
    }

    for (int c = 0; c < fFilterOrder*2+1; c++)
    {
	fNumCoeffs[c] = (fNumCoeffs[c] * den)/b;
    }

    for (int i = fNumCoeffs.size()- 1; i > fFilterOrder*2+1; i--)
    {
	fNumCoeffs.pop_back();
    }

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
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
vector<double> Butterworth::filter(vector<double>x)
{
    uint32_t len_x = x.size();
    uint32_t len_b = fNumCoeffs.size();
    uint32_t len_a = fDenomCoeffs.size();

    vector<double> zi(len_b);
    vector<double> filter_x(len_x);

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

/**
 ******************************************************************
 *
 * Function Name : TrinomialMultiply
 *
 * Description :
 *
 * Inputs : vectors to perform multiplicaiton on
 *
 * Returns : resulting vector
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
vector<double> Butterworth::TrinomialMultiply(vector<double> b, 
					      vector<double> c)
{
    SET_DEBUG_STACK;
    int i, j;
    vector<double> RetVal(4 * fFilterOrder);

    RetVal[2] = c[0];
    RetVal[3] = c[1];
    RetVal[0] = b[0];
    RetVal[1] = b[1];

    for (i = 1; i<fFilterOrder; ++i)
    {
	RetVal[2*(2*i+1)] += c[2*i]*RetVal[2*(2*i-1)]-c[2*i+1]*RetVal[2*(2*i-1)+1];
	RetVal[2*(2*i+1)+1] += c[2*i]*RetVal[2*(2*i-1)+1]+c[2*i+1]*RetVal[2*(2*i-1)];

	for (j = 2*i; j>1; --j)
	{
	    RetVal[2*j] += b[2*i]*RetVal[2*(j-1)]-b[2*i+1]*RetVal[2*(j-1)+1] +
		c[2*i]*RetVal[2*(j-2)]-c[2*i+1]*RetVal[2*(j-2)+1];
	    RetVal[2*j+1] += b[2*i]*RetVal[2*(j-1)+1]+b[2*i+1]*RetVal[2*(j-1)]+
		c[2*i]*RetVal[2*(j-2)+1]+c[2*i+1]*RetVal[2*(j-2)];
	}

	RetVal[2] += b[2*i]*RetVal[0]-b[2*i+1]*RetVal[1]+c[2*i];
	RetVal[3] += b[2*i]*RetVal[1]+b[2*i+1]*RetVal[0]+c[2*i+1];
	RetVal[0] += b[2*i];
	RetVal[1] += b[2*i+1];
    }

    SET_DEBUG_STACK;
    return RetVal;
}

/**
 ******************************************************************
 *
 * Function Name : ComputeLP
 *
 * Description : Analog lowpass filter coefficients. 
 *
 * Inputs : NONE
 *
 * Returns : vector of coefficients
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
vector<double> Butterworth::ComputeLP(void)
{
    SET_DEBUG_STACK;
    vector<double> NumCoeffs(fFilterOrder + 1);
    int m;

    NumCoeffs[0] = 1;
    NumCoeffs[1] = fFilterOrder;
    m = fFilterOrder/2;
    for (uint16_t i = 2; i <= m; ++i)
    {
	NumCoeffs[i] = (double)(fFilterOrder - i + 1)*NumCoeffs[i - 1] / i;
	NumCoeffs[fFilterOrder-i] = NumCoeffs[i];
    }
    NumCoeffs[fFilterOrder-1] = fFilterOrder;
    NumCoeffs[fFilterOrder]   = 1;

    SET_DEBUG_STACK;
    return NumCoeffs;
}

/**
 ******************************************************************
 *
 * Function Name : ComputeHP
 *
 * Description :
 *
 * Inputs : None
 *
 * Returns : Vector of coefficients
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
vector<double> Butterworth::ComputeHP(void)
{
    SET_DEBUG_STACK;
    vector<double> NumCoeffs;

    // Returns a vector of doubles. 
    NumCoeffs = ComputeLP();

    for (uint16_t i = 0; i <= fFilterOrder; ++i)
    {
	if (i % 2) NumCoeffs[i] = -NumCoeffs[i];
    }
    SET_DEBUG_STACK;
    return NumCoeffs;
}


/**
 ******************************************************************
 *
 * Function Name : Butterworth << operator
 *
 * Description : format all the channel information for output
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 27-Nov-14
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
ostream& operator<<(ostream& output, const Butterworth &n)
{
    SET_DEBUG_STACK;
    output << "============================================" << endl
	   << "    Filter Order: " << n.fFilterOrder << endl
	   << "    Lower Cutoff: " << n.fLowerCutoff << endl
	   << "    Upper Cutoff: " << n.fUpperCutoff << endl;
    output << "Denominator: ";
    for (uint16_t i=0;i<n.fDenomCoeffs.size(); i++)
    {
	output << n.fDenomCoeffs[i] << ",";
    }
    output << endl;
    output << "Numerator: ";
    for (uint16_t i=0;i<n.fNumCoeffs.size(); i++)
    {
	output << n.fNumCoeffs[i] << ",";
    }
    output << endl;
    output << "============================================" << endl;
    SET_DEBUG_STACK;
    return output;
}
