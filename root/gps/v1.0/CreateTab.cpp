/********************************************************************
 *
 * Module Name : CreateTab.cpp
 *
 * Author/Date : C.B. Lirakis / 15-Mar-05
 *
 * Description : Generic CreateTab
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
 * $RCSfile$
 * $Author$
 * $Date$
 * $Locker$
 * $Name$
 * $Revision$
 *
 * $Log: $
 *
 *
 * Copyright (C) BBNT 2005
 ********************************************************************/

#ifndef lint
static char rcsid[]="$Header$";
#endif

// System includes.
#include <iostream>
using namespace std;
#include <string>
#include <cmath>
#include <time.h>

// root includes
#include <TEllipse.h>
#include <TText.h>
#include <TView.h>
#include <TView3D.h>
#include <TBox.h>
#include <TGraph.h>
#include <TRootEmbeddedCanvas.h>
#include <TCanvas.h>

// Local Includes.
#include "CreateTab.hh"
#include "debug.h"

#define LABEL_SIZE      0.1
#define TEXT_SIZE       0.20

/**
 ******************************************************************
 *
 * Function Name : CreateTab constructor
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
CreateTab::CreateTab (TGCompositeFrame* p) : TGTab( p, 300,300)
{
    SET_DEBUG_STACK;
    this->Connect("Selected(Int_t)", "CreateTab", this, "DoTab(Int_t)");
    AddTab1();
    AddTab2();
    TGLayoutHints    *fL5 = new TGLayoutHints(kLHintsTop | kLHintsExpandX |
					      kLHintsExpandY, 2, 2, 5, 1);
    p->AddFrame(this, fL5);
    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : CreateTab destructor
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
CreateTab::~CreateTab ()
{
    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : CreateGraphicsFrame
 *
 * Description : Create the frame that holds the status and position
 * data.
 *
 * Inputs : none
 *
 * Returns : None
 *
 * Error Conditions :
 *
 * Unit Tested on: 14-Nov-05
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void CreateTab::CreateGraphicsFrame(TGCompositeFrame *putHere)
{
    SET_DEBUG_STACK;
    TGHorizontalFrame *graphicsFrame[2];
    
    Double_t rmin[] = {0.0, 0.0, 0.0};
    Double_t rmax[] = {1.0, 1.0, 1.0};

    /*
     * We have a transient frame, How do we want to divide this up?
     * For the moment, use 2 frames, the top containing the graphic
     * and the bottom containg the text data.
     *
     * Make the upper graphics frame first.
     */
    TGLayoutHints *tl =  new TGLayoutHints(kLHintsTop | kLHintsExpandX,
					   1, 1, 1, 1);
    // Top 
    graphicsFrame[0] = new TGHorizontalFrame(putHere, 10, 10);
    putHere->AddFrame(graphicsFrame[0], tl);
    // Bottom
    graphicsFrame[1] = new TGHorizontalFrame(putHere, 10, 10);
    putHere->AddFrame(graphicsFrame[1], tl);

    fPosPlot = new TRootEmbeddedCanvas("POSPLOT_EC", graphicsFrame[0], 
				       400, 400);
    graphicsFrame[0]->AddFrame(fPosPlot, tl);
    TCanvas *c1 = fPosPlot->GetCanvas();
    c1->SetGrid();
    c1->SetBorderMode(0);
   //c1->SetEditable(kFALSE);
    //c1->SetCrosshair(1);
    c1->cd();

    // Put plotting graphic in top frame. 
    fXY = new TGraph(200);
    fXY->SetPoint(0,41.5, -71.25);
    fXY->SetMarkerColor(2);
    fXY->SetMarkerStyle(20);
    fXY->Draw("ALP");
    fXYPointer = 0;
    fXY->Set(fXYPointer);

    // Want a drawing canvas in lower frame.
    fStatus_EC = new TRootEmbeddedCanvas("STATUS_EC", graphicsFrame[1], 
					 400, 100);
    graphicsFrame[1]->AddFrame(fStatus_EC, tl);
    fStatus_EC->GetCanvas()->SetFillColor(kBlack);
    fStatus_EC->GetCanvas()->SetBit(kNoContextMenu);

    TView *tv = fStatus_EC->GetCanvas()->GetView();
    if (tv == NULL)
    {
        TView3D *tv3d = new TView3D(1, rmin, rmax);
        fStatus_EC->GetCanvas()->SetView(tv3d);
    }
    CreateLEDs();
    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : CreateLEDs
 *
 * Description : Create the status LEDs and accompanying text
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
void CreateTab::CreateLEDs()
{
    SET_DEBUG_STACK;
    Double_t Y_increment = LABEL_SIZE/1.0;
    Double_t y;
    fDestPassedLED    = new TEllipse(-0.2, -0.80, 0.03, 0.03);
    fEnteredCircleLED = new TEllipse(-0.2, -0.87, 0.03, 0.03);
    fWopLED           = new TEllipse(-0.2, -0.94, 0.03, 0.03);
    fOnlineLED        = new TEllipse(0.95, -0.80, 0.03, 0.03);
    fOfflineLED       = new TEllipse(0.95, -0.87, 0.03, 0.03);
    fTurnLED          = new TEllipse(0.95, -0.94, 0.03, 0.03);

    fDestPassedLED->SetFillColor(kRed);
    fEnteredCircleLED->SetFillColor(kRed);
    fWopLED->SetFillColor(kRed);
    fOnlineLED->SetFillColor(kRed);
    fOfflineLED->SetFillColor(kRed);
    fTurnLED->SetFillColor(kRed);

    fDestPassedLED->SetLineStyle(0);
    fEnteredCircleLED->SetLineStyle(0);
    fWopLED->SetLineStyle(0);
    fOnlineLED->SetLineStyle(0);
    fOfflineLED->SetLineStyle(0);
    fTurnLED->SetLineStyle(0);

    fDestPassedLED->Draw();
    fEnteredCircleLED->Draw();
    fWopLED->Draw();
    fOnlineLED->Draw();
    fOfflineLED->Draw();
    fTurnLED->Draw();
    y = -0.80;
    fDestPassedText    = new TText(-0.9, y, "Dest Passed");
    y -= Y_increment;
    fEnteredCircleText = new TText(-0.9, y, "Entered Circle");
    y -= Y_increment;
    fWopText           = new TText(-0.9, y, "WOP");

    y = -0.80;
    fOnlineText        = new TText(0.2 , y, "Online");
    y -= Y_increment;
    fOfflineText       = new TText(0.2 , y, "Offline");
    y -= Y_increment;
    fTurnModeText      = new TText(0.2 , y, "Turn");

    fDestPassedText->SetTextColor(kWhite);
    fEnteredCircleText->SetTextColor(kWhite);
    fWopText->SetTextColor(kWhite);
    fOnlineText->SetTextColor(kWhite);
    fOfflineText->SetTextColor(kWhite);
    fTurnModeText->SetTextColor(kWhite);

    fDestPassedText->SetTextSize(LABEL_SIZE);
    fEnteredCircleText->SetTextSize(LABEL_SIZE);
    fWopText->SetTextSize(LABEL_SIZE);
    fOnlineText->SetTextSize(LABEL_SIZE);
    fOfflineText->SetTextSize(LABEL_SIZE);
    fTurnModeText->SetTextSize(LABEL_SIZE);

    fDestPassedText->Draw();
    fEnteredCircleText->Draw();
    fWopText->Draw();
    fOnlineText->Draw();
    fOfflineText->Draw();
    fTurnModeText->Draw();
    SET_DEBUG_STACK;
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
void CreateTab::AddTab1()
{
    SET_DEBUG_STACK;
    TGCompositeFrame *tf = this->AddTab("GPS Status");
    CreateGraphicsFrame(tf);  
    SET_DEBUG_STACK;
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
void CreateTab::DoTab(Int_t id)
{
    SET_DEBUG_STACK;
    printf("Tab item %d activated\n", id);
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

void CreateTab::FillPlots(Double_t X, Double_t Y, Double_t Z)
{
    SET_DEBUG_STACK;
    Double_t *x, *y;
    UInt_t   N, N2;
    TCanvas  *c;

    x = fXY->GetX();
    y = fXY->GetY();
    N = fXY->GetN();
    x[fXYPointer] = X;
    y[fXYPointer] = Y;
    fXYPointer++;
    if (fXYPointer >= N)
    {
	Double_t *temp = new Double_t[N];
	fXYPointer = N/4;
	N2             = N-fXYPointer;
	// Move X over.
	memcpy( temp, x, N*sizeof(Double_t));
        memcpy( x, &temp[N2], fXYPointer*sizeof(Double_t));
    }
    fHeight->Set(fXYPointer);
    c = fPosPlot->GetCanvas();
    c->Modified();
    c->Update();

    x = fHeight->GetX();
    y = fHeight->GetY();
    N = fHeight->GetN();
    if (fHeightPointer> 0)
    {
	x[fHeightPointer] = x[fHeightPointer-1];
    }
    y[fHeightPointer] = Z;

    fHeightPointer++;
    if (fHeightPointer >= N)
    {
	Double_t *temp = new Double_t[N];
	fHeightPointer = N/4;
	N2             = N-fHeightPointer;
	// Move X over.
	memcpy( temp, x, N*sizeof(Double_t));
        memcpy( x, &temp[N2], fHeightPointer*sizeof(Double_t));
    }
    fHeight->Set(fHeightPointer);
    c = fHeight_EC->GetCanvas();
    c->Modified();
    c->Update();
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
void CreateTab::AddTab2()
{
    SET_DEBUG_STACK;
    time_t now;

    time (&now);

    TGCompositeFrame *tf = this->AddTab("GPS Height");
    TGLayoutHints    *tl =  new TGLayoutHints(kLHintsTop | kLHintsExpandX,
					      1, 1, 1, 1);

    TGHorizontalFrame* graphicsFrame = new TGHorizontalFrame(tf, 10, 10);
    tf->AddFrame( graphicsFrame, tl);
    fHeight_EC = new TRootEmbeddedCanvas("HEIGHT_EC", graphicsFrame, 400, 400);
    graphicsFrame->AddFrame( fHeight_EC, tl);
    TCanvas *c1 = fHeight_EC->GetCanvas();
    c1->SetGrid();
    c1->SetBorderMode(0);
    c1->cd();

    fHeight        = new TGraph(100);
    fHeight->SetPoint( 0, 0.0, 0.0);
    fHeightPointer = 0;
    fHeight->Set(fHeightPointer);

    fHeight->Set(1);
    fHeight->SetMarkerColor(2);
    fHeight->SetMarkerStyle(20);
    fHeight->Draw("ALP");

    SET_DEBUG_STACK;
}
