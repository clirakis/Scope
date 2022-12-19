/********************************************************************
 *
 * Module Name : MSLIST.cpp
 *
 * Author/Date : C.B. Lirakis / 01-Feb-11
 *
 * Description : Generic module
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
#include "TGLabel.h"
#include "TGButton.h"
#include "TList.h"
#include "TString.h"
#include "TPRegexp.h"

// Local Includes.
#include "MSLIST.hh"
#include "GParse.hh"
const char *Available[] = {"GAIN", "MAX", "MEAN", "MID","OVERSHOOT","PP",
			   "RMS","UNDERSHOOT", "YTENERGY", "YTMNS_AREA",
			   "YTPLS_AREA","SFREQ","SMAG","THD",
			   "CROSS", "DELAY", "DUTY", "FALLTIME", "FREQ",
			   "PDELAY","PERIOD","PHASE","RISETIME", "SKEW",
			   "TTRIG","WIDTH", NULL};
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
    fMeas    = new TString(l);
    fValue   = 0.0;
    fEnabled = kFALSE;
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
    cout << "Delete: " << endl;
    cout << *fMeas << endl;
    delete fMeas; fMeas = NULL;
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
    if (fMeas)
	return fMeas->Data();

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
    if(fMeas->CompareTo(name, TString::kIgnoreCase) == 0)
    {
	return true;
    }
    return false;
}

ostream& operator<<(ostream& output, const MeasurementA &n)
{
    output << n.Text();
    if(n.State())
	output << " Check";
    output << " ,";
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
MSLIST::MSLIST()
{
    fMeasurements = new TList();
    fMeasurements->SetOwner();
    Init();
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
    delete fMeasurements;
    fMeasurements = NULL;
    cout << "MSLIST deleted." << endl;
}
ostream& operator<<(ostream& output, const MSLIST &n)
{
    MeasurementA *p;
    TListIter next(n.fMeasurements);

    while ((p = (MeasurementA *)next()))
    {
	output << *p;
    }
    return output;
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
void MSLIST::Init()
{
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
    MeasurementA *p;
    TListIter next(fMeasurements);
    while ((p = (MeasurementA *)next()))
    {
	if (p->Match(v))
	    return p;
    }
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
void MSLIST::Clear()
{
    MeasurementA *p;
    TListIter next(fMeasurements);
    while ((p = (MeasurementA *)next()))
    {
	p->SetState(kFALSE);
    }
}
