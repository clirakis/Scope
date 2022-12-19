
#ifndef GEO1_h_
#define GEO1_h_
#include <TTimer.h>
#include <TCanvas.h>
#include <TGeoMaterial.h>
#include <TGeoMedium.h>
#include <TGeoVolume.h>
#include <TGeoMatrix.h>

class Geo1 : public TTimer {
private:
    TGeoMaterial *mat;
    TGeoMedium *med;
    TGeoVolume *vol, *top;
    TGeoRotation r1;
    TGeoTranslation t1;
    TCanvas *c;
    Double_t theta;

public:
    Geo1(int stuff);
    virtual ~Geo1();
    virtual Bool_t Notify();
    void Paint(Option_t *option);
    void Animate();

    ClassDef(Geo1,0);
};
#endif
