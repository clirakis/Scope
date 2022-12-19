{
    TCanvas *Hobbes = new TCanvas("Dist","A test",5,5,1200,600);
    Hobbes->cd();
    TPad    *Calvin = new TPad("Calvin","Silly",0.02,0.02,0.99,0.99,10);
    Calvin->Draw();
    Calvin->cd();
    /*
     * Parameters:
     * [0] - I0
     * [1] - Phi0
     */
    Double_t Phi_0 = TMath::H()/(2.0*TMath::Qe());
    TF1 *Josephson = new TF1("Josephson", "[0]*sin(2.0*TMath::Pi()*x/[1])", 
			     0., 2.0*TMath::Pi());
    Josephson->SetParameter(0, 1.0);
    Josephson->SetParameter(1, Phi_0);
    Josephson->Draw();
}
