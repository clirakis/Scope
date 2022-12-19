/**
 ******************************************************************
 *
 * Module Name : ModuleDlg.cpp
 *
 * Author/Date : C. B. Lirakis/07-Feb-14
 *
 * Description : A dialog to display and manage channel characteristics.
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
#include "ModuleDlg.hh"
#include "ChannelFrame.hh"
#include "DSA602.hh"

/**
 ******************************************************************
 *
 * Function Name : ModuleDlg  Constructor
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
ModuleDlg::ModuleDlg(const TGWindow *main)
    : TGTransientFrame(gClient->GetRoot(), main, 60, 80)
{
    SetCleanup(kDeepCleanup);

    Connect("CloseWindow()", "ModuleDlg", this, "CloseWindow()");

    BuildDisplayArea();
    BuildButtonBox();

    SetWindowName("Module Data");
    SetIconName("Module Data");

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
void ModuleDlg::BuildButtonBox()
{
    TGButton *tb;

    // Create a frame to hold the buttons.
    TGCompositeFrame *ButtonFrame = new
        TGCompositeFrame(this, 600, 20, kHorizontalFrame);

    TGLayoutHints* fL2 = new
        TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5);

    tb = new TGTextButton( ButtonFrame, "  &Done  ");
    tb->Connect("Clicked()", "ModuleDlg", this, "Done()");
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
void ModuleDlg::BuildDisplayArea()
{
    Module*           pmg;
    TGCompositeFrame* tf;
    DSA602* ptr     = DSA602::GetThis();
    StatusAndEvent* pSE = ptr->pStatusAndEvent();
    unsigned char n = pSE->GetNModule();
    unsigned char i;

    TGTab *Tab = new TGTab(this, 600, 600);
    //Tab->Connect("Selected(Int_t)", "BCUI", this, "DoTab(Int_t)");
    TGLayoutHints *L5 = new TGLayoutHints(kLHintsTop | kLHintsExpandX |
                                          kLHintsExpandY, 2, 2, 5, 1);
    for (i=0;i<n;i++)
    {
	pmg = pSE->GetModule(i);
	if (pmg != NULL)
	{
	    tf = Tab->AddTab(pmg->ModuleString());
	    if (pmg->GetNChannel()>1)
	    {
		AddMultipleChannels( tf, pmg);
	    }
	    else
	    {
#if 0  // FIXME
		new ChannelFrame(tf, pmg->GetChannel(0));
#endif
	    }
	}
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
void ModuleDlg::CloseWindow()
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
void ModuleDlg::Done()
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
void ModuleDlg::DoClose()
{
   // Handle close button.
    SendCloseMessage();
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
void ModuleDlg::AddMultipleChannels(TGCompositeFrame* f, void *p)
{
#if 0
    ModuleGPIB* pmg = (ModuleGPIB *) p;
    int i,n;
    n = pmg->GetNChannel();

    for(i=0;i<n;i++)
    {
	new ChannelFrame(f, pmg->GetChannel(i));
    }
#endif
}

