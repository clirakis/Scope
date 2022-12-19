{
    TCanvas *Hobbes = new TCanvas("Dist","A test",5,5,600,600);
    TPad    *Calvin = new TPad("Calvin","Silly",0.02,0.02,0.99,0.99,10);
    Calvin->Range(-1.0, -1.0, 1.0,1.0);
    Calvin->Draw();
    Calvin->cd();
    CBLPolar *mine = new CBLPolar(1.0);
    mine->Draw();
}
