{
    gROOT->Reset();

    const UInt_t NRows = 1000;
    const UInt_t NCol  = 1000;
    const UInt_t size  = NRows * NCol;
    char *Clr = "#0000FF";

    Int_t Row,Col;
    UInt_t *scanline, Red, Green, Blue;
    Double_t scale, data[size];

    for (Row = 0; Row<NRows; Row++)
    {
	for(Col=0; Col< NCol; Col++)
	{
	    data[Col+Row*NCol] = 256 * Row;
	}
    }
    TASImage *tas = new TASImage("MyImage", data, NCol, NRows, NULL);
    tas->Draw();

    tas->SetConstRatio(kFALSE); 
    scale = 1.0;
    // draw an axis on the right side
    TGaxis *Yaxis = new TGaxis(scale*gPad->GetUxmax(),gPad->GetUymin(),
			       scale*gPad->GetUxmax(), gPad->GetUymax(),
			       0, 1000,510,"+L");
    Yaxis->SetLineColor(kRed);
    Yaxis->SetTextColor(kRed);
    Yaxis->Draw("G");

    // draw an axis on the right side
    TGaxis *Xaxis = new TGaxis(gPad->GetUxmin(),gPad->GetUymin(),
			       scale*gPad->GetUxmax(), gPad->GetUymin(),
			       0, 1000,510);
    Xaxis->SetLineColor(kRed);
    Xaxis->SetTextColor(kRed);
    Xaxis->Draw("G");

    tas->Draw();


    tas->SetPaletteEnabled(kFALSE);
}
