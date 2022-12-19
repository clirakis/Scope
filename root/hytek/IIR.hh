/**
 ******************************************************************
 *
 * Module Name : IIR.hh
 *
 * Author/Date : C.B. Lirakis / 08-Oct-08
 *
 * Description : Implimentation of IIR filter
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
 *******************************************************************
 */
#ifndef __IIR_h_
#define __IIR_h_
class TVectorD;
class IIR {
public:
    IIR(Int_t ntaps);
    IIR(const char *file);
    ~IIR();
    void AddCoeficient ( Int_t tap, Double_t A, Double_t B=0.0);
    void Apply( Double_t *x);
    void FillTest();
    Double_t AddSample(Double_t x);

private:
    //bool ReadMatlabDescription(const char *file);
    Int_t     fNTaps;
    TVectorD *fA;
    TVectorD *fB;
    TVectorD *fWork, *fSamples;
    Int_t     fP;
};
#endif
