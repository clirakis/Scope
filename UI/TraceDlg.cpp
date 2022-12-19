/**
 ******************************************************************
 *
 * Module Name : TraceDlg.cpp
 *
 * Author/Date : C. B. Lirakis/21-Dec-14
 *
 * Description : A dialog to display and manage displayed traces
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
#include <cmath>
#include <string.h>
#include <stdlib.h>

/// Root Includes
#include <TROOT.h>
#include <TGClient.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGTextEntry.h>
#include <TGMsgBox.h>
#include <TGTab.h>
#include <TVirtualX.h>


/// Local Includes.
#include "debug.h"
#include "TraceDlg.hh"
#include "TraceFrame.hh"
#include "Trace.hh"
#include "DSA602.hh"

/**
 ******************************************************************
 *
 * Function Name : TraceDlg  Constructor
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
TraceDlg::TraceDlg(const TGWindow *main)
    : TGTransientFrame(gClient->GetRoot(), main, 60, 80)
{
    SetCleanup(kDeepCleanup);

    Connect("CloseWindow()", "TraceDlg", this, "CloseWindow()");

    BuildDisplayArea();
    BuildButtonBox();

    SetWindowName("Trace Data");
    SetIconName("Trace Data");

    MapSubwindows();
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
void TraceDlg::BuildButtonBox()
{
    TGButton *tb;

    // Create a frame to hold the buttons.
    TGCompositeFrame *ButtonFrame = new
        TGCompositeFrame(this, 600, 20, kHorizontalFrame);

    TGLayoutHints* fL2 = new
        TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5);

    tb = new TGTextButton( ButtonFrame, "  &Done  ");
    tb->Connect("Clicked()", "TraceDlg", this, "Done()");
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
void TraceDlg::BuildDisplayArea()
{
    char title[32];
    TGCompositeFrame* tf;
    DSA602* ptr     = DSA602::GetThis();
    Trace* pTrace   = ptr->GetTrace();
    unsigned char n = pTrace->GetNTrace();
    unsigned char i;

    TGTab *Tab = new TGTab(this, 600, 600);
    TGLayoutHints *L5 = new TGLayoutHints(kLHintsTop | kLHintsExpandX |
                                          kLHintsExpandY, 2, 2, 5, 1);
    for (i=0;i<n;i++)
    {
	sprintf(title,"TRACE%1d", i);
	tf = Tab->AddTab(title);
	new TraceFrame(tf, ptr, i+1);
    }
    Tab->Resize();
    AddFrame(Tab, L5);
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
void TraceDlg::CloseWindow()
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
void TraceDlg::Done()
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
void TraceDlg::DoClose()
{
   // Handle close button.
    SendCloseMessage();
}
