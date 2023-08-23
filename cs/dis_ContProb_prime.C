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
    Double_t contprob_p = 0;
    for(Int_t i=0; i < nentries; i++){
        chain->GetEntry(i);
        if(flag_candidate==0) continue;
        if(var > 0.4) continue;

        contprob_p = log10((var-0)/(0.4-var));

        hist->Fill(contprob_p);
    }
}

void dis_ContProb_prime(){
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
    Double_t xmin=-4.2, xmax=4.2;  Double_t xbins=100;
    TH1::SetDefaultSumw2(1);
    TString a("Events/"); char b[100];  sprintf(b, "(%.4f)",(xmax-xmin)/xbins);
    TString ytitle = a + b;

    TH1D *hsigMC=new TH1D("hsigMC", "hsigMC", xbins, xmin, xmax);
    hsigMC->SetLineColor(kBlue);
    hsigMC->SetLineWidth(2);
    draw_hist(hsigMC,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/sigMC_sample.root","B0","ContProb");

    THStack* hc = new THStack("hc","total");

    TH1D *huubar=new TH1D("huubar", "huubar", xbins, xmin, xmax);
    huubar->SetLineColor(kBlack);huubar->SetFillColorAlpha(2, 0.5);
    huubar->SetLineWidth(1);
    draw_hist(huubar,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/uubar_sample.root","B0","ContProb");

    TH1D *hddbar=new TH1D("hddbar", "hddbar", xbins, xmin, xmax);
    hddbar->SetLineColor(kBlack);hddbar->SetFillColorAlpha(3, 0.5);
    hddbar->SetLineWidth(1);
    draw_hist(hddbar,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/ddbar_sample.root","B0","ContProb");

    TH1D *hssbar=new TH1D("hssbar", "hssbar", xbins, xmin, xmax);
    hssbar->SetLineColor(kBlack);hssbar->SetFillColorAlpha(4, 0.5);
    hssbar->SetLineWidth(1);
    draw_hist(hssbar,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/ssbar_sample.root","B0","ContProb");

    TH1D *hccbar=new TH1D("hccbar", "hccbar", xbins, xmin, xmax);
    hccbar->SetLineColor(kBlack);hccbar->SetFillColorAlpha(5, 0.5);
    hccbar->SetLineWidth(1);
    draw_hist(hccbar,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/ccbar_sample.root","B0","ContProb");

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
    cout<<hsigMC->GetEntries()<<endl;
    hsigMC->Scale(hc->GetMaximum()/hsigMC->GetMaximum());
    //hmisid->Scale(hsigMC->GetEntries()/hmisid->GetEntries());

    hc->GetXaxis()->SetTitle("ContProb^{TRANS}");
    hc->GetYaxis()->SetTitle(ytitle);
    hc->GetXaxis()->SetTitleOffset(0.90);
    hc->GetYaxis()->SetTitleOffset(1.20);

    //h_sigMC->SetMaximum(10000000);
    
    TLegend *legend = new TLegend(0.70,0.70,0.84,0.84);
    legend->AddEntry("hsigMC","signal MC","l");
    legend->AddEntry("huubar","u#bar{u}","f");
    legend->AddEntry("hddbar","d#bar{d}","f");
    legend->AddEntry("hssbar","s#bar{s}","f");
    legend->AddEntry("hccbar","c#bar{c}","f");

    legend->SetBorderSize(0);
    legend->SetFillColor(0);
    legend->Draw();

    //TArrow *xy1 = new TArrow(0.112, 0, 0.112, hsigMC->GetMaximum()*0.3,0.015,"<");
    //xy1->SetLineColor(kBlue);xy1->SetLineWidth(2);
    //xy1->Draw();

    //TArrow *xy2 = new TArrow(0.145, 0, 0.145, hsigMC->GetMaximum()*0.3,0.015,"<");
    //xy2->SetLineColor(kBlue);xy2->SetLineWidth(2);
    //xy2->Draw();

    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/cs/plots/dis_ContProb_prime.pdf");
}
