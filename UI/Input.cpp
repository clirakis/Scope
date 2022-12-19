/********************************************************************
 *
 * Module Name : Input.cpp
 *
 * Author/Date : C.B. Lirakis / 04-Jan-15
 *
 * Input : Generic module
 *
 * Restrictions/Limitations :
 *
 * Change Inputs :
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
#include <cstring>
#include <cstring>
#include <stdlib.h>
#include <arpa/inet.h>

// Local Includes.
#include "Input.hh"
#include "GParse.hh"
#include "DSA602_Utility.hh"

const char* Input::FString[] = {"ABS", "AVG", "CONVOLUTION", "CORRELATION", 
				"DEJITTER", "DELAY", "DIFF", "ENV", "EXP", 
				"FFTIMAG", "FFTMAG", "FFTPHASE", "FFTREAL", 
				"FILTER", "IFFT", "INTG", "INTP", "LN", 
				"LOG", "PIADD", "PISUB", "PULSE", "SIGNUM", 
				"SMOOTH", "SQRT", "NONE",
				NULL};

const char* Input::OString[] = {"PLUS", "MINUS", "MULTIPLY", "DIVIDE", 
				"NONE", NULL};
const char  Input::cop[4]    = {'+','-','*','/'};

/**
 ******************************************************************
 *
 * Function Name : Input Constructor
 *
 * Input : A description can contain a list of inputs. The inputs
 * described here are an operation or not on a module with an operator. 
 * The permissable operators and functions are given at the bottom of 
 * this file. 
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
Input::Input(const char *val)
{
    fFunction = kFUNCTION_NONE;
    fOperator = kOPERATOR_NONE;

    if (val!=NULL)
    {
	DecodeString(val);
    }
}

/**
 ******************************************************************
 *
 * Function Name : Input destructor
 *
 * Input :
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
Input::~Input()
{

}
/**
 ******************************************************************
 *
 * Function Name : Input destructor
 *
 * Input :
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
ostream& operator<<(ostream& output, const Input &n)
{
    output << "============================================" << endl
	   << "Input: " ;
#if 0
    if (n.fFunction != Input::kFUNCTION_NONE)
    {
	output << n.sFunction(false)<< "(" 
	       << n.fModule->ChannelString() << ")";
    }
    else
    {
	output << n.fModule->ChannelString();
    }
    if (n.fOperator != Input::kOPERATOR_NONE)
    {
	output << " " << n.sOperator();
    }
#endif
    output << endl << "============================================" << endl;
    return output;
}

/**
 ******************************************************************
 *
 * Function Name : DecodeString
 *
 * Input : Assume that pre-parsing has occured and the form of the input 
 * string looks something like ABS(L1) +
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
void Input::DecodeString(const char *val)
{
    char *p = NULL;
    char *s, tmp[16], *local;
    int   n;

    // Make a local copy of the data and then remove components as 
    // we parse them. 
    local = strdup(val);

    /*
     * Typical channel query data. page 272
     * 'TRACE1 DESCRIPTION:"R1 ON MAIN"
     */
    // Is there an operator of any type?
    for (n=0;n<4;n++)
    {
	if ((p=strchr(local, cop[n]))!= NULL)
	{
	    fOperator = (TraceOperators) n;
	    *p = ' ';
	    n = 4;
	}
    }

    // Determine if there is a function involved. 
    if ((p=strchr(local, '('))!= NULL)
    {
	n = p-local;
	memset(tmp, 0, sizeof(tmp));
	memcpy(tmp, local, n);
	DecodeFunction(tmp);

	// Extract module and channel
	s = strchr(local, ')');
	if (s!=NULL)
	{
	    n = s-p-1;
	    p++;
	    memset(tmp, 0, sizeof(tmp));
	    memcpy( tmp, p, n);
#if 0 // FIXME
	    fModule = new ModuleChannel(tmp);
#endif
	}
    }
    else
    {
#if 0 // FIXME
	fModule = new ModuleChannel(local);
#endif
    }
    free(local);
}

const char* Input::sFunction(bool Y) const
{
    //int i = 0;
    //if (Y) i = 1;

    return FString[fFunction];
}
const char* Input::sOperator(void) const
{
    return OString[fOperator];
}
void Input::DecodeFunction(const char* str)
{
    // set fFunction to best match. 
    const char *p;
    int i = 0;
    fFunction = kFUNCTION_NONE;

    while ((p=FString[i])!= NULL)
    {
	if (strcasestr(str, p) != NULL)
	{
	    fFunction = (TraceFunctions) i;
	    return;
	}
	i++;
    }
}
void Input::DecodeOperator(const char* str)
{
    // set fFunction to best match. 
    const char *p;
    int i = 0;
    fOperator = kOPERATOR_NONE;

    while ((p=OString[i])!= NULL)
    {
	if (strcasestr(str, p) != NULL)
	{
	    fOperator = (TraceOperators)i;
	    return;
	}
	i++;
    }
}
const char* Input::sString(void) const
{
    static char s[128];

#if 0 // FIXME
    memset(s, 0, sizeof(s));
    if (fFunction == kFUNCTION_NONE)
    {
	sprintf( s, "%s", fModule->ChannelString());
    }
    else
    {
	sprintf( s, "%s(%s)", sFunction(), fModule->ChannelString());
    }
    if (fOperator != kOPERATOR_NONE)
    {
	strcat( s, sOperator());
    }
#endif
    return s;
}
