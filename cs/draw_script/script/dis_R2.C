#include "TStyle.h"
#include "TCanvas.h"
#include "/home/belle2/yuanmk/useful/boot.h"


void draw_hist(TH1D* hist,string address, string tree, string variable){
    //if flag is true, the histogram is signal
    TChain* chain = new TChain(tree.c_str());

    chain -> Add(address.c_str());
    //set variables
    Double_t var;
    chain->SetBranchAddress(variable.c_str(), &var);

    //draw on hist
    Int_t nentries = chain->GetEntries();
    for(Int_t i=0; i < nentries; i++){
        chain->GetEntry(i);

        hist->Fill(var); 
    }
}


void dis_R2(){

    setStyle();
    string variable = "R2";

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
    Double_t xmin=0, xmax=1;  Int_t xbins=100;
    TH1::SetDefaultSumw2(1);
    TString a("Events/"); char b[100];  sprintf(b, "(%.2f)",(xmax-xmin)/xbins);
    TString ytitle = a + b;

    TH1D *hsigMC=new TH1D("hsigMC", "hsigMC", xbins, xmin, xmax);
    hsigMC->SetLineColor(kRed);
    hsigMC->SetLineWidth(3);

    draw_hist(hsigMC, "/home/belle2/yuanmk/analysis/B2KKpi/continuum/B2KKpi_cs_signalMC.root", "B0", variable);

    THStack* hc = new THStack("hc","continuum bkg");

    TH1D *huubar=new TH1D("huubar", "huubar", xbins, xmin, xmax);
    huubar->SetLineColor(3);huubar->SetFillColor(3);
    huubar->SetLineWidth(2);
    draw_hist(huubar, "/home/belle2/yuanmk/data/B2KKpi/cs/B2KKpi_cs_uubar/sub00/*_0000[0-9]_*.root", "tree", variable);

    huubar->Scale(1/6372.);
    hc->Add(huubar);


    TH1D *hccbar=new TH1D("hccbar", "hccbar", xbins, xmin, xmax);
    hccbar->SetLineColor(4);hccbar->SetFillColor(4);
    hccbar->SetLineWidth(2);
    draw_hist(hccbar, "/home/belle2/yuanmk/data/B2KKpi/cs/B2KKpi_cs_ccbar/sub00/*_0000[0-9]_*.root", "tree", variable);
    hccbar->Scale(1/5780.);
    hc->Add(hccbar);


    TH1D *hddbar=new TH1D("hddbar", "hddbar", xbins, xmin, xmax);
    hddbar->SetLineColor(5);hddbar->SetFillColor(5);
    hddbar->SetLineWidth(2);
    draw_hist(hddbar, "/home/belle2/yuanmk/data/B2KKpi/cs/B2KKpi_cs_ddbar/sub00/*_0000[0-9]_*.root", "tree", variable);
    hddbar->Scale(1/1593.);
    hc->Add(hddbar);

    TH1D *hssbar=new TH1D("hssbar", "hssbar", xbins, xmin, xmax);
    hssbar->SetLineColor(6);hssbar->SetFillColor(6);
    hssbar->SetLineWidth(2);
    draw_hist(hssbar, "/home/belle2/yuanmk/data/B2KKpi/cs/B2KKpi_cs_ssbar/sub00/*_0000[0-9]_*.root", "tree", variable);
    hssbar->Scale(1/1519.);
    hc->Add(hssbar);

    mbc->cd();
    hc->Draw("HIST");
    hsigMC->Draw("HIST same");
    hsigMC->Scale(hc->GetMaximum()/hsigMC->GetMaximum());

    hc->GetXaxis()->SetTitle(variable.c_str());
    hc->GetYaxis()->SetTitle(ytitle);
    hc->GetXaxis()->SetTitleOffset(0.90);
    hc->GetYaxis()->SetTitleOffset(1.0);

    string legend_position="right"; 
    if(legend_position == "left"){//left
        TLegend *legend = new TLegend(0.20,0.70,0.40,0.86);
        legend->AddEntry("hsigMC","signal MC","l");
        legend->AddEntry("huubar","uubar","lf");
        legend->AddEntry("hccbar","ccbar","lf");
        legend->AddEntry("hddbar","ddbar","lf");
        legend->AddEntry("hssbar","ssbar","lf");
        legend->SetBorderSize(0);
        legend->SetFillColor(0);
        legend->Draw();
    }
    else if(legend_position == "right"){//right
        TLegend *legend = new TLegend(0.70,0.70,0.86,0.86);
        legend->AddEntry("hsigMC","signal MC","l");
        legend->AddEntry("huubar","uubar","lf");
        legend->AddEntry("hccbar","ccbar","lf");
        legend->AddEntry("hddbar","ddbar","lf");
        legend->AddEntry("hssbar","ssbar","lf");
        legend->SetBorderSize(0);
        legend->SetFillColor(0);
        legend->Draw();
    }



    TArrow *xy1 = new TArrow(0.112, 0, 0.112, hsigMC->GetMaximum()*0.3,0.015,"<");
    xy1->SetLineColor(kBlue);xy1->SetLineWidth(2);
    //xy1->Draw();

    TArrow *xy2 = new TArrow(0.145, 0, 0.145, hsigMC->GetMaximum()*0.3,0.015,"<");
    xy2->SetLineColor(kBlue);xy2->SetLineWidth(2);
    //xy2->Draw();

    string pdf = "/home/belle2/yuanmk/analysis/B2KKpi/plots/cs/pdf/dis_"+variable+".pdf";
    string png = "/home/belle2/yuanmk/analysis/B2KKpi/plots/cs/png/dis_"+variable+".png";

    mbc->SaveAs(pdf.c_str());
    mbc->SaveAs(png.c_str());

    cout<<hsigMC->GetEntries()<<endl;
}