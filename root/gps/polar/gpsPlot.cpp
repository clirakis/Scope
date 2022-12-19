/**
 ******************************************************************
 *
 * Module Name : gpsPlot.cpp
 *
 * Author/Date : C.B. Lirakis / 29-June-08
 *
 * Description : 
 *
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
 * $RCSfile: spiceplot.cpp,v $
 * $Author: clirakis $
 * $Date: 2008/04/23 16:51:41 $
 * $Locker:  $
 * $Name:  $
 * $Revision: 1.1.1.1 $
 *
 * $Log: spiceplot.cpp,v $
 * Revision 1.1.1.1  2008/04/23 16:51:41  clirakis
 * Imported using TkCVS
 *
 *******************************************************************
 */

#ifndef lint
/// RCS Information
static char rcsid[]="$Header: /home/clirakis/cvs/rootmacros/spiceplot.cpp,v 1.1.1.1 2008/04/23 16:51:41 clirakis Exp $";
#endif

// System includes.
#include <iostream>
using namespace std;
#include <string>
#include <cmath>
#include <csignal>
#include <list>
#include <fstream>

/// Root Includes
#include <TROOT.h>
#include <TApplication.h> // To get access to gApplication
#include <RQ_OBJECT.h>
#include <TQObject.h>

#include <TGFrame.h>
#include <TGIcon.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGTextEntry.h>
#include <TGNumberEntry.h>
#include <TGMsgBox.h>
#include <TGMenu.h>
#include <TGCanvas.h>
#include <TGTab.h>
#include <TGSlider.h>
#include <TGButton.h>
#include <TGProgressBar.h>
#include <TRootEmbeddedCanvas.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TEnv.h>
#include <TRootHelpDialog.h>
#include <TGButtonGroup.h>

#include <TGToolBar.h>
#include <TGStatusBar.h>

#include <TEllipse.h>
#include <TText.h>
#include <TView.h>
#include <TView3D.h>
#include <TBox.h>
#include <TGraph.h>

#include "gpsPlot.hh"
#include "CreateTab.hh"
#include "Polar.hh"

const char *PrintPrg[]  = {"/usr/bin/lpr","/usr/bin/lp"};
const char *printName[] = {"zevonlaser","elvis"};
const char *filetypes[] = { 
    "Trace files",   "*.tr",
    "All files",     "*",
    "ROOT files",    "*.root",
    0,               0 };

/*
 * Enumerations for menu and other buttons. 
 */
enum SVPCommandIdentifiers {
   M_FILE_EXIT=100,
   M_FILE_OPEN,
   M_FILE_POLAR,
   M_FILE_PRINT,
   M_HELP_ABOUT,
   M_ZOOM_PLUS,
   M_ZOOM_MINUS,
   M_ZOOM_SELECTED,
   M_RELOAD,
};

// Toolbar stuff
const char *dialog_xpm_names[] = {
    "bld_open.png",
    "zoom_plus.xpm",
    "zoom_minus.xpm",
    "zoom_selected.xpm",
    "tb_refresh.xpm",
    0
};

// ToolBarData fields.
// pixmap TipText stay_down ID TGButton
ToolBarData_t toolbar_data[] = {
  { "",     "Load file",    kFALSE,     M_FILE_OPEN, NULL },
  { "", "Zoom Selected",     kTRUE,     M_ZOOM_PLUS, NULL },
  { "",        "UnZoom",         0,    M_ZOOM_MINUS, NULL },
  { "", "Zoom Selected",         0, M_ZOOM_SELECTED, NULL },
  { "",        "Reload",     kTRUE,        M_RELOAD, NULL },
  { "",            NULL,         0,               0, NULL }
};


static const char *HelpText1 = 
    "This dialog allows plotting of the traces produced by gnucap\n";

#define TIMEOUT_ON 1

/**
 ******************************************************************
 *
 * Function Name :
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
gpsPlot::gpsPlot(const TGWindow *p, UInt_t w, UInt_t h, int v) : 
    TGMainFrame( p, w, h,  kVerticalFrame)
{
    // Used to store GUI elements that need to be deleted in the destructor.
    fCleanup = new TList();
    fCleanup->SetOwner();
    Connect("CloseWindow()", "gpsPlot" , this, "CloseWindow()");

    CreateMenuBar();
    CreateToolBar();
    fTab = new CreateTab(this);
    //AddEmbeddedCanvas(1400, 900);
    CreateStatusBar();

    MapSubwindows();
    Resize();
    SetWindowName("Raw Display");
    MapWindow();
    Move(10,10);
    fVerbose      = v;

    fTimer = new TTimer();
    // Set it up to call PlotTimeoutProcedure once per second.
    fTimer->Connect("Timeout()", "gpsPlot", this, "TimeoutProc()");
#if TIMEOUT_ON==1
    fTimer->Start(500, kFALSE);
#endif
}
/**
 ******************************************************************
 *
 * Function Name : gpsPlot Destructor
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
gpsPlot::~gpsPlot()
{
    delete fCleanup;
}
/**
 ******************************************************************
 *
 * Function Name : AddEmbeddedCanvas
 *
 * Description : 
 *
 * Inputs : w - width of canvas
 *          h - height of canvas
 *
 * Returns : none
 *
 * Error Conditions :
 * 
 * Unit Tested on: 24-Apr-08
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void gpsPlot::AddEmbeddedCanvas(UInt_t w, UInt_t h)
{
    /*
     * The remaining space I want to be a frame. 
     * Inside this frame I will embed a canvas for
     * drawing the profile. 
     */

    TGHorizontalFrame *fGraphicsFrame = new TGHorizontalFrame(this, 600, 600);
    fCleanup->Add(fGraphicsFrame);

    fEmbeddedCanvas = new TRootEmbeddedCanvas("ec1", fGraphicsFrame, w, h);
    fCleanup->Add(fEmbeddedCanvas);
    TCanvas *c1 = fEmbeddedCanvas->GetCanvas();
    c1->Connect("ProcessedEvent(Int_t, Int_t, Int_t, TObject *)", 
		"gpsPlot", this,
		"ProcessedEvent(Int_t, Int_t, Int_t, TObject *)");
			     

    fGraphicsFrame->SetBackgroundColor(0);
    fGraphicsFrame->AddFrame( fEmbeddedCanvas, 
			      new TGLayoutHints(kLHintsExpandX |
						kLHintsExpandY, 2, 2, 2, 0));
    this->AddFrame( fGraphicsFrame, 
		    new TGLayoutHints(kLHintsExpandX |
				      kLHintsExpandY, 2, 2, 2, 0));
    // Finally put a graph to plot on in this frame.
    
    c1->SetBorderMode(0);
#if 0
    /* 
     * Set NoContextMenu iff 
     *  a) You don't want the user to fool with the axis 
     *     styles, un-zoom...
     *  b) You don't want to be able to edit the points on the graph.
     * This is a property of TObject
     */
    c1->SetBit(kNoContextMenu);
#endif
    c1->SetFillColor(0);
    c1->SetGrid();
    //c1->SetEditable(kFALSE);
    //c1->SetCrosshair(1);
    c1->cd();
}
/**
 ******************************************************************
 *
 * Function Name : CreateMenuBar
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
void gpsPlot::CreateMenuBar()
{
    TGPopupMenu *fMenuFile, *fMenuHelp;

    // Layout menu here. 
    // Create menubar and popup menus. The hint objects are used to place
    // and group the different menu widgets with respect to eachother.

    // First menu, named file, and only has exit associated with it. 
    // Standard File Menu
    fMenuFile = new TGPopupMenu(gClient->GetRoot());
    fCleanup->Add(fMenuFile);

    fMenuFile->AddEntry("O&pen"  , M_FILE_OPEN);
    fMenuFile->AddEntry("Polar" , M_FILE_POLAR);
    fMenuFile->AddEntry("P&rint" , M_FILE_PRINT);

    fMenuFile->AddSeparator();
    fMenuFile->AddEntry("E&xit"  , M_FILE_EXIT);

    /*
     * Disable buttons that are currently non-functional
     */
    fMenuFile->DisableEntry( M_FILE_PRINT);

    // Help menu
    fMenuHelp = new TGPopupMenu(gClient->GetRoot());
    fCleanup->Add(fMenuHelp);
    fMenuHelp->AddSeparator();
    fMenuHelp->AddEntry("&About"   , M_HELP_ABOUT);


    TGMenuBar *fMenuBar = new TGMenuBar( this, 1, 1, kHorizontalFrame);
    fCleanup->Add(fMenuBar);
    fMenuBar->AddPopup("&File", fMenuFile, new 
		       TGLayoutHints(kLHintsTop | kLHintsLeft, 
				     0, 4, 0, 0) 
		       );
    fMenuBar->AddPopup("&Help", fMenuHelp, 
		      new TGLayoutHints(kLHintsTop | kLHintsRight) );


    // Connect the callbacks for the menu items. 
    fMenuFile->Connect("Activated(Int_t)", "gpsPlot", this,
		       "HandleMenu(Int_t)");
    fMenuHelp->Connect("Activated(Int_t)", "gpsPlot", this,
		       "HandleMenu(Int_t)");

    AddFrame(fMenuBar, 
	     new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX,
			       0, 0, 1, 1));
}
/**
 ******************************************************************
 *
 * Function Name : CreateToolBar
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
void gpsPlot::CreateToolBar()
{
    TString Path;

    if(gSystem->Getenv("ROOTSYS") != NULL)
    {
        Path = gSystem->Getenv("ROOTSYS");
    }
    else
    {
        Path = "./";
    }

    Path+="/icons/";
    int spacing = 8;
    fToolBar = new TGToolBar(this, 60, 20, kHorizontalFrame | kRaisedFrame);
    fCleanup->Add(fToolBar);

    for (int i = 0; dialog_xpm_names[i]; i++) 
    {
        TString iconname(Path);
        iconname += dialog_xpm_names[i];
        toolbar_data[i].fPixmap = iconname.Data();
	fToolBar->AddButton(this, &toolbar_data[i], spacing);
    }
    AddFrame(fToolBar, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0,
					 0, 0));
    fToolBar->Connect("Pressed(Int_t)", "gpsPlot", this, 
		      "HandleToolBar(Int_t)");
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
void gpsPlot::HandleToolBar(Int_t id)
{
    TGButton *tb;

    switch (id) 
    {
    case M_FILE_OPEN:
	tb = fToolBar->GetButton(M_FILE_OPEN);
        tb->SetState(kButtonUp);
	break;
    case M_ZOOM_PLUS:
	tb = fToolBar->GetButton(M_ZOOM_PLUS);
        tb->SetState(kButtonUp);
	break;
    case M_ZOOM_MINUS:
	tb = fToolBar->GetButton(M_ZOOM_MINUS);
        tb->SetState(kButtonUp);
	break;
    case M_ZOOM_SELECTED:
	//	c1 = fEmbeddedCanvas->GetCanvas();
	//c1->SetEditable(kFALSE);
	break;
    case M_RELOAD:
        tb = fToolBar->GetButton(M_RELOAD);
        tb->SetState(kButtonUp);
	break;
    }
}
/**
 ******************************************************************
 *
 * Function Name :
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
void gpsPlot::CreateStatusBar()
{
    /*
     * Finally add a status bar at the bottom. 
     * parts is the breakup of the 3 subdivisions of the
     * status bar.
     */
    Int_t parts[] = {40, 40, 20};
    fStatusBar = new TGStatusBar( this, 10, 10, kHorizontalFrame);
    fCleanup->Add(fStatusBar);
    fStatusBar->SetParts(parts, 3);
    this->AddFrame( fStatusBar,  new 
		    TGLayoutHints( kLHintsExpandX , 2, 2, 2, 2));
    fStatusBar->SetText("Please Select Data to Display.",0);
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
void gpsPlot::HandleMenu(Int_t id)
{
    TRootHelpDialog *trh;

    // Handle menu items.
    switch (id) 
    {

    case M_FILE_EXIT:
	SendCloseMessage();
	break;
    case M_FILE_OPEN:
	break;
#if 0
    case M_FILE_PRINT:

        new TGPrintDialog 
	    ( gClient->GetRoot(), this, 400,300,  printName, PrintPrg, &rc);
	if (rc == kTRUE)
	{
	    // Print!
	    printf("All is well!\n");
	}

        // Only saves to PS file, not quite what I want. 
	//	gStyle->SetPaperSize(kUSLetter);
	c1 = fEmbeddedCanvas->GetCanvas();
	c1->Print();
	break;
#endif	
    case   M_HELP_ABOUT:
	trh = new TRootHelpDialog ( this,"BoomTools Help", 600, 400);
	trh->SetText(HelpText1);
	trh->Popup();
	break;

    default:
	printf("Menu item %d selected\n", id);
	break;
   }
}

/**
 ******************************************************************
 *
 * Function Name :
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */

/**
 ******************************************************************
 *
 * Function Name :
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void gpsPlot::CloseWindow()
{
    if (fTimer)
    {
        fTimer->Stop();
        fTimer->Disconnect("Timeout()");
        delete fTimer;
        fTimer = 0;
    }

   // Got close message for this MainFrame. Terminates the application.
   gApplication->Terminate(0);
}

/**
 ******************************************************************
 *
 * Function Name :
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void gpsPlot::ProcessedEvent(Int_t event, Int_t px, Int_t py, 
			     TObject *selected)
{
#if 0
    // Statics for rubber band zoom. 
    static Double_t x0, y0, x1, y1;
    static Int_t    pxold, pyold;
    static Int_t    px0, py0;
    static Int_t    linedrawn;

    Double_t        temp;
    Char_t          title[64];
    Double_t        x,y;

    TCanvas *c1 = fEmbeddedCanvas->GetCanvas();
    c1->AbsPixeltoXY(px,py,x,y); 
    switch(event)
    {
    case kMouseMotion:
	sprintf(title, "X: %8.4g Y: %8.1g", x, y);
	fStatusBar->SetText(title ,1);
	switch(PlotState)
	{
	case PLOT_STATE_ZOOM:
            if (linedrawn)
            {
                gVirtualX->DrawBox(px0, py0, pxold, pyold, TVirtualX::kHollow);
            }
            pxold = px; 
            pyold = py;
            linedrawn = 1;
	    gVirtualX->DrawBox(px0, py0, pxold, pyold, TVirtualX::kHollow);
	    break;
	}
	break;
    case kButton1Down:
	down_x = x;
	down_y = y;
	switch (PlotState)
	{
	case PLOT_STATE_ZOOM:
            gVirtualX->SetLineColor(-1);

            // Change line attributes only if necessary
	    //gPad->TAttLine::Modify(); 
            x0 = gPad->AbsPixeltoX(px);
            y0 = gPad->AbsPixeltoY(py);
            px0 = px; py0 = py;
            pxold = px; pyold = py;
            linedrawn = 0;
	    break;
	}
	break;
    case kButton1Up:
#if 0
	sprintf( title , "dt (ms): %8.2f dY: %8.1f", dx*1000.0, dy);
	fStatusBar->SetText(title ,2);
#endif
	switch (PlotState)
	{
	case PLOT_STATE_ZOOM:
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
	    TH1 *h = ftmg->GetHistogram();
	    if (h)
	    {
		h->GetXaxis()->SetRangeUser(x0,x1);
		h->GetYaxis()->SetRangeUser(y0,y1);
	    }
            gPad->Modified();
            gPad->Update();
	    PlotState = PLOT_STATE_NORMAL;
	    break;
	}
	break;
    case kButton3Down:
    case kButton1Motion:
    case kMouseEnter:
    case kMouseLeave: 
	break;
    default:
	printf("Event %d\n", event);
	break;
    }
#endif
}
/**
 ******************************************************************
 *
 * Function Name : TimeoutProc
 *
 * Description : Update screen data periodically.
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
void gpsPlot::TimeoutProc()
{
    fTab->Test();
    //cout << "Timeout" << endl;
}

