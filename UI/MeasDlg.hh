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
 * 27-Dec-22 CBL Removed MSLIST, it is now a part of MeasurementA. 
 *
 * Classification : Unclassified
 *
 * References :
 *
 *******************************************************************
 */
#ifndef __MEASDLG_hh_
#define __MEASDLG_hh_
#  include <RQ_OBJECT.h>

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
    /// User pressed the Read button, Read the data again. 
    void   DoRead();
    /// User pressed the Done button, end the dialog
    void   DoDone();
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
    TGLabel*       fTrace;      // Label of trace on display.
 
    // Only 6 measurements are available. 
    TGLabel*       fLabel[6];     // Labels and Data can be NULL. 
    TGLabel*       fData[6];
    TGCheckButton* fCB[32];
};

#endif
