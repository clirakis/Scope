/**
 ******************************************************************
 *
 * Module Name : TraceFrame.cpp
 *
 * Author/Date : C. B. Lirakis/23-Dec-14
 *
 * Description : A dialog displaying trace parameters
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References :
 * Trace.cpp
 * ADJtrace - page 44, Where is the trace on the screen?
 *    AdjTrace.cpp
 * Trace - page 292, How is the trace actually defined. 
 *    DefTrace.cpp
 *
 *******************************************************************
 */
#include <iostream>
using namespace std;
#include <string>

/// Root Includes
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include <TGComboBox.h>

/// Local Includes.
#include "debug.h"
#include "CLogger.hh"
#include "TraceFrame.hh"
#include "DSA602.hh"
#include "Trace.hh"
#include "DescDlg.hh"

/**
 ******************************************************************
 *
 * Function Name : TraceFrame
 *
 * Description : 
 *
 * Inputs : None
 *
 * Returns : None
 *
 * Error Conditions :
 *
 * Unit Tested on: 24-Dec-14
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
TraceFrame::TraceFrame(TGCompositeFrame* p, unsigned char n) : TGVerticalFrame(p, 100,300)
{
    SET_DEBUG_STACK;
    const Int_t Width = 120;
    TGLabel*   label;

    // Store the trace number this represents. Starts at 1 not 0.
    fNumber = n;

    TGGroupFrame *Trace_gf = new TGGroupFrame( this, "Trace data", 
						 kHorizontalFrame);

    // Rows, Columns, Interval between frames, hints
    Trace_gf->SetLayoutManager(new TGMatrixLayout(Trace_gf, 7, 2, 10, 2));
    AddFrame(Trace_gf);

    // 1
    label = new TGLabel(Trace_gf, new TGHotString("Accumulate:"));
    Trace_gf->AddFrame(label);
    fACCumulate = new TGComboBox( Trace_gf);
    fACCumulate->AddEntry("Infinte Persistance",  kINFPERSIST);
    fACCumulate->AddEntry("OFF",  kACC_OFF);
    fACCumulate->AddEntry("Variable Persistance", kVARPERSIST);
    fACCumulate->Resize( Width,20);
    fACCumulate->Connect("Selected(int)", "TraceFrame", this, 
		       "SetACCumulate(int)");
    Trace_gf->AddFrame(fACCumulate);

    // 2
    label = new TGLabel(Trace_gf, new TGHotString("AC State:"));
    Trace_gf->AddFrame(label);
    fACState = new TGLabel(Trace_gf, new TGHotString("Enhanced"));
    fACState->Resize( Width,20);
    Trace_gf->AddFrame(fACState);

    // 3
    label        = new TGLabel(Trace_gf, new TGHotString("Description:"));
    Trace_gf->AddFrame(label);
    fDescription = new TGTextButton(Trace_gf, 
			       new TGHotString("                                     "));
    fDescription->Resize( Width,20);
    Trace_gf->AddFrame(fDescription);
    fDescription->Connect("Clicked()","TraceFrame",this,"EditDescription()");

    // 4
    label = new TGLabel(Trace_gf, new TGHotString("Graticule Loc:"));
    Trace_gf->AddFrame(label);
    fGRLocation = new TGComboBox( Trace_gf);
    fGRLocation->AddEntry("UPPER",      1);
    fGRLocation->AddEntry("LOWER",  0);
    fGRLocation->Resize( Width,20);
    fGRLocation->Connect("Selected(int)", "TraceFrame", this, 
			 "SetGRLocation(int)");
    Trace_gf->AddFrame(fGRLocation);


    // 5
    label = new TGLabel(Trace_gf, new TGHotString("Waveform calc:"));
    Trace_gf->AddFrame(label);
    fWFMCalc = new TGLabel( Trace_gf, new TGHotString("Fast (integer)"));
    fWFMCalc->Resize( Width,20);
    Trace_gf->AddFrame(fWFMCalc);

    // 6
    label  = new TGLabel(Trace_gf, new TGHotString("X Unit:"));
    Trace_gf->AddFrame(label);
    fXUNit = new TGLabel(Trace_gf, new TGHotString("Waveform calc:"));
    Trace_gf->AddFrame(fXUNit);


    // 7
    label  = new TGLabel(Trace_gf, new TGHotString("Y Unit:"));
    Trace_gf->AddFrame(label);
    fYUNit = new TGLabel(Trace_gf, new TGHotString("Waveform calc:"));
    Trace_gf->AddFrame(fYUNit);

    // Add the frame and resize
    TGLayoutHints *L = new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX,
                                         5, 5, 5, 5);
    Resize();
    p->AddFrame( this, L);
    Update();
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : TraceFrame Destructor
 *
 * Description : Clean up
 *
 * Inputs : None
 *
 * Returns : None
 *
 * Error Conditions : NONE
 *
 * Unit Tested on:
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
TraceFrame::~TraceFrame(void)
{
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : Update
 *
 * Description : Update the parameters for this trace number. 
 *
 * Inputs : None
 *
 * Returns : None
 *
 * Error Conditions :
 *
 * Unit Tested on:
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void TraceFrame::Update(void)
{
    SET_DEBUG_STACK;
    DSA602*   scope   = DSA602::GetThis();
    Trace *   pTrace  = scope->GetTrace();

    // Get the current Data. 
    if(pTrace)
    {
	//AdjTrace* pAdjT   = pTrace->GetAdj(fNumber-1);
	DefTrace* pDefT   = pTrace->GetDef(fNumber-1);

	pTrace->Update();
	/*
	 * These are all values from issuing the command TR <ai>? 
	 */
	fACCumulate->Select( (Int_t)pDefT->Accumulate(false), kFALSE);

	if(pDefT->ACState(false))
	    fACState->SetText("Enhanced");
	else
	    fACState->SetText("Not Enhanced");

	fGRLocation->Select((Int_t)pDefT->GRLocation(false),kFALSE);
	if (pDefT->WFMCalc(false))
	    fWFMCalc->SetText("High Precision (floating point)");
	else
	    fWFMCalc->SetText("Fast (integer)");

	fDescription->SetText( pDefT->Description(false));
	fXUNit->SetText(UnitString(pDefT->XUNIT(false)));
	fYUNit->SetText(UnitString(pDefT->YUNIT(false)));
#if 0

	uint8_t n = pTrace->GetNTrace();
// FIXME
	if (ptr->GetDisplayTrace(n))
	{
	    fDisplay->SetState(kButtonDown, kFALSE);
	}
	else
	{
	    fDisplay->SetState(kButtonUp, kFALSE);
	}
#endif
    }
    else
    {
	CLogger::GetThis()->Log("# TraceFrame::Update number error. %d\n",
				fNumber);
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
 * Inputs : None
 *
 * Returns : None
 *
 * Error Conditions :
 *
 * Unit Tested on:
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
/**
 ******************************************************************
 *
 * Function Name : SetACCumulate
 *
 * Description : Set the trace accumulation to one of the input values. 
 *
 * Inputs : val
 *      kINFPERSIST - Selects infinite persistance. In this mode the 
 *                    waveform record points remain on the display 
 *                    indefinitely until some event clears the trace display. 
 * 
 *      kACC_OFF    - returns the trace to normal display mode. In normal
 *                    display mode, waveform record points are cleard 
 *                    from the display each time a new waveform record
 *                    is displayed. 
 *
 *      kVARPERSIST - selects variable persistance mode. In this mode
 *                    the waveform records remain on the display for 
 *                    the length of time specified by DISPLAY PERSISTANCE
 *                    before being cleared from the display.
 *
 *
 * Returns : None
 *
 * Error Conditions :
 *
 * Unit Tested on:
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void TraceFrame::SetACCumulate(int val)
{
    SET_DEBUG_STACK;
    DSA602*   scope   = DSA602::GetThis();
    Trace *   pTrace  = scope->GetTrace();

    // Get the current Data. 
    if(pTrace)
    {
	/*
	 * fNumber is the current trace number (1) but the index 
	 * starts at zero. 
	 */
	DefTrace *pdt = pTrace->GetDef(fNumber-1);
	pdt->Accumulate((ACCUMULATE) val);
    }
    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : SetGRLocation
 *
 * Description : Moves the selected waveform to the upper or lower 
 *               graticule. 
 *
 * Inputs : None
 *
 * Returns : None
 *
 * Error Conditions : GPIB error
 *
 * Unit Tested on: 31-Dec-22
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void TraceFrame::SetGRLocation(int val)
{
    SET_DEBUG_STACK;
    DSA602*   scope   = DSA602::GetThis();
    Trace *   pTrace  = scope->GetTrace();

    // Get the current Data. 
    if(pTrace)
    {
	/*
	 * fNumber is the current trace number (1) but the index 
	 * starts at zero. 
	 */
	DefTrace *pdt = pTrace->GetDef(fNumber-1);
	bool upper = (val==1);
	pdt->SetGRLocation(upper);
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
 * Inputs : None
 *
 * Returns : None
 *
 * Error Conditions :
 *
 * Unit Tested on:
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */

/**
 ******************************************************************
 *
 * Function Name : 
 *
 * Description : 
 *
 * Inputs : None
 *
 * Returns : None
 *
 * Error Conditions :
 *
 * Unit Tested on:
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void TraceFrame::EditDescription(void)
{
    SET_DEBUG_STACK;
    new DescDlg(this, NULL);
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : 
 *
 * Description : 
 *
 * Inputs : None
 *
 * Returns : None
 *
 * Error Conditions :
 *
 * Unit Tested on:
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
