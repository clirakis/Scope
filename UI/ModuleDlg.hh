/**
 ******************************************************************
 *
 * Module Name : ModuleDlg.hh
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
#ifndef __MODULEDLG_hh_
#define __MODULEDLG_hh_
#  include <TGFrame.h>
#  include <RQ_OBJECT.h>

class TGLabel;

class ModuleDlg : public TGTransientFrame
{
    ClassDef(ModuleDlg, 0);
public:
    /// Constructor
    ModuleDlg (const TGWindow *parent);

    /// Close the window
    void   CloseWindow();
    /// User pressed the Done button, end the dialog
    void   Done();
    /// Close the window
    void   DoClose();

private:
    /// Build the Ok and Cancel Buttons
    void BuildButtonBox();
    /// 
    void BuildDisplayArea();
    void FillDisplayArea();
    void AddMultipleChannels(TGCompositeFrame*, void *);
};

#endif
