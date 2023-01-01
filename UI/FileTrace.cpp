/********************************************************************
 *
 * Module Name : FileTrace.cpp
 *
 * Author/Date : C.B. Lirakis / 23-May-21
 *
 * Description : Generic FileTrace
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

// Use root as a save mechanism. 
#include <TFile.h>
#include <TGraph.h>

// Local Includes.
#include "debug.h"
#include "CLogger.hh"
#include "SPlot.hh"
#include "DSA602.hh"
#include "FileTrace.hh"

/**
 ******************************************************************
 *
 * Function Name : FileTrace constructor
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
FileTrace::FileTrace (const char *Filename)
{
    SET_DEBUG_STACK;
    // If the suffix root is found, save in cern root format. 
    if (strstr(Filename, "root"))
    {
	SaveRoot(Filename);
    }
    else if (strstr(Filename, "csv"))
    {
	SaveCSV(Filename);
    }
    else
    {
	CLogger::GetThis()->Log("# Unrecognized file format %s\n",
				Filename);
    }
    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : FileTrace destructor
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
FileTrace::~FileTrace (void)
{
    SET_DEBUG_STACK;
    SET_DEBUG_STACK;
}


/**
 ******************************************************************
 *
 * Function Name : FileTrace function
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
bool FileTrace::SaveRoot(const char *Name)
{
    SET_DEBUG_STACK;
    DSA602*      scope = DSA602::GetThis();
    CLogger*     log   = CLogger::GetThis();
    /*
     * Open a TFile to store everything in. 
     * scope of tf is local.
     */
    TFile tf ( Name, "CREATE", "DSA602 Scope Data" );
    
    /*
     * Assume the tgraph exists and write it to a file. 
     * Get an erro on write. Not assocated with a file.
     * Kinda works.  
     */
    cout << "Calling TGRAPH Write." << endl;
    SPlot::GetThis()->GetGraph()->Write();

    cout << "CLosing file." << endl;
    tf.Write();
    tf.Close();
    SET_DEBUG_STACK;
    return true;
}
/**
 ******************************************************************
 *
 * Function Name : FileTrace function
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
bool FileTrace::LoadRoot(const char *Name)
{
    SET_DEBUG_STACK;
    cout << __FUNCTION__ << " NOT IMPLEMENTED " << endl;
    SET_DEBUG_STACK;
    return true;
}
/**
 ******************************************************************
 *
 * Function Name : FileTrace function
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
bool FileTrace::SaveCSV(const char *Name)
{
    SET_DEBUG_STACK;
    cout << __FUNCTION__ << " NOT IMPLEMENTED " << endl;
    SET_DEBUG_STACK;
    return true;
}
/**
 ******************************************************************
 *
 * Function Name : FileTrace function
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
bool FileTrace::LoadCSV(const char *Name)
{
    SET_DEBUG_STACK;
    cout << __FUNCTION__ << " NOT IMPLEMENTED " << endl;
    SET_DEBUG_STACK;
    return true;
}
