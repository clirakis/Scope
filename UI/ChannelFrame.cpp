/**
 ******************************************************************
 *
 * Module Name : ChannelDlg.cpp
 *
 * Author/Date : C. B. Lirakis/07-Feb-14
 *
 * Description : A dialog displaying waveform parameters
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
#include <iostream>
using namespace std;
#include <string>

/// Root Includes
#include <TROOT.h>
#include <TGClient.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGTextEntry.h>
#include <TGNumberEntry.h>
#include <TGComboBox.h>
#include <TGMsgBox.h>

/// Local Includes.
#include "debug.h"
#include "ChannelFrame.hh"
#include "DSA602.hh"
#include "Channel.hh"

/**
 ******************************************************************
 *
 * Function Name : 
 *
 * Description : 
 *
 * Inputs : None
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
ChannelFrame::ChannelFrame(TGCompositeFrame* p) : TGVerticalFrame(p, 100,300)
{
    //char     tmp[32];
    TGLabel* label;
//    ChannelGPIB* ch = (ChannelGPIB *) c;
//    fChannelGPIB = c;

    TGGroupFrame *Channel_gf = new TGGroupFrame( this, "Channel data", 
						 kHorizontalFrame);
#if 0
    // Rows, Columns, Interval between frames, hints
    Channel_gf->SetLayoutManager(new TGMatrixLayout(Channel_gf, 
						    ch->NBaseCommands(), 
						    2, 10, 2));

    // 1
    if (ch->Active(Channel::AMPOFFSET))
    {
	label = new TGLabel(Channel_gf, new TGHotString("Amplifer Offset:"));
	Channel_gf->AddFrame(label);
	fAMPoffset = new TGNumberEntry( Channel_gf, ch->AmpOffset(), 5, -1,
					TGNumberFormat::kNESReal, 
					TGNumberFormat::kNEAAnyNumber);
	fAMPoffset->Connect("ValueSet(Long_t)", "ChannelFrame", this, 
			    "SetAMPoffset(long)");
	Channel_gf->AddFrame(fAMPoffset);
    }

    // 2
    if (ch->Active(Channel::BW))
    {
	label = new TGLabel(Channel_gf, new TGHotString("Bandwidth:"));
	Channel_gf->AddFrame(label);
	fBW = new TGNumberEntry( Channel_gf, ch->Bandwidth(), 5, -1,
				 TGNumberFormat::kNESReal, 
				 TGNumberFormat::kNEAAnyNumber);
	fBW->Connect("ValueSet(Long_t)", "ChannelFrame", this, "SetBW(long)");
	Channel_gf->AddFrame(fBW);
    }
    // 3
    if (ch->Active(Channel::BWLO))
    {
	label = new TGLabel(Channel_gf, new TGHotString("Bandwidth Lo:"));
	Channel_gf->AddFrame(label);
	fBWLo = new TGNumberEntry( Channel_gf, ch->Bandwidth_Lo(), 5, -1,
				   TGNumberFormat::kNESReal, 
				   TGNumberFormat::kNEAAnyNumber);
	fBWLo->Connect("ValueSet(Long_t)", "ChannelFrame", this,
		       "SetBWLo(long)");
	Channel_gf->AddFrame(fBWLo);
    }
    // 4
    if (ch->Active(Channel::BWHI))
    {
	label = new TGLabel(Channel_gf, new TGHotString("Bandwidth Hi:"));
	Channel_gf->AddFrame(label);
	fBWHi = new TGNumberEntry( Channel_gf, ch->Bandwidth_Hi(), 5, -1,
				   TGNumberFormat::kNESReal, 
				   TGNumberFormat::kNEAAnyNumber);
	fBWHi->Connect("ValueSet(Long_t)", "ChannelFrame", this, 
		       "SetBWHi(long)");
	Channel_gf->AddFrame(fBWHi);
    }
    // 5
    if (ch->Active(Channel::CCOUPLING))
    {
	label = new TGLabel(Channel_gf, new TGHotString("Coupling:"));
	Channel_gf->AddFrame(label);
	fCOUpling = new TGComboBox( Channel_gf);
	fCOUpling->AddEntry("AC",  COUPLING_AC);
	fCOUpling->AddEntry("DC",  COUPLING_DC);
	fCOUpling->AddEntry("OFF", COUPLING_OFF);
	fCOUpling->AddEntry("VC",  COUPLING_VC);
	fCOUpling->Resize(100,20);
	fCOUpling->Connect("Selected(int)", "ChannelFrame", this, 
			   "SetCOUpling(int)");
	Channel_gf->AddFrame(fCOUpling);
	fCOUpling->Select((Int_t)ch->Coupling(),kFALSE);
    }
    // 6
    if (ch->Active(Channel::CIMPEDANCE))
    {
	label = new TGLabel(Channel_gf, new TGHotString("Impedence:"));
	Channel_gf->AddFrame(label);
	fIMPedence = new TGComboBox( Channel_gf);
	fIMPedence->AddEntry("50  OHM", FIFTY);
	fIMPedence->AddEntry("1 M OHM", ONE_MEG);
	fIMPedence->AddEntry("1 G OHM", ONE_GIG);
	fIMPedence->Resize(100,20);
	fIMPedence->Connect("Selected(int)", "ChannelFrame", this, 
			    "SetIMPedence(int)");
	Channel_gf->AddFrame(fIMPedence);
	fIMPedence->Select((Int_t)ch->Impedence(),kFALSE);
    }
    // 7
    if (ch->Active(Channel::PROBE))
    {
	label = new TGLabel(Channel_gf, new TGHotString("Probe:"));
	Channel_gf->AddFrame(label);
	fPRObe = new TGLabel(Channel_gf, ch->Probe());
	Channel_gf->AddFrame(fPRObe);
    }
    // 8
    if (ch->Active(Channel::PROTECT))
    {
	label = new TGLabel(Channel_gf, new TGHotString("     "));
	Channel_gf->AddFrame(label);
	fPROTect = new TGCheckButton(Channel_gf,"Protect");
	Channel_gf->AddFrame(fPROTect);
	if (ch->Protect())
	{
	    fPROTect->SetState(kButtonUp, kFALSE);
	}
	else
	{
	    fPROTect->SetState(kButtonDown, kFALSE);
	}
	fPROTect->Connect("Clicked()", "ChannelFrame", this, 
			  "SetPROTect()");
    }
    // 9
    if (ch->Active(Channel::SENSITIVITY))
    {
	label = new TGLabel(Channel_gf, new TGHotString("Sensitivity:"));
	Channel_gf->AddFrame(label);
	fSENsitivity = new TGNumberEntry( Channel_gf, ch->Sensitivity(), 5, -1,
					  TGNumberFormat::kNESReal, 
					  TGNumberFormat::kNEAAnyNumber);
	fSENsitivity->Connect("ValueSet(Long_t)", "ChannelFrame", this, 
			      "SetSENsitivity(long)");
	Channel_gf->AddFrame(fSENsitivity);
    }
    // 10
    if (ch->Active(Channel::UNITS))
    {
	label = new TGLabel(Channel_gf, new TGHotString("UNITS:"));
	Channel_gf->AddFrame(label);
	fUNIts = new TGLabel(Channel_gf, new TGHotString(ch->Units()));
	Channel_gf->AddFrame(fUNIts);
    }
    // 11
    if (ch->Active(Channel::VCOFFSET))
    {
	label = new TGLabel(Channel_gf, new TGHotString("VC offset:"));
	Channel_gf->AddFrame(label);
	fVCOffset = new TGNumberEntry( Channel_gf, ch->VCOffset(), 5, -1,
				       TGNumberFormat::kNESReal, 
				       TGNumberFormat::kNEAAnyNumber);
	fVCOffset->Connect("ValueSet(Long_t)", "ChannelFrame", this, 
			   "SetVCOffset(long)");
	Channel_gf->AddFrame(fVCOffset);
    }
    // 12
    if (ch->Active(Channel::OFFSET))
    {
	label = new TGLabel(Channel_gf, new TGHotString("Offset:"));
	Channel_gf->AddFrame(label);
	fOFFset = new TGNumberEntry( Channel_gf, ch->Offset(), 5, -1,
				     TGNumberFormat::kNESReal, 
				     TGNumberFormat::kNEAAnyNumber);
	fOFFset->Connect("ValueSet(Long_t)", "ChannelFrame", this, 
			 "SetOFFset(long)");
	Channel_gf->AddFrame(fOFFset);
    }
    // Finalize Channel group frame. 
    AddFrame(Channel_gf, new TGLayoutHints(kLHintsLeft, 2, 2, 2, 2));

    if (ch->NMinusCommands() > 0)
    {
	// =========================================================
	// Only used if differential. 
	// Minus frame ==============================================
	TGGroupFrame *Minus_gf = new TGGroupFrame( this, "Minus", 
						   kHorizontalFrame);
	// Rows, Columns, Interval between frames, hints
	Minus_gf->SetLayoutManager(new TGMatrixLayout(Minus_gf, 3, 2, 10, 2));
	// 1
	label = new TGLabel(Minus_gf, new TGHotString("Coupling:"));
	Minus_gf->AddFrame(label);
	fMNSCoupling = new TGComboBox( Minus_gf);
	fMNSCoupling->AddEntry("AC",  COUPLING_AC);
	fMNSCoupling->AddEntry("DC",  COUPLING_DC);
	fMNSCoupling->AddEntry("OFF", COUPLING_OFF);
	fMNSCoupling->AddEntry("VC",  COUPLING_VC);
	fMNSCoupling->Resize(100,20);
	fMNSCoupling->Connect("Selected(int)", "ChannelFrame", this, 
			      "SetMNSCoupling(int)");
	Minus_gf->AddFrame(fMNSCoupling);
	fMNSCoupling->Select((Int_t)ch->MNS_Coupling(),kFALSE);

	// 2
	label = new TGLabel(Minus_gf, new TGHotString("Offset:"));
	Minus_gf->AddFrame(label);
	fMNSOffset = new TGNumberEntry( Minus_gf, ch->MNS_Offset(), 5, -1,
					TGNumberFormat::kNESReal, 
					TGNumberFormat::kNEAAnyNumber,
					TGNumberFormat::kNELLimitMinMax,
					-1.0, 1.0);
	fMNSOffset->Connect("ValueSet(Long_t)", "ChannelFrame", this, 
			    "SetMNSOffset(long)");
	Minus_gf->AddFrame(fMNSOffset);

	// 3
	label = new TGLabel(Minus_gf, new TGHotString("Probe:"));
	Minus_gf->AddFrame(label);
	fMNSProbe = new TGLabel(Minus_gf, ch->MNS_Probe());
	Minus_gf->AddFrame(fMNSProbe);
	// Finalize Minus group frame. 
	AddFrame(Minus_gf, new TGLayoutHints(kLHintsLeft, 2, 2, 2, 2));
    }
    if(ch->NPlusCommands()>0)
    {
	// =========================================================
	// Only used if differential. 
	// Plus frame ==============================================
	TGGroupFrame *Plus_gf = new TGGroupFrame( this, "Plus", 
						  kHorizontalFrame);
	// Rows, Columns, Interval between frames, hints
	Plus_gf->SetLayoutManager(new TGMatrixLayout(Plus_gf, 3, 2, 10, 2));
	// 1
	label = new TGLabel(Plus_gf, new TGHotString("Coupling:"));
	Plus_gf->AddFrame(label);
	fPLSCoupling = new TGComboBox( Plus_gf);
	fPLSCoupling->AddEntry("AC",  COUPLING_AC);
	fPLSCoupling->AddEntry("DC",  COUPLING_DC);
	fPLSCoupling->AddEntry("OFF", COUPLING_OFF);
	fPLSCoupling->AddEntry("VC",  COUPLING_VC);
	fPLSCoupling->Resize(100,20);
	fPLSCoupling->Connect("Selected(int)", "ChannelFrame", this, 
			      "SetPLSCoupling(int)");
	Plus_gf->AddFrame(fPLSCoupling);
	fPLSCoupling->Select((Int_t)ch->PLS_Coupling(),kFALSE);

	// 2
	label = new TGLabel(Plus_gf, new TGHotString("Offset:"));
	Plus_gf->AddFrame(label);
	fPLSOffset = new TGNumberEntry( Plus_gf, ch->PLS_Offset(), 5, -1,
					TGNumberFormat::kNESReal, 
					TGNumberFormat::kNEAAnyNumber,
					TGNumberFormat::kNELLimitMinMax,
					-1.0, 1.0);
	fPLSOffset->Connect("ValueSet(Long_t)", "ChannelFrame", this, 
			    "SetPLSOffset(long)");
	Plus_gf->AddFrame(fPLSOffset);

	// 3
	label = new TGLabel(Plus_gf, new TGHotString("Probe:"));
	Plus_gf->AddFrame(label);
	fPLSProbe = new TGLabel(Plus_gf, ch->PLS_Probe());
	Plus_gf->AddFrame(fPLSProbe);
	// Finalize Plus group frame. 
	AddFrame(Plus_gf, new TGLayoutHints(kLHintsLeft, 2, 2, 2, 2));
    }
#endif
    TGLayoutHints *L = new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX,
                                         5, 5, 5, 5);
    Resize();
    p->AddFrame( this, L);

}
/**
 ******************************************************************
 *
 * Function Name : 
 *
 * Description : 
 *
 * Inputs : None
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
ChannelFrame::~ChannelFrame()
{

}
/**
 ******************************************************************
 *
 * Function Name : 
 *
 * Description : 
 *
 * Inputs : None
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
void ChannelFrame::Update(void)
{
#if 0
    ChannelGPIB* ch = (ChannelGPIB *) fChannelGPIB;
    ch->Update();
    // 1
    if (ch->Active(Channel::AMPOFFSET))
    {
	fAMPoffset->SetNumber(ch->AmpOffset());
    }

    // 2
    if (ch->Active(Channel::BW))
    {
	fBW->SetNumber(ch->Bandwidth());
    }
    // 3
    if (ch->Active(Channel::BWLO))
    {
	fBWLo->SetNumber(ch->Bandwidth_Lo());
    }
    // 4
    if (ch->Active(Channel::BWHI))
    {
	fBWHi->SetNumber(ch->Bandwidth_Hi());
    }
    // 5
    if (ch->Active(Channel::CCOUPLING))
    {
	fCOUpling->Select((Int_t)ch->Coupling(),kFALSE);
    }
    // 6
    if (ch->Active(Channel::CIMPEDANCE))
    {
	fIMPedence->Select((Int_t)ch->Impedence(),kFALSE);
    }
    // 7 - SKIP label only.
    if (ch->Active(Channel::PROTECT))
    {
	if (ch->Protect())
	{
	    fPROTect->SetState(kButtonUp, kFALSE);
	}
	else
	{
	    fPROTect->SetState(kButtonDown, kFALSE);
	}
    }
    // 9
    if (ch->Active(Channel::SENSITIVITY))
    {
	fSENsitivity->SetNumber(ch->Sensitivity());
    }
    // 10 -> Skip label only
#if 0
    if (ch->Active(Channel::UNITS))
    {
	fUNIts->Settext(ch->Units()));
    }
#endif
    // 11
    if (ch->Active(Channel::VCOFFSET))
    {
	fVCOffset->SetNumber(ch->VCOffset());
    }
    // 12
    if (ch->Active(Channel::OFFSET))
    {
	fOFFset->SetNumber(ch->Offset());
    }

    if (ch->NMinusCommands() > 0)
    {
	// =========================================================
	// Only used if differential. 
	// Minus frame ==============================================

	// 1
	fMNSCoupling->Select((Int_t)ch->MNS_Coupling(),kFALSE);

	// 2
	fMNSOffset->SetNumber( ch->Offset());

	// 3 - Text only
    }
    if(ch->NPlusCommands()>0)
    {
	// =========================================================
	// Only used if differential. 
	// Plus frame ==============================================
	fPLSCoupling->Select((Int_t)ch->PLS_Coupling(),kFALSE);

	// 2
	fPLSOffset->SetNumber(ch->Offset());

	// 3 - String only
	//fPLSProbe
    }
#endif
}
/**
 ******************************************************************
 *
 * Function Name : 
 *
 * Description : 
 *
 * Inputs : None
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
void ChannelFrame::Apply(void)
{
}
void ChannelFrame::SetAMPoffset(long val)
{ 
#if 0
    ChannelGPIB* ch = (ChannelGPIB *) fChannelGPIB;
    ch->SendCommand(Channel::AMPOFFSET, fAMPoffset->GetNumber());
#endif
}
void ChannelFrame::SetBW(long val)
{ 
#if 0
    // NOT sure how this works. 
    cout<< __func__ << endl;
    ChannelGPIB* ch = (ChannelGPIB *) fChannelGPIB;
    ch->SendCommand(Channel::BW, fBW->GetNumber());
#endif
}
void ChannelFrame::SetBWHi(long val){ cout<< __func__ << endl;}
void ChannelFrame::SetBWLo(long val){ cout<< __func__ << endl;}
void ChannelFrame::SetCOUpling(int val)
{
#if 0
    cout<< __func__ << " " << val << endl;
    ChannelGPIB* ch = (ChannelGPIB *) fChannelGPIB;
    ch->SendCommand(Channel::CCOUPLING, (COUPLING) val);
#endif
}
void ChannelFrame::SetIMPedence(int val)
{ 
#if 0
    ChannelGPIB* ch = (ChannelGPIB *) fChannelGPIB;
    ch->SendCommand(Channel::CIMPEDANCE, (IMPEDANCE) val);
#endif
}
void ChannelFrame::SetMNSCoupling(int val)
{ 
#if 0
    ChannelGPIB* ch = (ChannelGPIB *) fChannelGPIB;
    ch->SendCommand(Channel::MNSCOUPLING, (COUPLING) val);
#endif
}
void ChannelFrame::SetMNSOffset(long val)
{ 
#if 0
    ChannelGPIB* ch = (ChannelGPIB *) fChannelGPIB;
    ch->SendCommand(Channel::MNSOFFSET, fMNSOffset->GetNumber());
#endif
}
void ChannelFrame::SetOFFset(long val)
{ 
    cout<< __func__ << endl;
#if 0
    ChannelGPIB* ch = (ChannelGPIB *) fChannelGPIB;
    ch->SendCommand(Channel::OFFSET, fOFFset->GetNumber());
#endif
}
void ChannelFrame::SetPLSCoupling(int val)
{ 
#if 0
    ChannelGPIB* ch = (ChannelGPIB *) fChannelGPIB;
    ch->SendCommand(Channel::PLSCOUPLING, (COUPLING) val);
#endif
}
void ChannelFrame::SetPLSOffset(long val)
{
#if 0
    ChannelGPIB* ch = (ChannelGPIB *) fChannelGPIB;
    ch->SendCommand(Channel::PLSOFFSET, fPLSOffset->GetNumber());
#endif
}
void ChannelFrame::SetPROTect()
{ 
#if 0
    ChannelGPIB* ch = (ChannelGPIB *) fChannelGPIB;
    ch->SendCommand(Channel::PROTECT, (bool)fPROTect->GetState());
#endif
}
void ChannelFrame::SetSENsitivity(long val)
{ 
#if 0
    ChannelGPIB* ch = (ChannelGPIB *) fChannelGPIB;
    cout<< __func__ << " " << fSENsitivity->GetNumber() << endl;
    ch->SendCommand(Channel::SENSITIVITY, fSENsitivity->GetNumber());
#endif
}

void ChannelFrame::SetVCOffset(long val)
{ 
    cout<< __func__ << endl;
#if 0
    ChannelGPIB* ch = (ChannelGPIB *) fChannelGPIB;
    ch->SendCommand(Channel::VCOFFSET, fVCOffset->GetNumber());
#endif
}
