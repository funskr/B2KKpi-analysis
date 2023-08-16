#include "TStyle.h"
#include "TCanvas.h"
#include "/home/belle2/yuanmk/useful/boot.h"

void draw_hist(TH1D* hist,string address, string file, string variable){
    //if flag is true, the histogram is signal
    TChain* chain = new TChain(file.c_str());

    chain -> Add(address.c_str());
    //set variables
    Double_t var;
    chain->SetBranchAddress(variable.c_str(), &var);

    Int_t flag_candidate;
    chain->SetBranchAddress("flag_candidate", &flag_candidate);

    //draw on hist
    Int_t nentries = chain->GetEntries();
    for(Int_t i=0; i < nentries; i++){
        chain->GetEntry(i);
        if(flag_candidate==0) continue;

        hist->Fill(var);
    }
}

void dis_ContProb(){
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
    Double_t xmin=0, xmax=1;  Double_t xbins=100;
    TH1::SetDefaultSumw2(1);
    TString a("Events/"); char b[100];  sprintf(b, "(%.4f)",(xmax-xmin)/xbins);
    TString ytitle = a + b;

    TH1D *hsigMC=new TH1D("hsigMC", "hsigMC", xbins, xmin, xmax);
    hsigMC->SetLineColor(kBlue);
    hsigMC->SetLineWidth(2);
    draw_hist(hsigMC,"/home/belle2/yuanmk/data/B2KKpi/sample/sigMC_sample.root","B0","ContProb");

    THStack* hc = new THStack("hc","total");

    TH1D *huubar=new TH1D("huubar", "huubar", xbins, xmin, xmax);
    huubar->SetLineColor(2);huubar->SetFillColor(2);
    huubar->SetLineWidth(2);
    draw_hist(huubar,"/home/belle2/yuanmk/data/B2KKpi/sample/uubar_sample.root","B0","ContProb");

    TH1D *hddbar=new TH1D("hddbar", "hddbar", xbins, xmin, xmax);
    hddbar->SetLineColor(3);hddbar->SetFillColor(3);
    hddbar->SetLineWidth(2);
    draw_hist(hddbar,"/home/belle2/yuanmk/data/B2KKpi/sample/ddbar_sample.root","B0","ContProb");

    TH1D *hssbar=new TH1D("hssbar", "hssbar", xbins, xmin, xmax);
    hssbar->SetLineColor(4);hssbar->SetFillColor(4);
    hssbar->SetLineWidth(2);
    draw_hist(hssbar,"/home/belle2/yuanmk/data/B2KKpi/sample/ssbar_sample.root","B0","ContProb");

    TH1D *hccbar=new TH1D("hccbar", "hccbar", xbins, xmin, xmax);
    hccbar->SetLineColor(5);hccbar->SetFillColor(5);
    hccbar->SetLineWidth(2);
    draw_hist(hccbar,"/home/belle2/yuanmk/data/B2KKpi/sample/ccbar_sample.root","B0","ContProb");

    hc->Add(huubar);
    hc->Add(hddbar);
    hc->Add(hssbar);
    hc->Add(hccbar);

    mbc->cd();
    //gPad->SetLogy();
    hc->Draw("HIST");
    hsigMC->Draw("HIST same");
    //hsigMC->Draw("HIST");
    //hmisid->Draw("HIST same");

    hsigMC->Scale(hc->GetMaximum()/hsigMC->GetMaximum());
    //hmisid->Scale(hsigMC->GetEntries()/hmisid->GetEntries());

    hc->GetXaxis()->SetTitle("ContProb");
    hc->GetYaxis()->SetTitle(ytitle);
    hc->GetXaxis()->SetTitleOffset(0.90);
    hc->GetYaxis()->SetTitleOffset(1.20);

    //h_sigMC->SetMaximum(10000000);
    
    TLegend *legend = new TLegend(0.25,0.70,0.40,0.84);
    legend->AddEntry("hsigMC","signal MC","l");
    legend->AddEntry("huubar","u#bar{u}","lf");
    legend->AddEntry("hddbar","d#bar{d}","lf");
    legend->AddEntry("hssbar","s#bar{s}","lf");
    legend->AddEntry("hccbar","c#bar{c}","lf");

    legend->SetBorderSize(0);
    legend->SetFillColor(0);
    legend->Draw();

    //TArrow *xy1 = new TArrow(0.112, 0, 0.112, hsigMC->GetMaximum()*0.3,0.015,"<");
    //xy1->SetLineColor(kBlue);xy1->SetLineWidth(2);
    //xy1->Draw();

    //TArrow *xy2 = new TArrow(0.145, 0, 0.145, hsigMC->GetMaximum()*0.3,0.015,"<");
    //xy2->SetLineColor(kBlue);xy2->SetLineWidth(2);
    //xy2->Draw();

    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/cs/plots/dis_ContProb.pdf");
}
