/********************************************************************
 *
 * Module Name : IIR.cpp
 *
 * Author/Date : C.B. Lirakis / 08-Oct-08
 *
 * Description : IIR module
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
 ********************************************************************/

#ifndef lint
static char rcsid[]="$Header$";
#endif

// System includes.
#include <iostream>
using namespace std;
#include <string>
#include <cmath>
#include <fstream>

// root includes
#include <TVectorD.h>

// Local Includes.
#include "IIR.hh"

/**
 ******************************************************************
 *
 * Function Name : module constructor
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
IIR::IIR (Int_t ntaps)
{
    fType     = TYPE_NONE;
    AllocateArrays(ntaps);
    FillTest();
}

/**
 ******************************************************************
 *
 * Function Name : module constructor
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
IIR::IIR(const char *file)
{
    ReadMatlabDescription(file);
}
/**
 ******************************************************************
 *
 * Function Name : IIR destructor
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
IIR::~IIR ()
{
    delete fA;
    delete fB;
    delete fWork;
    delete fSamples;
}
/**
 ******************************************************************
 *
 * Function Name : module constructor
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
void IIR::AllocateArrays(Int_t N)
{
    fNTaps    = N;
    fA        = new TVectorD(fNTaps);
    fB        = new TVectorD(fNTaps);
    fWork     = new TVectorD(fNTaps);
    fSamples  = new TVectorD(fNTaps);
    fP        = 0;
    fNormalization = 1.0;
    fA->Zero();
    fB->Zero();
    fWork->Zero();
    fSamples->Zero();
}
/**
 ******************************************************************
 *
 * Function Name : IIR function
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
bool IIR::ReadMatlabDescription(const char *Name)
{
    Char_t  line[256], *p, *q;
    Bool_t  rc = kFALSE;
    Bool_t  ParseNumbers = kFALSE;
    Int_t   N, Count = 0;

    if (Name == NULL)
    {
	return rc;
    }
    ifstream in(Name, ios_base::in);
    if (in.fail())
    {
	cout << "Failed to open input file." << endl;
    }
    else
    {
	while (!in.eof())
	{
	    in.getline(line, sizeof(line));
	    if (strstr( line, "FIR") != NULL)
	    {
		fType = TYPE_FIR;
	    }
	    else if(strstr( line, "IIR") != NULL)
	    {
		fType = TYPE_IIR;
		cerr << "WARNING, IIR parsing and implimentation not complete" << endl;
	    }
	    else if((p = strstr( line, "Filter Length")) != NULL)
	    {
		q = strchr( line, (int)':');
		q++;
		N = atoi(q);
		AllocateArrays(N);
	    }
	    else if ( line[0] == '%')
	    {
		// Comment line, skip.
	    }
	    else if (strstr( line, "Numerator:") != NULL)
	    {
		ParseNumbers = kTRUE;
	    }
	    else if (ParseNumbers)
	    {
		if (Count < fNTaps)
		{
		    (*fA)(Count) = atof(line);
		}
		Count++;
	    }
	}
	in.close();
	fNormalization = fA->Sum() * fA->GetNoElements();
    }
    return rc;
}

/**
 ******************************************************************
 *
 * Function Name : IIR function
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
void IIR::AddCoeficient ( Int_t tap, Double_t A, Double_t B)
{
    if (tap<fNTaps)
    {
	fA[tap] = A;
	fB[tap] = B;
    }
}
/**
 ******************************************************************
 *
 * Function Name : IIR function
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
void IIR::FillTest()
{
    const Double_t val[] = {
	-0.02293439637154541174690614013798040105, 
	0.003577433200523213573801539766350288119,
	0.005011986125179099249571379459666786715,
	0.007413870270539838283618827574628085131,
	0.01068609371415164301100109156550388434,
	0.014723509243337210700142136943213699851,
	0.019403768610165787744659127156410249881,
	0.024577971785213360150779138280086044688,
	0.030144111450129441875933622441152692772,
	0.035832451770599349494261787185678258538,
	0.04149656259442192907105351196150877513 ,
	0.046936667537008043560309289432552759536,
	0.051951651718512306676878154121368424967,
	0.056345031659316334449183472088407143019,
	0.059954833106355590044422854134609224275,
	0.062648164951129389765860366878769127652,
	0.064297884161728061092055952485679881647,
	0.064854985583656904468696779986203182489,
	0.064297884161728061092055952485679881647,
	0.062648164951129389765860366878769127652,
	0.059954833106355590044422854134609224275,
	0.056345031659316334449183472088407143019,
	0.051951651718512306676878154121368424967,
	0.046936667537008043560309289432552759536,
	0.04149656259442192907105351196150877513,
	0.035832451770599349494261787185678258538,
	0.030144111450129441875933622441152692772,
	0.024577971785213360150779138280086044688,
	0.019403768610165787744659127156410249881,
	0.014723509243337210700142136943213699851,
	0.01068609371415164301100109156550388434,
	0.007413870270539838283618827574628085131,
	0.005011986125179099249571379459666786715,
	0.003577433200523213573801539766350288119,
	-0.02293439637154541174690614013798040105 
    };
    fA->SetElements(val);
    fNormalization = fA->Sum() * fA->GetNoElements();
}
/**
 ******************************************************************
 *
 * Function Name : IIR function
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
Double_t IIR::AddSample(Double_t x)
{
    Double_t rc = 0.0;
    if (fP == fNTaps)
    {
	*fWork = *fSamples;
	fSamples->SetSub(0, fWork->GetSub(1,fNTaps-1));
	fP = fNTaps-1;
    }

    (*fSamples)[fP] = x;

    fP++;

    (*fWork) = (*fA) * (*fSamples);

    rc = fWork->Sum()/fNormalization;
    return rc;
}
/**
 ******************************************************************
 *
 * Function Name : IIR function
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
void IIR::Print()
{
    cout << "Filter type: ";
    switch(fType)
    {
    case TYPE_NONE:
	cout << "Not defined!";
	break;
    case TYPE_FIR:
	cout << "FIR ";
	break;
    case TYPE_IIR:
	cout << "IIR ";
	break;
    }
    cout << " Taps: " << fNTaps << endl;
}
