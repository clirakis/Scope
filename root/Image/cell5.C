
void cell5()
{
    gROOT->Reset();


    TCanvas *Calvin = new TCanvas("Calvin","X Y Z",10,10,800,800);
    TPad    *Hobbes = new TPad("Hobbes","Silly",0.01,0.01,0.99,0.99,10);
    Hobbes->Draw();
    Hobbes->cd();

    TASImage *cells = CreateCells(25,25);
    //    cells->PutPixel ( 5, 5, "#000000");
    PutData( 12, 0, kTRUE, cells);
    //GetData( 0, 0, cells);
    //    SourceRule( 0, 1, cells);
    //DumpRow(0, cells);
    UpdateCells( cells);
}
TASImage *CreateCells(UInt_t NRows, UInt_t NCol)
{
    char         *Background   = "#FFFFFF";

    cells = new TASImage(NCol, NRows);
    cells->FillRectangle( Background, 0,0, cells->GetWidth(), 
			  cells->GetHeight());
    cells->Draw("X");
    cells->SetConstRatio(kFALSE); 

    cells->SetPaletteEnabled(kFALSE);

    printf("Cells created. %X\n", cells);
    return cells;
}
void UpdateCells(TASImage *cells)
{
    Int_t X,Y;
    Int_t W = cells->GetWidth();
    Int_t H = cells->GetHeight();
    Int_t rc;

    for (Y = 1; Y<H; Y++)
    {
	for (X = 0; X<W; X++)
	{
	    rc = SourceRule( X, Y-1, cells);
	    //	    cout << " RC " << rc << endl;
	    PutData( X, Y, rc, cells);
	}
    }
    cells->Paint();
}
Int_t SourceRule( Int_t X, Int_t Y, TASImage *cells)
{
    Bool_t One, Two, Three; 
    Int_t rv;

    //    cout << "Source Rule " << X << " " << Y << endl;

    One   = GetData( X-1, Y, cells);
    Two   = GetData( X, Y, cells);
    Three = GetData( X+1, Y, cells);
#if 0
    if (One) cout << "One" << endl;
    if (Two) cout << "Two" << endl;
    if (Three) cout << "Three" << endl;
#endif
    rv = kFALSE;

    if (One && Two & Three)
    {
	rv = kFALSE;
	//cout << "ONE TWO THREE NO PAINT" << endl;
    }
    else if (One && Two)
    {
	//cout <<" ONE AND TWO PAINT" << endl;
	rv = kTRUE;
    }
    else if (One & Three)
    {
	//cout <<" ONE AND THREE PAINT" << endl;
	rv = kTRUE;
    }
    else if (Two & Three)
    {
	//cout <<" TWO AND THREE PAINT" << endl;
	rv = kTRUE;
    }
    else if (One | Two | Three)
    {
	//cout <<" SINGLE PAINT" << endl;
	rv = kTRUE;
    }
    else if ( !One & !Two & !Three)
    {
	//cout << " None " << endl;
	rv = kFALSE;
    }
    else 
    {
	cout << "Othter" << endl;
    }
    //cout << "RV " << rv << endl;

    return rv;
}

void PutData( Int_t X, Int_t Y, Bool_t On, TASImage *cells)
{
    Int_t Index, Xp, Yp;
    Int_t W = cells->GetWidth();
    Int_t H = cells->GetHeight();
    Xp = X;
    //Yp = H - Y - 1;
    Yp = Y;
    if (Xp >= W)
    {
	Xp = 0;
    }
    else if (Xp<0)
    {
	Xp = W-1;
    }
    if (Yp>H)
    {
	Yp = 0;
    }
    else if (Yp<0)
    {
	Yp = H-1;
    }
    //cout << " Put data " << X << " " << Y << " " << On << endl;
    if (On)
    {
	cells->PutPixel(Xp, Yp, "#000000");
    }
    else
    {
	cells->PutPixel(Xp, Yp, "#FFFFFF");
    }
}
Bool_t GetData( Int_t X, Int_t Y, TASImage *cells)
{
    Int_t Index, Xp, Yp;
    Int_t W = cells->GetWidth();
    Int_t H = cells->GetHeight();
    Bool_t rv;

    TArrayD *d = cells->GetArray( W, H);

    Xp = X;
    Yp = H - Y - 1;

    if (Xp>=W)
    {
	Xp = 0;
    }
    else if (Xp<0)
    {
	Xp = W-1;
    }
    if (Yp>=H)
    {
	Yp = 0;
    }
    else if (Yp<0)
    {
	Yp = H-1;
    }
    Index = Yp * W + Xp;
    rv = ((*d)[Index] == 0.0);
    //    cout << "Size " << d->GetSize() << endl;
#if 0
    cout << " Xp " << Xp
	 << " Yp " << Yp
	 << " Index " << Index << " " << (*d)[Index] << endl;
    if (rv)
    {
	cout << "Black " << X << " " << Y << endl;
    }
    else
    {
	cout << "White " << X << " " << Y << endl;
    }
#endif
    return rv;
}
void DumpRow( Int_t Y, TASImage *cells)
{
    Int_t X, Index;
    Int_t W = cells->GetWidth();
    Int_t H = cells->GetHeight();
    Bool_t rv;

    TArrayD *d = cells->GetArray( W, H);

    for(X=0;X<W;X++)
    {
	Index = Y * W + X;

	cout << (*d)[Index] << " ";
	//cout << (*d)[Y][X] << endl;
    }
    cout << endl;


}
