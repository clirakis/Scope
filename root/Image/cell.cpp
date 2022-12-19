
#include "cell.hh"
void Cell::Cell(UInt_t NCol, UInt_t NRow)
{
    TCanvas *Calvin = new TCanvas("Calvin","X Y Z",10,10,800,800);
    TPad    *Hobbes = new TPad("Hobbes","Silly",0.01,0.01,0.99,0.99,10);
    Hobbes->Draw();
    Hobbes->cd();

    CreateCells(NCol, NRow);

    PutData( 12, 0, kTRUE);
    UpdateCells();
}
void Cell::CreateCells(UInt_t NCol, UInt_t NRow)
{
    char         *Background   = "#FFFFFF";

    cout << "Image " << NCol << " " << NRow << endl;

    cells = new TASImage(NCol, NRow);
    cells->FillRectangle( Background, 0,0, NCol, NRow);
    cells->Draw("X");
    cells->SetConstRatio(kFALSE); 
    cells->SetPaletteEnabled(kFALSE);
}
void Cell::UpdateCells()
{
    UInt_t X,Y;
    UInt_t W = cells->GetWidth();
    UInt_t H = cells->GetHeight();
    UInt_t rc;

    cout << " WH " << W << " " << H << endl;
    for (Y = 1; Y<H; Y++)
    {
	for (X = 0; X<W; X++)
	{
	    rc = SourceRule( X, Y-1);
	    //	    cout << " RC " << rc << endl;
	    PutData( X, Y, rc);
	}
    }
    cells->Paint();
}
UInt_t Cell::SourceRule( UInt_t X, UInt_t Y)
{
    Bool_t One, Two, Three; 
    UInt_t rv;

    //    cout << "Source Rule " << X << " " << Y << endl;

    One   = GetData( X-1, Y);
    Two   = GetData( X, Y);
    Three = GetData( X+1, Y);
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

void Cell::PutData( UInt_t X, UInt_t Y, Bool_t On)
{
    UInt_t Index, Xp, Yp;
    UInt_t W = cells->GetWidth();
    UInt_t H = cells->GetHeight();
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
Bool_t Cell::GetData( UInt_t X, UInt_t Y)
{
    UInt_t Index, Xp, Yp;
    UInt_t W = cells->GetWidth();
    UInt_t H = cells->GetHeight();
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
void Cell::DumpRow( UInt_t Y)
{
    UInt_t X, Index;
    UInt_t W = cells->GetWidth();
    UInt_t H = cells->GetHeight();
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
