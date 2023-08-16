#include "TStyle.h"
#include "TCanvas.h"
#include "/home/belle2/yuanmk/useful/boot.h"

void draw_hist(TH1D* hist,string address, string file, string variable, bool flag=true){
    //if flag is true, the histogram is signal
    TChain* chain = new TChain(file.c_str());

    chain -> Add(address.c_str());
    //set variables
    Double_t var;
    chain->SetBranchAddress(variable.c_str(), &var);

    Double_t isSignal;
    chain->SetBranchAddress("pi0_isSignal", &isSignal);
    Double_t dr;
    chain->SetBranchAddress("dr", &dr);
    Double_t dz;
    chain->SetBranchAddress("dz", &dz);

    //draw on hist
    Int_t nentries = chain->GetEntries();
    for(Int_t i=0; i < nentries; i++){
        chain->GetEntry(i);
        //if(dr>0.04) continue;
        //if(fabs(dz)>0.2) continue;

        if(flag){
            if(isSignal == 1){
                //cout<<"ture mc:"<<isSignal<<endl;
                hist->Fill(var);
            }
                
        }
        else if(!flag){
            if(isSignal == 0 || isnan(isSignal)){
                //cout<<"false mc:"<<isSignal<<endl;
                hist->Fill(var);
            }
        }
        else{
            //cout<<"erro:"<<isSignal<<endl;
        }            
    }
}

void dis_masspi0_chi2(){
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
    Double_t xmin=0, xmax=20;  Double_t xbins=100;
    TH1::SetDefaultSumw2(1);
    TString a("Events/"); char b[100];  sprintf(b, "(%.2f)",(xmax-xmin)/xbins);
    TString ytitle = a + b;

    TH1D *hsigMC=new TH1D("hsigMC", "hsigMC", xbins, xmin, xmax);
    hsigMC->SetLineColor(kRed);
    hsigMC->SetLineWidth(2);
    draw_hist(hsigMC,"/home/belle2/yuanmk/data/B2KKpi/sigMC/root/B2KKpi_chisqrd_test.root","B0","chiSqrd", true);

    TH1D *hmisid=new TH1D("hmisid", "hmisid", xbins, xmin, xmax);
    hmisid->SetLineColor(kBlue);
    hmisid->SetLineWidth(2);
    draw_hist(hmisid,"/home/belle2/yuanmk/data/B2KKpi/sigMC/root/B2KKpi_chisqrd_test.root","B0","chiSqrd", false);

    mbc->cd();
    gPad->SetLogy();
    hsigMC->Draw("HIST");
    hmisid->Draw("HIST same");

    //hsigMC->Scale(1/hsigMC->GetEntries());
    //hmisid->Scale(hsigMC->GetEntries()/hmisid->GetEntries());

    hsigMC->GetXaxis()->SetTitle("#chi^{2}");
    hsigMC->GetYaxis()->SetTitle(ytitle);
    hsigMC->GetXaxis()->SetTitleOffset(0.90);
    hsigMC->GetYaxis()->SetTitleOffset(1.20);

    //h_sigMC->SetMaximum(10000000);
    
    TLegend *legend = new TLegend(0.30,0.60,0.50,0.84);
    legend->AddEntry("hsigMC","truly constructed","lf");
    legend->AddEntry("hmisid","misconstructed","lf");

    legend->SetBorderSize(0);
    legend->SetFillColor(0);
    legend->Draw();

    TArrow *xy1 = new TArrow(0.112, 0, 0.112, hsigMC->GetMaximum()*0.3,0.015,"<");
    xy1->SetLineColor(kBlue);xy1->SetLineWidth(2);
    //xy1->Draw();

    TArrow *xy2 = new TArrow(0.145, 0, 0.145, hsigMC->GetMaximum()*0.3,0.015,"<");
    xy2->SetLineColor(kBlue);xy2->SetLineWidth(2);
    //xy2->Draw();

    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/sigMC/plots/png/dis_masspi0_chi2.png");
    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/sigMC/plots/pdf/dis_masspi0_chi2.pdf");

    cout<<"truely identified:"<<hsigMC->GetEntries()<<endl;
    cout<<"misidentified:"<<hmisid->GetEntries()<<endl;
}
