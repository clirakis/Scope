
void cell3()
{
    gROOT->Reset();


    TCanvas *Calvin = new TCanvas("Calvin","X Y Z",10,10,800,800);
    TPad    *Hobbes = new TPad("Hobbes","Silly",0.01,0.01,0.99,0.99,10);
    Hobbes->Draw();
    Hobbes->cd();

    TASImage *cells = CreateCells(25,25);
    cells->PutPixel ( 5, 5, "#000000");
    UpdateCells( cells);
}
TASImage *CreateCells(UInt_t NRows, UInt_t NCol)
{
    char         *Background   = "#FFFFFF";
    Double_t     scalex, scaley;
    Double_t     offsetx, offsety;

    cells = new TASImage(NCol, NRows);
    cells->FillRectangle( Background, 0,0, cells->GetWidth(), 
			  cells->GetHeight());
    cells->Draw("X");
    cells->SetConstRatio(kFALSE); 

    scalex  = 0.9;
    scaley  = 0.9;
    offsetx = (1.0-scalex) *  gPad->GetUxmax();
    offsety = (1.0-scaley) *  gPad->GetUymax();

    // draw an axis on the right side
    TGaxis *Yaxis = new TGaxis(gPad->GetUxmax() - offsetx,
			       scaley*gPad->GetUymin()+offsety,
			       gPad->GetUxmax() - offsetx,
			       scaley*gPad->GetUymax(),
			       0, NRows,510,"+L");
    Yaxis->SetLineColor(kRed);
    Yaxis->SetTextColor(kRed);
    Yaxis->Draw("G");

    // draw an axis on the right side
    TGaxis *Xaxis = new TGaxis(gPad->GetUxmin()+offsetx,
			       scaley*gPad->GetUymin() + offsety,
			       scalex*gPad->GetUxmax(),
			       scaley*gPad->GetUymin() + offsety,
			       0, NCol,510);
    Xaxis->SetLineColor(kRed);
    Xaxis->SetTextColor(kRed);

    Xaxis->Draw("G");
    cells->SetPaletteEnabled(kFALSE);

    printf("Cells created. %X\n", cells);
    return cells;

}
void UpdateCells(TASImage *cells)
{
    Int_t Row,Col, Index;
    Int_t W = cells->GetWidth();
    Int_t H = cells->GetHeight();

    TArrayD *d = cells->GetArray( W, H);

    for (Row = 0; Row<H; Row++)
    {
	for (Col = 0; Col<W; Col++)
	{
	    Index = Row * W + Col;
	    if ((*d)[Index] < 1.0)
	    {
		printf("Row: %d Col %d D %X\n", Row, Col, (*d)[Index]);
	    }
	}
    }
}
