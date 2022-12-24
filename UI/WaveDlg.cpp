/**
 ******************************************************************
 *
 * Module Name : WaveDlg.cpp
 *
 * Author/Date : C. B. Lirakis/07-Feb-14
 *
 * Description : A dialog displaying waveform parameters
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 *    24-Dec-22 CBL  Operational. But only for waveform 1. 
 *
 * Classification : Unclassified
 *
 * References :
 * DSA602 Programmers Reference manual, page: 191
 * Measurement.hh
 *
 *******************************************************************
 */
#include <iostream>
using namespace std;

/// Root Includes
#include <TROOT.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TVirtualX.h>

/// Local Includes.
#include "debug.h"
#include "WaveDlg.hh"
#include "DSA602.hh"

/**
 ******************************************************************
 *
 * Function Name : WaveDlg  Constructor
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
WaveDlg::WaveDlg(const TGWindow *main)
    : TGTransientFrame(gClient->GetRoot(), main, 60, 80)
{
    SetCleanup(kDeepCleanup);

    Connect("CloseWindow()", "WaveDlg", this, "CloseWindow()");

    BuildDisplayArea();
    BuildButtonBox();
    Update();

    SetWindowName("Waveform Data");
    SetIconName("WaveForm Data");

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
    Resize();
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
void WaveDlg::BuildButtonBox()
{
    TGButton *tb;

    // Create a frame to hold the buttons.
    TGCompositeFrame *ButtonFrame = new
        TGCompositeFrame(this, 600, 20, kHorizontalFrame|kSunkenFrame);

    TGLayoutHints* fL2 = new
        TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5);

    TGLayoutHints *lh = new TGLayoutHints(
	kLHintsLeft|kLHintsTop|kLHintsExpandX, 2, 2, 2, 2);

    tb = new TGTextButton( ButtonFrame, "  &OK  ");
    tb->Connect("Clicked()", "WaveDlg", this, "DoOK()");
    ButtonFrame->AddFrame(tb, fL2);

    tb = new TGTextButton( ButtonFrame, "  &Cancel  ");
    tb->Connect("Clicked()", "WaveDlg", this, "DoCancel()");
    ButtonFrame->AddFrame(tb, fL2);

    ButtonFrame->Resize();
    AddFrame(ButtonFrame, lh);
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
void WaveDlg::BuildDisplayArea()
{
    TGLabel      *label;
    //Int_t        row, col;
    TGGroupFrame *gf = new TGGroupFrame( this, "Waveform data", 
                                         kHorizontalFrame);
    TGLayoutHints *lh = new TGLayoutHints(
	kLHintsLeft|kLHintsTop|kLHintsExpandX|kLHintsExpandY, 2, 2, 2, 2);
    //row = 0;
    //col = 0;
    // Rows, Columns, Interval between frames, hints
    gf->SetLayoutManager(new TGMatrixLayout( gf, 10, 2, 10, 2));

    // 1
    label = new TGLabel(gf, new TGHotString("Waveform:"));
    gf->AddFrame(label);
    fWaveform = new TGLabel( gf, TGHotString("1"));
    gf->AddFrame(fWaveform);

    // 2
    label = new TGLabel(gf, new TGHotString("Rise time:"));
    gf->AddFrame(label);
    fRisetime= new TGLabel( gf, TGHotString("0.000000"));
    gf->AddFrame(fRisetime);

    // 3
    label = new TGLabel(gf, new TGHotString("Fall time:"));
    gf->AddFrame(label);
    fFalltime = new TGLabel( gf, TGHotString("0.000000"));
    gf->AddFrame(fFalltime);

    // 4
    label = new TGLabel(gf, new TGHotString("Frequency:"));
    gf->AddFrame(label);
    fFrequency = new TGLabel( gf, TGHotString("         0Hz"));
    gf->AddFrame(fFrequency);
#if 0
    //
    label = new TGLabel(gf, new TGHotString(":"));
    gf->AddFrame(label);
    f= new TGLabel( gf, TGHotString(""));
    gf->AddFrame();
#endif
    // 5
    label = new TGLabel(gf, new TGHotString("RMS:"));
    gf->AddFrame(label);
    fRMS = new TGLabel( gf, TGHotString("0.000000"));
    gf->AddFrame(fRMS);

    // 6
    label = new TGLabel(gf, new TGHotString("Gain:"));
    gf->AddFrame(label);
    fGain = new TGLabel( gf, TGHotString("0.000000"));
    gf->AddFrame(fGain);

    // 7
    label = new TGLabel(gf, new TGHotString("Max:"));
    gf->AddFrame(label);
    fMax = new TGLabel( gf, TGHotString("0.000000"));
    gf->AddFrame(fMax);

    // 8
    label = new TGLabel(gf, new TGHotString("Min:"));
    gf->AddFrame(label);
    fMin = new TGLabel( gf, TGHotString("0.000000"));
    gf->AddFrame(fMin);

    // 9
    label = new TGLabel(gf, new TGHotString("Mean:"));
    gf->AddFrame(label);
    fMean = new TGLabel( gf, TGHotString("0.000000"));
    gf->AddFrame(fMean);

    // 10
    label = new TGLabel(gf, new TGHotString("Mid:"));
    gf->AddFrame(label);
    fMid = new TGLabel( gf, TGHotString("0.000000"));
    gf->AddFrame(fMid, lh);
    gf->Resize();

    AddFrame(gf, lh);
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
void WaveDlg::CloseWindow()
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
void WaveDlg::DoOK()
{
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
void WaveDlg::DoCancel()
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
void WaveDlg::DoClose()
{
   // Handle close button.
    SendCloseMessage();
}
/**
 ******************************************************************
 *
 * Function Name : Update
 *
 * Description : Fill the widgets with data. 
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
void WaveDlg::Update(void)
{
    DSA602*      scope = DSA602::GetThis();
    Measurement *pmeas  = scope->pMeasurement();
    char s[32];

    // fWaveform  FIXME
    pmeas->Update();

    sprintf(s, "%g", pmeas->Risetime().Value());
    fRisetime->SetText(s);

    sprintf(s, "%g", pmeas->Falltime().Value());    
    fFalltime->SetText(s);

    sprintf(s, "%g", pmeas->Frequency().Value());    
    fFrequency->SetText(s);

    sprintf(s, "%g", pmeas->RMS().Value());    
    fRMS->SetText(s);

    sprintf(s, "%g", pmeas->Gain().Value());    
    fGain->SetText(s);

    sprintf(s, "%g", pmeas->Max().Value());    
    fMax->SetText(s);

    sprintf(s, "%g", pmeas->Min().Value());    
    fMin->SetText(s);

    sprintf(s, "%g", pmeas->Mean().Value());    
    fMean->SetText(s);

    sprintf(s, "%g", pmeas->Midpoint().Value());    
    fMid->SetText(s);

}
