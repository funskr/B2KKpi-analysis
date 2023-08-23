#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
using namespace RooFit;
 
void fit_mkppim()
{
   // S e t u p   m o d e l
   // ---------------------
    // Declare variables x,mean,sigma with associated name, title, initial value and allowed range
    Double_t xmin = 1.80, xmax = 1.92;

    RooRealVar x("InvM_KpPim", "InvM_KpPim", xmin, xmax);
    RooRealVar mean("mean", "mean of gaussians", 1.86, 1.84, 1.89);
    RooRealVar sigma("sigma", "width of gaussians", 0.01, 0, 10);

    RooGaussian sig("sig", "Signal component 1", x, mean, sigma);

    // Sum the signal components into a composite signal pdf
    //RooRealVar sig1frac("sig1frac", "fraction of component 1 in signal", 0.8, 0., 1.);
    //RooAddPdf sig("sig", "Signal", RooArgList(sig1, sig2), sig1frac);
    //two gauss distribution convertion
   //RooFFTConvPdf sig("sig","gauss (x) gauss", x, sig1, sig2) ;


// Build  background PDF:
    //RooRealVar C1("C1", "C1", 300, 0, 1000);
    //RooRealVar C2("C2", "C2", -0.03, -10, 10);
    //RooAbsPdf *poly1 = new RooPolynomial("poly1", "poly1", B_M, RooArgList(C1,C2),0);
    //RooRealVar npoly1("npoly1", "#poly1 events", 0.80*nentries, 0.0, 1.2*nentries);

   RooRealVar c0("c0","coefficient #0", 0, -1, 1) ;
   RooRealVar c1("c1","coefficient #1", 0, -1, 1) ;
   //RooRealVar c2("c2","coefficient #2",-0.1, 0,5.) ; 
   RooChebychev bkg("bkg","background p.d.f.", x, RooArgSet(c0,c1)) ;
   //RooPolynomial bkg("bkg","background p.d.f.", mpi0,RooArgSet(c0,c1)) ;
 
   //RooRealVar nsig("nsig", "#signal events", 200, 0., 1E+6);
   //RooRealVar nbkg("nbkg", "#background events", 800, 0., 1000000);
   RooRealVar fsig("fsig", "#signal events", 0.9, 0., 1);
   RooAddPdf model("model", "model", RooArgList(sig, bkg), fsig);
 
   // Generate a dataset of 1000 events in x from gauss
   TChain* chain = new TChain("B0");
   chain->Add("/home/belle2/yuanmk/data/B2KKpi/sample/ana/mixed_sample.root");
   chain->Add("/home/belle2/yuanmk/data/B2KKpi/sample/ana/charged_sample.root");

   //std::unique_ptr<RooDataSet> data{gauss.generate(x, 10000)};
   RooDataSet data("data","dataset with Mpi0", chain, x);

   // Fit model to data
   // -----------------------------
   // Fit pdf to data
   model.fitTo(data, PrintLevel(-1));

   RooPlot *xframe = x.frame(Title("Fit to M_{K^{+}#pi^{-}}"));
   data.plotOn(xframe);
   model.plotOn(xframe);
   model.plotOn(xframe, Components(bkg), LineStyle(kDashed));
   model.plotOn(xframe, Components(sig), LineStyle(kDashed),LineColor(kRed));
   //model.plotOn(xframe, Components(RooArgSet(bkg,sig)), LineColor(kBlue), LineStyle(kDashed));
 
   // Print values of mean and sigma (that now reflect fitted values and errors)
   mean.Print();
   sigma.Print();
   model.Print("t");
 
   // Draw all frames on a canvas
   TCanvas *c = new TCanvas("c1", "c1", 800, 600); //c->Divide(2);
   c->cd();
   gPad->SetLeftMargin(0.15);
   xframe->GetYaxis()->SetTitleOffset(1.6);
   xframe->GetXaxis()->SetTitle("M_{K^{+}#pi^{-}}/(GeV/c^{2})");
   xframe->Draw();

    Double_t ymax = xframe->GetMaximum();
    //cout<<ymax<<endl;
    TPaveText *pt = new TPaveText(xmin+0.02*(xmax-xmin), ymax*0.6, xmin+0.3*(xmax-xmin), ymax*0.9);
    string str_mean = "mean = " + to_string(mean.getValV()) + " #pm " + to_string(mean.getError());
    string str_sigma = "sigma = " + to_string(sigma.getValV()) + " #pm " + to_string(sigma.getError());
    string str_c0 = "c0 = " + to_string(c0.getValV()) + " #pm " + to_string(c0.getError());
    string str_c1 = "c1 = " + to_string(c1.getValV()) + " #pm " + to_string(c1.getError());
    //cout<<str_mean<<endl;
    pt->AddText(str_mean.c_str());
    pt->AddText(str_sigma.c_str());
    pt->AddText(str_c0.c_str());
    pt->AddText(str_c1.c_str());
    pt->Draw();

    TArrow *xy1 = new TArrow(mean.getValV()-4.68*sigma.getValV(), 0, mean.getValV()-4.68*sigma.getValV(), xframe->GetMaximum()*0.2,0.015,"<");
    xy1->SetLineColor(kRed);xy1->SetLineWidth(2);
    xy1->Draw();

    TArrow *xy2 = new TArrow(mean.getValV()+4.68*sigma.getValV(), 0, mean.getValV()+4.68*sigma.getValV(), xframe->GetMaximum()*0.2,0.015,"<");
    xy2->SetLineColor(kRed);xy2->SetLineWidth(2);
    xy2->Draw();

    cout<<"mKppim constrain: "<<mean.getValV()-4.68*sigma.getValV()<<" < mKppim < "<<mean.getValV()+4.68*sigma.getValV()<<endl;

    c->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/bkg_bbar/pdf/mkppim_fit.pdf");
    c->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/bkg_bbar/png/mkppim_fit.png");
}