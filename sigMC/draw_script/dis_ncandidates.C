#include "TStyle.h"
#include "TCanvas.h"
#include "/home/belle2/yuanmk/useful/boot.h"

void draw_hist(TH1D* hist,string address, string file, string variable, bool flag=true){
    //if flag is true, the histogram is signal
    TChain* chain = new TChain(file.c_str());

    chain -> Add(address.c_str());
    //set variables
    Int_t var;
    chain->SetBranchAddress(variable.c_str(), &var);

    Int_t __candidate__;
    chain->SetBranchAddress("__candidate__", &__candidate__);
    Double_t dr;
    chain->SetBranchAddress("dr", &dr);
    Double_t dz;
    chain->SetBranchAddress("dz", &dz);


    //draw on hist
    Int_t nentries = chain->GetEntries();
    for(Int_t i=0; i < nentries; i++){
        chain->GetEntry(i);
        if(dr>0.04) continue;
        if(fabs(dz)>0.2) continue;
        if(__candidate__ != 0) continue;

        //if(var==1)
        hist->Fill(var);
    }
}

void dis_ncandidates(){
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
    Double_t xmin=0, xmax=12;  Double_t xbins=100;
    TH1::SetDefaultSumw2(1);
    TString a("Events/"); char b[100];  sprintf(b, "(%.4f)",(xmax-xmin)/xbins);
    TString ytitle = a + b;


    TH1D *hsigMC=new TH1D("hsigMC", "hsigMC", xbins, xmin, xmax);
    hsigMC->SetLineColor(kBlue);
    hsigMC->SetLineWidth(2);
    draw_hist(hsigMC,"/home/belle2/yuanmk/data/B2KKpi/sigMC/root/B2KKpi_all.root","B0","__ncandidates__", true);

    mbc->cd();
    gPad->SetLogy();
    hsigMC->Draw("HIST");
    //hsigMC->Draw("HIST");
    //hmisid->Draw("HIST same");

    //hsigMC->Scale(1/hsigMC->GetEntries());
    //hmisid->Scale(hsigMC->GetEntries()/hmisid->GetEntries());

    hsigMC->GetXaxis()->SetTitle("ncandidates");
    hsigMC->GetYaxis()->SetTitle(ytitle);
    hsigMC->GetXaxis()->SetTitleOffset(0.90);
    hsigMC->GetYaxis()->SetTitleOffset(1.20);

    //h_sigMC->SetMaximum(10000000);
    
    TLegend *legend = new TLegend(0.60,0.60,0.88,0.84);
    legend->AddEntry("hsigMC","Signal MC","l");

    legend->SetBorderSize(0);
    legend->SetFillColor(0);
    legend->Draw();

    TArrow *xy1 = new TArrow(0.112, 0, 0.112, hsigMC->GetMaximum()*0.3,0.015,"<");
    xy1->SetLineColor(kBlue);xy1->SetLineWidth(2);
    //xy1->Draw();

    TArrow *xy2 = new TArrow(0.145, 0, 0.145, hsigMC->GetMaximum()*0.3,0.015,"<");
    xy2->SetLineColor(kBlue);xy2->SetLineWidth(2);
    //xy2->Draw();

    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/sigMC/plots/png/dis_ncandidates.png");
    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/sigMC/plots/pdf/dis_ncandidates.pdf");

    cout<<hsigMC->GetEntries()<<endl;
    cout<<hsigMC->GetMean()<<endl;
}