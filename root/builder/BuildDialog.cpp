/**
 ******************************************************************
 *
 * Module Name : SVPDialog.cpp
 *
 * Author/Date : C.B. Lirakis / 12-Jul-04
 *
 * Description : Real time display, one hell of a lot of this code
 *               has been taken from guitest.C. 
 *
 *               Credit where credit is due. This would not have been
 *               possible without using root. (http:://root.cern.ch)
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
 * $RCSfile: SVPDialog.cpp,v $
 * $Author:   clirakis  $
 * $Date:   15 Jul 2005 09:33:38  $
 * $Locker: clirakis $
 * $Name:  $
 * $Revision:   1.12  $
 *
 * $Log:   /usr/local/iss60/library/iss60/RTDLib/SVPDialog.cpv  $
 *******************************************************************
 */

#ifndef lint
/// RCS Information
static char rcsid[]="$Header:   /usr/local/iss60/library/iss60/RTDLib/SVPDialog.cpv   1.12   15 Jul 2005 09:33:38   clirakis  $";
#endif

// System includes.
#include <iostream>
using namespace std;
#include <cmath>


/// Root Includes
#include <TROOT.h>
#include <TApplication.h>
#include <TVirtualX.h>
#include <TGResourcePool.h>
#include <TGListBox.h>
#include <TGListTree.h>
#include <TGFSContainer.h>
#include <TGClient.h>
#include <TGFrame.h>
#include <TGIcon.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGTextEntry.h>
#include <TGNumberEntry.h>
#include <TGMsgBox.h>
#include <TGMenu.h>
#include <TGCanvas.h>
#include <TGComboBox.h>
#include <TGTab.h>
#include <TGSlider.h>
#include <TGDoubleSlider.h>
#include <TGFileDialog.h>
#include <TGTextEdit.h>
#include <TGShutter.h>
#include <TGProgressBar.h>
#include <TGColorSelect.h>
#include <TRootEmbeddedCanvas.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TRandom.h>
#include <TSystem.h>
#include <TSystemDirectory.h>
#include <TEnv.h>
#include <TFile.h>
#include <TKey.h>
#include <TVirtualX.h>
#include <TGTextEditDialogs.h> // TGPrintDialog
#include <TRootHelpDialog.h>
#include <TLegendEntry.h>

/// Local Includes.
#include "BuildDialog.hh"


// Enumerations for menu and other buttons.
enum SVPCommandIdentifiers {
   M_FILE_EXIT=100,
   M_DRAW_LINE,
   M_HELP_ABOUT,
};

static const char *HelpText1 =
"This dialog allows the user to visualize the Sound Velocity Profile (SVP)\n"
"saved to disk from the Sonde system.\n";

// ToolBarData fields.
// pixmap TipText stay_down ID TGButton
ToolBarData_t toolbar_data[] = {
  { "line.xpm"         ,"Connect Objects",    kTRUE,   M_DRAW_LINE, NULL },
  { NULL               ,            NULL,         0,             0, NULL },
};

/**
 ******************************************************************
 *
 * Function Name : BuildDialog constructor
 *
 * Description : Using root T_Graphics primatives, build
 *               a dialog for the user to select, display and
 *               edit a sound velocity profile.
 *
 * Inputs : Standard root dialog arguments.
 *
 * Returns : fully constructed dialog
 *
 * Error Conditions :
 *
 * Unit Tested on:  30-Jul-04
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
BuildDialog::BuildDialog(const TGWindow *p, UInt_t w, UInt_t h) : 
    TGMainFrame(p, w, h)
{
    Mode = DISPLAY;

    SetCleanup(kDeepCleanup);

    // Create a dialog window. A dialog window pops up with respect to its
    // "main" window. fMain is the top level widget.
    Connect("CloseWindow()", "BuildDialog", this, "CloseWindow()");

    // Layout menu here.
    CreateMenuBar(); 

    CreateToolBar();

    CreateStatusBar();
    
    AddShutter();

    AddEmbeddedCanvas();

    /*
     * Set the application window manager attributes.
     * SetMWMHints is based on:
     * value, funcs, input
     */
    SetMWMHints( kMWMDecorAll, 
		 kMWMFuncAll,
		 kMWMInputSystemModal );

    this->SetWindowName("Drawing Display");
    this->MapSubwindows();
    this->Resize();
    this->MapWindow();

}
/**
 ******************************************************************
 *
 * Function Name : BuildDialog Destructor
 *
 * Description : delete anything that is dynamically allocated.
 *
 * Inputs : none
 *
 * Returns : none
 *
 * Error Conditions : none
 *
 * Unit Tested on: 30-Jul-04
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
BuildDialog::~BuildDialog()
{
}
/**
 ******************************************************************
 *
 * Function Name : CloseWindow
 *
 * Description : Pretty simple, delete this.
 *
 * Inputs : none
 *
 * Returns : none
 *
 * Error Conditions : none
 * 
 * Unit Tested on:  30-Jul-04
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void BuildDialog::CloseWindow()
{
    void Terminate(int sig);
    Terminate(0);
    //    gApplication->Terminate(0);
}
/**
 ******************************************************************
 *
 * Function Name : CreateMenuBar
 *
 * Description : Create a menu bar on the frame, add to it the file
 * and help.
 *
 * MOdified on 19-Jan-06 to remove Save and SaveAs. 
 * This is not a good idea except in the editor.
 * 
 * Inputs : none
 *
 * Returns : none
 *
 * Error Conditions : none
 *
 * Unit Tested on: 13-Jul-05
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
 void BuildDialog::CreateMenuBar()
 {
    // Create menubar and popup menus. The hint objects are used to place
    // and group the different menu widgets with respect to eachother.
    fMenuBarLayout = new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX,
                       0, 0, 1, 1);

    fMenuBarItemLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft,
                       0, 4, 0, 0);

    fMenuBarHelpLayout = new TGLayoutHints(kLHintsTop | kLHintsRight);

    // First menu, named file, and only has exit associated with it.
    // Standard File Menu
    fMenuFile = new TGPopupMenu(gClient->GetRoot());

    fMenuFile->AddSeparator();
    fMenuFile->AddEntry("E&xit"  , M_FILE_EXIT);


    // Help menu
    fMenuHelp = new TGPopupMenu(gClient->GetRoot());
    fMenuHelp->AddSeparator();
    fMenuHelp->AddEntry("&About"   , M_HELP_ABOUT);


    fMenuBar = new TGMenuBar(this, 1, 1, kHorizontalFrame);
    fMenuBar->AddPopup("&File", fMenuFile, fMenuBarItemLayout);
    fMenuBar->AddPopup("&Help", fMenuHelp, fMenuBarHelpLayout);


    // Connect the callbacks for the menu items. 
    fMenuFile->Connect("Activated(Int_t)", "BuildDialog", this,
               "HandleMenu(Int_t)");
    fMenuHelp->Connect("Activated(Int_t)", "BuildDialog", this,
               "HandleMenu(Int_t)");

    this->AddFrame(fMenuBar, fMenuBarLayout);

    // Base Frame is complete with menubar at the top.
}

/**
 ******************************************************************
 *
 * Function Name : CreateToolBar
 *
 * Description : Perform all functions associtated with the creation
 * of the tool bar. This method creates the tool bar.
 * This list is intimately connected with the structure array
 * toolbar_data. Toolbar data is a struture that is defined by the 
 * root package and defines the button pixmap, bubble help, behaviour,
 * unique numerical id, and carries the TGButton value when created. 
 * it is terminated with a null string value. 
 *
 * Inputs : none
 *
 * Returns : none
 *
 * Error Conditions : none
 *
 * Unit Tested on: 13-Jul-05
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void BuildDialog::CreateToolBar()
{
    int spacing = 8;
    fToolBar = new TGToolBar(this, 60, 20, kHorizontalFrame | kRaisedFrame);

    for (int i = 0; toolbar_data[i].fPixmap ; i++) 
    {
        if (strlen(toolbar_data[i].fPixmap) == 0)
        {
            spacing = 8;
            continue;
        }
        fToolBar->AddButton(this, &toolbar_data[i], spacing);
        spacing = 0;
    }

    AddFrame(fToolBar, new TGLayoutHints(kLHintsTop | 
					 kLHintsExpandX, 0, 0, 0, 0));
    fToolBar->Connect("Pressed(Int_t)", "BuildDialog", this, 
		      "HandleToolBar(Int_t)");
    fToolBar->Connect("Released(Int_t)", "BuildDialog", this, 
		      "HandleToolBarU(Int_t)");
}

/**
 ******************************************************************
 *
 * Function Name : CreateStatusBar
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 *
 * Unit Tested on: 13-Jul-05
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void BuildDialog::CreateStatusBar()
{
    TGLayoutHints *statusLayout = new 
    TGLayoutHints(kLHintsBottom| kLHintsExpandX,
              0, 0, 5, 0);

    // Finally add a status bar at the bottom. parts is the
    // breakup of the 3 subdivisions of the status bar.
    Int_t parts[] = { 90, 10};
    fStatusBar = new TGStatusBar( this, 50, 10, kHorizontalFrame);
    fStatusBar->SetParts(parts, 2);
    this->AddFrame( fStatusBar, statusLayout);

    fStatusBar->SetText("Waiting.", 0);
}
/**
 ******************************************************************
 *
 * Function Name : AddEmbeddedCanvas
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 *
 * Unit Tested on: 13-Jul-05
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void BuildDialog::AddEmbeddedCanvas()
{
    TGLayoutHints *fL3 = new TGLayoutHints(kLHintsBottom | kLHintsExpandX |
                               kLHintsExpandY, 2, 2, 0, 0);

    // The remaining space I want to be a frame. Inside this frame
    // I will embed a canvas for drawing the profile.
    fGraphicsFrame = new TGHorizontalFrame(this, 60, 20);

    fEmbeddedCanvas = new TRootEmbeddedCanvas(0, fGraphicsFrame, 600, 600);

    fL4 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX |
                kLHintsExpandY, 10, 5, 5, 5);

    fGraphicsFrame->AddFrame( fEmbeddedCanvas, fL4);

    this->AddFrame( fGraphicsFrame, fL3);

    // Finally put a graph to plot on in this frame.
    TCanvas *c1 = fEmbeddedCanvas->GetCanvas();

    /*
     * Set NoContextMenu iff 
     *  a) You don't want the user to fool with the axis
     *     styles, un-zoom...
     *  b) You don't want to be able to edit the points on the graph.
     * This is a property of TObject
     */
    c1->SetBit(kNoContextMenu);

    /*
     * You'd think that setting this bit would change the
     * qualitities on the pre-existing canvas. However,
     * thie causes the program to open a brand new window
     * for the plot. This problem is in TGraph.
     */
    c1->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", 
		"BuildDialog", this,
		"CanvasEvent(Int_t,Int_t,Int_t,TObject*)");
    c1->SetEditable(kTRUE);
    c1->SetGrid();
    c1->cd();

    c1->SetFillColor(kBlack);
    c1->Draw();

    fEmbeddedCanvas->Resize();
    fGraphicsFrame->Resize();

    // Done making the graphics frame and canvas. 
}
/**
 ******************************************************************
 *
 * Function Name :  AddShutter
 *
 * Description : 
 *
 * Inputs : none
 *
 * Returns : none
 *
 * Error Conditions : none
 *
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void BuildDialog::AddShutter()
{
    //    fShutter = new TGShutter( this, kSunkenFrame);
}
/**
 ******************************************************************
 *
 * Function Name : HandleMenu
 *
 * Description : Handle menu items for this dialog.
 *
 * Inputs : ID for menu event.
 *
 * Returns : none
 *
 * Error Conditions : none
 *
 * Unit Tested on: 30-Jul-04
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void BuildDialog::HandleMenu(Int_t id)
{
    TRootHelpDialog *trh;

    // Handle menu items.
    switch (id) 
    {
    case M_FILE_EXIT:
        this->SendCloseMessage();
	break;
    case   M_HELP_ABOUT:
        trh = new TRootHelpDialog ( this,"BuildDialog Help", 600, 400);
        trh->SetText(HelpText1);
        trh->Popup();
	break;
    default:
	break;
    }
}

/**
 ******************************************************************
 *
 * Function Name : HandleToolBar
 *
 * Description : Handle toolbar items for this dialog.
 *
 * Inputs : ID for menu event.
 *
 * Returns : none
 *
 * Error Conditions : none
 *
 * Unit Tested on:
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void BuildDialog::HandleToolBar(Int_t id)
{
    switch (id) 
    {
    case M_DRAW_LINE:
	Mode = DRAW_LINE;
	break;
    }
}
/**
 ******************************************************************
 *
 * Function Name : HandleToolBar
 *
 * Description : Handle toolbar items for this dialog.
 *
 * Inputs : ID for menu event.
 *
 * Returns : none
 *
 * Error Conditions : none
 *
 * Unit Tested on:
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void BuildDialog::HandleToolBarU(Int_t id)
{
    switch (id) 
    {
    case M_DRAW_LINE:
        fToolBar->GetButton(M_DRAW_LINE)->SetState(kButtonUp);
	Mode = DISPLAY;
	break;
    }
}
/**
 ******************************************************************
 *
 * Function Name : ZOom
 *
 * Description : 
 *
 * Inputs : 
 *
 * Returns : none
 *
 * Error Conditions : none
 *
 * Unit Tested on:
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void BuildDialog::Zoom(Int_t event, Int_t px, Int_t py)
{
    Double_t temp;
    // Zoom implimentation
    switch (event) 
    {
    case kButton1Down:
	gVirtualX->SetLineColor(-1);

	// Change line attributes only if necessary
	gPad->TAttLine::Modify(); 
	x0 = gPad->AbsPixeltoX(px);
	y0 = gPad->AbsPixeltoY(py);
	px0 = px; py0 = py;
	pxold = px; pyold = py;
	linedrawn = 0;
	break;
    case kButton1Motion:
	if (linedrawn)
	{
	    gVirtualX->DrawBox(px0, py0, pxold, pyold, TVirtualX::kHollow);
	}
	pxold = px; 
	pyold = py;
	linedrawn = 1;
	gVirtualX->DrawBox(px0, py0, pxold, pyold, TVirtualX::kHollow);
	break;
    case kButton1Up:
	gPad->GetCanvas()->FeedbackMode(kFALSE);
	if (px == px0) return;
	if (py == py0) return;
	x1 = gPad->AbsPixeltoX(px);
	y1 = gPad->AbsPixeltoY(py);

	if (x1 < x0)
	{
	    temp = x0; 
	    x0   = x1;
	    x1   = temp;
	}
	if (y1 < y0)
	{
	    temp = y0;
	    y0   = y1;
	    y1   = temp;
	}
#if 0
	GetXaxis()->SetRangeUser(x0,x1);
	GetYaxis()->SetRangeUser(y0,y1);
#endif
	gPad->Modified();
	gPad->Update();
	gPad->Modified(kTRUE);
	break;
    }
}
/**
 ******************************************************************
 *
 * Function Name : ZOom
 *
 * Description : 
 *
 * Inputs : 
 *
 * Returns : none
 *
 * Error Conditions : none
 *
 * Unit Tested on:
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void BuildDialog::DrawLine(Int_t event, Int_t px, Int_t py)
{
    Double_t temp;

    switch (event) 
    {
    case kButton1Down:
	gVirtualX->SetLineColor(-1);

	// Change line attributes only if necessary
	gPad->TAttLine::Modify(); 
	x0 = gPad->AbsPixeltoX(px);
	y0 = gPad->AbsPixeltoY(py);
	px0 = px; py0 = py;
	break;
    case kButton1Motion:
	gVirtualX->DrawLine(px0, py0, px, py);
	break;
    case kButton1Up:
	gPad->GetCanvas()->FeedbackMode(kFALSE);
	if (px == px0) return;
	if (py == py0) return;
	x1 = gPad->AbsPixeltoX(px);
	y1 = gPad->AbsPixeltoY(py);

	if (x1 < x0)
	{
	    temp = x0; 
	    x0   = x1;
	    x1   = temp;
	}
	if (y1 < y0)
	{
	    temp = y0;
	    y0   = y1;
	    y1   = temp;
	}
#if 0
	GetXaxis()->SetRangeUser(x0,x1);
	GetYaxis()->SetRangeUser(y0,y1);
#endif
	// Now put this in the list of things to be drawn.
	gPad->Modified();
	gPad->Update();
	gPad->Modified(kTRUE);
	break;
    }
}
 /**
 ******************************************************************
 *
 * Function Name : CanvasEvent
 *
 * Description : 
 *
 * Inputs : 
 *
 * Returns : none
 *
 * Error Conditions : none
 *
 * Unit Tested on:
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void BuildDialog::CanvasEvent(Int_t event, Int_t px, Int_t py, 
			      TObject *selected)
{
#if 0
    TCanvas *c = (TCanvas *) gTQSender;
    printf("Canvas %s: event=%d, x=%d, y=%d, selected=%s\n", c->GetName(),
	   event, x, y, selected->IsA()->GetName());
#endif
    switch(Mode)
    {
    case DISPLAY:
	break;
    case ZOOM:
	Zoom( event, px, py);
	break;
    case DRAW_LINE:
	break;
    }
}
