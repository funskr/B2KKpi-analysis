#include "TStyle.h"
#include "TCanvas.h"
#include "/home/belle2/yuanmk/useful/boot.h"

void draw_hist(TH2D* hist,string file, string branch){
    //if flag is true, the histogram is signal
    TChain* chain = new TChain(branch.c_str());

    chain -> Add(file.c_str());
    //set variables
    Double_t ContProb;
    chain->SetBranchAddress("ContProb", &ContProb);
    Int_t flag_candidate;
    chain->SetBranchAddress("flag_candidate", &flag_candidate);

    Double_t InvM_KpPim;
    chain->SetBranchAddress("InvM_KpPim", &InvM_KpPim);
    Double_t InvM_KmPip;
    chain->SetBranchAddress("InvM_KmPip", &InvM_KmPip);

    Double_t InvM_KpKm;
    chain->SetBranchAddress("InvM_KpKm", &InvM_KpKm);

    //draw on hist
    Int_t nentries = chain->GetEntries();
    for(Int_t i=0; i < nentries; i++){
        chain->GetEntry(i);
        if(flag_candidate==0) continue;
        if(ContProb>0.4) continue;

        if(InvM_KpKm > 1.8484 && InvM_KpKm < 1.8806) continue;

        if(InvM_KmPip > 1.8408 && InvM_KmPip < 1.8875) continue;
        if(InvM_KpPim > 1.8408 && InvM_KpPim < 1.8875) continue;

        hist->Fill(InvM_KmPip, InvM_KpPim);
    }
}

void dis2_mkpi(){
    setStyle();

    TCanvas *mbc=new TCanvas("mbc","mbc",720,600);
    mbc->SetFillColor(0);
    mbc->SetFrameFillColor(0);
    gStyle->SetPadColor(0);
    gStyle->SetCanvasColor(0);
    gStyle->SetOptStat(0);
    mbc->SetLeftMargin(0.15);
    mbc->SetTopMargin(0.1);
    mbc->SetRightMargin(0.1);
    mbc->SetBottomMargin(0.15);
   
    // set ytitle
    Double_t xmin=1.5, xmax=2.5;  Double_t xbins=300;
    //Double_t ymin=1.5, ymax=2.5;  Double_t ybins=100;

    TH2D *h_BBbar=new TH2D("h_BBbar", "h_BBbar", xbins, xmin, xmax, xbins, xmin, xmax);
    h_BBbar->SetMarkerColor(kBlue);
    draw_hist(h_BBbar,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/mixed_sample.root", "B0");
    draw_hist(h_BBbar,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/charged_sample.root", "B0");

    mbc->cd();
    //gPad->SetLogy();
    h_BBbar->Draw("");
    //hsigMC->Draw("HIST");
    //hmisid->Draw("HIST same");

    //hsigMC->Scale(1/hsigMC->GetEntries());
    //hmisid->Scale(hsigMC->GetEntries()/hmisid->GetEntries());

    h_BBbar->GetXaxis()->SetTitle("M_{#pi^{+}K^{-}}/(GeV/c^{2})");
    h_BBbar->GetYaxis()->SetTitle("M_{#pi^{-}K^{+}}/(GeV/c^{2})");
    h_BBbar->GetXaxis()->SetTitleOffset(1.0);
    h_BBbar->GetYaxis()->SetTitleOffset(1.0);

    //h_sigMC->SetMaximum(10000000);
    
    //TLegend *legend = new TLegend(0.20,0.75,0.35,0.89);
    //legend->AddEntry("hsigMC","Signal MC","l");
    //legend->AddEntry("hmisid","misconstructed","lf");

    //legend->SetBorderSize(0);
    //legend->SetFillColor(0);
    //legend->Draw();

    //mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/bkg_bbar/pdf/dis2_mkpi.pdf");
    //mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/bkg_bbar/png/dis2_mkpi.png");

    //mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/bkg_bbar/pdf/dis2_mkpi_mkkveto.pdf");
    //mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/bkg_bbar/png/dis2_mkpi_mkkveto.png");

    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/bkg_bbar/pdf/dis2_mkpi_mkkveto_mkpiveto.pdf");
    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/bkg_bbar/png/dis2_mkpi_mkkveto_mkpiveto.png");

    //cout<<"truely identified:"<<hsigMC->GetEntries()<<endl;
    cout<<"Total events:"<<h_BBbar->GetEntries()<<endl;
}