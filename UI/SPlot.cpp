/**
 ******************************************************************
 *
 * Module Name : SPlot.cpp
 *
 * Author/Date : C.B. Lirakis / 2-Feb-14
 *
 * Description : Plot the output the scope
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
 *******************************************************************
 */
// System includes.
#include <iostream>
using namespace std;
#include <string>
#include <cmath>
#include <csignal>
#include <list>
#include <fstream>
#include <cstdio>

/// Root Includes
#include <TROOT.h>
#include <TApplication.h>
#include <TVirtualX.h>
#include <TGButton.h>
#include <TGMenu.h>
#include <TGCanvas.h>
#include <TGFileDialog.h>
#include <TGButton.h>
#include <TRootEmbeddedCanvas.h>
#include <TGToolBar.h>
#include <TGStatusBar.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TSystem.h>
#include <TRootHelpDialog.h>
#include <TGButtonGroup.h>
#include <TLegend.h>
#include <TMarker.h>
#include <TAxis.h>
#include <TGraph.h>
#include <TGMsgBox.h>

#include "debug.h"
#include "SPlot.hh"
#include "WaveDlg.hh"
#include "MeasDlg.hh"
#include "WFMPRE.hh"
#include "ModuleDlg.hh"
#include "TimeDlg.hh"
#include "TraceDlg.hh"
#include "FFTDlg.hh"
#include "DSA602.hh"
#include "Version.hh"

const int Scope_GPIB_A = 5;

const char *PrintPrg[]  = {"/usr/bin/lpr","/usr/bin/lp"};
const char *printName[] = {"zevonlaser","elvis"};
const char *filetypes[] = { 
    "Trace files",   "*.tr",
    "DC files",      "*.dc",
    "AC files",      "*.ac",
    "All files",     "*",
    "ROOT files",    "*.root",
    0,               0 };

static const char *SPSaveTypes[] = {
    "PostScript",   "*.ps",
    "Encapsulated PostScript", "*.eps",
    "PDF",          "*.pdf",
    "SVG",          "*.svg",
    "Gif files",    "*.gif",
    0,              0 };

/*
 * Enumerations for menu and other buttons. 
 */
enum SVPCommandIdentifiers {
   M_FILE_EXIT=100,
   M_FILE_OPEN,
   M_FILE_PRINT,
   M_VIEW_TIMEBASE,
   M_VIEW_MODULE,
   M_VIEW_MEASUREMENT,
   M_VIEW_WAVEFORM,
   M_VIEW_FFT,
   M_VIEW_TRACE,
   M_TRACE_ADD,
   M_TRACE_EDIT,
   M_TRACE_DELETE,
   M_HELP_ABOUT,
   M_ZOOM_PLUS,
   M_ZOOM_MINUS,
   M_ZOOM_SELECTED,
   M_RELOAD,
   M_FILE_SAVEAS
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
    "Interface to DSA602 digitizing scope/analyzer Version";

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
SPlot::SPlot(const TGWindow *p, UInt_t w, UInt_t h) : 
    TGMainFrame( p, w, h,  kVerticalFrame)
{
    SetCleanup(kDeepCleanup);

    // Used to store GUI elements that need to be deleted in the destructor.
    Connect("CloseWindow()", "SPlot" , this, "CloseWindow()");

    CreateMenuBar();
    CreateToolBar();
    /*
     * Two big frames stacked one on top of the other.
     * Graphics
     * Buttons
     */
    AddEmbeddedCanvas(1400, 600);
    AddControls();

    CreateStatusBar();

    MapSubwindows();
    Resize();
    SetWindowName("Scope Data");
    MapWindow();
    Move(10,10);
    PlotState    = PLOT_STATE_NORMAL;
    X            = Y = 0.0;
    fCurrentFile = 0;
    fLastDir     = new TString(".");

    //ftmg         = NULL;
    //fLegend      = NULL;
    fGraph       = NULL;
    fZoomLevel   = 2;

    // Finally create scope object
    fScope = (void *) new DSA602( Scope_GPIB_A);
    UpdateTraceButtons();
}
/**
 ******************************************************************
 *
 * Function Name : SPlot Destructor
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
SPlot::~SPlot(void)
{
    DSA602* s = (DSA602*) fScope;

    if (fCurrentFile)
    {
	delete fCurrentFile;
    }
    delete fLastDir;
    delete s;
}
/**
 ******************************************************************
 *
 * Function Name : AddControls
 *
 * Description : 
 *
 * Inputs : none
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
void SPlot::AddControls(void)
{
    TGTextButton*      button;
    TGHorizontalFrame* HFrame;
    char title[8];
    int  i;

    TGLayoutHints* L2 = new TGLayoutHints(kLHintsExpandX |
					  kLHintsExpandY, 2, 2, 2, 0);
    TGLayoutHints* L3 = new TGLayoutHints( kLHintsExpandY, 2, 2, 2, 0);

     // ========================   Buttons  ================================
    HFrame = new TGHorizontalFrame(this, 600, 600);

    button = new TGTextButton( HFrame, "Get Data", 1);
    button->Connect("Clicked()","SPlot", this, "GetData()");
    HFrame->AddFrame(button, L2);

    // --
    TGHButtonGroup *gf = new TGHButtonGroup( HFrame, "Trace:");
    gf->SetRadioButtonExclusive(kTRUE);

    /*
     * Up to 8 traces can be tracked at any given moment. 
     * Which do you want? 
     */
    for (i=0;i<8;i++)
    {
	sprintf( title, "%d", i);
	/*
	 * Add the radio button with a trace number given by i. 
	 */
	fTrace[i] = new TGRadioButton(gf, title, i);
	fTrace[i]->Connect( "Clicked()", "SPlot", this, "SetTrace()");
	gf->AddFrame(fTrace[i]);
    }
    gf->Resize();
    HFrame->AddFrame(gf, L2);
    HFrame->Resize();
    // --
    this->AddFrame( HFrame, L3);
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
void SPlot::AddEmbeddedCanvas(UInt_t w, UInt_t h)
{
    TGHorizontalFrame* HFrame;

    TGLayoutHints *L1 = new TGLayoutHints(kLHintsTop|kLHintsLeft
                                          |kLHintsExpandX,2, 2, 2, 2);
    TGLayoutHints* L2 = new TGLayoutHints(kLHintsExpandX |
					  kLHintsExpandY, 2, 2, 2, 0);

    // ==================== Graphics ====================================
    HFrame = new TGHorizontalFrame(this, 600, 600);

    fEmbeddedCanvas = new TRootEmbeddedCanvas("ec1", HFrame, w, h);

    TCanvas *c1 = fEmbeddedCanvas->GetCanvas();
    c1->Connect("ProcessedEvent(Int_t, Int_t, Int_t, TObject *)", 
		"SPlot", this,
		"ProcessedEvent(Int_t, Int_t, Int_t, TObject *)");
			     
    HFrame->SetBackgroundColor(0);
    HFrame->AddFrame( fEmbeddedCanvas, L2);
    this->AddFrame( HFrame, L1);
    
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
void SPlot::CreateMenuBar(void)
{
    TGPopupMenu *MenuFile, *MenuView, *MenuHelp;
	//*MenuEdit

    // Layout menu here. 
    // Create menubar and popup menus. The hint objects are used to place
    // and group the different menu widgets with respect to eachother.

    // First menu, named file, and only has exit associated with it. 
    // Standard File Menu
    MenuFile = new TGPopupMenu(gClient->GetRoot());

    //MenuFile->AddEntry("O&pen"  , M_FILE_OPEN);
    //MenuFile->AddEntry("SaveA&s", M_FILE_SAVEAS);
    MenuFile->AddEntry("P&rint" , M_FILE_PRINT);

    MenuFile->AddSeparator();
    MenuFile->AddEntry("E&xit"  , M_FILE_EXIT);

    /*
     * Disable buttons that are currently non-functional
     */
    MenuFile->DisableEntry( M_FILE_PRINT);
    /*********************************************************
     * Edit menu
     ********************************************************/


    /*********************************************************
     * Trace submenus
     ********************************************************/
    TGPopupMenu *TraceCascade = new TGPopupMenu(gClient->GetRoot());
    TraceCascade->AddEntry("Add", M_TRACE_ADD);
    TraceCascade->AddEntry("Edit", M_TRACE_EDIT);
    TraceCascade->AddEntry("Delete", M_TRACE_DELETE);

    /*********************************************************
     * View menu
     ********************************************************/
    MenuView = new TGPopupMenu(gClient->GetRoot());
    MenuView->AddEntry("Time base"   , M_VIEW_TIMEBASE);
    MenuView->AddEntry("Module"      , M_VIEW_MODULE);
    MenuView->AddEntry("Waveform"    , M_VIEW_WAVEFORM);
    MenuView->AddPopup("Trace"       , TraceCascade);
    MenuView->AddEntry("FFT"         , M_VIEW_FFT);
    MenuView->AddEntry("Measurement" , M_VIEW_MEASUREMENT);

    /*********************************************************
     * Help menu
     ********************************************************/
    MenuHelp = new TGPopupMenu(gClient->GetRoot());
    MenuHelp->AddSeparator();
    MenuHelp->AddEntry("&About"   , M_HELP_ABOUT);


    TGMenuBar *MenuBar = new TGMenuBar( this, 1, 1, kHorizontalFrame);

    MenuBar->AddPopup("&File", MenuFile, new 
		       TGLayoutHints(kLHintsTop | kLHintsLeft, 
				     0, 4, 0, 0));

    MenuBar->AddPopup("&View", MenuView, new TGLayoutHints(
			  kLHintsTop | kLHintsLeft, 
			  0, 4, 0, 0));

    MenuBar->AddPopup("&Help", MenuHelp, 
		      new TGLayoutHints(kLHintsTop | kLHintsRight) );


    // Connect the callbacks for the menu items. 
    MenuFile->Connect("Activated(Int_t)", "SPlot", this,
		       "HandleMenu(Int_t)");
    MenuView->Connect("Activated(Int_t)", "SPlot", this,
		       "HandleMenu(Int_t)");
    MenuHelp->Connect("Activated(Int_t)", "SPlot", this,
		       "HandleMenu(Int_t)");

    AddFrame(MenuBar, 
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
void SPlot::CreateToolBar(void)
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

    for (int i = 0; dialog_xpm_names[i]; i++) 
    {
        TString iconname(Path);
        iconname += dialog_xpm_names[i];
        toolbar_data[i].fPixmap = iconname.Data();
	fToolBar->AddButton(this, &toolbar_data[i], spacing);
    }
    AddFrame(fToolBar, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0,
					 0, 0));
    fToolBar->Connect("Pressed(Int_t)", "SPlot", this, 
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
void SPlot::HandleToolBar(Int_t id)
{
    TGButton *tb;
    TCanvas *c1;

    switch (id) 
    {
    case M_FILE_OPEN:
	DoLoad();
	tb = fToolBar->GetButton(M_FILE_OPEN);
        tb->SetState(kButtonUp);
	break;
    case M_ZOOM_PLUS:
	Zoom();
	tb = fToolBar->GetButton(M_ZOOM_PLUS);
        tb->SetState(kButtonUp);
	break;
    case M_ZOOM_MINUS:
	UnZoom();
	tb = fToolBar->GetButton(M_ZOOM_MINUS);
        tb->SetState(kButtonUp);
	break;
    case M_ZOOM_SELECTED:
	c1 = fEmbeddedCanvas->GetCanvas();
	c1->SetEditable(kFALSE);
	PlotState = PLOT_STATE_ZOOM;
	break;
    case M_RELOAD:
        tb = fToolBar->GetButton(M_RELOAD);
        tb->SetState(kButtonUp);
	OpenAndParseFile( NULL);
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
void SPlot::CreateStatusBar(void)
{
    /*
     * Finally add a status bar at the bottom. 
     * parts is the breakup of the 3 subdivisions of the
     * status bar.
     */
    Int_t parts[] = {40, 40, 20};
    fStatusBar = new TGStatusBar( this, 10, 10, kHorizontalFrame);
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
void SPlot::HandleMenu(Int_t id)
{
    char HelpText[512];
    TRootHelpDialog *trh;
    TCanvas *c1;

    // Handle menu items.
    switch (id) 
    {

    case M_FILE_EXIT:
	SendCloseMessage();
	break;
    case M_FILE_OPEN:
	DoLoad();
	break;
    case M_FILE_SAVEAS:
	DoSaveAs();
	break;
    case M_FILE_PRINT:
#if 0
        new TGPrintDialog 
	    ( gClient->GetRoot(), this, 400,300,  printName, PrintPrg, &rc);
	if (rc == kTRUE)
	{
	    // Print!
	    printf("All is well!\n");
	}
#endif
        // Only saves to PS file, not quite what I want. 
	//	gStyle->SetPaperSize(kUSLetter);
	c1 = fEmbeddedCanvas->GetCanvas();
	c1->Print();
	break;
    case M_VIEW_TIMEBASE:
	new TimeDlg( this);
	break;
    case M_VIEW_MODULE:
	new ModuleDlg( this);
	break;
    case M_VIEW_WAVEFORM:
	new WaveDlg( this, fScope);
	break;
    case M_VIEW_FFT:
	new FFTDlg( this);
	break;
    case M_TRACE_EDIT:
	new TraceDlg( this);
	break;
    case M_TRACE_DELETE:
	break;
    case M_TRACE_ADD:
	break;
    case M_VIEW_MEASUREMENT:
	cout << "Measurement." << endl;
	new MeasDlg( this);
	break;
    case M_HELP_ABOUT:
	sprintf( HelpText, "%s %d.%d\n", HelpText1, MAJOR_VERSION, 
		 MINOR_VERSION);
	trh = new TRootHelpDialog ( this,"DSA602 Help", 600, 400);
	trh->SetText(HelpText);
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
void SPlot::SetCurrentFileName(const char *File)
{
    /* Delete current file name */
    if (fCurrentFile)
    {
	delete fCurrentFile;
    }
    fCurrentFile = new TString(File);
    if(fVerbose>0) 
    {
	cout << "Current " << fCurrentFile << endl;
    }
    fStatusBar->SetText( fCurrentFile->Data(), 0);
}

/**
 ******************************************************************
 *
 * Function Name : DoLoad
 *
 * Description : Handle the Load menu option by bringing up a file
 *               selection dialog. 
 *
 * Inputs : none
 *
 * Returns : none
 *
 * Error Conditions : 
 * 
 * Unit Tested on: 25-Apr-08
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void SPlot::DoLoad(void)
{
    TGFileInfo fi;

    fi.fFileTypes = filetypes;
    fi.fIniDir    = StrDup(fLastDir->Data());

    new TGFileDialog( gClient->GetRoot(), 0, kFDOpen, &fi);
    *fLastDir = fi.fIniDir;
    OpenAndParseFile(fi.fFilename);
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
void SPlot::PlotMe(Int_t Index)
{
    gPad->Clear();
    gPad->Update();
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
void SPlot::CloseWindow(void)
{
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
void SPlot::ProcessedEvent(Int_t event, Int_t px, Int_t py, 
			     TObject *selected)
{
    // Statics for rubber band zoom. 
    static Double_t x0, y0, x1, y1;
    static Int_t    pxold, pyold;
    static Int_t    px0, py0;
    static Int_t    linedrawn;

    Double_t        temp;
    Char_t          title[64];
    Double_t        x,y;

    TCanvas *c1 = fEmbeddedCanvas->GetCanvas();
    c1->AbsPixeltoXY( px, py, x, y); 

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
	    //TH1 *h = ftmg->GetHistogram();
	    TH1 *h = fGraph->GetHistogram();
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
}
/**
 ******************************************************************
 *
 * Function Name : UnZoom
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
void SPlot::UnZoom(void)
{
    TH1 *h = fGraph->GetHistogram();
    if (h)
    {
	cout << "Got Histogram " << endl;
	h->GetXaxis()->UnZoom();
	h->GetYaxis()->UnZoom();
    }
    gPad->Update();
}
/**
 ******************************************************************
 *
 * Function Name : Zoom
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
void SPlot::ZoomAxis(TAxis *a)
{
    Double_t x,y, delta;
    x = a->GetXmin();
    y = a->GetXmax();
    delta = fabs(y-x);
    delta /= (2.0*fZoomLevel);
    if (x>0.0)
    {
	x += delta;
    }
    else
    {
	x -= delta;
    }
    if (y>0.0)
    {
	y -= delta;
    }
    else
    {
	y += delta;
    }
    cout << " Set range User " << x << " " << y << endl;
    a->SetRangeUser(x,y);
}

/**
 ******************************************************************
 *
 * Function Name : Zoom
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
void SPlot::Zoom(void)
{
    TH1 *h = fGraph->GetHistogram();
    TAxis *ax;
    if (h)
    {
	ax = h->GetXaxis();
	ZoomAxis(ax);
	// Now do y axis. 
	ax = h->GetYaxis();
	ZoomAxis(ax);
	fZoomLevel *= 2.0;
    }
    gPad->Update();
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
bool SPlot::OpenAndParseFile(const char *file)
{
#if 0
    char       msg[512];
    ifstream   *file_in;
    TGraph     *tg;
    TList      *graphs;
    Double_t   x, y;
    TH1F       *hist;
    TList      *TraceTitles;
    TString    *col1Title;
    Int_t      NColumns;    // Number of columns of data. 
#endif

    if (file != NULL)
    {
	SetCurrentFileName(file);
    }
    else
    {
	cout <<"Reloading file. " << endl;
    }

#if 0
    file_in = new ifstream( fCurrentFile->Data(), ifstream::in );
    if (file_in->fail())
    {
	sprintf (msg, "File not found: %s", fCurrentFile->Data());
	new TGMsgBox( gClient->GetRoot(), NULL, "Error", 
		      msg, kMBIconExclamation);
	fStatusBar->SetText( "NONE", 0);
    }
    else
    {
	new TGMsgBox( gClient->GetRoot(), NULL, "Error", 
		      "DOES NOTHING!", kMBIconExclamation);
	fStatusBar->SetText( "NONE", 0);
	if (fVerbose > 0) 
	{
	    cout << "Open and Parse File" << endl;
	}
	/* Clean up on reload or new load */
	//CleanGraphObjects();
	//CreateGraphObjects();
	//TraceTitles = new TList();
    }
#endif
    return true;
}
bool SPlot::CreateGraphObjects(void)
{
    //ftmg         = new TMultiGraph();
    //fLegend      = new TLegend(0.80, 0.75, 0.95, 0.89);

    return true;
}
bool SPlot::CleanGraphObjects(void)
{
    gPad->Clear();

    if (fGraph)
    {
	delete fGraph;
	fGraph = NULL;
    }
    return true;
}

/**
 ******************************************************************
 *
 * Function Name : DoSaveAs
 *
 * Description : Handle the Save menu option by bringing up a file
 *               selection dialog. 
 *
 * Inputs : none
 *
 * Returns : none
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
void SPlot::DoSaveAs(void)
{

    TGFileInfo fi;

    fi.fFileTypes = SPSaveTypes;
    fi.fIniDir    = StrDup(fLastDir->Data());

    new TGFileDialog( gClient->GetRoot(), this, kFDSave, &fi);
    if (fi.fFilename == NULL)
    {
	// No action to be taken!
	return;
    }
    cout << "Filename " << fi.fFilename << endl;
    if (strlen(fi.fFilename) > 0)
    {
	TCanvas *c1 = fEmbeddedCanvas->GetCanvas();
	c1->SaveAs(fi.fFilename);
    }
}
/**
 ******************************************************************
 *
 * Function Name : GetData
 *
 * Description : See what traces are available on the scope and pull them. 
 *
 * Inputs : none
 *
 * Returns : None
 *
 * Error Conditions :
 *
 * Unit Tested on:
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void SPlot::GetData(void)
{
    double *Y, *X;
    DSA602* scope = (DSA602*) fScope;
    Int_t retval;

    Int_t n           = 0;
    Int_t TraceNumber = 0;

#if 0
    /*
     * FIXME!
     * The lower level code in the library does not work. 
     * Looking to see if there is any data at all to retrieve. 
     * This does not do that correctly. 
     */
    for(Int_t i=0;i<8;i++)
    {
	if(scope->GetDisplayTrace(i)) TraceNumber = i+1;
    }
    if (TraceNumber==0)
    {
        new TGMsgBox(fClient->GetRoot(),
                     this, "NO Trace", "No trace selected to retrieve.",
                     kMBIconExclamation, kMBOk, &retval);
	return;
    }
#endif
    SET_DEBUG_STACK;

    /*
     * for the given trace number, return the X and Y point set. 
     * n contains the number of points in the curve if present. 
     */
#if 0
    cout << __FUNCTION__ << " "
	 << " Get the data for trace: " << TraceNumber << endl;
#endif
    n = scope->Curve(TraceNumber, &X, &Y);
    //cout << "Number points: " << n << endl;

    if (n>0)
    {
	if (fGraph) delete fGraph;
	fGraph = new TGraph( n, X, Y);
	// FIXME
	//fGraph->SetTitle(scope->fWFMPRE->fWFId);
	fGraph->Draw("ALP");
// 	TH1 *h = fGraph->GetHistogram();
// 	if (h)
// 	{
// 	    h->SetXTitle(scope->fWFMPRE->XAxis());
// 	    h->SetYTitle(scope->fWFMPRE->YAxis());
// 	}
	gPad->Update();
	free(Y);
	free(X);
    }
    else
    {
        new TGMsgBox(fClient->GetRoot(),
                     this, "NO POINTS", "Received no data from scope.",
                     kMBIconExclamation, kMBOk, &retval);
    }
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : SetTrace
 *
 * Description : Handle the radio buttons that select which trace
 * to display. Strictly one at a time right now. 
 *
 * Inputs : none
 *
 * Returns : None
 *
 * Error Conditions :
 *
 * Unit Tested on:
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void SPlot::SetTrace(void)
{
    SET_DEBUG_STACK;
    DSA602* scope = (DSA602*) fScope;
    Int_t   i;
    /*
     * Which button sent us this message? We need the id as an index. 
     */
    TGButton *btn = (TGButton *) gTQSender;
    Int_t id      = btn->WidgetId();
    /* 
     * Make sure the radio behaviour is followed. 
     * uncheck the old button. 
     */
    for (Int_t i=0; i<8; i++)
    {
	/*
	 * loop over all the buttons. Make sure they aren't set unless
	 * they are the desired button. 
	 * The ids given are in the order of the buttons. This makes it easy
	 * to make a match on which button needs to be pressed. 
	 */
	if ((i!=id) && (fTrace[i]->IsOn()))
	{
	    fTrace[i]->SetState(kButtonUp);
	}
    }
    /*
     * Finally record the trace to be downloaded. 
     * FIXME ---- The SetDisplayTrace function is not shall we say functional.
     */
    scope->SetDisplayTrace(id, true);
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : UpdateTraceButtons
 *
 * Description : I think I was trying to see what traces were available
 *               on the screen. This clearly does not do that.
 *
 * Inputs : none
 *
 * Returns : None
 *
 * Error Conditions :
 *
 * Unit Tested on:
 *
 * Unit Tested by:
 *
 *
 *******************************************************************
 */
void SPlot::UpdateTraceButtons(void)
{
    DSA602* scope = (DSA602*) fScope;
    int i;
    // FIXME!
    return;
    for (i=0;i<8;i++)
    {

	if (scope->GetDisplayTrace(i))
	{
	    //fTrace[i]->SetState(kButtonDown, kFALSE);
	    cout << "Scope says, get trace: " << i << endl;
	}
// 	else
// 	{
// 	    fTrace[i]->SetState(kButtonUp, kFALSE);
// 	}
    }
}
