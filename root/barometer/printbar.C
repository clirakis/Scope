void printbar( const char *filename)
{
  //***************************************************************** 
  // 
  // Modified   By  Reason
  // --------   --  ------
  // 03-Jul-01  CBL Analyze barometer data
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
  // Copyright (C) BBN Technologies  2000
  //***************************************************************** 
    // Make a drawing surface. 
    TCanvas *Calvin = new TCanvas("Calvin","X Y Z",10,10,800,800);
    TPad    *Hobbes = new TPad("Hobbes","Silly",0.02,0.02,0.98,0.98,10);
    Hobbes->Draw();
    Hobbes->SetGrid();
    Hobbes->cd();

    if (filename == NULL)
    {
	printf("No filename provided\n");
    }
    TGraph *tg = new TGraph( filename);
    tg->Draw("ALP");

    TH1F *hbs = tg->GetHistogram();
    if (hbs != NULL)
    {
	hbs->SetXTitle("Time (mm-dd hh:mm)");
	hbs->SetYTitle("mBar");
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
	// 
	// The %F defines the offset, I typically use the epoch.
	// Why I have to plug in +6 hours is beyond me.
	hbs->GetXaxis()->SetTimeFormat("%b %d %H:%M %F1970-01-01 06:00:00");
    }
    Hobbes->SaveAs("baro.jpg");
}
