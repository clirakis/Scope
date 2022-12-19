


{
    TCanvas *Hobbes = new TCanvas("Dist","A test",5,5,600,600);
    TPad    *Calvin = new TPad("Calvin","Silly",0.02,0.02,0.99,0.99,10);
    Calvin->Range(-1.0, -1.0, 1.0,1.0);
    Calvin->Draw();
    Calvin->cd();
    // Cross hair at middle. 

    TArc *arc = new TArc(0.0, 0.0, 0.5, 0.0, 360.0);
    arc->Draw();
    TEllipse *ellipse = new TEllipse(0.0, 0.0, 0.6, 0.0, 0.0, 360.0, 0.0);
    ellipse->SetLineColor(2);
    ellipse->Draw();
}
