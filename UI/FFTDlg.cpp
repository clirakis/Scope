/**
 ******************************************************************
 *
 * Module Name : FFTDlg.cpp
 *
 * Author/Date : C. B. Lirakis/23-Dec-14
 *
 * Description : A dialog displaying FFT parameters
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
#include "DSA602.hh"
#include "FFTDlg.hh"
#include "DSAFFT.hh"

/**
 ******************************************************************
 *
 * Function Name : FFTDlg  Constructor
 *
 * Description : 
 *
 * Inputs : main - pointer to parent window
 *
 * Returns : Constructed GUI dialog
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
FFTDlg::FFTDlg(const TGWindow *main)
    : TGTransientFrame(gClient->GetRoot(), main, 60, 40)
{
    SetCleanup(kDeepCleanup);

    Connect("CloseWindow()", "FFTDlg", this, "CloseWindow()");

    BuildDisplayArea();
    BuildButtonBox();

    SetWindowName("FFT Setup");
    SetIconName("FFT Setup");

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
 * Unit Tested on: 24-Dec-14
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void FFTDlg::BuildButtonBox()
{
    TGButton *tb;

    // Create a frame to hold the buttons.
    TGCompositeFrame *ButtonFrame = new
        TGCompositeFrame(this, 600, 20, kHorizontalFrame);

    TGLayoutHints* fL2 = new
        TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5);

    tb = new TGTextButton( ButtonFrame, "  &Done  ");
    tb->Connect("Clicked()", "FFTDlg", this, "Done()");
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
 * Unit Tested on: 24-Dec-14
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void FFTDlg::BuildDisplayArea()
{
    TGLabel      *label;
    TGGroupFrame *gf = new TGGroupFrame( this, "FFT setup", 
                                         kHorizontalFrame);
    // Rows, Columns, Interval between frames, hints
    gf->SetLayoutManager(new TGMatrixLayout( gf, 6, 2, 10, 2));

    // 1
    label = new TGLabel(gf, new TGHotString("AVG:"));
    gf->AddFrame(label);
    fAVG = new TGCheckButton( gf, "AVG");
    //fAVG->Resize( 60, 20);
    gf->AddFrame(fAVG);
    fAVG->Connect("Clicked()", "FFTDlg", this, "SetAVG()");

    // 2
    label = new TGLabel(gf, new TGHotString("DC Sup:"));
    gf->AddFrame(label);
    fDCSUP =  new TGCheckButton( gf, "DC Sup");
    gf->AddFrame(fDCSUP);
    fDCSUP->Connect("Clicked()", "FFTDlg", this, "SetDCSUP()");

    // 3
    label = new TGLabel(gf, new TGHotString("Unwrap"));
    gf->AddFrame(label);
    fPHAse =  new TGCheckButton( gf, "Phase");
    gf->AddFrame(fPHAse);
    fPHAse->Connect("Clicked()", "FFTDlg", this, "SetPHAse()");

    // 4 
    label = new TGLabel(gf, new TGHotString("Filter:"));
    gf->AddFrame(label);
    fFILTer =  new TGCheckButton( gf, "Filter");
    gf->AddFrame(fFILTer);
    fFILTer->Connect("Clicked()", "FFTDlg", this, "SetFILTer()");

    // 5
    label = new TGLabel(gf, new TGHotString("Format:"));
    gf->AddFrame(label);
    fFORMat = new TGComboBox( gf);
    fFORMat->AddEntry("dBm",      kDBM);
    fFORMat->AddEntry("dB Fund",  kDBFUND);
    fFORMat->AddEntry("dBV Peak", kDBVPEAK);
    fFORMat->AddEntry("dBV RMS",  kDBVRMS);
    fFORMat->AddEntry("V Peak",   kVPEAK);
    fFORMat->AddEntry("V RMS",    kVRMS);
    fFORMat->Resize(100,20);
    fFORMat->Connect("Selected(int)", "FFTDlg", this, "SetFORMat(int)");
    gf->AddFrame(fFORMat);


    // 6
    label = new TGLabel(gf, new TGHotString("Window:"));
    gf->AddFrame(label);
    fWINDow = new TGComboBox( gf);
    fWINDow->AddEntry("Blackman",        kBLACKMAN);
    fWINDow->AddEntry("Blackman-Harris", kBLHARRIS);
    fWINDow->AddEntry("Hamming",         kHAMMING);
    fWINDow->AddEntry("Hanning",         kHANNING);
    fWINDow->AddEntry("Rectangular",     kRECTANGULAR);
    fWINDow->AddEntry("Triangular",      kTRIANGULAR);
    fWINDow->Resize(100,20);
    fWINDow->Connect("Selected(int)", "FFTDlg", this, "SetWINDow(int)");
    gf->AddFrame(fWINDow);
    AddFrame(gf, new TGLayoutHints(kLHintsLeft, 2, 2, 2, 2));
    Resize();
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
void FFTDlg::CloseWindow()
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
 * Unit Tested on: 24-Dec-14
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void FFTDlg::Done()
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
 * Returns : None    else

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
void FFTDlg::DoClose()
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
 * Unit Tested on: 24-Dec-14
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void FFTDlg::Update(void)
{
#if 0   // FIXME
    DSA602* h = DSA602::GetThis();

    if (h->AVG())
    {
	fAVG->SetState(kButtonDown, kFALSE);
    }
    else
    {
	fAVG->SetState(kButtonUp, kFALSE);
    }

    if (h->DCSUP())
    {
	fDCSUP->SetState(kButtonDown, kFALSE);
    }
    else
    {
	fDCSUP->SetState(kButtonUp, kFALSE);
    }

    if (h->PHAse())
    {
	fPHAse->SetState(kButtonDown, kFALSE);
    }
    else
    {
	fPHAse->SetState(kButtonUp, kFALSE);
    }

    if (h->FILTer())
    {
	fFILTer->SetState(kButtonDown, kFALSE);
    }
    else
    {
	fFILTer->SetState(kButtonUp, kFALSE);
    }

    fFORMat->Select((Int_t)h->FORMat(),kFALSE);
    fWINDow->Select((Int_t)h->WINDow(),kFALSE);

    h->Update();
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
 * Unit Tested on: 24-Dec-14
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void FFTDlg::SetFORMat(int index)
{
#if 0
    DSAFFTGPIB* h = (DSAFFTGPIB *) fGPIB;
    h->SetFormat((FFTFORMAT)index);
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
 * Unit Tested on: 24-Dec-14
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void FFTDlg::SetWINDow(int index)
{
#if 0
    DSAFFTGPIB* h = (DSAFFTGPIB *) fGPIB;
    h->SetWindow((FFT_WINDOW)index);
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
 * Unit Tested on: 24-Dec-14
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void FFTDlg::SetAVG(void)
{
#if 0
    DSAFFTGPIB* h = (DSAFFTGPIB *) fGPIB;
    h->SendCommand(DSAFFT::CAVG, (bool) fAVG->GetState());
#endif
}
void FFTDlg::SetDCSUP(void)
{
#if 0
    DSAFFTGPIB* h = (DSAFFTGPIB *) fGPIB;
    h->SendCommand(DSAFFT::CDCSUP, (bool) fDCSUP->GetState());
#endif
}
void FFTDlg::SetPHAse(void)
{
#if 0
    DSAFFTGPIB* h = (DSAFFTGPIB *) fGPIB;
    h->SetPhase((bool) fPHAse->GetState());
#endif
}
void FFTDlg::SetFILTer(void)
{
#if 0
    DSAFFTGPIB* h = (DSAFFTGPIB *) fGPIB;
    h->SetFilter((bool) fFILTer->GetState());
#endif
}
