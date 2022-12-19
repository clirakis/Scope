/**
 ******************************************************************
 *
 * Module Name : TimeDlg.hh
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
#ifndef __TIMEDLG_hh_
#define __TIMEDLG_hh_
#    include <TGTextEntry.h>
#    include <TGButton.h>
#    include <RQ_OBJECT.h>

class TGLabel;
class TGComboBox;

class TimeDlg : public TGTransientFrame
{
    ClassDef(TimeDlg, 0);
public:
    /// Constructor
    TimeDlg (const TGWindow *parent);

    /// Close the window
    void   CloseWindow();
    /// User pressed the OK button, end the dialog
    void   Done();
    /// Close the window
    void   DoClose();
    void   Update();
    void   SetTime(int);
    void   SetLength(int);

private:

    /// Build the Ok and Cancel Buttons
    void BuildButtonBox();
    /// 
    void BuildDisplayArea();
    void FillDisplayArea();
    void UpdateLength(int);
    void UpdateXIncr(void);

    TGComboBox* fTime;
    TGComboBox* fLength;
    TGLabel*    fXIncr;
};

#endif
