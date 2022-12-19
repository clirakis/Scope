/**
 ******************************************************************
 *
 * Module Name : Kalman.hh
 *
 * Author/Date : C.B. Lirakis / 18-Nov-07
 *
 * Description :
 *
 * 9 state system, State vector is composed of:
 *
 *                      x, y, z, Vx, Vy, Vz, Ax, Ay, Az
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

class Kalman {
private:
    TMatrixD *P0;             /* Initial Covariance         */
    TMatrixD *PMinus, *PPlus; /* Covariance         */
    TMatrixD *H, *Ht;         /* Measurement        */
    TMatrixD *R;              /* Measurement error  */
    TMatrixD *Q;              /* Process noise      */
    TMatrixD *K;              /* Kalman gain        */
    TVectorD *X, *XMinus, *XPlus;     /* Current and old measurements */
    TMatrixD *I;              /* Identity matrix.   */
    TMatrixD *Pt, *PtT;       /* Transistion matrix */
    TMatrixD *work;           /* A working matrix   */
    void ComputeTransitionMatrix(Double_t dt);

public:
    Kalman();
    ~Kalman();

    void SetMeasurement ( const TVector3 *X = NULL, const TVector3 *V = NULL, 
			  const TVector3 *A = NULL);
    void CopyX();

    void SetMeasurementError( Double_t X, Double_t V, Double_t A);
    void SetProcessNoise( Double_t X, Double_t V, Double_t A);
    void ProcessMeasurement( );
    void Print();
    void Reset();
    TVector3 GetX();
    TVector3 GetV();
    TVector3 GetA();
};
#endif
