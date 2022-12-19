{
    TCanvas *Hobbes = new TCanvas("Dist","A test",5,5,1200,600);
    Hobbes->cd();
    TPad    *Calvin = new TPad("Calvin","Silly",0.01,0.01,0.99,0.99,10);
    Calvin->Draw();
    Calvin->cd();
    Calvin->SetGrid();
#if 0
    TF1 *diode = new TF1("Diode", 
			 "exp(TMath::Qe()*x/([1]*TMath::K()*[0]))-1.0", 
			 0.0, 2.0);
    diode->SetParameter(0, 273.15); // Temperature 
    diode->SetParameter(1, 1.0); // emmision coefficient

    diode->Draw();
#else
    TF2 *diode = new TF2("Diode", 
			 "exp(TMath::Qe()*x/([0]*TMath::K()*y))-1.0", 
			 0.0, 0.8, 273.15, 310.0);
    diode->SetParameter(0, 1.0); // emmision coefficient
    diode->Draw();
    
#endif
    TH1 *hbs = diode->GetHistogram();
    if (hbs)
    {
	hbs->SetYTitle("Temperature (K)");
	hbs->SetXTitle("Voltage");
	hbs->SetLabelSize(0.03,"X");
	hbs->SetLabelSize(0.03,"Y");
    }
}
