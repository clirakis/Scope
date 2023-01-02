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
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGComboBox.h>
#include <TGNumberEntry.h>
#include <TVirtualX.h>


/// Local Includes.
#include "debug.h"
#include "CLogger.hh"
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
    SET_DEBUG_STACK;
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
    gf->SetLayoutManager(new TGMatrixLayout( gf, 7, 2, 10, 2));

    // 1
    label = new TGLabel(gf, new TGHotString("AVG:"));
    gf->AddFrame(label);
    fAVG = new TGCheckButton( gf, "AVG");
    //fAVG->Resize( 60, 20);
    gf->AddFrame(fAVG);
    fAVG->Connect("Clicked()", "FFTDlg", this, "SetAVG()");

    // 2
    label = new TGLabel(gf, new TGHotString("NAVG:"));
    gf->AddFrame(label);
    fNAvg = new TGNumberEntry( gf, 2.0, 5, -1,
			       TGNumberFormat::kNESInteger, 
			       TGNumberFormat::kNEAPositive,
			       TGNumberFormat::kNELLimitMinMax,
			       2.0, 65534.0);
    fNAvg->Connect("ValueSet(Long_t)", "FFTDlg", this, "SetNAvg(long)");
    gf->AddFrame(fNAvg);

    // 3
    label = new TGLabel(gf, new TGHotString("DC Sup:"));
    gf->AddFrame(label);
    fDCSUP =  new TGCheckButton( gf, "DC Sup");
    gf->AddFrame(fDCSUP);
    fDCSUP->Connect("Clicked()", "FFTDlg", this, "SetDCSUP()");

    // 4
    label = new TGLabel(gf, new TGHotString("Unwrap"));
    gf->AddFrame(label);
    fPHAse =  new TGCheckButton( gf, "Phase");
    gf->AddFrame(fPHAse);
    fPHAse->Connect("Clicked()", "FFTDlg", this, "SetPHAse()");

    // 5
    label = new TGLabel(gf, new TGHotString("Filter:"));
    gf->AddFrame(label);
    fFILTer =  new TGCheckButton( gf, "Filter");
    gf->AddFrame(fFILTer);
    fFILTer->Connect("Clicked()", "FFTDlg", this, "SetFILTer()");

    // 6
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


    // 7
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
 * Unit Tested on: 24-Dec-14
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void FFTDlg::Done()
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
    SET_DEBUG_STACK;
   // Handle close button.
    SendCloseMessage();
}
/**
 ******************************************************************
 *
 * Function Name : Update
 *
 * Description :  Update all the fields with the FFT data.
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
void FFTDlg::Update(void)
{
    SET_DEBUG_STACK;
    DSA602* scope = DSA602::GetThis();
    DSAFFT* pFFT  = scope->GetFFT();

    if(pFFT->Update())
    {
	if (pFFT->AVG())
	{
	    fAVG->SetState(kButtonDown, kFALSE);
	}
	else
	{
	    fAVG->SetState(kButtonUp, kFALSE);
	}

	if (pFFT->DCSUP())
	{
	    fDCSUP->SetState(kButtonDown, kFALSE);
	}
	else
	{
	    fDCSUP->SetState(kButtonUp, kFALSE);
	}

	if (pFFT->PHAse())
	{
	    fPHAse->SetState(kButtonDown, kFALSE);
	}
	else
	{
	    fPHAse->SetState(kButtonUp, kFALSE);
	}

	// Not quite sure what this is. 
	if (pFFT->FILTer())
	{
	    fFILTer->SetState(kButtonDown, kFALSE);
	}
	else
	{
	    fFILTer->SetState(kButtonUp, kFALSE);
	}

	fFORMat->Select((Int_t)pFFT->FORMat(),kFALSE);
	fWINDow->Select((Int_t)pFFT->WINDow(),kFALSE);

	fNAvg->SetNumber((Double_t) pFFT->NAvg(), kFALSE);
    }
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : SetFORmat
 *
 * Description : Set the format of the output. Can be one of the following. 
 * kDBM, kDBFUND, kDBVPEAK, kDBVRMS, kVPEAK, kVRMS
 *
 * Inputs : index from combo box tied to the above
 *
 * Returns : None
 *
 * Error Conditions : GPIB write error
 *
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void FFTDlg::SetFORMat(int index)
{
    SET_DEBUG_STACK;
    DSA602* scope = DSA602::GetThis();
    DSAFFT* pFFT  = scope->GetFFT();
    pFFT->SetFormat((FFTFORMAT)index);
    SET_DEBUG_STACK;
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
    SET_DEBUG_STACK;
    DSA602* scope = DSA602::GetThis();
    DSAFFT* pFFT  = scope->GetFFT();
    pFFT->SetWindow((FFT_WINDOW)index);
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : SetAVG
 *
 * Description : if the TGCheckbutton is true, set the AVG on
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
void FFTDlg::SetAVG(void)
{
    SET_DEBUG_STACK;
    DSA602* scope = DSA602::GetThis();
    DSAFFT* pFFT  = scope->GetFFT();
    pFFT->SendCommand(DSAFFT::kCAVG, (bool) fAVG->GetState());
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : SetDCSUP
 *
 * Description : if the TGCheckbutton is true, set the DC Supresson on
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
void FFTDlg::SetDCSUP(void)
{
    SET_DEBUG_STACK;
    DSA602* scope = DSA602::GetThis();
    DSAFFT* pFFT  = scope->GetFFT();
    pFFT->SendCommand(DSAFFT::kCDCSUP, (bool) fDCSUP->GetState());
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : SetPHAse
 *
 * Description : if the TGCheckbutton is true, set the Phase wrap on
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
void FFTDlg::SetPHAse(void)
{
    SET_DEBUG_STACK;
    DSA602* scope = DSA602::GetThis();
    DSAFFT* pFFT  = scope->GetFFT();
    pFFT->SendCommand(DSAFFT::kCPHASE, (bool) fPHAse->GetState());
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : SetFILTer
 *
 * Description : if the TGCheckbutton is true, set the Filter on, 
 *               I'm not sure this is a real command. 
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
void FFTDlg::SetFILTer(void)
{
    SET_DEBUG_STACK;
    DSA602* scope = DSA602::GetThis();
    DSAFFT* pFFT  = scope->GetFFT();
    pFFT->SendCommand(DSAFFT::kCFILTER, fFILTer->GetState());
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : SetNAvg
 *
 * Description : 
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
void FFTDlg::SetNAvg(long val)
{
    SET_DEBUG_STACK;
    DSA602* scope = DSA602::GetThis();
    DSAFFT* pFFT  = scope->GetFFT();
    pFFT->SetNAvg((uint16_t) val);
    SET_DEBUG_STACK;
}
