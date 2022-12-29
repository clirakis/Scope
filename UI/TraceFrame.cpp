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
    Trace_gf->SetLayoutManager(new TGMatrixLayout(Trace_gf, 8, 2, 10, 2));
    AddFrame(Trace_gf);

    // 1
    label = new TGLabel(Trace_gf, new TGHotString(" "));
    Trace_gf->AddFrame(label);
    
    fDisplay = new TGCheckButton(Trace_gf, "Display");
    Trace_gf->AddFrame(fDisplay);
    fDisplay->Connect("Clicked()", "TraceFrame", this, "SetDisplay()");


    // 2
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

    // 3
    label = new TGLabel(Trace_gf, new TGHotString("AC State:"));
    Trace_gf->AddFrame(label);
    fACState = new TGComboBox( Trace_gf);
    fACState->AddEntry("Enhanced",      1);
    fACState->AddEntry("Not Enhanced",  0);
    fACState->Resize( Width,20);
    fACState->Connect("Selected(int)", "TraceFrame", this, 
		       "SetACState(int)");
    Trace_gf->AddFrame(fACState);

    // 4
    label        = new TGLabel(Trace_gf, new TGHotString("Description:"));
    Trace_gf->AddFrame(label);
    fDescription = new TGTextButton(Trace_gf, 
			       new TGHotString("                                     "));
    fDescription->Resize( Width,20);
    Trace_gf->AddFrame(fDescription);
    fDescription->Connect("Clicked()","TraceFrame",this,"EditDescription()");

    // 5
    label = new TGLabel(Trace_gf, new TGHotString("Graticule Loc:"));
    Trace_gf->AddFrame(label);
    fGRLocation = new TGComboBox( Trace_gf);
    fGRLocation->AddEntry("UPPER",      1);
    fGRLocation->AddEntry("LOWER",  0);
    fGRLocation->Resize( Width,20);
    fGRLocation->Connect("Selected(int)", "TraceFrame", this, 
			 "SetGRLocation(int)");
    Trace_gf->AddFrame(fGRLocation);


    // 6
    label = new TGLabel(Trace_gf, new TGHotString("Waveform calc:"));
    Trace_gf->AddFrame(label);
    fWFMCalc = new TGComboBox( Trace_gf);
    fWFMCalc->AddEntry("High Precision",      1);
    fWFMCalc->AddEntry("Fast",  0);
    fWFMCalc->Resize( Width,20);
    fWFMCalc->Connect("Selected(int)", "TraceFrame", this, 
			 "SetWFMCalc(int)");
    Trace_gf->AddFrame(fWFMCalc);

    // 7
    label  = new TGLabel(Trace_gf, new TGHotString("X Unit:"));
    Trace_gf->AddFrame(label);
    fXUNit = new TGLabel(Trace_gf, new TGHotString("Waveform calc:"));
    Trace_gf->AddFrame(fXUNit);


    // 8
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
    AdjTrace* pT      = pTrace->GetData(fNumber-1);    

    // Get the current Data. 
    if(pT)
	pT->Update();
    else
    {
	CLogger::GetThis()->Log("# TraceFrame::Update number error. %d\n",
				fNumber);
	return;
    }
#if 0
    fACCumulate->Select((Int_t)pWFM->ACCumulate(),kFALSE);
    fACState->Select((Int_t)pWFM->ACState(),kFALSE);
    fGRLocation->Select((Int_t)pWFM->GRLocation(),kFALSE);
    fWFMCalc->Select((Int_t)ch->WFMTYPE(),kFALSE);
    fDescription->SetText( pWFM->DEScription());
    fXUNit->SetText(UnitString(pWFM->XUnit()));
    fYUNit->SetText(UnitString(pWFM->YUnit()));
    n = ch->TraceNumber();

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
void TraceFrame::Apply(void)
{
    SET_DEBUG_STACK;
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
void TraceFrame::SetACCumulate(int val)
{
    SET_DEBUG_STACK;
    cout<< __func__ << " " << val << endl;
#if 0
    TraceGPIB* ch = (TraceGPIB *) fTraceGPIB;
    ch->SendCommand(Trace::CACCUMULATE, (ACCUMULATE) val);
#endif
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
void TraceFrame::SetACState(int val)
{
    SET_DEBUG_STACK;
    cout<< __func__ << " " << val << endl;
#if 0
    TraceGPIB* ch = (TraceGPIB *) fTraceGPIB;
    ch->SendCommand(Trace::CACCUMULATE, (ACCUMULATE) val);
#endif
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
void TraceFrame::SetGRLocation(int val)
{
    SET_DEBUG_STACK;
    cout<< __func__ << " " << val << endl;
    //TraceGPIB* ch = (TraceGPIB *) fTraceGPIB;
    //ch->SendCommand(Trace::CACCUMULATE, (ACCUMULATE) val);
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
void TraceFrame::SetWFMCalc(int val)
{
    SET_DEBUG_STACK;
    cout<< __func__ << " " << val << endl;
    //TraceGPIB* ch = (TraceGPIB *) fTraceGPIB;
    //ch->SendCommand(Trace::CACCUMULATE, (ACCUMULATE) val);
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
void  TraceFrame::SetDisplay(void)
{
    SET_DEBUG_STACK;
#if 0
    DSA602* ptr   = DSA602::GetThis();
    TraceGPIB* ch = (TraceGPIB *) fTraceGPIB;
    Bool_t val    = fDisplay->GetState();
    int n         = ch->TraceNumber();
    ptr->SetDisplayTrace(n, val);
#endif
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
