
#ifndef COMPASS_h_
#define COMPASS_h_
#include <TTimer.h>
#include <TCanvas.h>
#include <TGeoMaterial.h>
#include <TGeoMedium.h>
#include <TGeoVolume.h>
#include <TGeoMatrix.h>

class Compass : public TTimer {
private:
    TGeoMaterial *mat;
    TGeoMedium *med;
    TGeoVolume *vol, *top;
    TGeoRotation r1;
    TGeoTranslation t1;
    TCanvas *c;
    Double_t theta;
    TView *tv;

public:
    Compass(int stuff);
    virtual ~Compass();
    virtual Bool_t Notify();
    void Paint(Option_t *option);
    void Animate();

    ClassDef(Compass,0);
};
#endif
