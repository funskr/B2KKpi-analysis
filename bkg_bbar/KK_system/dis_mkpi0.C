#include "TStyle.h"
#include "TCanvas.h"
#include "/home/belle2/yuanmk/useful/boot.h"

void draw_hist(TH1D* hist,string file, string branch){
    //if flag is true, the histogram is signal
    TChain* chain = new TChain(branch.c_str());

    chain -> Add(file.c_str());
    //set variables
    Double_t ContProb;
    chain->SetBranchAddress("ContProb", &ContProb);
    Int_t flag_candidate;
    chain->SetBranchAddress("flag_candidate", &flag_candidate);

    Double_t InvM_KpPi0;
    chain->SetBranchAddress("InvM_KpPi0", &InvM_KpPi0);
    Double_t InvM_KmPi0;
    chain->SetBranchAddress("InvM_KmPi0", &InvM_KmPi0);

    //draw on hist
    Double_t Kp_E_s = 0; Double_t pi0_E_s = 0;
    Double_t E_tot = 0; Double_t px_tot = 0; Double_t py_tot = 0; Double_t pz_tot = 0;
    const Double_t mass_K = 0.493677;
    Int_t nentries = chain->GetEntries();
    //cout<<"Km_E:pi0_E_s"<<endl;
    for(Int_t i=0; i < nentries; i++){
        chain->GetEntry(i);
        if(flag_candidate==0) continue;
        if(ContProb>0.4) continue;
        
        hist->Fill(InvM_KpPi0);
        hist->Fill(InvM_KmPi0);
    }
}

void dis_mkpi0(){
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
    Double_t xmin=0, xmax=5;  Double_t xbins=100;
    TH1::SetDefaultSumw2(1);
    TString a("Events/"); char b[100];  sprintf(b, "(%.2f)",(xmax-xmin)/xbins);
    TString ytitle = a + b;


    TH1D *hsigMC=new TH1D("hsigMC", "hsigMC", xbins, xmin, xmax);
    hsigMC->SetLineColor(kBlue);
    hsigMC->SetLineWidth(2);
    draw_hist(hsigMC,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/sigMC_sample.root", "B0");

    TH1D *hbbbar=new TH1D("hbbbar", "hbbbar", xbins, xmin, xmax);
    hbbbar->SetLineColor(kRed);
    hbbbar->SetLineWidth(2);
    draw_hist(hbbbar,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/mixed_sample.root","B0");
    draw_hist(hbbbar,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/charged_sample.root","B0");

    mbc->cd();
    //gPad->SetLogy();
    hsigMC->Draw("HIST");
    hbbbar->Draw("HIST same");
    //hmisid->Draw("HIST same");

    //hsigMC->Scale(1/hsigMC->GetEntries());
    Float_t rightmax = 3*hbbbar->GetMaximum();
    if(rightmax <= 0) rightmax = 1;
    Float_t scale = hsigMC->GetMaximum()/rightmax;
    //hsigMC->Scale(1/hsigMC->GetEntries());
    hbbbar->Scale(scale);

    hsigMC->GetXaxis()->SetTitle("M_{K^{#pm}#pi^{0}}/(GeV/c^{2})");
    hsigMC->GetYaxis()->SetTitle(ytitle);
    hsigMC->GetXaxis()->SetTitleOffset(0.90);
    hsigMC->GetYaxis()->SetTitleOffset(1.20);

    auto axis_x = hsigMC->GetXaxis(); 
    TGaxis* axis_right = new TGaxis(axis_x->GetXmax(),0,
                                axis_x->GetXmax(), hsigMC->GetMaximum()*1.05,
                                0,rightmax,510,"+L");
    axis_right->SetLineColor(kRed);
    axis_right->SetLabelColor(kRed);
    axis_right->Draw();
    
    TLegend *legend = new TLegend(0.20,0.75,0.35,0.89);
    legend->AddEntry("hsigMC","Signal MC","l");
    legend->AddEntry("hbbbar","BBbar MC","lf");

    legend->SetBorderSize(0);
    legend->SetFillColor(0);
    legend->Draw();

    TArrow *xy1 = new TArrow(0.112, 0, 0.112, hsigMC->GetMaximum()*0.3,0.015,"<");
    xy1->SetLineColor(kBlue);xy1->SetLineWidth(2);
    //xy1->Draw();

    TArrow *xy2 = new TArrow(0.145, 0, 0.145, hsigMC->GetMaximum()*0.3,0.015,"<");
    xy2->SetLineColor(kBlue);xy2->SetLineWidth(2);
    //xy2->Draw();

    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/bkg_bbar/pdf/dis_mkpi0.pdf");
    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/bkg_bbar/png/dis_mkpi0.png");
}
