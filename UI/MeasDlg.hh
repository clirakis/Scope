/**
 ******************************************************************
 *
 * Module Name : MeasDlg.hh
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
#ifndef __MEASDLG_hh_
#define __MEASDLG_hh_
#    include <TGTextEntry.h>
#    include <TGButton.h>
#    include <RQ_OBJECT.h>

class MSLIST;
class TGLabel;
class TGCheckButton;

class MeasDlg : public TGTransientFrame
{
    ClassDef(MeasDlg, 0);
public:
    /// Constructor
    MeasDlg (const TGWindow *parent);
    ~MeasDlg ();
    
    // SLOTS
    /// Close the window
    void   CloseWindow();
    /// User pressed the OK button, end the dialog
    void   DoOK();
    /// User pressed the Cancel button, end the dialog
    void   DoCancel();
    /// Close the window
    void   DoClose();
    void   DoApply();

private:
    /// Build the Ok and Cancel Buttons
    void BuildButtonBox();
    /// 
    void BuildDisplayArea();
    void ReadState();
    void ReadValue();
    void CreateLabels();
    void Clear();

    // private variables
    void*          fScope;
    MSLIST*        fMeas;
    TGLabel*       fLabel[6];   // Only 6 measurements are available. 
    TGLabel*       fData[6];
    TGCheckButton* fCB[26];
};

#endif
