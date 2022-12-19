/**
 ******************************************************************
 *
 * Module Name : TraceDlg.hh
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
#ifndef __TRACEDLG_hh_
#define __TRACEDLG_hh_
#    include <TGTextEntry.h>
#    include <TGButton.h>
#    include <RQ_OBJECT.h>

class TraceDlg : public TGTransientFrame
{
    ClassDef(TraceDlg, 0);
public:
    /// Constructor
    TraceDlg (const TGWindow *parent);

    /// Close the window
    void   CloseWindow();
    /// User pressed the Done button, end the dialog
    void   Done();
    /// Close the window
    void   DoClose();

private:

    /// Build the Ok and Cancel Buttons
    void BuildButtonBox();
    void BuildDisplayArea();
    void FillDisplayArea();
};

#endif
