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
        TGCompositeFrame(this, 600, 20, kHorizontalFrame|kSunkenFrame);

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
	kLHintsTop|kLHintsLeft|kLHintsExpandX, 2, 2, 2, 2);

    // 1 =================================================================

    TGHorizontalFrame *hf = new TGHorizontalFrame( frame, 200, 100);
    frame->AddFrame(hf, lh);
    label = new TGLabel(hf, new TGHotString("Time base:"));
    hf->AddFrame(label);
    fTime = new TGComboBox(hf);
    for (i=0; i<TimeBase::kTB_END;i++)
    {
	fTime->AddEntry(TimeBase::Period[i].label, i);
    }
    fTime->Select(0,kFALSE);
    fTime->Resize( 40, 40);
    hf->AddFrame(fTime, lh);
    fTime->Connect("Selected(int)", "TimeDlg", this, "SetTime(int)");

    // 2 =================================================================
    /*
     * Make a frame of radio buttons that permit selection 
     * of the number of samples per trace. These vary based on the
     * allowed number of samples per time period. 
     */
    TGGroupFrame *gf=new TGGroupFrame(frame,"SampleLength",kHorizontalFrame);
    for (i=TimeBase::k512; i<TimeBase::k_LENGTH_END ; i++)
    {
	pRB = new TGRadioButton( gf, new TGHotString(LengthText[i]), i);
	gf->AddFrame(pRB);
	pRB->Connect("Clicked()", "TimeDlg", this, "SetLength()");
	fLength[i] = pRB;
    }
    gf->Resize(40, 60);
    frame->AddFrame(gf, lh);

    // 3 =================================================================
    hf = new TGHorizontalFrame( frame, 200, 100);
    frame->AddFrame(hf, lh);
    label = new TGLabel(hf, new TGHotString("XIncr:"));
    hf->AddFrame(label);
    fXIncr = new TGLabel(hf, new TGHotString("XXXXXX"));
    hf->AddFrame(fXIncr);

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
    TimeBase* tb = TimeBase::GetThis();
    Double_t  t; 
    Int_t     i;
 
    // TESTME
#ifdef WINDOW
    // This may be window or main depending on setup???
    t = tb->WindowTime();
#else
    t = tb->MainTime();
#endif
    // Find index
    i = tb->IndexFromTime(t);

    // Set it in the dialog box to the correct value. 
    fTime->Select( i, kFALSE);

    // update the combo box with the proper selections. 
    UpdateLength(i);
    UpdateXIncr();
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : SetTime
 *
 * Description : Set the time base for the trace. 
 *
 * Inputs : index - which one of the values for the timebase was selected?
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
void TimeDlg::SetTime(int index)
{
    SET_DEBUG_STACK;
    TimeBase *tb = TimeBase::GetThis();
    /*
     * Since I initially wrote this I didn't know the difference between
     * main and window. For the moment, we will use main. 
     *
     * This is in response to a callback associated with fTime index. 
     */
    tb->MainTime((TimeBase::PERIOD)index);   // main window only. 
    tb->WindowTime((TimeBase::PERIOD)index); // Set both the same
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
 * Unit Tested on: 
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
    double    len;
    Bool_t    rv;

    // Make sure this index is up to date before making inquiry. 
    tb->SampleLengthsFromTimeIndex((TimeBase::PERIOD)index);

    // Turn all buttons off, enable valid lengths. 
    for (uint32_t i=0;i<TimeBase::k_LENGTH_END;i++)
    {
	rv = tb->SampleLengthByIndex(i).valid;
	fLength[i]->SetEnabled(rv);
	fLength[i]->SetState(kButtonUp);
    }
#if 0
    // And which one is currently checked?
    len = tb->WindowLength(true);
#else
    len = tb->MainLength(true);
#endif
    int32_t idx = tb->IndexFromLength(len);
    fLength[idx]->SetState(kButtonDown);
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : SetLength
 *
 * Description : The user has chosen an available sample length. 
 *               send the selection to the scope. This is handled through
 *               presses on a radio button. 
 *
 * Inputs : NONE, inferred is the ID of the radio button
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
void TimeDlg::SetLength(void)
{
    SET_DEBUG_STACK;
    /*
     * Which button sent us this message? We need the id as an index. 
     */
    TGButton *btn = (TGButton *) gTQSender;
    Int_t id = btn->WidgetId();
    /* 
     * Make sure the radio behaviour is followed. 
     * uncheck the old button. 
     */
    for (Int_t i=0; i<TimeBase::k_LENGTH_END; i++)
    {
	/*
	 * loop over all the buttons. Make sure they aren't set unless
	 * they are the desired button. 
	 * The ids given are in the order of the buttons. This makes it easy
	 * to make a match on which button needs to be pressed. 
	 */
	if ((i!=id) && (fLength[i]->IsOn()))
	{
	    fLength[i]->SetState(kButtonUp);
	}
    }
    TimeBase *tb = TimeBase::GetThis();
    tb->MainLength(id); 
    UpdateXIncr();
}
void TimeDlg::UpdateXIncr(void)
{
    SET_DEBUG_STACK;
    char s[32];
    TimeBase *tb = TimeBase::GetThis();

    memset(s, 0, sizeof(s));
    double val = tb->MainXIncrement();
    sprintf(s, "%g", val);
    fXIncr->SetText(s);
    SET_DEBUG_STACK;
}
