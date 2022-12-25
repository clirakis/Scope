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
    static const UInt_t   kMaxReadout = 6;
    
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
    void   ButtonChecked();

private:
    /// Build the Ok and Cancel Buttons
    void BuildButtonBox();
    /// 
    void BuildUserArea();

    void ReadState();
    void Clear();
    void Update();

    // private variables
    MSLIST*        fMeas;
    TGLabel*       fTrace;      // Label of trace on display.
 
    // Only 6 measurements are available. 
    TGLabel*       fLabel[kMaxReadout];     // Labels and Data can be NULL. 
    TGLabel*       fData[kMaxReadout];
    TGCheckButton* fCB[26];
};

#endif
