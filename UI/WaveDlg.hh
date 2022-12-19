/**
 ******************************************************************
 *
 * Module Name : WaveDlg.hh
 *
 * Author/Date : C.B. Lirakis/7-Feb-14
 *
 * Description : A dialog displaying the waveform parameters.
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
#ifndef __WAVEDLG_hh_
#define __WAVEDLG_hh_
#    include <TGTextEntry.h>
#    include <TGButton.h>
#    include <RQ_OBJECT.h>

class TGLabel;

class WaveDlg : public TGTransientFrame
{
    ClassDef(WaveDlg, 0);
public:
    /// Constructor
    WaveDlg (const TGWindow *parent, void *p);

    /// Close the window
    void   CloseWindow();
    /// User pressed the OK button, end the dialog
    void   DoOK();
    /// User pressed the Cancel button, end the dialog
    void   DoCancel();
    /// Close the window
    void   DoClose();

private:
    /// Build the Ok and Cancel Buttons
    void BuildButtonBox();
    /// 
    void BuildDisplayArea();
    void FillDisplayArea();
    void    *fScope;
    TGLabel *fWaveform, *fRisetime;
    TGLabel *fFalltime, *fFrequency, *fRMS, *fGain, *fMax, *fMin, *fMean;
    TGLabel *fMid;
};

#endif
