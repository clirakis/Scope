/**
 ******************************************************************
 *
 * Module Name : 
 *
 * Author/Date : C.B. Lirakis / 01-Nov-05
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
 * Copyright (C) BBNT 2005
 *******************************************************************
 */
#ifndef  __JJ_H_
# define __JJ_H_
// Root include
# include <TFormula.h>
# include <TMath.h>

const Double_t Phi_0 = TMath::H()/(2.0*TMath::Qe());

class JJ {
public:
    JJ();
    ~JJ();
private:
    TFormula *Current;
    /// Critical Current for the junction. 
    Double_t Ic;      
};
#endif
