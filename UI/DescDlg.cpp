/**
 ******************************************************************
 *
 * Module Name : DescDlg.cpp
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
#include "DescDlg.hh"
#include "Description.hh"
#include "DSA602.hh"
#include "DSA602_Utility.hh"

/**
 ******************************************************************
 *
 * Function Name : DescDlg  Constructor
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
DescDlg::DescDlg(const TGWindow *main, void *p)
    : TGTransientFrame(gClient->GetRoot(), main, 60, 40)
{
    SetCleanup(kDeepCleanup);

    Connect("CloseWindow()", "DescDlg", this, "CloseWindow()");

    BuildDisplayArea();
    BuildButtonBox();

    SetWindowName("Desc base Data");
    SetIconName("Desc base Data");

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
void DescDlg::BuildButtonBox()
{
    TGButton *tb;

    // Create a frame to hold the buttons.
    TGCompositeFrame *ButtonFrame = new
        TGCompositeFrame(this, 600, 20, kHorizontalFrame);

    TGLayoutHints* fL2 = new
        TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5);

    tb = new TGTextButton( ButtonFrame, "  &Done  ");
    tb->Connect("Clicked()", "DescDlg", this, "Done()");
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
void DescDlg::BuildDisplayArea()
{
    const Int_t  Width  = 100;
    DSA602* ptr         = DSA602::GetThis();
    StatusAndEvent* pSE = ptr->pStatusAndEvent();
    unsigned char n     = pSE->GetNModule();
    Module*      pModule;
    TGLabel*     label;
    Int_t        i, j;
    Char_t       tmp[8];

    TGGroupFrame *gf = new TGGroupFrame( this, "Trace Description", 
                                         kHorizontalFrame);
    // Rows, Columns, Interval between frames, hints
    gf->SetLayoutManager(new TGMatrixLayout( gf, 6, 2, 10, 2));

    // 1
    label = new TGLabel(gf, new TGHotString("Function:"));
    gf->AddFrame(label);
    fFunction[0] = new TGComboBox(gf);
    i=0;
    while (Input::FString[i] != NULL)
    {
	fFunction[0]->AddEntry(Input::FString[i], i);
	i++;
    }
    fFunction[0]->Select(0,kFALSE);
    fFunction[0]->Resize( Width, 20);
    gf->AddFrame(fFunction[0]);

    // 2 is ON or VS?
    label = new TGLabel(gf, new TGHotString("Type:"));
    gf->AddFrame(label);
    fTime = new TGCheckButton(gf, "Time");
    fTime->Resize( Width, 20);
    gf->AddFrame(fTime);
  
    // 3
    label = new TGLabel(gf, new TGHotString("Module:"));
    gf->AddFrame(label);
    fInput[0] = new TGComboBox(gf);
    j = 0;
    for (i=0;i<n;i++)
    {
	memset(tmp, 0, sizeof(tmp));
	// loop over available modules. 
	pModule = pSE->GetModule(i);
	switch(pModule->Type())
	{
	case Module::kTYPE_NONE:
	    break;
	case Module::kTYPE_11A32:
	    sprintf(tmp, "%c1", SlotChar(pModule->Slot()));
	    fInput[0]->AddEntry( tmp,j); j++;
	    sprintf(tmp, "%c2", SlotChar(pModule->Slot()));
	    fInput[0]->AddEntry( tmp,j); j++;
	    break;
	case Module::kTYPE_11A33:
	    sprintf(tmp, "%c1", SlotChar(pModule->Slot()));
	    fInput[0]->AddEntry( tmp,j); j++;
	    break;
	}
    }
    fInput[0]->Resize( Width, 20);
    gf->AddFrame(fInput[0]);

    // 4
    label = new TGLabel(gf, new TGHotString("Window:"));
    gf->AddFrame(label);
    i = 0;
    fWindow = new TGComboBox(gf);
    while (Description::WString[i] != NULL)
    {
	fWindow->AddEntry(Description::WString[i], i);
	i++;
    }
    fWindow->Resize( Width, 20);
    gf->AddFrame(fWindow);

    // 5
    label = new TGLabel(gf, new TGHotString("Function:"));
    gf->AddFrame(label);
    fFunction[1] = new TGComboBox(gf);
    i=0;

    while (Input::FString[i] != NULL)
    {
	fFunction[1]->AddEntry(Input::FString[i], i);
	i++;
    }
    fFunction[1]->Select(0,kFALSE);
    fFunction[1]->Resize( Width, 20);
    gf->AddFrame(fFunction[1]);

    // 6
    label = new TGLabel(gf, new TGHotString("Module:"));
    gf->AddFrame(label);
    fInput[1] = new TGComboBox(gf);
    j = 0;
    for (i=0;i<n;i++)
    {
	memset(tmp, 0, sizeof(tmp));
	// loop over available modules. 
	pModule = pSE->GetModule(i);
	switch((UInt_t)pModule->Type())
	{
	case Module::kTYPE_NONE:
	    break;
	case Module::kTYPE_11A32:
	    sprintf(tmp, "%c1", SlotChar(pModule->Slot()));
	    fInput[1]->AddEntry( tmp,j); j++;
	    sprintf(tmp, "%c2", SlotChar(pModule->Slot()));
	    fInput[1]->AddEntry( tmp,j); j++;
	    break;
	case Module::kTYPE_11A33:
	    sprintf(tmp, "%c1", SlotChar(pModule->Slot()));
	    fInput[1]->AddEntry( tmp,j); j++;
	    break;
	}
    }
    fInput[1]->Resize( Width, 20);
    gf->AddFrame(fInput[1]);

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
void DescDlg::CloseWindow()
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
void DescDlg::Done()
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
void DescDlg::DoClose()
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
void DescDlg::Update(void)
{
}
