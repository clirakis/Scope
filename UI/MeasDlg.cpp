/**
 ******************************************************************
 *
 * Module Name : MeasDlg.cpp
 * 
 * Author/Date : C. B. Lirakis/07-Feb-14
 *
 * Description : A dialog displaying waveform parameters
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 * 24-Dec-22  CBL fix checkboxes to setup a list of measurements to make
 *
 * Classification : Unclassified
 *
 * References :
 *     MSList.cpp, MSList.hh
 * 
 *******************************************************************
 */
#include <iostream>
using namespace std;
/// Root Includes
#include <TROOT.h>
#include <TGClient.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGTextEntry.h>
#include <TGMsgBox.h>
#include <TVirtualX.h>

/// Local Includes.
#include "debug.h"
#include "MeasDlg.hh"
#include "Measurement.hh"
#include "DSA602.hh"

/**
 ******************************************************************
 *
 * Function Name : MeasDlg  Constructor
 *
 * Description : 
 *
 * Inputs : main - pointer to parent window
 *          p - pointer to scope info
 *
 * Returns : Constructed GUI dialog
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
MeasDlg::MeasDlg(const TGWindow *main)
    : TGTransientFrame(gClient->GetRoot(), main, 60, 80)
{
    SET_DEBUG_STACK;

    SetCleanup(kDeepCleanup);

    Connect("CloseWindow()", "MeasDlg", this, "CloseWindow()");

    BuildUserArea();
    BuildButtonBox();
    ReadState();
    Update();

    SetWindowName("Waveform Data");
    SetIconName("WaveForm Data");

    MapSubwindows();
    // Read the state of the measurement list. 
    // This will populate the CB list etc. 

    Resize();

    // position relative to the parent's window
    Window_t wdum;
    int ax, ay;

    // window covers right half of parent window
    gVirtualX->TranslateCoordinates(
                    main->GetId(), this->GetParent()->GetId(),
                    0,
                    (Int_t)(((TGFrame *) main)->GetHeight() - this->GetHeight()) >> 1,
                    ax, ay, wdum);

    Move((((TGFrame *) main)->GetWidth() >> 1) + ax, ay);
    SetWMPosition((((TGFrame *) main)->GetWidth() >> 1) + ax, ay);
    MapWindow();
    fClient->WaitFor(this);
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : MeasDlg Destructor
 *
 * Description : Clean up after ourselves
 *
 * Inputs : NONE
 *
 * Returns : NONE
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 25-Dec-22
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
MeasDlg::~MeasDlg(void)
{
    SET_DEBUG_STACK;

    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : BuildButtonBox
 *
 * Description : Creates the GUI buttons Ok and Cancel
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
void MeasDlg::BuildButtonBox(void)
{
    SET_DEBUG_STACK;
    TGButton *tb;

    // Create a frame to hold the buttons.
    TGCompositeFrame *ButtonFrame = new
        TGCompositeFrame(this, 600, 20, kHorizontalFrame);

    TGLayoutHints* fL2 = new
        TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5);

    tb = new TGTextButton( ButtonFrame, "  &Read  ");
    tb->Connect("Clicked()", "MeasDlg", this, "DoRead()");
    ButtonFrame->AddFrame(tb, fL2);

    tb = new TGTextButton( ButtonFrame, "  &Done  ");
    tb->Connect("Clicked()", "MeasDlg", this, "DoDone()");
    ButtonFrame->AddFrame(tb, fL2);

    ButtonFrame->Resize();
    AddFrame(ButtonFrame, new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2));
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : BuildDisplayArea
 *
 * Description : Make an area that enables the user to both read
 * the waveform measurements upon request and setup those measurements. 
 *               
 * Inputs : None
 *
 * Returns : None
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
void MeasDlg::BuildUserArea(void)
{
    SET_DEBUG_STACK;
    DSA602*     scope = DSA602::GetThis();
    Measurement *meas = scope->pMeasurement();

    // Put all of this into a really big Vertical Frame. 
    TGVerticalFrame *vf = new TGVerticalFrame(this, 200, 400);

    TGLayoutHints   *vlx = new TGLayoutHints(
	kLHintsTop|kLHintsLeft|kLHintsExpandX, 2, 2, 2, 2);
    TGLayoutHints   *vl = new TGLayoutHints(
	kLHintsTop|kLHintsLeft, 2, 2, 2, 2);

    // Label the waveform number for what is to follow.
    fTrace = new TGLabel(vf, new TGHotString("Trace: 1"));
    vf->AddFrame(fTrace, vl);

    TGHorizontalFrame *hf = new TGHorizontalFrame( vf, 200, 400);
    vf->AddFrame( hf, vl);

    // Now add everything. 

    // ===========================================================
    TGGroupFrame*  gf = new TGGroupFrame( hf, "Available:", 
                                         kHorizontalFrame);
    /*
     * Make this parametric on the number of columns. 
     */
    UInt_t n = Measurement::kNMeasurements;
    UInt_t ncols = 4;
    UInt_t nrows = n/ncols;
    const char *name;
    if (n%2>0) nrows++;

    /*
     * Checkbox group frame.
     * make it square
     * Rows, Columns, Interval between frames, hints
     * if n is odd add another row. 
     *
     */
    gf->SetLayoutManager(new TGMatrixLayout( gf, nrows, ncols+1, 2, 2));

    // Add all the checkbuttons.
    for(uint8_t i=0; i<Measurement::kNMeasurements; i++)
    {
	name = meas->GetByIndex((Measurement::MTYPES)i)->Text();
	fCB[i] = new TGCheckButton( gf, new TGHotString(name), i);
	fCB[i]->Connect("Clicked()", "MeasDlg", this, "ButtonChecked()");
	gf->AddFrame(fCB[i]);
    }

#if 0 // DON'T need this. 
    // Add an apply button as the last thing
    tb = new TGTextButton( gf, "  &Apply  ");
    tb->Connect("Clicked()", "MeasDlg", this, "DoApply()");
    gf->AddFrame(tb);
#endif
    hf->AddFrame(gf, vlx);


    // ===========================================================

    /* 
     * Add on the right side the Values we have available for readout. 
     * and a refresh button. 
     */
    // Create measurement group frame. 
    TGGroupFrame* Labels = new TGGroupFrame( hf, "Values:",kHorizontalFrame);
    Labels->SetLayoutManager(new TGMatrixLayout(Labels, 
						Measurement::kMaxReadout, 
						2, 10, 2));
    hf->AddFrame(Labels, vl);
    // Create the Labels for the activated data to readout. 
    for (UInt_t i=0;i<Measurement::kMaxReadout;i++)
    {
	fLabel[i] = new TGLabel( Labels, "NONE            ");
	Labels->AddFrame(fLabel[i]);
	fData[i]  = new TGLabel( Labels, "             0.0");
	Labels->AddFrame(fData[i]);
    }
    Labels->Resize();

    vf->Resize();
    AddFrame( vf, vlx);
    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : CloseWindow
 *
 * Description : Closes the window
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
void MeasDlg::CloseWindow()
{
    SET_DEBUG_STACK;
    // Called when closed via window manager action.
    delete this;
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : DoRead
 *
 * Description : User pressed the Read button -
 *               Read and display a new set of data. 
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
void MeasDlg::DoRead(void)
{
    SET_DEBUG_STACK;
    Update();
}
/**
 ******************************************************************
 *
 * Function Name : DoDone
 *
 * Description : Close the window
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
void MeasDlg::DoDone(void)
{
    SET_DEBUG_STACK;
    SendCloseMessage();
}
/**
 ******************************************************************
 *
 * Function Name : DoClose
 *
 * Description : Closes the window
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
void MeasDlg::DoClose(void)
{
    SET_DEBUG_STACK;
   // Handle close button.
    SendCloseMessage();
}
/**
 ******************************************************************
 *
 * Function Name : ButtonChecked
 *
 * Description : Handle an event when one of the Checkboxes is clicked. 
 *
 * Inputs : None
 *
 * Returns : None
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
void MeasDlg::ButtonChecked(void)
{
    SET_DEBUG_STACK;
    DSA602*     scope = DSA602::GetThis();
    Measurement *meas = scope->pMeasurement();
    /*
     * Which button sent us this message? We need the id as an index. 
     */
    TGButton *btn = (TGButton *) gTQSender;
    Int_t     id  = btn->WidgetId();
    // Check the stat of the button. 
    Bool_t status = fCB[id]->IsOn();

    /*
     * If I have done things right, then the id of the button 
     * should be the same as the request to be made in measurement.hh
     * Therefore we can use those functions to do the right thing. 
     */
    if (status)
    {
	/*
	 * If it is on, set another measurement, but check total first. 
	 * before we react, lets check the number of buttons checked. 
	 *
	 * SetActive will return true if all conditions are met. 
	 */
	if (meas->SetActive((Measurement::MTYPES)id))
	{
	    // Its ok, set the checkbox. 
	    fCB[id]->SetState(kButtonDown); 
	}
	else
	{
	    fCB[id]->SetState(kButtonUp);
	}
    }
    else
    {
	fCB[id]->SetState(kButtonUp);
	// Now remove this ID from the list. 
	meas->SetInactive((Measurement::MTYPES)id);
    }
    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : ReadState
 *
 * Description : For the checkboxes, which measurements are currently 
 *               active?
 *
 * Inputs : None
 *
 * Returns : None
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
void MeasDlg::ReadState(void)
{
    SET_DEBUG_STACK;
    DSA602*      scope = DSA602::GetThis();
    Measurement* pmeas = scope->pMeasurement();
    UInt_t i;

    // First turn all the checkboxes off. 
    for (i=0;i<Measurement::kNMeasurements;i++)
    {
	fCB[i]->SetState(kButtonUp);
    }

    // How many active items in the list. 
    UInt_t N = pmeas->ActiveList();
    Int_t index = 0;
    // Loop over them and retrived the ids. 
    for (i=0;i<N;i++)
    {
	index = pmeas->ActiveIndex(i);
	fCB[index]->SetState(kButtonDown);
    }
    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : Clear
 *
 * Description : For all the labels and checkboxes turn them off. 
 *
 * Inputs : NONE
 *
 * Returns : NONE
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 25-Dec-22
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void MeasDlg::Clear(void)
{
    UInt_t        i;
    for (i=0;i<6;i++)
    {
	fLabel[i]->SetText("NONE            ");
	fData[i]->SetText( "             0.0");
    }
    for (i=0;i<Measurement::kNMeasurements;i++)
    {
	fCB[i]->SetState(kButtonUp);
    }
}
/**
 ******************************************************************
 *
 * Function Name : Update
 *
 * Description : Fill the widgets with data. 
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
void MeasDlg::Update(void)
{
    DSA602*       scope = DSA602::GetThis();
    Measurement*  pmeas  = scope->pMeasurement();
    MeasurementA* pA;
    char          s[32];

    // Query the data. 
    pmeas->Update();

    // Loop over the known values and query the ones that are active
    // How many active items in the list. 
    UInt_t N = pmeas->ActiveList();
    UInt_t index = 0;
    // Loop over the active elements  and retrived the ids. 
    for (UInt_t i=0;i<N;i++)
    {
	index = pmeas->ActiveIndex(i);
	pA = pmeas->GetByIndex((Measurement::MTYPES)index);
	fLabel[i]->SetText(pA->Text());
	sprintf(s, "%g", pA->Value());
	fData[i]->SetText(s);
    }
    SET_DEBUG_STACK;
}
