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
#    include <TGFrame.h>
#    include <RQ_OBJECT.h>

class TGLabel;
class TGComboBox;
class TGRadioButton;

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
    void   DoClose(void);
    void   Update(void);
    void   SetTime(int);
    void   SetLength(void);

private:

    /// Build the Ok and Cancel Buttons
    void BuildButtonBox();
    /// 
    void BuildDisplayArea();
    void FillDisplayArea();
    /*!
     * Set the length radio box to 
     * a) which lengths are valid to select given the input time index. 
     * b) set the approprate button. 
     *
     * This assumes that an timebase Update has been issued. 
     *
     * inputs:
     *   index - time index selected
     */
    void UpdateLength(int index);
    void UpdateXIncr(void);

    TGComboBox* fTime;
    TGRadioButton *fLength[9];
    //TGComboBox* fLength;
    TGLabel*    fXIncr;
};

#endif
