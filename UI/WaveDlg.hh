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
#    include <RQ_OBJECT.h>

class TGLabel;

class WaveDlg : public TGTransientFrame
{
    ClassDef(WaveDlg, 0);
public:
    /// Constructor
    WaveDlg (const TGWindow *parent);

    /// Close the window
    void   CloseWindow(void);
    /// User pressed the OK button, end the dialog
    void   DoOK();
    /// User pressed the Cancel button, end the dialog
    void   DoCancel(void);
    /// Close the window
    void   DoClose(void);

private:
    /// Build the Ok and Cancel Buttons
    void BuildButtonBox(void);
    /// 
    void BuildDisplayArea(void);

    void Update(void);

    TGLabel *fWaveform, *fRisetime;
    TGLabel *fFalltime, *fFrequency, *fRMS, *fGain, *fMax, *fMin, *fMean;
    TGLabel *fMid;
};

#endif
