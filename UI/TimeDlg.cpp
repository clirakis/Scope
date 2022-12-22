/**
 ******************************************************************
 *
 * Module Name : TimeDlg.cpp
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
#include <string>

/// Root Includes
#include <TROOT.h>
#include <TGLabel.h>
#include <TGComboBox.h>
#include <TVirtualX.h>
#include <TGButton.h>

/// Local Includes.
#include "debug.h"
#include "TimeDlg.hh"
#include "TimeBase.hh"

/**
 ******************************************************************
 *
 * Function Name : TimeDlg  Constructor
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
TimeDlg::TimeDlg(const TGWindow *main)
    : TGTransientFrame(gClient->GetRoot(), main, 60, 40)
{
    SET_DEBUG_STACK;
    SetCleanup(kDeepCleanup);

    Connect("CloseWindow()", "TimeDlg", this, "CloseWindow()");

    BuildDisplayArea();
    BuildButtonBox();

    SetWindowName("Time base Data");
    SetIconName("Time base Data");

    MapSubwindows();
    Resize();

    Update();
    // position relative to the parent's window
    Window_t wdum;
    int ax, ay;

    // window covers right half of parent window
    gVirtualX->TranslateCoordinates(
                    main->GetId(), this->GetParent()->GetId(),
                    0,
                    (Int_t)(((TGFrame *) main)->GetHeight() - 
			    this->GetHeight()) >> 1,
                    ax, ay, wdum);

    Resize();
    Move((((TGFrame *) main)->GetWidth() >> 1) + ax, ay);
    SetWMPosition((((TGFrame *) main)->GetWidth() >> 1) + ax, ay);
    MapWindow();
    fClient->WaitFor(this);
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
void TimeDlg::BuildButtonBox(void)
{
    SET_DEBUG_STACK;
    TGButton *tb;

    // Create a frame to hold the buttons.
    TGCompositeFrame *ButtonFrame = new
        TGCompositeFrame(this, 600, 20, kHorizontalFrame);

    TGLayoutHints* fL2 = new
        TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5);

    tb = new TGTextButton( ButtonFrame, "  &Done  ");
    tb->Connect("Clicked()", "TimeDlg", this, "Done()");
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
void TimeDlg::BuildDisplayArea(void)
{
    SET_DEBUG_STACK;
    const char *LengthText[TimeBase::k_LENGTH_END] = {
	"512","1024","2048","4096","5120", "8192", "16384", "20464", "32768"};
    TGLabel       *label;
    Int_t         i;
    TGRadioButton *pRB;

    TGVerticalFrame *frame = new TGVerticalFrame( this, 200,400);
    TGLayoutHints *lh = new TGLayoutHints(
	kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY, 2, 2, 2, 2);

    // 1 =================================================================
    label = new TGLabel(frame, new TGHotString("Time base:"));
    frame->AddFrame(label);
    fTime = new TGComboBox(frame);
    for (i=0; i<TimeBase::kTB_END;i++)
    {
	fTime->AddEntry(TimeBase::Period[i].label, i);
    }

    fTime->Select(0,kFALSE);
    fTime->Resize( 60, 20);
    frame->AddFrame(fTime, lh);
    fTime->Connect("Selected(int)", "TimeDlg", this, "SetTime(int)");

    // 2 =================================================================
    TGGroupFrame  *gf = new TGGroupFrame( this, "SampleLength", 
					  kHorizontalFrame);
    frame->AddFrame(gf, lh);

    for (i=TimeBase::k512; i<TimeBase::k_LENGTH_END ; i++)
    {
	pRB = new TGRadioButton( gf, new TGHotString(LengthText[i]), i);
	gf->AddFrame(pRB);
	//pRB->Connect("Clicked()", "TimeDlg", this, "SetLength()");
	fLength[i] = pRB;
    }

    // 3 =================================================================
    label = new TGLabel(frame, new TGHotString("XIncr:"));
    frame->AddFrame(label);
    fXIncr = new TGLabel(frame, new TGHotString("XXXXXX"));
    frame->AddFrame(fXIncr);

    frame->Resize();
    AddFrame(frame, lh);
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
void TimeDlg::CloseWindow(void)
{
    // Called when closed via window manager action.
    SET_DEBUG_STACK;
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
void TimeDlg::Done()
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
void TimeDlg::DoClose()
{
    SET_DEBUG_STACK;
   // Handle close button.
    SendCloseMessage();
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
 * Unit Tested on: 14-Dec-14
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void TimeDlg::Update(void)
{
    SET_DEBUG_STACK;
    char s[32];
    Int_t i = 0;
    TimeBase *tb = TimeBase::GetThis();

    // TESTME
#if 0
    // Obtain the time index from the header. 
    // Get the current values of the waveform 
    DSA602::GetThis()->UpdatewaveformHeader();
    Int_t i = 0;
    // Not sure how to do this in the new style. 
#else
    // This may be window or main depending on setup???
    Double_t t = tb->WindowTime();
    // Find index
    i = tb->IndexFromTime(t);
#endif
    // Set it in the dialog box to the correct value. 
    fTime->Select( i, kFALSE);

    // update the combo box with the proper selections. 
    UpdateLength(i);

    memset(s, 0, sizeof(s));
    //sprintf(s, "%f", h->XIncrement());
    sprintf(s, "%6.2f", tb->MainXIncrement());
    fXIncr->SetText(s);
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
 * Unit Tested on: 14-Dec-14
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void TimeDlg::SetTime(int index)
{
    SET_DEBUG_STACK;
    //double  val = TimeBase::Period[index].DT;

#if 0 // TODO
    TimeGPIB* h = (TimeGPIB *) fGPIB;
    //cout << "UPDATE DT: " << val << endl;
    h->SendCommand( Timebase::TIME, val);
#else
    TimeBase *tb = TimeBase::GetThis();
    // Since I initially wrote this I didn't know the difference between
    // main and window. For the moment, we will use main. 
    tb->MainTime(index);   // indexes should be the same TESTME
#endif
    UpdateLength(index);
    UpdateXIncr();
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : UpdateLength
 *
 * Description : Based on user selection of time, update the available
 *               sample lengths
 *
 * Inputs : None
 *
 * Returns : None
 *
 * Error Conditions :
 *
 * Unit Tested on: 14-Dec-14
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void TimeDlg::UpdateLength(int index)
{
    SET_DEBUG_STACK;
    // Everything is in the class timebase. 
    TimeBase *tb = TimeBase::GetThis();
    Bool_t rv;

    // Make sure this index is up to date before making inquiry. 
    tb->SampleLengthsFromTimeIndex((TimeBase::PERIOD)index);

    for (uint32_t i=0;i<TimeBase::k_LENGTH_END;i++)
    {
	rv = tb->SampleLengthByIndex(i).valid;
	fLength[i]->SetEnabled(rv);
	fLength[i]->SetState(kButtonUp);
    }
    // And which one is currently checked?
    double wl = tb->WindowLength(true);
    //cout << "Current window length." << wl << endl;
    int32_t idx = tb->IndexFromLength(wl);
    fLength[idx]->SetState(kButtonDown);
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : SetLength
 *
 * Description : The user has chosen an available sample length. 
 *               send the selection to the scope. 
 *
 * Inputs : index from drop down dialog. 
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
void TimeDlg::SetLength(int index)
{
    SET_DEBUG_STACK;
    cout << "Radio button clicked." << index << endl;
    TimeBase *tb = TimeBase::GetThis();
    tb->MainLength(index); // This isn't really correct yet. FIXME
    UpdateXIncr();
}
void TimeDlg::UpdateXIncr(void)
{
    SET_DEBUG_STACK;
    char s[32];
    TimeBase *tb = TimeBase::GetThis();

    memset(s, 0, sizeof(s));
    sprintf(s, "%f", tb->MainXIncrement());
    fXIncr->SetText(s);
    SET_DEBUG_STACK;
}
