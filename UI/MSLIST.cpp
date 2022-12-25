/********************************************************************
 *
 * Module Name : MSLIST.cpp
 *
 * Author/Date : C.B. Lirakis / 01-Feb-11
 *
 * Description : MSLIST manages all the possible measurements that
 * can be made for the UI. 
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References :
 *
 ********************************************************************/
// System includes.
#include <iostream>
using namespace std;
#include <string>
#include <cmath>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

// Root includes
//#include "TGLabel.h"
//#include "TGButton.h"
#include "TList.h"
#include "TString.h"
#include "TPRegexp.h"

// Local Includes.
#include "debug.h"
#include "Measurement.hh"
#include "MSLIST.hh"
#include "GParse.hh"

/*
 * List of avaible things to set and or read on any waveform. 
 * Add +1 to be NULL terminated. 
 */
const char *Available[Measurement::kNMeasurements+1] = {
    "GAIN", "MAX", "MEAN", "MID", "MIN", "OVERSHOOT","PP", "RMS","UNDERSHOOT",
    "YTENERGY", "YTMNS_AREA", "YTPLS_AREA",
    "SFREQ","SMAG","THD",
    "CROSS", "DELAY", "DUTY", "FALLTIME", "FREQ", "PDELAY","PERIOD",
    "PHASE","RISETIME", "SKEW", "TTRIG","WIDTH", NULL};
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
MeasurementA::MeasurementA(const char *l)
{
    SET_DEBUG_STACK;
    fMeas    = new TString(l);
    fValue   = 0.0;
    fEnabled = kFALSE;
    SET_DEBUG_STACK;
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
MeasurementA::~MeasurementA()
{
    SET_DEBUG_STACK;
//     cout << __FUNCTION__ << " Delete: " << endl;
//     cout << *fMeas << endl;
    delete fMeas; fMeas = NULL;
    SET_DEBUG_STACK;
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
const char *MeasurementA::Text(void) const
{
    SET_DEBUG_STACK;
    if (fMeas)
	return fMeas->Data();
    SET_DEBUG_STACK;
    return NULL;
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
bool MeasurementA::Match(const char *name)
{
    SET_DEBUG_STACK;
    if(fMeas->CompareTo(name, TString::kIgnoreCase) == 0)
    {
	return true;
    }
    SET_DEBUG_STACK;
    return false;
}

ostream& operator<<(ostream& output, const MeasurementA &n)
{
    SET_DEBUG_STACK;
    output << n.Text();
    if(n.State())
	output << " Check";
    output << " ,";
    SET_DEBUG_STACK;
    return output;
}


/**
 ******************************************************************
 *
 * Function Name : MSLIST constructor
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
MSLIST::MSLIST(void)
{
    SET_DEBUG_STACK;
    fMeasurements = new TList();
    fMeasurements->SetOwner();
    Init();
    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : MSLIST destructor
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
MSLIST::~MSLIST()
{
    SET_DEBUG_STACK;
    delete fMeasurements;
    fMeasurements = NULL;
    cout << __FUNCTION__ << " MSLIST deleted." << endl;
    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : MSLIST output stream formatter
 *
 * Description :
 *
 * Inputs :
 *    output - output stream to fill
 *    n - MSLIST elements
 *
 * Returns : 
 *    fully populated output stream
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
ostream& operator<<(ostream& output, const MSLIST &n)
{
    SET_DEBUG_STACK;
    MeasurementA *p;
    TListIter next(n.fMeasurements);

    while ((p = (MeasurementA *)next()))
    {
	output << *p;
    }
    SET_DEBUG_STACK;
    return output;
}

/**
 ******************************************************************
 *
 * Function Name : Init
 *
 * Description : Initialze the list will all the available names 
 * that can be selected for measurement. Note, that not all of those
 * are applicable for every case. 
 *
 * Inputs : none
 *
 * Returns : none
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
void MSLIST::Init(void)
{
    SET_DEBUG_STACK;
    MeasurementA* m;
    const char*  p;
    int          i = 0;
 
    while ((p = Available[i]) != NULL)
    {
	m = new MeasurementA(p);
	fMeasurements->Add(m);
	i++;
    }
    // List is made, populate
    SET_DEBUG_STACK;
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
void MSLIST::FillState(const char *s)
{
    SET_DEBUG_STACK;
    const char tokens[] = "[,]";
    MeasurementA *m;
    TString     input(s);
    Int_t       n = input.First(" ");

    if (n>0)
    {
	/* Skip command */
	TString Remainder = input(n+1, input.Length()-n-1);
	TStringToken p(Remainder, tokens);
	while(p.NextToken())
	{
	    if((m = Find(p.Data()))!=NULL)
	    {
		//cout << "Found: " << p.Data() << endl;
		m->SetState(kTRUE);
	    }
	}
    }
    SET_DEBUG_STACK;
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
void MSLIST::FillValue(const char *s)
{
    SET_DEBUG_STACK;
    // GPARSE will not work here
    // There is a second value that is returned
    // EQ True measurement equals value
    // LT less than
    // GT
    // UN uncertain
    // ER Error occured. 
    // Example data: 
    // MEAS FREQ:0.0E+0,ER,RISETIME:0.0E+0,EQ,PP:5.85937E+1,EQ
    GParse gp(s);
    TListIter next(fMeasurements);
    MeasurementA* p;

    while ((p = (MeasurementA *)next()))
    {
	if (p->State())
	{
	    p->SetValue(atof(gp.Value(p->Text())));
	}
    }
    SET_DEBUG_STACK;
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
MeasurementA*  MSLIST::Find(const char *v)
{
    SET_DEBUG_STACK;
    MeasurementA *p;
    TListIter next(fMeasurements);
    while ((p = (MeasurementA *)next()))
    {
	if (p->Match(v))
	    return p;
    }
    SET_DEBUG_STACK;
    return NULL;
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
void MSLIST::Clear(void)
{
    SET_DEBUG_STACK;
    MeasurementA *p;
    TListIter next(fMeasurements);
    while ((p = (MeasurementA *)next()))
    {
	p->SetState(kFALSE);
    }
    SET_DEBUG_STACK;
}
