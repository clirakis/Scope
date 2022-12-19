/**
 ******************************************************************
 *
 * Module Name : User.cpp
 *
 * Author/Date : C.B. Lirakis / 15-Jan-02 
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
 * Copyright (C) BBNT Solutions LLC  2001
 *******************************************************************
 */

#ifndef lint
/** RCS Information */
static char rcsid[]="$Header: /home/clirakis/code/lassen/RCS/User.cpp,v 1.1 2003/08/08 17:24:47 clirakis Exp $";
#endif

/* System includes. */
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#else
#include <iostream>
#include <cmath>
#include <ctime>
using namespace std;
#endif
/** Local Includes. */
#include "logdata.h"


/* Root Stuff. */
#include "TROOT.h"
#include "TMapFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TRandom.h"
#include "TSystem.h"
#include "debug.h"


/* root variables. */
TMapFile *mfile;

TH1F*   hpx;
TH2F*   hpxpy;
TProfile*  hprof;

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
int InitializeLogData()
{
    SET_DEBUG_STACK;
    // A trick,  Fons Rademakers' comments.
    // Set preferred map address. Find out preferred map address as follows:
    // 1) Run consumer program to find the preferred map address:
    //       $ root
    //       root [0] m = TMapFile::Create("dummy.map", "recreate", 10000000);
    //       root [1] m.Print()
    //       Memory mapped file:   dummy.map
    //       Title:
    //       Option:               CREATE
    //       Mapped Memory region: 0x40b4c000 - 0x40d95f00 (2.29 MB)
    //       Current breakval:     0x40b53000
    //       root [2] .q
    //       $ rm dummy.map
    //    Remember begin of mapped region, i.e. 0x40b4c000
    //
    // 2) Add to producer program, just before creating the TMapFile:
    //       TMapFile::SetMapAddress(0x40b4c000);
    //
    // Repeat this if more than one map file is being used.
    //
    // The above procedure allow programs using, e.g., different number of
    // shared libraries (that cause the default mapping address to be
    // different) to create shared memory regions in the same location
    // without overwriting a shared library. The above assumes the consumer
    // program is larger (i.e. has more shared memory occupied) than the
    // producer. If this is not true inverse the procedure.

    TMapFile::SetMapAddress(0x41438000);
    // Create a new memory mapped file. The memory mapped file can be
    // opened in an other process on the same machine and the objects
    // stored in it can be accessed.
    mfile = TMapFile::Create("hsimple.map","RECREATE", 100000, "A test");

    // Create a 1d, a 2d and a profile histogram. These objects will
    // be automatically added to the current directory, i.e. mfile.
    hpx    = new TH1F("hpx","This is the px distribution",100,-4,4);
    hpxpy  = new TH2F("hpxpy","py vs px",40,-4,4,40,-4,4);
    hprof  = new TProfile("hprof","Profile of pz versus px",100,-4,4,0,20);
    
    // Set a fill color for the TH1F
    hpx->SetFillColor(48);
    
    // Print status of mapped file
    mfile->Print();

    return 0;
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
void LogData(const float *data, int nchannel)
{
    SET_DEBUG_STACK;

    // Endless loop filling histograms with random numbers
    Float_t px, py, pz;
    int ii = 0;
    while (1) {
	gRandom->Rannor(px,py);
	pz = px*px + py*py;
	hpx->Fill(px);
	hpxpy->Fill(px,py);
	hprof->Fill(px,pz);
	if (!(ii % 10)) {
	    mfile->Update();       // updates all objects in shared memory
         if (!ii) mfile->ls();  // print contents of mapped file after
      }                         // first update
      ii++;
      gSystem->Sleep(100);   // sleep for 0.1 seconds
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
 *******************************************************************
 */
void FinalizeLogData()
{
    SET_DEBUG_STACK;

    cout << "User  finalize called!" << endl;
}
