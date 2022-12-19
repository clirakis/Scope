/**
 ******************************************************************
 *
 * Module Name : ipend.cpp
 *
 * Author/Date : C.B. Lirakis / 28-Nov-07
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

// Root includes
#include <TMath.h>
#include <TEnv.h>

/// Local Includes.
#include "ipend.hh"
#include "debug.h"

const int ND = 4;

/**
 ******************************************************************
 *
 * Function Name : IPendulum constructor. 
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
IPendulum::IPendulum()
{
    Double_t t = 0.1; /* Seconds */

    fTransition = new TMatrixD( ND, ND);
    fH          = new TMatrixD( ND, ND);
    fZ          = new TMatrixD( ND, ND);

    fOffset     = new TVectorD(ND);
    fX          = new TVectorD(ND);
    fXP         = new TVectorD(ND);
    (*fH)(0,0)     = t;
    (*fH)(1,0)     = 1.0;
    (*fH)(2,2)     = t;
    (*fH)(3,2)     = 1.0;

    (*fZ)(0,0)     = 1.0;
    (*fZ)(2,2)     = 1.0;
    LoadPrefs();

    ComputeTransitionMatrix();

    fH->Print();
    fZ->Print();
    cout << "Transition " ;
    fTransition->Print();
    cout << "Offset " ;
    fOffset->Print();
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
IPendulum::~IPendulum()
{
    SavePrefs();
    delete fTransition;
    delete fOffset;
    delete fX;
    delete fH;
    delete fZ;
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
void IPendulum::SetMeasurement (Double_t x, Double_t xdot, Double_t phi, 
				Double_t phidot )
{
    (*fX)(0) = x;
    (*fX)(1) = xdot;
    (*fX)(2) = phi;
    (*fX)(3) = phidot;
    cout << "Initial ";
    fX->Print();
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
void IPendulum::ComputeTransitionMatrix()
{
    Double_t q, num, den;


    num = fPendulumInertia + fPendulumMass*pow(fPendulumLength,2.0);
    den = fPendulumInertia * (fCartMass + fPendulumMass) +
	fCartMass*fPendulumMass*pow(fPendulumLength, 2.0);
				     

    (*fOffset)(0) = 0.0;

    (*fOffset)(1) = (fPendulumInertia + 
		     fPendulumMass*pow(fPendulumLength,2.0))/den;

    (*fOffset)(2) = 0.0;
    (*fOffset)(3) = (fPendulumMass*fPendulumLength)/q;


    (*fTransition)(0,0) = 0.0;
    (*fTransition)(0,1) = 1.0;
    (*fTransition)(0,2) = 0.0;
    (*fTransition)(0,3) = 0.0;

    (*fTransition)(1,0) = 0.0;
    (*fTransition)(1,1) = -fCartFriction*num/den;
    (*fTransition)(1,2) = pow(fPendulumMass,2.0) * TMath::Gn() *
	pow(fPendulumLength,2.0)/den;
    (*fTransition)(1,3) = 0.0;


    (*fTransition)(2,0) = 0.0;
    (*fTransition)(2,1) = 0.0;
    (*fTransition)(2,2) = 0.0;
    (*fTransition)(2,3) = 1.0;

    (*fTransition)(3,0) = 0.0;
    (*fTransition)(3,1) = -fPendulumMass*fPendulumLength*fCartFriction/den;
    (*fTransition)(3,2) = fPendulumMass*TMath::Gn()*fPendulumLength *
	(fCartMass+fPendulumMass)/den;
    (*fTransition)(3,3) = 0.0;

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
void IPendulum::ProcessMeasurement()
{
    (*fXP) = (*fTransition) * (*fX) + (*fOffset);
    (*fX)  = (*fH) * (*fXP) + (*fZ) * (*fX);
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
void IPendulum::Print()
{
    fX->Print();
    fXP->Print();
    //fTransition->Print();
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
void IPendulum::Reset()
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
Bool_t IPendulum::LoadPrefs()
{

    fParameters = new TEnv("ipend.rc");

    fCartMass        = fParameters->GetValue( "CartMass", 0.5);
    fCartFriction    = fParameters->GetValue( "CartFriction", 0.1);


    fPendulumMass    = fParameters->GetValue( "PendulumMass", 0.2);
    fPendulumLength  = fParameters->GetValue( "PendulumLength", 0.3);
    fPendulumInertia = fParameters->GetValue( "PendulumInertia", 0.006);

    return kTRUE;
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
Bool_t IPendulum::SavePrefs()
{
    fParameters->SetValue( "CartMass", fCartMass);
    fParameters->SetValue( "CartFriction", fCartFriction);
    fParameters->SetValue( "PendulumMass", fPendulumMass);
    fParameters->SetValue( "PendulumLength", fPendulumLength);
    fParameters->SetValue( "PendulumInertia", fPendulumInertia);

    fParameters->SaveLevel(kEnvLocal);

    return kTRUE;
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
TVectorD IPendulum::GetX()
{
    return (*fX);
}

