/**
 ******************************************************************
 *
 * Module Name : DescDlg.hh
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
#ifndef __DESCDLG_hh_
#define __DESCDLG_hh_
#  include <RQ_OBJECT.h>
#  include <TGFrame.h>
class TGLabel;
class TGComboBox;
class TGCheckBox;

class DescDlg : public TGTransientFrame
{
    ClassDef(DescDlg, 0);
public:
    /// Constructor
    DescDlg (const TGWindow *parent, void *p);

    /// Close the window
    void   CloseWindow();
    /// User pressed the OK button, end the dialog
    void   Done();
    /// Close the window
    void   DoClose();
    void   Update();

private:
    /// Build the Ok and Cancel Buttons
    void BuildButtonBox();
    /// 
    void BuildDisplayArea();
    void FillDisplayArea();

    void*          pPtrDefTrace;
    TGComboBox*    fFunction[2];
    TGComboBox*    fInput[2];
    TGComboBox*    fWindow;
    TGLabel*       fDescription; // The current definition string. 
    TGCheckButton* fTime;
};

#endif
