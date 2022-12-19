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
#include "TFile.h"
#include "TH1.h"
#include "debug.h"


/* root variables. */
/* root variables. */
TFile *hfile;
TROOT *myroot;
TH1D *fft;
                                                                                
static bool Initialized = false;
static time_t start;
static int Norm;
static int NEntries, NSum;
static double *x, *sum;
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
    double y;
    SET_DEBUG_STACK;
    /*
     * Initialize Root package.
     */
    myroot = ::new TROOT("DAQ","ADC Data analysis");
                                                                                
    /* Connect to a file. */
    hfile = new TFile("gps.root","RECREATE","PCI-1200 data analysis");
                                                                                
    NEntries = 20;
    fft = new TH1D("FFT", "Channel 0 FFT", NEntries, 0.,
                   (double)(NEntries/2));
    Norm =0;
    // Allocate space for x-axis for group fill on histogram.
    x = new double[NEntries];
    if (x == NULL)
    {
        cerr << "Error allocating x vector for histogram." << endl;
    }
    else
    {
        for (int i=0;i<NEntries;i++)
        {
            y = (double) i;
            x[i] = y;
        }
    }
    sum = new double[NEntries];
    if (sum == NULL)
    {
        cerr << "Error allocating sum vector for histogram." << endl;
    }
    else
    {
        for (int i=0;i<NEntries;i++)
        {
            y = (double) i;
            sum[i] = 1.0;
        }

    }
 
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
    int i;
    SET_DEBUG_STACK;
    // Fill histo
    for (i=0;i<10;i++)
	fft->FillN(NEntries,x,sum,1);
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
   // Save all objects in this file
    hfile->Write();
                                                                                

    // Close the file. Note that this is automatically done when you leave
    // the application.
    hfile->Close();
    delete hfile;
    if (x!=NULL)
        delete[] x;
    if (sum!=NULL)
        delete[] sum;
}
