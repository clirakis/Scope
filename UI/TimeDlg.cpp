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
#include <TGClient.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGComboBox.h>
#include <TGMsgBox.h>
#include <TVirtualX.h>


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
                    (Int_t)(((TGFrame *) main)->GetHeight() - this->GetHeight()) >> 1,
                    ax, ay, wdum);

    Move((((TGFrame *) main)->GetWidth() >> 1) + ax, ay);
    SetWMPosition((((TGFrame *) main)->GetWidth() >> 1) + ax, ay);
    MapWindow();
    fClient->WaitFor(this);
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
void TimeDlg::BuildButtonBox()
{
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
void TimeDlg::BuildDisplayArea()
{
    TGLabel      *label;
    Int_t        i;
    TGGroupFrame *gf = new TGGroupFrame( this, "Time base data", 
                                         kHorizontalFrame);
    // Rows, Columns, Interval between frames, hints
    gf->SetLayoutManager(new TGMatrixLayout( gf, 3, 2, 10, 2));

    // 1
    label = new TGLabel(gf, new TGHotString("Time base:"));
    gf->AddFrame(label);
    fTime = new TGComboBox(gf);
    i=0;
#if 0  // FIXME - I have no idea what fAllowed is.
    while (Timebase::fAllowed[i].label != NULL)
    {
	fTime->AddEntry(Timebase::fAllowed[i].label, i);
	i++;
    }
#endif
    fTime->Select(0,kFALSE);
    fTime->Resize( 60, 20);
    gf->AddFrame(fTime);
    fTime->Connect("Selected(int)", "TimeDlg", this, "SetTime(int)");

    // 2
    label = new TGLabel(gf, new TGHotString("NPoints:"));
    gf->AddFrame(label);
    fLength = new TGComboBox(gf);
    fLength->Resize( 60, 20);
    gf->AddFrame(fLength);
    fLength->Connect("Selected(int)", "TimeDlg", this, "SetLength(int)");

    // 3
    label = new TGLabel(gf, new TGHotString("XIncr:"));
    gf->AddFrame(label);
    fXIncr = new TGLabel(gf, new TGHotString("XXXXXX"));
    gf->AddFrame(fXIncr);

    //Resize();
    AddFrame(gf, new TGLayoutHints(kLHintsLeft, 2, 2, 2, 2));
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
void TimeDlg::CloseWindow()
{
    // Called when closed via window manager action.

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
#if 0  // FIXME
    TimeGPIB* h = (TimeGPIB *) fGPIB;
    char s[32];

    h->Update();
    Int_t i = h->TimeIndex();
    fTime->Select( i, kFALSE);

    // update the combo box with the proper selections. 
    UpdateLength(i);

    memset(s, 0, sizeof(s));
    sprintf(s, "%f", h->XIncrement());
    fXIncr->SetText(s);
#endif
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
#if 0
    TimeGPIB* h = (TimeGPIB *) fGPIB;
    double  val = Timebase::fAllowed[index].DT;
    //cout << "UPDATE DT: " << val << endl;
    h->SendCommand( Timebase::TIME, val);
    UpdateLength(index);
    UpdateXIncr();
#endif
}
/**
 ******************************************************************
 *
 * Function Name : 
 *
 * Description : based on the time interval, update the time combo box.
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
#if 0
    TimeGPIB* h = (TimeGPIB *) fGPIB;
    unsigned short bits = Timebase::fAllowed[index].AllowedLengths;
    int i;
    char s[32];

    fLength->RemoveAll();
    for (i=0;i<10;i++)
    {
	if ((bits&(1<<i))>0)
	{
	    memset(s, 0, sizeof(s));
	    sprintf(s, "%d", Timebase::fTB_Length[i]);
	    fLength->AddEntry(s,i);
	}
    }
    fLength->Select( h->LengthIndex(), kFALSE);
#endif
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
void TimeDlg::SetLength(int index)
{
#if 0
    TimeGPIB* h = (TimeGPIB *) fGPIB;
    double  val = Timebase::fTB_Length[index];
    //cout << "UPDATE LENGTH: " << val << endl;
    h->SendCommand( Timebase::LENGTH, val);
    UpdateXIncr();
#endif
}
void TimeDlg::UpdateXIncr(void)
{
#if 0
    char s[32];
    TimeGPIB* h = (TimeGPIB *) fGPIB;
    h->Update();
    
    memset(s, 0, sizeof(s));
    sprintf(s, "%f", h->XIncrement());
    fXIncr->SetText(s);
#endif
}
