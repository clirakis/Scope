/**
 ******************************************************************
 *
 * Module Name : User.cpp
 *
 * Author/Date : C.B. Lirakis / 16-Nov-06
 *
 * Description : root analysis
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
 * $RCSfile: User.cpp,v $
 * $Author: clirakis $
 * $Date: 2003/08/08 17:24:47 $
 * $Locker:  $
 * $Name:  $
 * $Revision: 1.1 $
 *
 * $Log: User.cpp,v $
 * Revision 1.1  2003/08/08 17:24:47  clirakis
 * Initial revision
 *
 *
 *
 *******************************************************************
 */

#ifndef lint
/** RCS Information */
static char rcsid[]="$Header: /home/clirakis/code/lassen/RCS/User.cpp,v 1.1 2003/08/08 17:24:47 clirakis Exp $";
#endif

/* System includes. */
#include <iostream>
#include <cmath>
using namespace std;

/** Local Includes. */
#include "User.hh"

/* Root Stuff. */
#include "TROOT.h"
#include "TFile.h"
#include "TNtupleD.h"
#include "TH1.h"
#include "TH2.h"
#include "debug.h"


/* root variables. */
TFile   *hfile;
TNtupleD *ntuple;
TROOT   *myroot;

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
 *******************************************************************
 */
bool UserInitialize()
{
    SET_DEBUG_STACK;
    /*
     * Setup NTUPLE
     * These are the names of the variables that we want to monitor.
     */
    char *raw_names = "Time:Real:Imag";

    /*
     * Initialize Root package.
     */
    myroot = ::new TROOT("nmea","Data analysis");

    /* Connect to a file. */
    hfile = new TFile("sho.root","RECREATE","SHO data analysis");

    /*
     * Create Ntuple.
     */
    ntuple = new TNtupleD("SHO","SHO ntuple",raw_names);

    return true;
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
 *******************************************************************
 */
void UserDo(double x, double y, double z)
{
    SET_DEBUG_STACK;
    Double_t val[10];
    val[0] = x;
    val[1] = y;
    val[2] = z;

    ntuple->Fill(val);
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
 *******************************************************************
 */
void UserFinalize()
{
    SET_DEBUG_STACK;
    cout << "User  finalize called!" << endl;
    // Save all objects in this file
    hfile->Write();

    // Close the file. Note that this is automatically done when you leave
    // the application.
    hfile->Close();
    delete hfile;
}
