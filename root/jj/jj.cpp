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
#include "jj.hh"

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
void CreateCanvas()
{
    TCanvas *Hobbes = new TCanvas("Dist","A test",5,5,1200,600);
    Hobbes->cd();
    TPad    *Calvin = new TPad("Calvin","Silly",0.01,0.01,0.99,0.99,10);
    Calvin->Draw();
    Calvin->cd();
    Calvin->SetGrid();
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
JJ::JJ()
{
    /*
     * Parameters:
     * [0] - I0
     * [1] - Phi0
     */
    TF1 *Current = new TF1("JJCurrent", "[0]*sin(2.0*TMath::Pi()*x/[1])");
    Josephson->SetParameter(0, 1.0);
    Josephson->SetParameter(1, Phi_0);
    Josephson->Draw();
}
