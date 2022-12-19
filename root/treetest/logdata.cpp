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
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TRandom.h"
#include "TTree.h"


#include "debug.h"


/* root variables. */
TH1F *hpx;
TH2F *hpxpy;
TProfile *hprof;
TFile *hfile;   
TTree *tree;

// Global variables and structure definitions for this example program.
typedef struct {Float_t x,y,z;} POINT;
typedef struct {
    Int_t ntrack,nseg,nvertex;
    UInt_t flag;
    Float_t temperature;
} EVENTN;
static POINT point;
static EVENTN eventn;
static Float_t p[3];
static int EC = 0;

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
    // Create a new ROOT binary machine independent file.
    // Note that this file may contain any kind of ROOT objects, 
    // histograms,trees pictures, graphics objects, detector geometries, 
    // tracks, events, etc..
    // This file is now becoming the current directory.
    hfile = new TFile ("htree.root","RECREATE",
		       "Demo ROOT file with histograms & trees");

    // Create some histograms and a profile histogram
    hpx   = new TH1F("hpx","This is the px distribution",100,-4,4);
    hpxpy = new TH2F("hpxpy","py ps px",40,-4,4,40,-4,4);
    hprof = new TProfile("hprof","Profile of pz versus px",100,-4,4,0,20);

    // Define some simple structures

    // Create a ROOT Tree
    tree = new TTree("T","An example of ROOT tree with a few branches");
    tree->Branch("ecounter",&EC,"EC/i");
    tree->Branch("point",&point,"x:y:z");
    tree->Branch("eventn",&eventn,
		 "ntrack/I:nseg:nvertex:flag/i:temperature/F");
    tree->Branch("hpx","TH1F",&hpx,128000,0);

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

    Float_t px,py,pz;
    Float_t random;
    //--------------------Here we start a loop on 1000 events
    for ( Int_t i=0; i<1000; i++) 
    {
	gRandom->Rannor(px,py);
	pz = px*px + py*py;
	random = gRandom->Rndm(1);
	
	//         Fill histograms
	hpx->Fill(px);
	hpxpy->Fill(px,py,1);
	hprof->Fill(px,pz,1);

	//         Fill structures
	p[0] = px;
	p[1] = py;
	p[2] = pz;
	point.x = 10*(random-1);;
	point.y = 5*random;
	point.z = 20*random;
	eventn.ntrack  = Int_t(100*random);
	eventn.nseg    = Int_t(2*eventn.ntrack);
	eventn.nvertex = 1;
	eventn.flag    = Int_t(3*random+0.5);
	eventn.temperature = 20+random;
	EC++;
	// Fill the tree. For each event, save the 2 structures and 3 objects
	// In this simple example, the objects hpx, hprof and hpxpy are 
	// slightly different from event to event. We expect a big 
	// compression factor!
	tree->Fill();
    } //--------------End of the loop

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
    tree->Print();

    // Save all objects in this file
    hfile->Write();
    
    // Close the file. Note that this is automatically done when you leave
    // the application.
    hfile->Close();
    cout << "User  finalize called!" << endl;
}
