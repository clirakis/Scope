/**
 ******************************************************************
 *
 * Module Name : ChannelFrame.hh
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
#ifndef __CHANNELFRAME_hh_
#define __CHANNELFRAME_hh_
#  include <TGFrame.h>
#  include <RQ_OBJECT.h>


class TGLabel;
class TGNumberEntry;
class TGComboBox;
class TGCheckButton;

class ChannelFrame : public TGHorizontalFrame
{
    ClassDef(ChannelFrame, 0);
public:
    ChannelFrame(TGCompositeFrame*, void* pChannel);
    ~ChannelFrame();

    void Apply(void);   // Get any changed commands
    void Update(void);  // Update from mainframe. 

    // slots
    void SetAMPoffset(long val);
    void SetBW(long val);
    void SetBWHi(long val);
    void SetBWLo(long val);
    void SetCOUpling(int val);
    void SetIMPedance(int val);
    void SetMNSCoupling(int val);
    void SetMNSOffset(long val);
    void SetOFFset(long val);
    void SetPLSCoupling(int val);
    void SetPLSOffset(long val);
    void SetPROTect();
    void SetSENsitivity(long val);
    void SetVCOffset(long val);

private:

    void*          fpChannel; 
    // Pointers to type of display field.
    // Prefixes in use, MN - minus, PL - plus applies to 
    //     differential amplifiers.
    TGNumberEntry* fAMPoffset;
    TGNumberEntry* fBW;
    TGNumberEntry* fBWHi;
    TGNumberEntry* fBWLo;
    TGComboBox*    fCOUpling;
    TGComboBox*    fIMPedance;
    TGComboBox*    fMNSCoupling;
    TGNumberEntry* fMNSOffset;
    TGLabel*       fMNSProbe;
    TGNumberEntry* fOFFset;
    TGComboBox*    fPLSCoupling;
    TGNumberEntry* fPLSOffset;
    TGLabel*       fPLSProbe;
    TGLabel*       fPRObe;
    TGCheckButton* fPROTect;
    TGNumberEntry* fSENsitivity;
    TGLabel*       fUNIts;
    TGNumberEntry* fVCOffset;
};


#endif
