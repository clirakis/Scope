/**
 ******************************************************************
 *
 * Module Name : TraceFrame.hh
 *
 * Author/Date : C.B. Lirakis/21-Dec-14
 *
 * Description : A dialog displaying the trace parameters.
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
#ifndef __TRACEFRAME_hh_
#define __TRACEFRAME_hh_
#  include <RQ_OBJECT.h>
#  include <TGFrame.h>

class TGLabel;
class TGComboBox;
class TGCheckButton;
class TGTextButton;

class TraceFrame : public TGVerticalFrame
{
    ClassDef(TraceFrame, 0);
public:
    /*!
     * A frame to hold the trace data, inputs are
     * parent frame
     * num - trace number. 
     */
    TraceFrame(TGCompositeFrame* parent, unsigned char trace_number );
    ~TraceFrame();

    void Update(void);  // Update from mainframe. 

    // slots
    void SetACCumulate(int val);
    void SetGRLocation(int val);
    void EditDescription(void);

private:
    UInt_t         fNumber;      // Trace number

    /*
     * All of the properties associated with the TRACE command. 
     */
    TGComboBox*    fACCumulate;
    TGLabel*       fACState;        // Read only. 
    TGTextButton*  fDescription;
    TGComboBox*    fGRLocation;
    TGLabel*       fWFMCalc;        // Read only
    TGLabel*       fXUNit;          // Read only
    TGLabel*       fYUNit;          // Read only

    /*
     * All of the properties assocated with the ADJTrace command. 
     */
};


#endif
