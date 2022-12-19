/********************************************************************
 *
 * Module Name : Description.cpp
 *
 * Author/Date : C.B. Lirakis / 19-Dec-14
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

// Local Includes.
#include "Description.hh"
#include "GParse.hh"
#include "DSA602_Utility.hh"

const char* Description::WString[] = {"MAIN", "WIN1", "WIN2", "NONE", NULL};

/**
 ******************************************************************
 *
 * Function Name : Description Constructor
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
Description::Description(const char *val)
{
    fTime     = true;     // true - Y vs Time "L1 ON MAIN", false Y VS X
    fWindow   = WINDOW_NONE;
    fX        = NULL;
    fY        = NULL;

    if (val!=NULL)
    {
	DecodeString(val);
    }
}

/**
 ******************************************************************
 *
 * Function Name : Description destructor
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 27-Nov-14
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
Description::~Description()
{
    delete fX;
    delete fY;
}
/**
 ******************************************************************
 *
 * Function Name : Description destructor
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 27-Nov-14
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
ostream& operator<<(ostream& output, const Description &n)
{

    output << "============================================" << endl
	   << "Description: " ;
    if (n.fTime) 
    {
	output << *n.fX 
	       << " Time base on: " << n.sWindow();
    }
    else
    {
	output << *n.fY << " VS " << *n.fX;
    }
    output << "============================================" << endl;
    return output;
}

/**
 ******************************************************************
 *
 * Function Name : DecodeString
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 27-Nov-14
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void Description::DecodeString(const char *val)
{
    const char *p = NULL;
    char  tmp[32];
    int   n;
    /*
     * Typical channel query data. page 272
     * 'TRACE1 DESCRIPTION:"R1 ON MAIN",ACCUMULATE:OFF,ACSTATE:NENHANCED,GRLOCATION:UPPER,GRTYPE:LINEAR,WFMCALC:FAST,XUNIT:SECONDS,YUNIT:VOLTS;TRACE2 DESCRIPTION:"FFTmag(WFM1) ON MAIN",ACCUMULATE:OFF,ACSTATE:NENHANCED,GRLOCATION:UPPER,GRTYPE:LINEAR,WFMCALC:HIPREC,XUNIT:HERTZ,YUNIT:DIVS'
     Number of bytes read: 278
     *
     * We will only focus on the description type here
     * assume the rest has been removed 
     * Look first for the keyword ON to determine if it is an XY or
     * time base plot. The Window descriptions below are time base. 
     * eg: MAIN. 
     */
    p = strcasestr(val, "ON");
    if (p!=NULL)
    {
	// time base.
	fTime = true; 

	// Everything to the left of the word ON is a input description. 
	// This falls short at the moment, it does not have a list of 
	// operations which it should have. 
	n = p-val;
	memset( tmp, 0, sizeof(tmp));
	memcpy( tmp, val, n);
	fX = new Input(tmp);

	//  Extract window. 
	p += 2;
	DecodeTimeWindow(p);

    }
    else if ((p = strcasestr(val,"VS")) != NULL)
    {
	// This is the case where it is Y VS X
	fTime = false;
	// Left of vs is Y
	n = p-val;
	memset( tmp, 0, sizeof(tmp));
	memcpy( tmp, val, n);
	fY = new Input(tmp);

	p += 2;
	n = strlen(val) - (p-val) + 1;
	memset( tmp, 0, sizeof(tmp));
	memcpy( tmp, val, n);
	fX = new Input(tmp);
	fWindow = WINDOW_NONE;
    }
}

const char* Description::sWindow(void) const
{
    return WString[fWindow];
}
void Description::DecodeTimeWindow(const char *s)
{
    if (strcasecmp(s,"MAIN") != 0)
    {
	fWindow = MAIN_WINDOW;
    }
    else if (strcasecmp(s,"WIN1") != 0)
    {
	fWindow = WINDOW_1;
    }
    else if (strcasecmp(s,"WIN2") != 0)
    {
	fWindow = WINDOW_2;
    }
}
const char*  Description::sString(void) const
{
    static char s[128];
    memset( s, 0, sizeof(s));

    if (fTime)
    {
	sprintf( s, "%s ON %s", fX->sString(),sWindow());
    }
    else
    {
	sprintf( s, "%s vs %s", fY->sString(),fX->sString());
    } 
    return s;
}
