{
  gSystem->Load("libGeom");
  TGeoManager *geom = new TGeoManager( "Example", "Example geometry" );
  TGeoMaterial *mat = new TGeoMaterial( "Vacuum", 0, 0, 0 );
  TGeoMedium *med1 = new TGeoMedium( "Medium 1", 0, mat );
  mat = new TGeoMaterial( "Al", 26.98, 13, 2.7 );
  TGeoMedium *med2 = new TGeoMedium( "S", 1, mat );
  TGeoVolume *top = geom->MakeBox( "TOP", med1, 800., 800., 800. );
  geom->SetTopVolume( top );
  TGeoVolume *sphere=new TGeoVolume("SPH", new TGeoSphere( "sphere", 90, 100, 0, 45), med2);
  top->AddNode(sphere,1);
  geom->CloseGeometry();
  top->Draw();
  if( gPad )
    gPad->x3d( "OPENGL" );
}

