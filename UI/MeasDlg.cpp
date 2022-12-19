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
 *
 * Classification : Unclassified
 *
 * References :
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
    fMeas  = new MSLIST;

    SetCleanup(kDeepCleanup);

    Connect("CloseWindow()", "MeasDlg", this, "CloseWindow()");

    BuildDisplayArea();
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
}
MeasDlg::~MeasDlg()
{
    // 09-Feb-14 These never get deleted.
    //delete fMeas;
    //fMeas = NULL;
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
void MeasDlg::BuildButtonBox()
{
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
}
/**
 ******************************************************************
 *
 * Function Name : BuildDisplayArea
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
void MeasDlg::BuildDisplayArea()
{
    MeasurementA*  p;
    TGTextButton*  tb;
    TGGroupFrame*  gf = new TGGroupFrame( this, "Available:", 
                                         kHorizontalFrame);
    Int_t i;
    Int_t n = fMeas->Length();

    // Checkbox group frame.
    // Rows, Columns, Interval between frames, hints
    gf->SetLayoutManager(new TGMatrixLayout( gf, n/2+1, 2, 10, 2));


    // Add all the checkbuttons.
    TListIter next(fMeas->GetList());
    i = 0;
    while ((p = (MeasurementA *)next()))
    {
	fCB[i] = new TGCheckButton( gf, new TGHotString(p->Text()));
	gf->AddFrame(fCB[i]);
	i++;
    }

    // Add an apply button as the last thing
    tb = new TGTextButton( gf, "  &Apply  ");
    tb->Connect("Clicked()", "MeasDlg", this, "DoApply()");
    gf->AddFrame(tb);
    AddFrame(gf, new TGLayoutHints(kLHintsLeft, 2, 2, 2, 2));

    // *
    // Create measurement group frame. 
    TGGroupFrame* Labels = new TGGroupFrame( this, "Values:", 
					     kHorizontalFrame);
    Labels->SetLayoutManager(new TGMatrixLayout( Labels, 6, 2, 10, 2));

    for (i=0;i<6;i++)
    {
	fLabel[i] = new TGLabel( Labels, "NONE            ");
	Labels->AddFrame(fLabel[i]);
	fData[i]  = new TGLabel( Labels, "             0.0");
	Labels->AddFrame(fData[i]);
    }
    AddFrame( Labels, new TGLayoutHints(kLHintsLeft, 2, 2, 2, 2));
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
    // Called when closed via window manager action.
    cout << "Close window." << endl;
    delete this;
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
void MeasDlg::DoClose()
{
   // Handle close button.
    SendCloseMessage();
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
void MeasDlg::ReadState()
{
    DSA602* p = (DSA602 *) fScope;
    char msg[256];

    p->Command("MSLIST?", msg, sizeof(msg));
    fMeas->FillState(msg);
    //cout << *fMeas << endl;
    // After read state, we then know what items we can interrogate. 
    CreateLabels();
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
    DSA602*  p = (DSA602 *) fScope;
    char     msg[256];
    Int_t    i;
    Double_t val;
    const char*    s;

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
void MeasDlg::CreateLabels()
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
void MeasDlg::Clear()
{
    Int_t        i;
    for (i=0;i<6;i++)
    {
	fLabel[i]->SetText("NONE            ");
	fData[i]->SetText( "             0.0");
    }
    for (i=0;i<26;i++)
    {
	fCB[i]->SetState(kButtonUp);
    }
}
