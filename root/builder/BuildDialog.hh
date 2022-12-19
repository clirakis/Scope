/**
 ******************************************************************
 *
 * Module Name : BuildDialog.hh
 *
 * Author/Date : C.B. Lirakis / 10-Feb-06
 *
 * Description : SVP display class
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References :
 *
 *
 * RCS header info.
 * ----------------
 * $RCSfile: SVPDialog.hh,v $
 * $Author:   clirakis  $
 * $Date:   15 Jul 2005 09:34:14  $
 * $Locker:  $
 * $Name:  $
 * $Revision:   1.13  $
 *
 * $Log:   /usr/local/iss60/library/iss60/RTDLib/SVPDialog.hhv  $
 *
 *
 *
 *******************************************************************
 */
#ifndef __BUILDDIALOG_h_
#define __BUILDDIALOG_h_

//   ROOT includes.
#  include <TROOT.h>
#  include <TGFrame.h>
#  include <TList.h>
#  include <TGraph.h>
#  include <TMultiGraph.h>
#  include <TLegend.h>
#  include <TRootEmbeddedCanvas.h>
#  include <TGMenu.h>
#  include <TGButton.h>
#  include <TGStatusBar.h>
#  include <TEnv.h>
#  include <TStyle.h>
#  include <TPave.h>
#  include <TPaveLabel.h>
#  include <TGToolBar.h>
#  include <TGShutter.h>

/**
 * Class to handle a Sound Velocity Profile dialog.
 * Multiples here!
 */
class BuildDialog : public TGMainFrame {

private:
    TGMenuBar           *fMenuBar;
    TGPopupMenu         *fMenuFile, *fMenuHelp;

    TGCompositeFrame    *fFrame1;
    TGHorizontalFrame   *fGraphicsFrame;
    TGButton            *fOkButton, *fEditButton;

    TGLayoutHints       *fL1, *fL2, *fL3, *fL4, *fMenuBarLayout,
            *fMenuBarItemLayout, *fMenuBarHelpLayout;
    TRootEmbeddedCanvas *fEmbeddedCanvas;
    TGShutter        *fShutter;

    // Private variables for rubber band zoom.
    // Statics for rubber band zoom. 
    Double_t x0, y0, x1, y1;
    Int_t pxold, pyold;
    Int_t px0, py0;
    Int_t linedrawn;

    // How will we handle various events?? What mode are we in?
    enum CanvasMode { DISPLAY, ZOOM, DRAW_LINE};
    CanvasMode Mode;

    // Use the status bar to show the name of the selected SVP.
    TGStatusBar *fStatusBar;

    // Make a toolbar for editing data
    TGToolBar *fToolBar;

    void CreateMenuBar();
    void CreateToolBar();
    void CreateStatusBar();
    void AddEmbeddedCanvas();
    void AddShutter();
    void Zoom(Int_t event, Int_t px, Int_t py);
    void DrawLine(Int_t event, Int_t px, Int_t py);

public:
    /// Main constructor with default values.
    BuildDialog(const TGWindow *p, UInt_t w = 1200, UInt_t h = 400);

    /// Destructor, clean up after ourselves.
    virtual ~BuildDialog();

    // slots - Methods by which we handle callbacks.
    void CloseWindow();
    void HandleMenu(Int_t id);
    /*
     * Select an item.
     */
    void HandleToolBar(Int_t id);
    /* 
     * Unselect that item
     */
    void HandleToolBarU(Int_t id);

    void CanvasEvent(Int_t event, Int_t x, Int_t y, TObject *selected);



ClassDef(BuildDialog, 0); // Define the class for root.
};

#endif
