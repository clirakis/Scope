/**
 ******************************************************************
 *
 * Module Name : ipend.hh
 *
 * Author/Date : C.B. Lirakis / 28-Nov-07
 *
 * Description :
 *
 * inverted pendulum state space simulation. Vector consists of:
 *
 *                      X X' phi phi'
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
#ifndef __KALMAN_hh_
#define __KALMAN_hh_
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TVector3.h"
class TEnv;

class IPendulum {
private:

    TMatrixD  *fTransition;
    TMatrixD  *fObservable;     /* Not sure if this is necessary. */
    TVectorD  *fOffset;
    TVectorD  *fX;              /* X X' Phi Phi'      */
    TVectorD  *fXP;             /* X' X" Phi' Phi"    */
    TMatrixD  *fH, *fZ;         /* Matrix from fXP to fX */

    Double_t  fCartMass, fCartFriction; 
    Double_t  fPendulumMass, fPendulumLength, fPendulumInertia;
    TEnv      *fParameters;

    Bool_t    LoadPrefs();
    Bool_t    SavePrefs();
public:
    IPendulum();
    ~IPendulum();

    void SetMeasurement (Double_t x, Double_t xdot, Double_t phi, 
			 Double_t phidot );
    void ComputeTransitionMatrix();

    void ProcessMeasurement( );
    void Print();
    void Reset();
    TVectorD GetX();

};
#endif
