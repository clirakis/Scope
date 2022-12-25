/**
 ******************************************************************
 *
 * Module Name : MSLIST.hh
 *
 * Author/Date : C.B. Lirakis / 26-Jan-14
 *
 * Description : MSLIST - A list of all possible measurement values. 
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References : DSA602A Programming Reference Manual
 *
 * 
 *******************************************************************
 */
#ifndef __MSLIST_hh_
#define __MSLIST_hh_
#    include "TObject.h"
class TList;
class TString;

/*!
 * Keep track of all of the possible measurements we can make
 * off a waveform. Class MeasurementA stores on element of this list
 * and allows the user to manipulate the data in the list.
 */
class MeasurementA : public TObject {
public:
    MeasurementA(const char *l);
    ~MeasurementA();
    const char *Text(void) const;
    bool Match(const char *l);
    inline void     SetState(Bool_t s)     {fEnabled = s;};
    inline Bool_t   State() const          {return fEnabled;};
    inline Double_t Value(void)            {return fValue;};
    inline void     SetValue(double value) {fValue = value;};
    friend ostream& operator<<(ostream& output, const MeasurementA &n); 

private:
    TString       *fMeas;
    Double_t       fValue;
    Bool_t         fEnabled;
};

/*!
 * This is the actual List of the available measurements. 
 * It will initialize on calling. 
 */
class MSLIST {
public:
    MSLIST();
    ~MSLIST();
    friend ostream& operator<<(ostream& output, const MSLIST &n); 
    void  FillState(const char *val);
    void  FillValue(const char *val);
    MeasurementA* Find(const char *v);
    inline int Length() {return fMeasurements->GetSize();};
    inline TList* GetList() {return fMeasurements;};

    // Upto 6 available at any time.
private:
    void  Init(void);
    void  Clear(void);
    TList *fMeasurements;
};

#endif
