#include "TStyle.h"
#include "TCanvas.h"
#include "/home/belle2/yuanmk/useful/boot.h"

void draw_hist(TH1D* hist,string address, string file, string variable, int flag=1){
    //if flag is true, the histogram is signal
    TChain* chain = new TChain(file.c_str());

    chain -> Add(address.c_str());
    //set variables
    Double_t var;
    chain->SetBranchAddress(variable.c_str(), &var);

    Double_t qrMC;
    chain->SetBranchAddress("qrMC", &qrMC);

    //draw on hist
    Int_t nentries = chain->GetEntries();
    for(Int_t i=0; i < nentries; i++){
        chain->GetEntry(i);

        if(flag == 1){
            if(qrMC == -1){
                //cout<<"flavor tag side B:"<<isSignal<<endl;
                hist->Fill(var);
            }
                
        }
        else if(flag == 2){
            if(qrMC == 1 ){
                //cout<<"false mc:"<<isSignal<<endl;
                hist->Fill(var);
            }
        }
        else if(flag == 3){
            if(qrMC == 0 ){
                //cout<<"false mc:"<<isSignal<<endl;
                hist->Fill(var);
            }
        }
    }
}

void dis_FANN_qr(){
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
    Double_t xmin=-1.0, xmax=1.0;  Double_t xbins=100;
    TH1::SetDefaultSumw2(1);
    TString a("Events/"); char b[100];  sprintf(b, "(%.2f)",(xmax-xmin)/xbins);
    TString ytitle = a + b;

    TH1D *h_B0=new TH1D("h_B0", "h_B0", xbins, xmin, xmax);
    h_B0->SetLineColor(kRed);
    h_B0->SetLineWidth(2);
    draw_hist(h_B0,"/home/belle2/yuanmk/analysis/B2KKpi/sigMC/ana_B2KKpi_signalMC_flavor_tag.root","B0","FANN_qrCombined", 1);

    TH1D *h_B0bar=new TH1D("h_B0bar", "h_B0bar", xbins, xmin, xmax);
    h_B0bar->SetLineColor(kBlue);
    h_B0bar->SetLineWidth(2);
    draw_hist(h_B0bar,"/home/belle2/yuanmk/analysis/B2KKpi/sigMC/ana_B2KKpi_signalMC_flavor_tag.root","B0","FANN_qrCombined", 2);

    TH1D *h_other=new TH1D("h_other", "h_other", xbins, xmin, xmax);
    h_other->SetLineColor(kGreen);
    h_other->SetLineWidth(2);
    draw_hist(h_other,"/home/belle2/yuanmk/analysis/B2KKpi/sigMC/ana_B2KKpi_signalMC_flavor_tag.root","B0","FBDT_qrCombined", 3);

    mbc->cd();
    //gPad->SetLogy();
    h_B0->Draw("HIST");
    h_B0bar->Draw("HIST same");
    h_other->Draw("HIST same");

    //h_B0->Scale(1/h_B0->GetEntries());
    //h_B0bar->Scale(h_B0->GetEntries()/h_B0bar->GetEntries());

    h_B0->GetXaxis()->SetTitle("qr_{FANN}");
    h_B0->GetYaxis()->SetTitle(ytitle);
    h_B0->GetXaxis()->SetTitleOffset(0.90);
    h_B0->GetYaxis()->SetTitleOffset(1.20);

    //h_sigMC->SetMaximum(10000000);
    
    TLegend *legend = new TLegend(0.30,0.60,0.50,0.84);
    legend->AddEntry("h_B0","B0","l");
    legend->AddEntry("h_B0bar","anti-B0","l");
    legend->AddEntry("h_other", "qrMC=0", "l");

    legend->SetBorderSize(0);
    legend->SetFillColor(0);
    legend->Draw();

    TArrow *xy1 = new TArrow(0.112, 0, 0.112, h_B0->GetMaximum()*0.3,0.015,"<");
    xy1->SetLineColor(kBlue);xy1->SetLineWidth(2);
    //xy1->Draw();

    TArrow *xy2 = new TArrow(0.145, 0, 0.145, h_B0->GetMaximum()*0.3,0.015,"<");
    xy2->SetLineColor(kBlue);xy2->SetLineWidth(2);
    //xy2->Draw();

    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/sigMC/plots/png/dis_FANN_qr.png");
    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/sigMC/plots/pdf/dis_FANN_qr.pdf");

    cout<<h_B0->GetEntries()+h_B0bar->GetEntries()+h_other->GetEntries()<<endl;
}
