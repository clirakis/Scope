void myimage()
{
    const Int_t Width  = 25;
    const Int_t Height = 25;
    const char *Background = "#FFFFFFFF";

    TCanvas *Calvin = new TCanvas("Calvin","X Y Z",10,10,800,800);
    TPad    *Hobbes = new TPad("Hobbes","Silly",0.01,0.01,0.99,0.99,10);
    Hobbes->Draw();
    Hobbes->SetGrid();
    Hobbes->cd();

    TASImage *Image = new TASImage( Width, Height);
    Image->FillRectangle( Background, 0,0, Image->GetWidth(), 
			 Image->GetHeight());

    Image->Draw();

    Image->DrawCircle( 0, 0, 10);
}
