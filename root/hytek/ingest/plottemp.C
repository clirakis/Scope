void plottemp( const char *filename)
{
  //***************************************************************** 
  // 
  // Modified   By  Reason
  // --------   --  ------
  // 21-Sep-08  CBL Analyze temperature data
  //
  // RCS header info.
  // ----------------
  // $RCSfile: plotbar.C,v $
  // $Author: clirakis $
  // $Date: 2004/01/01 16:21:52 $
  // $Locker: clirakis $
  // $Name:  $
  // $Revision: 1.1 $
  //
  // $Log: plotbar.C,v $
  // Revision 1.1  2004/01/01 16:21:52  clirakis
  // Initial revision
  //
  // Revision 1.1  2003/08/08 23:32:10  clirakis
  // Initial revision
  //
  // Revision 1.2  2000/01/25 13:57:21  clirakis
  // Added log message to header.
  //
  //
  //***************************************************************** 
    // Make a drawing surface. 
    TCanvas *Calvin = new TCanvas("Calvin","Temperature",10,10,800,800);
    TPad    *Hobbes = new TPad("Hobbes","Silly",0.01,0.01,0.99,0.99,10);
    Hobbes->Draw();
    Hobbes->SetGrid();
    Hobbes->cd();
    Double_t *X, *Y;
    Int_t N;

    if (filename == NULL)
    {
	printf("No filename provided\n");
	return;
    }
    TFile *tf = new TFile(filename);

    //    DEGF->Draw("CH0:N","","goff");
    DEGF->Draw("FILTER:N","","goff");
    DEGF->SetMarkerStyle(20);
    N = DEGF->GetEntries();
    Y = DEGF->GetV1();
    X = DEGF->GetV2();

    TGraph *tg = new TGraph( N, X, Y);
    tg->Draw("ALP");

    TH1F *hbs = tg->GetHistogram();
    hbs = NULL;
    if (hbs != NULL)
    {
	hbs->SetXTitle("Time (mm-dd hh:mm)");
	hbs->SetYTitle("degC");
	hbs->SetLabelSize(0.02,"X");
	hbs->SetLabelSize(0.02,"Y");
	// Sets time on the X axis
	hbs->GetXaxis()->SetTimeDisplay(1);
	//
	// One can choose a different time format than the one chosen by default
	// The time format is the same as the one of the C strftime() function
	// It's a string containing the following formats :
	//    for date :
	//      %a abbreviated weekday name
	//      %b abbreviated month name
	//      %d day of the month (01-31)
	//      %m month (01-12)
	//      %y year without century
	//      %Y year with century
	//
	//    for time :
	//      %H hour (24-hour clock)
	//      %I hour (12-hour clock)
	//      %p local equivalent of AM or PM
	//      %M minute (00-59)
	//      %S seconds (00-61)
	//      %% %
	// The other characters are output as is.
	// 	// The %F defines the offset, I typically use the epoch.
	// Why I have to plug in +6 hours is beyond me.
	//hbs->GetXaxis()->SetTimeFormat("%b %d %H:%M %F1970-01-01 06:00:00");
    }
}
