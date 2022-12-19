/**
 ******************************************************************
 *
 * Module Name : FFTDlg.hh
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
#ifndef __FFTDLG_hh_
#define __FFTDLG_hh_
#    include <TGTextEntry.h>
#    include <TGButton.h>
#    include <RQ_OBJECT.h>

class TGCheckButton;
class TGComboBox;

class FFTDlg : public TGTransientFrame
{
    ClassDef(FFTDlg, 0);
public:
    /// Constructor
    FFTDlg (const TGWindow *parent);

    /// Close the window
    void   CloseWindow(void);
    /// User pressed the OK button, end the dialog
    void   Done(void);
    /// Close the window
    void   DoClose(void);
    void   Update(void);
    // SLOTS
    void   SetAVG(void);
    void   SetDCSUP(void);
    void   SetPHAse(void);
    void   SetFILTer(void);
    void   SetFORMat(int);
    void   SetWINDow(int);

private:

    /// Build the Ok and Cancel Buttons
    void BuildButtonBox();
    /// 
    void BuildDisplayArea();
    void FillDisplayArea();
    void UpdateLength(int);
    void UpdateXIncr(void);

    TGCheckButton* fAVG;
    TGCheckButton* fDCSUP;
    TGCheckButton* fPHAse;
    TGCheckButton* fFILTer;
    TGComboBox*    fFORMat;
    TGComboBox*    fWINDow;
};

#endif
