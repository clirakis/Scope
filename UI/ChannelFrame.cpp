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
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include <TGComboBox.h>

/// Local Includes.
#include "debug.h"
#include "ChannelFrame.hh"
#include "DSA602.hh"
#include "Channel.hh"
#include "Module.hh"

/**
 ******************************************************************
 *
 * Function Name : ChannelFrame
 *
 * Description : Really, this is what is going on in a module. 
 * This is replicted per module. Encapsulating it this way makes it easy. 
 *
 * Inputs : 
 *    p - parent frame
 *    m - current module to display. 
 *
 * Returns : None
 *
 * Error Conditions : NONE
 *
 * Unit Tested on:
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
ChannelFrame::ChannelFrame(TGCompositeFrame* p, void *pChannel) : 
    TGHorizontalFrame(p, 100,300)
{
    SET_DEBUG_STACK;
    TGLabel* label;
    char s[64];
    /*
     * Embed all of the sub frames into a horizontial frame. 
     *
     * Which slot in the mainframe is this? 
     * Store pChannel in fpChannel for later reference. 
     */
    fpChannel = pChannel;
    Channel*  pCH = (Channel*) pChannel;

    // =================================================================
    sprintf( s, "Channel %d", pCH->Number());
    TGGroupFrame *Channel_gf = new TGGroupFrame( this, s, kHorizontalFrame);

    // Rows, Columns, Interval between frames, hints
    Channel_gf->SetLayoutManager(new TGMatrixLayout(Channel_gf, 
						    11, 2, 10, 2));
    // 1 *
    if (pCH->Applicable(Channel::kAMPOFFSET))
    {
	label = new TGLabel(Channel_gf, new TGHotString("Amplifer Offset:"));
	Channel_gf->AddFrame(label);
	fAMPoffset = new TGNumberEntry( Channel_gf, 0.0, 5, -1,
					TGNumberFormat::kNESReal, 
					TGNumberFormat::kNEAAnyNumber);
	fAMPoffset->Connect("ValueSet(Long_t)", "ChannelFrame", this, 
			    "SetAMPoffset(long)");
	Channel_gf->AddFrame(fAMPoffset);
    }

    // 2 FIXME - Don't know upper and lower limits. 
    if (pCH->Applicable(Channel::kBW))
    {
	label = new TGLabel(Channel_gf, new TGHotString("Bandwidth:"));
	Channel_gf->AddFrame(label);
	fBW = new TGNumberEntry( Channel_gf, 0.0, 5, -1,
				 TGNumberFormat::kNESReal, 
				 TGNumberFormat::kNEAAnyNumber);
	fBW->Connect("ValueSet(Long_t)", "ChannelFrame", this, "SetBW(long)");
	Channel_gf->AddFrame(fBW);
    }
    // 3 FIXME - Don't know upper and lower limits.
    if (pCH->Applicable(Channel::kBWLO))
    {
	label = new TGLabel(Channel_gf, new TGHotString("Bandwidth Lo:"));
	Channel_gf->AddFrame(label);
	fBWLo = new TGNumberEntry( Channel_gf, 0.0, 5, -1,
				   TGNumberFormat::kNESReal, 
				   TGNumberFormat::kNEAAnyNumber);
	fBWLo->Connect("ValueSet(Long_t)", "ChannelFrame", this,
		       "SetBWLo(long)");
	Channel_gf->AddFrame(fBWLo);
    }
    // 4 FIXME - Don't know upper and lower limits.
    if (pCH->Applicable(Channel::kBWHI))
    {
	label = new TGLabel(Channel_gf, new TGHotString("Bandwidth Hi:"));
	Channel_gf->AddFrame(label);
	fBWHi = new TGNumberEntry( Channel_gf, 0.0, 5, -1,
				   TGNumberFormat::kNESReal, 
				   TGNumberFormat::kNEAAnyNumber);
	fBWHi->Connect("ValueSet(Long_t)", "ChannelFrame", this, 
		       "SetBWHi(long)");
	Channel_gf->AddFrame(fBWHi);
    }
    // 5 OK
    if (pCH->Applicable(Channel::kCCOUPLING))
    {
	label = new TGLabel(Channel_gf, new TGHotString("Coupling:"));
	Channel_gf->AddFrame(label);
	fCOUpling = new TGComboBox( Channel_gf);
	fCOUpling->AddEntry("AC",  kCOUPLING_AC);
	fCOUpling->AddEntry("DC",  kCOUPLING_DC);
	fCOUpling->AddEntry("OFF", kCOUPLING_OFF);
	fCOUpling->AddEntry("VC",  kCOUPLING_VC);
	fCOUpling->Resize(100,20);
	fCOUpling->Connect("Selected(int)", "ChannelFrame", this, 
			   "SetCOUpling(int)");
	Channel_gf->AddFrame(fCOUpling);
	//fCOUpling->Select((Int_t)ch->Coupling(),kFALSE); FIXME
    }
    // 6 OK
    if (pCH->Applicable(Channel::kIMPEDANCE))
    {
	label = new TGLabel(Channel_gf, new TGHotString("Impedance:"));
	Channel_gf->AddFrame(label);
	fIMPedance = new TGComboBox( Channel_gf);
	fIMPedance->AddEntry("50 OHM", kFIFTY);
	fIMPedance->AddEntry("1M OHM", kONE_MEG);
	fIMPedance->AddEntry("1G OHM", kONE_GIG);
	fIMPedance->Resize(100,20);
	fIMPedance->Connect("Selected(int)", "ChannelFrame", this, 
			    "SetIMPedance(int)");
	Channel_gf->AddFrame(fIMPedance);
    }
    // 7 EH OK, could change this to be a text box later
    if (pCH->Applicable(Channel::kPROBE))
    {
	label = new TGLabel(Channel_gf, new TGHotString("Probe:"));
	Channel_gf->AddFrame(label);
	fPRObe = new TGLabel(Channel_gf, "NONE");
	Channel_gf->AddFrame(fPRObe);
    }
    // 8 
    if (pCH->Applicable(Channel::kPROTECT))
    {
	label = new TGLabel(Channel_gf, new TGHotString("     "));
	Channel_gf->AddFrame(label);
	fPROTect = new TGCheckButton(Channel_gf,"Protect");
	Channel_gf->AddFrame(fPROTect);
	fPROTect->Connect("Clicked()", "ChannelFrame", this, "SetPROTect()");
    }
    // 9 
    if (pCH->Applicable(Channel::kSENSITIVITY))
    {
	label = new TGLabel(Channel_gf, new TGHotString("Sensitivity:"));
	Channel_gf->AddFrame(label);
	fSENsitivity = new TGNumberEntry( Channel_gf, 0.0, 5, -1,
					  TGNumberFormat::kNESReal, 
					  TGNumberFormat::kNEAAnyNumber);
	fSENsitivity->Connect("ValueSet(Long_t)", "ChannelFrame", this, 
			      "SetSENsitivity(long)");
	Channel_gf->AddFrame(fSENsitivity);
    }
    // 10
    if (pCH->Applicable(Channel::kUNITS))
    {
	label = new TGLabel(Channel_gf, new TGHotString("UNITS:"));
	Channel_gf->AddFrame(label);
	fUNIts = new TGLabel(Channel_gf, new TGHotString("NONE"));
	Channel_gf->AddFrame(fUNIts);
    }
    // 11 
    if (pCH->Applicable(Channel::kVCOFFSET))
    {
	label = new TGLabel(Channel_gf, new TGHotString("VC offset:"));
	Channel_gf->AddFrame(label);
	fVCOffset = new TGNumberEntry( Channel_gf, 0.0, 5, -1,
				       TGNumberFormat::kNESReal, 
				       TGNumberFormat::kNEAAnyNumber);
	fVCOffset->Connect("ValueSet(Long_t)", "ChannelFrame", this, 
			   "SetVCOffset(long)");
	Channel_gf->AddFrame(fVCOffset);
    }
    // 12
    if (pCH->Applicable(Channel::kOFFSET))
    {
	label = new TGLabel(Channel_gf, new TGHotString("Offset:"));
	Channel_gf->AddFrame(label);
	fOFFset = new TGNumberEntry( Channel_gf, 0.0, 5, -1,
				     TGNumberFormat::kNESReal, 
				     TGNumberFormat::kNEAAnyNumber);
	fOFFset->Connect("ValueSet(Long_t)", "ChannelFrame", this, 
			 "SetOFFset(long)");
	Channel_gf->AddFrame(fOFFset);
    }
    // Finalize Channel group frame. 
    AddFrame(Channel_gf, new TGLayoutHints(kLHintsLeft, 2, 2, 2, 2));

    if (pCH->IsDifferential())
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
	fMNSCoupling->AddEntry("AC",  kCOUPLING_AC);
	fMNSCoupling->AddEntry("DC",  kCOUPLING_DC);
	fMNSCoupling->AddEntry("OFF", kCOUPLING_OFF);
	fMNSCoupling->AddEntry("VC",  kCOUPLING_VC);
	fMNSCoupling->Resize(100,20);
	fMNSCoupling->Connect("Selected(int)", "ChannelFrame", this, 
			      "SetMNSCoupling(int)");
	Minus_gf->AddFrame(fMNSCoupling);
	//fMNSCoupling->Select((Int_t)c,kFALSE); FIXME

	// 2
	label = new TGLabel(Minus_gf, new TGHotString("Offset:"));
	Minus_gf->AddFrame(label);
	fMNSOffset = new TGNumberEntry( Minus_gf, 0.0, 5, -1,
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
	fMNSProbe = new TGLabel(Minus_gf, "NONE");
	Minus_gf->AddFrame(fMNSProbe);
	// Finalize Minus group frame. 
	AddFrame(Minus_gf, new TGLayoutHints(kLHintsLeft, 2, 2, 2, 2));
    }
    if(pCH->IsDifferential())
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
	fPLSCoupling->AddEntry("AC",  kCOUPLING_AC);
	fPLSCoupling->AddEntry("DC",  kCOUPLING_DC);
	fPLSCoupling->AddEntry("OFF", kCOUPLING_OFF);
	fPLSCoupling->AddEntry("VC",  kCOUPLING_VC);
	fPLSCoupling->Resize(100,20);
	fPLSCoupling->Connect("Selected(int)", "ChannelFrame", this, 
			      "SetPLSCoupling(int)");
	Plus_gf->AddFrame(fPLSCoupling);
	// fPLSCoupling->Select((Int_t)ch->PLS_Coupling(),kFALSE); FIXME

	// 2
	label = new TGLabel(Plus_gf, new TGHotString("Offset:"));
	Plus_gf->AddFrame(label);
	fPLSOffset = new TGNumberEntry( Plus_gf, 0.0, 5, -1,
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
	fPLSProbe = new TGLabel(Plus_gf, "NONE");
	Plus_gf->AddFrame(fPLSProbe);
	// Finalize Plus group frame. 
	AddFrame(Plus_gf, new TGLayoutHints(kLHintsLeft, 2, 2, 2, 2));
    }

    TGLayoutHints *L = new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX,
                                         5, 5, 5, 5);
    Resize();
    p->AddFrame( this, L);

    // Finally, fill in the values. 
    Update();
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : ChannelFrame Destructor.
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
ChannelFrame::~ChannelFrame(void)
{
    SET_DEBUG_STACK;

}
/**
 ******************************************************************
 *
 * Function Name : Update
 *
 * Description : Update all the values in the Frame. 
 *
 * Inputs : None
 *
 * Returns : None
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
void ChannelFrame::Update(void)
{
    SET_DEBUG_STACK;
    Channel*  pCH = (Channel*) fpChannel;

    pCH->Update();

    // 1
    if (pCH->Applicable(Channel::kAMPOFFSET))
    {
	fAMPoffset->SetNumber(pCH->AmpOffset());
    }

    // 2
    if (pCH->Applicable(Channel::kBW))
    {
	fBW->SetNumber(pCH->Bandwidth());
    }
    // 3 
    if (pCH->Applicable(Channel::kBWLO))
    {
	fBWLo->SetNumber(pCH->Bandwidth_Lo());
    }
    // 4
    if (pCH->Applicable(Channel::kBWHI))
    {
	fBWHi->SetNumber(pCH->Bandwidth_HI());
    }
    // 5
    if (pCH->Applicable(Channel::kCCOUPLING))
    {
	fCOUpling->Select((Int_t)pCH->Coupling(false),kFALSE);
    }
    // 6
    if (pCH->Applicable(Channel::kIMPEDANCE))
    {
	fIMPedance->Select((Int_t)pCH->Impedance(false),kFALSE);
    }
    // 7 - SKIP label only.
    if (pCH->Applicable(Channel::kPROTECT))
    {
	if (pCH->Protect(false))
	{
	    fPROTect->SetState(kButtonUp, kFALSE);
	}
	else
	{
	    fPROTect->SetState(kButtonDown, kFALSE);
	}
    }
    // 9
    if (pCH->Applicable(Channel::kSENSITIVITY))
    {
	fSENsitivity->SetNumber(pCH->Sensitivity(false));
    }
    // 10 -> Skip label only
    if (pCH->Applicable(Channel::kUNITS))
    {
	fUNIts->SetText(pCH->Units(false).c_str());
    }
    // 11
    if (pCH->Applicable(Channel::kVCOFFSET))
    {
	fVCOffset->SetNumber(pCH->VCOffset(false));
    }
    // 12
    if (pCH->Applicable(Channel::kOFFSET))
    {
	fOFFset->SetNumber(pCH->Offset(false));
    }

    if (pCH->IsDifferential())
    {
	/* 
	 *=========================================================
	 * Only used if differential. 
	 * Minus frame 
	 *=========================================================
	 */

	// 1
	fMNSCoupling->Select((Int_t)pCH->MNS_Coupling(false),kFALSE);

	// 2
	fMNSOffset->SetNumber( pCH->Offset(false));

	// 3 - Text only
    }
    if(pCH->IsDifferential())
    {
	// =========================================================
	// Only used if differential. 
	// Plus frame ==============================================
	fPLSCoupling->Select((Int_t)pCH->PLS_Coupling(false),kFALSE);

	// 2
	fPLSOffset->SetNumber(pCH->Offset(false));

	// 3 - String only
	//fPLSProbe
    }
    SET_DEBUG_STACK;
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
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : SetAMPoffset
 *
 * Description : Set the value to subtract from the differential input
 *               on the 11A33. page 79
 *
 * Inputs : val - Ignored
 *
 * Returns : None
 *
 * Error Conditions : GPIB error
 *
 * Unit Tested on: 28-Dec-22
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void ChannelFrame::SetAMPoffset(long val)
{ 
    SET_DEBUG_STACK;
    Channel*  pCH = (Channel*) fpChannel;
    pCH->SendCommand(Channel::kAMPOFFSET, fAMPoffset->GetNumber());
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : SetBW
 *
 * Description : Set the Bandwidth of the input channel. (As aggregagte.)
 * FIXME: Need to determine the limits
 *
 * Inputs : val - not used.
 *
 * Returns : None
 *
 * Error Conditions : GPIB error
 *
 * Unit Tested on:
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void ChannelFrame::SetBW(long val)
{ 
    SET_DEBUG_STACK;
    Channel*  pCH = (Channel*) fpChannel;
    pCH->SendCommand(Channel::kBW, fBW->GetNumber());
    SET_DEBUG_STACK
}
/**
 ******************************************************************
 *
 * Function Name : SetBWHi
 *
 * Description : Set the high bandwith. 
 *
 * Inputs : val not used. 
 *
 * Returns : None
 *
 * Error Conditions : GPIB Error
 *
 * Unit Tested on:
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void ChannelFrame::SetBWHi(long val)
{    
    SET_DEBUG_STACK;
    Channel*  pCH = (Channel*) fpChannel;
    pCH->SendCommand(Channel::kBWHI, fBWHi->GetNumber());
    SET_DEBUG_STACK
}
/**
 ******************************************************************
 *
 * Function Name : SetBWLo
 *
 * Description : Set the lower edge of the bandwidth
 *
 * Inputs : Val - not used. 
 *
 * Returns : None
 *
 * Error Conditions : GPIB error
 *
 * Unit Tested on:
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void ChannelFrame::SetBWLo(long val)
{
    SET_DEBUG_STACK;
    Channel*  pCH = (Channel*) fpChannel;
    pCH->SendCommand(Channel::kBWHI, fBWHi->GetNumber());
    SET_DEBUG_STACK
}
/**
 ******************************************************************
 *
 * Function Name : Set Coupling
 *
 * Description : set DC/AC/OFF
 *
 * Inputs : val - one of the enum COUPLING values in the drop down list. 
 *
 * Returns : None
 *
 * Error Conditions : GPIB error
 *
 * Unit Tested on: 28-Dec-22
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void ChannelFrame::SetCOUpling(int val)
{
    SET_DEBUG_STACK;
    Channel*  pCH = (Channel*) fpChannel;
    pCH->Coupling((COUPLING) val);
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : SetIMPedance
 *
 * Description : callback for the selection dialog for seting
 *               the input impedance for a channel. (50, 1M, 100M)
 *
 *               The value supplied by the callback is a pointer to 
 *               the enum. 
 *
 * Inputs : Val - See the enum IMPEDANCE in DSA602_Types.hh
 *
 * Returns : None
 *
 * Error Conditions : NONE
 *
 * Unit Tested on: 28-Dec-22
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void ChannelFrame::SetIMPedance(int val)
{ 
    SET_DEBUG_STACK;
    Channel*  pCH = (Channel*) fpChannel;
    pCH->SendCommand( Channel::kIMPEDANCE, (IMPEDANCE)val);
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : SetMNSCoupling
 *
 * Description : Set the minus input coupling (on a differential amplifier) 
 *               coupling value. 
 *
 * Inputs : value which emulates the order on enum COUPLING 
 *
 * Returns : None
 *
 * Error Conditions : GPIB ERROR
 *
 * Unit Tested on:
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void ChannelFrame::SetMNSCoupling(int val)
{ 
    SET_DEBUG_STACK;
    Channel*  pCH = (Channel*) fpChannel;
    pCH->SendCommand(Channel::kMNSCOUPLING, (COUPLING) val);
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : SetMNSOffset
 *
 * Description : Set the offset to the Minus input channel 
 *               on a differential amplifier. 
 *
 * Inputs : val - ignored
 *
 * Returns : None
 *
 * Error Conditions : GPIB error
 *
 * Unit Tested on:
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void ChannelFrame::SetMNSOffset(long val)
{ 
    SET_DEBUG_STACK;
    Channel*  pCH = (Channel*) fpChannel;
    pCH->SendCommand(Channel::kMNSOFFSET, fMNSOffset->GetNumber());
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : SetOFFset
 *
 * Description : Set the trace offset for the channel
 *
 * Inputs : val ignored. 
 *
 * Returns : None
 *
 * Error Conditions : GPIB error
 *
 * Unit Tested on:
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void ChannelFrame::SetOFFset(long val)
{ 
    SET_DEBUG_STACK;
    Channel*  pCH = (Channel*) fpChannel;
    pCH->SendCommand(Channel::kOFFSET, fOFFset->GetNumber());
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : SetPLSCoupling
 *
 * Description : Set the Coupling to the Plus input channel 
 *               on a differential amplifier. 
 *
 * Inputs : val - value which emulates the order on enum COUPLING
 *
 * Returns : None
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
void ChannelFrame::SetPLSCoupling(int val)
{ 
    SET_DEBUG_STACK;
    Channel*  pCH = (Channel*) fpChannel;
    pCH->SendCommand(Channel::kPLSCOUPLING, (COUPLING) val);
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : SetPLSOffset
 *
 * Description : Set the offset to the Plus input channel 
 *               on a differential amplifier.
 *
 * Inputs : val - ignored
 *
 * Returns : None
 *
 * Error Conditions : GPIB Error
 *
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void ChannelFrame::SetPLSOffset(long val)
{
    SET_DEBUG_STACK;
    Channel*  pCH = (Channel*) fpChannel;
    pCH->SendCommand(Channel::kPLSOFFSET, fPLSOffset->GetNumber());
    SET_DEBUG_STACK;
}
/**
 ******************************************************************
 *
 * Function Name : SetPROTect
 *
 * Description : Protects the setting on the differential amplifer
 *
 * Inputs : None
 *
 * Returns : None
 *
 * Error Conditions : GPIB Error
 *
 * Unit Tested on: 28-Dec-22
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void ChannelFrame::SetPROTect(void)
{ 
    SET_DEBUG_STACK;
    Channel*  pCH = (Channel*) fpChannel;
    pCH->SendCommand(Channel::kPROTECT, (bool)fPROTect->GetState());
    SET_DEBUG_STACK;

}
/**
 ******************************************************************
 *
 * Function Name : SetSENsitivity
 *
 * Description : Set the sensitivity numbers
 *
 * Inputs : val - ignored
 *
 * Returns : None
 *
 * Error Conditions : GPIB error
 *
 * Unit Tested on:
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void ChannelFrame::SetSENsitivity(long val)
{ 
    SET_DEBUG_STACK;
    Channel*  pCH = (Channel*) fpChannel;
    pCH->SendCommand(Channel::kSENSITIVITY, fSENsitivity->GetNumber());
    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : SetVCOffset
 *
 * Description : Differential amplifer compensation offset
 *
 * Inputs : val - ignored
 *
 * Returns : None
 *
 * Error Conditions : GPIB error
 *
 * Unit Tested on:
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void ChannelFrame::SetVCOffset(long val)
{ 
    SET_DEBUG_STACK;
    Channel*  pCH = (Channel*) fpChannel;
    pCH->SendCommand(Channel::kVCOFFSET, fVCOffset->GetNumber());
    SET_DEBUG_STACK;
}
