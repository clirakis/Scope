/**
 ******************************************************************
 *
 * Module Name : 
 *
 * Author/Date : C.B. Lirakis / 18-Nov-07
 *
 * Description :
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
#include "Kalman.hh"
#include "debug.h"

const int ND = 9;

/**
 ******************************************************************
 *
 * Function Name : Kalman constructor. 
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
Kalman::Kalman()
{
    Int_t i;

    P0     = new TMatrixD( ND, ND);
    PMinus = new TMatrixD( ND, ND);
    PPlus  = new TMatrixD( ND, ND);
    H      = new TMatrixD( ND, ND);
    Ht     = new TMatrixD( ND, ND);
    R      = new TMatrixD( ND, ND);
    Q      = new TMatrixD( ND, ND);
    I      = new TMatrixD( ND, ND);
    K      = new TMatrixD( ND, ND);
    Pt     = new TMatrixD( ND, ND);
    PtT    = new TMatrixD( ND, ND);
    work   = new TMatrixD( ND, ND);

    XMinus = new TVectorD(ND);
    XPlus  = new TVectorD(ND);
    X      = new TVectorD( ND);

    for(i=0;i<ND;i++)
    {
	(*I)(i,i)  = 1.0;
	(*Pt)(i,i) = 1.0;
	(*H)(i,i)  = 1.0;
	(*P0)(i,i) = 1.0e6;
    }

    Ht->Transpose(*H);

    SetMeasurementError( 1.0, 1.0, 1.0);
    SetProcessNoise ( 1.0e2, 5.0, 1.0e-1);

    ComputeTransitionMatrix(1.0);   /* Assume 1 seond steps. */
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
Kalman::~Kalman()
{
    delete P0;
    delete PMinus;
    delete PPlus;
    delete XMinus;
    delete XPlus;
    delete H;
    delete R;
    delete Q;
    delete X;
    delete XMinus;
    delete I;
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
void Kalman::SetMeasurement ( const TVector3 *Xi, const TVector3 *V, 
			      const TVector3 *A)
{
    Int_t i;

    (*X) = (*XMinus);

    for (i=0;i<3;i++)
    {
	if (Xi != NULL)
	    (*X)(i)   = (*Xi)(i);
	if (V != NULL)
	    (*X)(i+3) = (*V)(i);
	if (A != NULL)
	    (*X)(i+6) = (*A)(i);
    }
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
void Kalman::SetMeasurementError( Double_t Xi, Double_t V, Double_t A)
{
    Int_t i;
    for (i=0;i<ND;i++)
    {
	if (i<3)
	{
	    (*R)(i,i)   = Xi;
	} 
	else if (i<6)
	{
	    (*R)(i,i)   = V;
	}
	else
	{
	    (*R)(i,i)   = A;
	}
    }
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
void Kalman::SetProcessNoise( Double_t Xi, Double_t V, Double_t A)
{
    Int_t i;
    for (i=0;i<ND;i++)
    {
	if (i<3)
	{
	    (*Q)(i,i)   = Xi;
	} 
	else if (i<6)
	{
	    (*Q)(i,i)   = V;
	}
	else
	{
	    (*Q)(i,i)   = A;
	}
    }
}
/**
 ******************************************************************
 *
 * Function Name : ComputeTransitionMatrix
 *
 * Description : Given a time interval, determine what the 
 *               transition matrix elements are. 
 *
 * Inputs : dt - time in seconds.
 *
 * Returns : none
 *
 * Error Conditions : none
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void Kalman::ComputeTransitionMatrix(Double_t dt)
{
    /*
     * precompute for dt data intervals.
     * In this case it is pretty simple. X = X0 + V dt + a dt*dt
     */
    /* Contributions of velocity to position. */
    (*Pt)(0,3) = dt;
    (*Pt)(1,4) = dt;
    (*Pt)(2,5) = dt;
    /* Contributions of acceleration to position. */
    (*Pt)(0,6) = dt*dt/2.0;
    (*Pt)(1,7) = dt*dt/2.0;
    (*Pt)(2,8) = dt*dt/2.0;

    /* Contributions of acceleration to velocity */
    (*Pt)(3,6) = dt;
    (*Pt)(4,7) = dt;
    (*Pt)(5,8) = dt;

    PtT->Transpose(*Pt);

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
void Kalman::ProcessMeasurement()
{

    /*
     * State diagram page 219 Brown and Hwang. 
     *
     * Compute new Kalman gain - R is the measurement error, conceviably this
     * could be from the actual measuring device. It should be a diagonal
     * matrix the size of H. The number of diagonal elements is the size of 
     * the
     * measured value.
     */
    
    (*work) = (*H) * (*PMinus) * (*Ht) + (*R); 
    work->Invert();

    /* 
     * Gotta divide by work, 
     * Since C evaluates right to left, I assume that inverting work
     * and then multiplying it by the PMinus Ht combo should work. 
     */
    (*K)    = (*work) * (*PMinus) * (*Ht); 

    (*XPlus) = (*XMinus) + (*K) * ((*X) - (*H) * (*XMinus));

    (*PPlus) = ( (*I) - (*K)*(*H)) * (*PMinus);

    (*XMinus) = (*Pt) * (*XPlus);

    (*PMinus) = (*Pt) * (*PPlus) * (*PtT) + (*Q);
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
void Kalman::Print()
{
    XMinus->Print();
    PMinus->Print();
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
void Kalman::Reset()
{
    (*XPlus)  = (*X);
    (*XMinus) = (*X);
    (*PMinus) = (*P0);
    (*PPlus)  = (*P0);
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
void Kalman::CopyX()
{
    (*X) = (*XMinus);
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
TVector3 Kalman::GetX()
{
    TVector3 rv( (*XMinus)(0), (*XMinus)(1), (*XMinus)(2));
    return rv;
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
TVector3 Kalman::GetV()
{
    TVector3 rv( (*XMinus)(3), (*XMinus)(4), (*XMinus)(5));
    return rv;
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
TVector3 Kalman::GetA()
{
    TVector3 rv( (*X)(6), (*X)(7), (*X)(8));
    return rv;
}
