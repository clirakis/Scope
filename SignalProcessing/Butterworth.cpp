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
 * https://stackoverflow.com/questions/10373184/bandpass-butterworth-filter-implementation-in-c
 *
 * https://exstrom.com/journal/sigproc/
 *
 ********************************************************************/
// System includes.
#include <iostream>
using namespace std;
#include <iomanip>
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
 * Description : Construct a filter of the type indicated in the last argument
 *
 * Inputs :
 *         FilterOrder
 *         Lcutoff    - Lower frequency cutoff, nomalized
 *         Ucutoff    - Upper frequency cutoff, normalized
 *         Type       - Bandpass, Lowpass, Highpass
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
Butterworth::Butterworth (uint16_t FilterOrder, double Lcutoff, double Ucutoff,
			  FilterType Type)
{
    SET_DEBUG_STACK;
    fFilterOrder = FilterOrder;
    fLowerCutoff = Lcutoff;
    fUpperCutoff = Ucutoff;
    fType        = Type;

    switch(Type)
    {
    case kBANDPASS:
	ComputeDenCoeffs();
	ComputeNumCoeffs();
	break;
    case kLOWPASS:
    case kHIPASS:
	cout << "NOT IMPLEMENTED." << endl;
	break;
    case kALOWPASS: // analog low pass. 
	// nothing to do right now. 
	break;
    }

    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : Butterworth destructor
 *
 * Description : Does nothing right now
 *
 * Inputs : NONE
 *
 * Returns : NONE
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 16-Jan-23
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
 * Function Name : ClearAll
 *
 * Description : Reset all the vectors in prep for a recalculation 
 *
 * Inputs : NONE
 *
 * Returns : NONE
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
void Butterworth::ClearAll(void)
{
    SET_DEBUG_STACK;
    fDenomCoeffs.clear();
    fNumCoeffs.clear();
    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : ComputeDenCoeffs
 *
 * Description : Compute the a's of the filter, for a bandpass implementation
 *
 * Inputs : NONE
 *
 * Returns : NONE
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on:  16-Jan-23
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

    // Temporary vectors for operations. 
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
    /*
     * Why preincrement rather than post increment? 
     */
    for (k = 3; k<= 2*fFilterOrder; ++k)
    {
	fDenomCoeffs[k] = fDenomCoeffs[2*k-2];
    }

    /*
     * Get rid of the extra space used in the calculation.
     * different
     */ 
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
 * Description : compute numerator, in an FIR, these are the b 
 * coefficients. Compute the vector of the numerator and put it into
 * <vector> fNumCoeffs. Bandpass!
 *
 * Inputs : NONE
 *
 * Returns : NONE
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 16-Jan-23
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
    uint16_t       i;

    // https://en.cppreference.com/w/cpp/numeric/complex
    // Different
    vector<complex<double>> NormalizedKernel(2*fFilterOrder+1);

    /*
     * Create a vector of linearly increasing numbers. 
     * Not sure this is the most efficient way to do this. 
     */
    vector<double> Numbers;
    for (i = 0;i<2*fFilterOrder+1;i++)
    {
	Numbers.push_back((double)i);
    }

    TCoeffs = ComputeHP();  // Check

    /*
     * Good
     * OK - Should probably make sure this is clear at some point.
     */ 
    for (i = 0;i<fFilterOrder; ++i)
    {
	fNumCoeffs.push_back(TCoeffs[i]);
	fNumCoeffs.push_back(0.0);
    }
    fNumCoeffs.push_back(TCoeffs[fFilterOrder]);

    //  ==========================================
    double cp[2];
    cp[0] = 4.0*tan(M_PI_2*fLowerCutoff);
    cp[1] = 4.0*tan(M_PI_2*fUpperCutoff);

    //center frequency
    double Wn = sqrt(cp[0] * cp[1]);
    Wn = 2.0*atan2(Wn, 4);

#if 0
    // Commenting on how the data is normalized, see below. 
    double kern; // Not sure if this is used
    kern = exp(-j*w*(0:length(b)-1));
    b = real(b*(kern*den(:))/(kern*b(:)));
#endif

    const std::complex<double> result(-1, 0);
    for (i=0; i<2*fFilterOrder+1; i++)
    {
	NormalizedKernel[i] = exp(-sqrt(result)*Wn*Numbers[i]);
    }

    double b   = 0;
    double den = 0;
    for (i=0; i<2*fFilterOrder+1; i++)
    {
	b   += real(NormalizedKernel[i] * fNumCoeffs[i]);
	den += real(NormalizedKernel[i] * fDenomCoeffs[i]);
    }

    for (i=0; i<2*fFilterOrder+1; i++)
    {
	fNumCoeffs[i] = (fNumCoeffs[i] * den)/b;
    }

    // This in general is not in the other code. 
    for (i=fNumCoeffs.size()-1; i>=(2*fFilterOrder+1); i--)
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

/**
 ******************************************************************
 *
 * Function Name : TrinomialMultiply
 *
 * Description : Looks ok , Bandpass function
 *
 * Inputs : vectors to perform multiplicaiton
 *
 * Returns : resulting vector
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 16-Jan-23
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
 * Description : Analog lowpass filter coefficients. BANDPASS
 * CHECK
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

    NumCoeffs[0] = 1;
    NumCoeffs[1] = fFilterOrder;
    for (uint16_t i = 2; i <= fFilterOrder/2; ++i)
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
 * Description : High pass coefficients, BANSPASS
 *               Check
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
* Function Name : ALowPass
*
* Description : Analog lowpass filter coefficients computation
*          https://exstrom.com/journal/sigproc/alpbw.c
*          https://en.wikipedia.org/wiki/Butterworth_filter
*          https://www.electronics-tutorials.ws/filter/filter_8.html
*          https://www.electronics-notes.com/articles/radio/rf-filters/butterworth-formula-equations-calculations.php
*          "Electronics Equations Handbook" by Stephen J Erst
*              1989 tab books, page 62
*
*          https://www.rfcafe.com/references/electrical/butter-proto-values.htm
*                has a table for checking this out. 
*          Resulting values
*          - The termination resistances are assumed to equal 1.
*          - For a termination resistance equal to R 
*          - L(actual) = L(output) * R
*          - C(actual) = C(output) / R
*          
*
* Inputs : 
*      ntaps       - Filter order
*      Frequency   - 3dB point in Hz
*      Termination - True, source and load resistances are equal.
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
vector<double> Butterworth::ALowPass(uint16_t ntaps, double Frequency, 
				     bool Termination)
{
    SET_DEBUG_STACK;
    double   omega = 2.0* M_PI * Frequency;
    double   x;                              // scratch variable. 
    uint16_t i, j, k;                        // index variable
    uint16_t m;
    double   *b, *c, *u, *v;
    double   *vec_b, *vec_c;                 // working vectors 
    double   *tmpptr;                        // temporary pointer
    double   a[3], r;
    uint16_t na, nb, nc, nu, nv;
    vector<double> rv;

    fFilterOrder = ntaps;

    /*
     * If the source and sink resistance values are the same
     * then the code is pretty easy. 
     */
    if(Termination)
    {
	for(i=0; i<fFilterOrder; ++i)
	{
	    x = 2.0*sin(M_PI*(double)(2*i+1)/(double)(2*fFilterOrder))/omega;
	    rv.push_back(x);
	}
	return rv;
    }

    /*
     * use calloc instead of malloc to get zeros
     * also be super careful so we can free the vectors when done. 
     */
    vec_b = (double *)calloc((fFilterOrder+1), sizeof(double));
    vec_c = (double *)calloc((fFilterOrder+1), sizeof(double));
    b = vec_b;
    c = vec_c;

    /*
     * initialize the Butterworth polynomial
     * Is the filter odd or even in filter order? 
     * https://people.eecs.ku.edu/~demarest/212/Butterworth%20Polynomials.pdf
     */
    if(fFilterOrder % 2 == 1)
    {
	// Odd
	b[0] = b[1] = 1.0;
	nb   = 2;
	m    = (fFilterOrder - 1)/2;   // elements to iterate over. 
    }
    else
    {
	// Even
	b[0] = 1.0;
	nb   = 1;
	m    = fFilterOrder/2;
    }

    /*
     * calculate the Butterworth polynomial
     * https://people.eecs.ku.edu/~demarest/212/Butterworth%20Polynomials.pdf
     * loop over the number of taps
     */
    for(k=0; k<m; ++k)
    {
	/*
	 * Setup working variables in A.
	 * This is essentially a truncated sinc function. 
	 */
	a[0] = 1.0;
	a[1] = 2.0*sin(M_PI*(double)(2*k+1)/(double)(2*fFilterOrder));
	a[2] = 1.0;

	na = 3;             // max number of elements in a
	nc = na + nb - 1;   // max number of elements in c to process.

	/* 
	 * perform the convolution: a[0,...,na-1] * b[0,...,nb-1]
	 * producing the sequence c[0,...,nc-1] nc=na+nb-1
	 * nb is 2 (odd) or 1(even)
	 */
	for(i = 0; i < nb; ++i)
	{
	    /*
	     * for each element of c perform the convolution of a and b
	     * loop over j, j starts at either 0 or (i-na+1) if i < na
	     * termination is j<=i
	     */
	    c[i] = 0.0;
	    for(j = i < na ? 0 : i - na + 1; j <= i; ++j)
	    {
		c[i] += a[i-j] * b[j];
	    }
	}
	// Do the same here
	for(i = nb; i < nc; ++i)
	{
	    c[i] = 0.0;
	    for(j = i < na ? 0 : i - na + 1; j < nb; ++j)
	    {
		c[i] += a[i-j]*b[j];
	    }
	}

	/* 
	 * swap b and c arrays using pointers. 
	 * d is a temp pointer
	 */
	tmpptr  = b;
	b       = c;
	c       = tmpptr;
	nb      = nc;
    }

#if 0
    // print Butterworth polynomial coefficient values
    cout << "Polynomial coefficient values:" << endl;
    for(i=0; i<nb; ++i) 
    {
	cout << i << " " << b[i];
    }
    cout << endl;
#endif

    // put odd coefficients in c and even coefficients in b
    for(i=1, nb=1, nc=0; i<=fFilterOrder; i+=2, ++nc)
    {
	c[(i-1)/2] = b[i];
	if(i < fFilterOrder)
	{
	    b[(i+1)/2] = b[i+1];
	    ++nb;
	}
    }

    /*
     * make u the larger of b and c
     * make v the smaller of b and c
     */
    if(nb > nc)
    {
	u = b;
	nu = nb;
	v = c;
	nv = nc;
    }
    else
    {
	u = c;
	nu = nc;
	v = b;
	nv = nb;
    }

    // calculate component values
#if 0
    cout << std::fixed << std::setw(6) << std::setprecision(6);
    cout << "Filter component values for 3dB point at. "
	 << Frequency  << endl;
#endif
    bool odd = true;
    /*
     * nu count on capacitor values
     * nv count on inductor values 
     */
    while(nu>0 && nv>0)
    {
	if(odd)
	{
	    r=u[nu-1]/v[nv-1];
	    rv.push_back(r/omega);
	    //cout << "val: " << r/omega << endl;
	    for(i=1; i<nv; ++i) 
	    {
		u[nu-i-1]=u[nu-i-1]-r*v[nv-i-1];
	    }
	    --nu;
	    odd = false;
	}
	else
	{
	    r=v[nv-1]/u[nu-1];
	    rv.push_back(r/omega);
	    //cout << "val " << r/omega << endl;
	    for(i=1; i<nu; ++i) 
	    {
		v[nv-i-1]=v[nv-i-1]-r*u[nu-i-1];
	    }
	    --nv;
	    odd = true;
	}
    }
    //cout << endl;
    free(vec_b);
    free(vec_c);

    SET_DEBUG_STACK;
    return rv;
}


/**
 ******************************************************************
 *
 * Function Name : Butterworth << operator
 *
 * Description : format all the channel information for output
 *
 * Inputs : 
 *       output stream to format 
 *       Butterworth class to print out
 *
 * Returns : fully functional stream
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 16-Jan-23
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
ostream& operator<<(ostream& output, const Butterworth &n)
{
    SET_DEBUG_STACK;
    uint16_t i;
    output << std::fixed << std::setw(6) << std::setprecision(6);
    output << "============================================" << endl
	   << "    Filter Order: " << n.fFilterOrder << endl
	   << "    Lower Cutoff: " << n.fLowerCutoff << endl
	   << "    Upper Cutoff: " << n.fUpperCutoff << endl << endl;

    output << "(a) Denominator (" << n.fDenomCoeffs.size() << "): " 
	   << endl << "     ";
    for (i=0;i<n.fDenomCoeffs.size(); i++)
    {
	output << n.fDenomCoeffs[i] << ", ";
	if ((i>0) && (i%5 == 0)) output << endl << "     ";
    }
    output << endl;

    output << "(b) Numerator (" << n.fNumCoeffs.size() << "): " 
	   << endl << "     ";

    for (i=0;i<n.fNumCoeffs.size(); i++)
    {
	output << n.fNumCoeffs[i] << ", ";
	if ((i>0) && (i%5 == 0)) output << endl << "     ";
    }
    output << endl;
    output << "============================================" << endl;
    SET_DEBUG_STACK;
    return output;
}
