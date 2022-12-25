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
#include "MSLIST.hh"
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

    /* 
     * Setup a list for potential measurements. 
     */
    fMeas  = new MSLIST;

    SetCleanup(kDeepCleanup);

    Connect("CloseWindow()", "MeasDlg", this, "CloseWindow()");

    BuildUserArea();
    BuildButtonBox();

    SetWindowName("Waveform Data");
    SetIconName("WaveForm Data");

    MapSubwindows();
    // Read the state of the measurement list. 
    // This will populate the CB list etc. 
    ReadState();
    ReadValue();

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
 * Function Name : Clear
 *
 * Description : For all the labels and checkboxes turn them off. 
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
MeasDlg::~MeasDlg(void)
{
    SET_DEBUG_STACK;
    delete fMeas;
    fMeas = NULL;
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

    tb = new TGTextButton( ButtonFrame, "  &OK  ");
    tb->Connect("Clicked()", "MeasDlg", this, "DoOK()");
    ButtonFrame->AddFrame(tb, fL2);

    tb = new TGTextButton( ButtonFrame, "  &Cancel  ");
    tb->Connect("Clicked()", "MeasDlg", this, "DoCancel()");
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
    MeasurementA*  p;
    TGTextButton*  tb;

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
    Int_t n = fMeas->Length();
    Int_t ncols = 4;
    Int_t nrows = n/ncols;
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
    TListIter next(fMeas->GetList());
    UInt_t i = 0;
    while ((p = (MeasurementA *)next()))
    {
	fCB[i] = new TGCheckButton( gf, new TGHotString(p->Text()), i);
	fCB[i]->Connect("Clicked()", "MeasDlg", this, "ButtonChecked()");
	gf->AddFrame(fCB[i]);
	i++;
    }

    // Add an apply button as the last thing
    tb = new TGTextButton( gf, "  &Apply  ");
    tb->Connect("Clicked()", "MeasDlg", this, "DoApply()");
    gf->AddFrame(tb);
    hf->AddFrame(gf, vlx);


    // ===========================================================

    /* 
     * Add on the right side the Values we have available for readout. 
     * and a refresh button. 
     */
    // Create measurement group frame. 
    TGGroupFrame* Labels = new TGGroupFrame( hf, "Values:",kHorizontalFrame);
    Labels->SetLayoutManager(new TGMatrixLayout(Labels, kMaxReadout, 2, 10, 2));
    hf->AddFrame(Labels, vl);
    // Create the Labels for the activated data to readout. 
    for (UInt_t i=0;i<MeasDlg::kMaxReadout;i++)
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
    cout << "Close window." << endl;
    delete this;
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : DoOK
 *
 * Description : User pressed the OK button -
 *               Set the user-provided storage to the new minimum
 *               and maximum and close the window
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
void MeasDlg::DoOK()
{
#if 0
    Int_t retval;
    const char *DisplayError = "Remote Display Error";
    const char *DisplayErrorMessage = "Please enter the address of the remote display";

    display->Insert(0, fDisplayName->GetText());
    if (display->IsWhitespace())
    {
        new TGMsgBox(fClient->GetRoot(),
                     this, DisplayError, DisplayErrorMessage,
                     kMBIconExclamation, kMBOk, &retval);
        return;
    }

    displaySet = kTRUE;
    *result = 0;
#endif
    SendCloseMessage();
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : DoCancel
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
void MeasDlg::DoCancel()
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
    /*
     * Which button sent us this message? We need the id as an index. 
     */
    TGButton *btn = (TGButton *) gTQSender;
    Int_t     id  = btn->WidgetId();
    // Check the stat of the button. 
    Bool_t status = fCB[id]->IsOn();
    TList*     lm = fMeas->GetList();
    MeasurementA* p;


    // FIXME --- Need to actually do something now. 
    if (status)
    {
	/*
	 * If it is on, set another measurement, but check total first. 
	 * before we react, lets check the number of buttons checked. 
	 */
	TListIter     next(lm);
	UInt_t n = 0;  // initialize count to zero. 
	while ((p = (MeasurementA *)next()))
	{
	    if (p->State())
	    {
		n++;
	    }
	}
	p = (MeasurementA *)lm->At(id);
	if (n<kMaxReadout)
	{
	    // Its ok, set the checkbox. 
	    //fCB[id]->SetState(kButtonDown); 
	    p->SetState(true);
	}
	else
	{
	    fCB[id]->SetState(kButtonUp);
	    p->SetState(false);
	}
    }
    else
    {
	//fCB[id]->SetState(kButtonDown);
	p = (MeasurementA *)lm->At(id);
	p->SetState(false);
    }

    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : 
 *
 * Description : Creates the widgets we want to display data in. 
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
void MeasDlg::ReadState(void)
{
    SET_DEBUG_STACK;
    DSA602* p = DSA602::GetThis();
    char msg[256];
#if 0
    p->Command("MSLIST?", msg, sizeof(msg));
    fMeas->FillState(msg);
    //cout << *fMeas << endl;
    // After read state, we then know what items we can interrogate. 
    CreateLabels();
#endif
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : 
 *
 * Description : Creates the widgets we want to display data in. 
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
void MeasDlg::ReadValue()
{
    DSA602*  p = DSA602::GetThis();
    char     msg[256];
    Int_t    i;
    Double_t val;
    const char*    s;

#if 0
    p->Command("MEAS?", msg, sizeof(msg));
    fMeas->FillValue(msg);
    cout << "Measurement: " << msg << endl;

    // After read state, Populate values
    for (i=0;i<6;i++)
    {
	const TGString *tgs = fLabel[i]->GetText();

	if (tgs->CompareTo("NULL") > 0)
	{
	    s = tgs->Data();
	    //cout << "Label:" << s << " " << *fMeas->Find(s) << endl;
	    val = fMeas->Find(s)->Value();
	    sprintf(msg,"%10.2g", val);
	    fData[i]->SetText(msg);
	}
    }
#endif
}
/**
 ******************************************************************
 *
 * Function Name : DoApply
 *
 * Description : User pressed the Apply button for the measurement dialog.
 *               Query results, buld and send the message.
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
void MeasDlg::DoApply()
{
    cout << "Apply not yet populated." << endl;
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
void MeasDlg::CreateLabels(void)
{
    MeasurementA* p;
    TListIter    next(fMeas->GetList());
    Int_t        i, j;

    Clear();
    i = j = 0;
    while ((p = (MeasurementA *)next()))
    {
	if (p->State())
	{
	    fCB[i]->SetState(kButtonDown);
	    fLabel[j]->SetText(p->Text());
	    j++;
	}
	i++;
    }
}
/**
 ******************************************************************
 *
 * Function Name : Clear
 *
 * Description : For all the labels and checkboxes turn them off. 
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
