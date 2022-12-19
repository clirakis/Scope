/// \file
/// \ingroup tutorial_http
///  This program creates and fills one and two dimensional histogram
///  Macro used to demonstrate usage of custom HTML page in custom.htm
///  One can use plain JavaScript to assign different actions with HTML buttons
///
/// \macro_code
///
/// \author Sergey Linev

#include <TH1.h>
#include <TH2.h>
#include <THttpServer.h>
#include <TRandom3.h>

void mine(void)
{

    TCanvas* fCanvas1 = new TCanvas("Canvas1","GPS changes",200,10,700,500);
    fCanvas1->SetFillColor(42);
    fCanvas1->GetFrame()->SetFillColor(21);
    fCanvas1->GetFrame()->SetBorderSize(6);
    fCanvas1->GetFrame()->SetBorderMode(-1);
    fCanvas1->Divide(2,2);
    fCanvas1->SetGrid(1);
    fCanvas1->SetGrid(2);

    // Create two histograms
    TH1F *hpx = new TH1F("hpx","This is the px distribution",100,-4,4);
    TH2F *hpxpy = new TH2F("hpxpy","py vs px",40,-4,4,40,-4,4);

    // http server with port 8080, use jobname as top-folder name
    THttpServer* serv = new THttpServer("http:8082");

    // use custom web page as default
    serv->SetDefaultPage("mine.htm");

    // Fill histograms randomly
    TRandom3 random;
    Float_t px, py;

    // press Ctrl-C to stop macro
    while (!gSystem->ProcessEvents()) 
    {
	random.Rannor(px,py);
	hpx->Fill(px);
	hpxpy->Fill(px,py);

	fCanvas1->cd(1);
	hpx->Draw();
	fCanvas1->cd(2);
	hpxpy->Draw();

	fCanvas1->Modified();
	fCanvas1->Update();

	sleep (1);
    }
}
