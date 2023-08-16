#include "TStyle.h"
#include "TCanvas.h"
#include "/home/belle2/yuanmk/useful/boot.h"

void draw_hist(TH2D* hist,string file, string branch, bool flag=true){
    //if flag is true, the histogram is signal
    TChain* chain = new TChain(branch.c_str());

    chain -> Add(file.c_str());
    //set variables
    //Double_t dr, dz;
    //chain->SetBranchAddress("dr", &dr);
    //chain->SetBranchAddress("dz", &dz);

    Double_t isSignal;
    chain->SetBranchAddress("pi0_isSignal", &isSignal);

    Double_t pi0_p;
    chain->SetBranchAddress("pi0_p", &pi0_p);

    Double_t pi0_daughterAngle_0_1;
    chain->SetBranchAddress("pi0_daughterAngle_0_1", &pi0_daughterAngle_0_1);

    Int_t flag_candidate;
    chain->SetBranchAddress("flag_candidate", &flag_candidate);

    //draw on hist
    Int_t nentries = chain->GetEntries();
    for(Int_t i=0; i < nentries; i++){
        chain->GetEntry(i);
        //if(dr>0.04) continue;
        //if(fabs(dz)>0.2) continue;
        if(flag_candidate==0) continue;

        if(flag){
            if(isSignal == 1){
                //cout<<"ture mc:"<<isSignal<<endl;
                hist->Fill(pi0_daughterAngle_0_1, pi0_p);
            }
                
        }
        else if(!flag){
            if(isSignal == 0 || isnan(isSignal)){
                //cout<<"false mc:"<<isSignal<<endl;
                hist->Fill(pi0_daughterAngle_0_1, pi0_p);
            }
        }
        else{
            //cout<<"erro:"<<isSignal<<endl;
        }       


    }
}

void dis2_pion_deltatheta_mom(){
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
    Double_t xmin=0, xmax=2;  Double_t xbins=100;
    Double_t ymin=0, ymax=3.5;  Double_t ybins=100;

    TH2D *h_sigMC=new TH2D("h_sigMC", "h_sigMC", xbins, xmin, xmax, ybins, ymin, ymax);
    h_sigMC->SetMarkerColor(kBlue);
    draw_hist(h_sigMC,"/home/belle2/yuanmk/data/B2KKpi/sample/sigMC_sample.root", "B0", true);

    TH2D *h_scf=new TH2D("h_scf", "h_scf", xbins, xmin, xmax, ybins, ymin, ymax);
    h_scf->SetMarkerColor(kRed);
    draw_hist(h_scf,"/home/belle2/yuanmk/data/B2KKpi/sample/sigMC_sample.root", "B0", false);

    //TH1D *hmisid=new TH1D("hmisid", "hmisid", xbins, xmin, xmax);
    //hmisid->SetLineColor(kBlue);hmisid->SetFillColor(kBlue);
    //hmisid->SetLineWidth(2);
    //draw_hist(hmisid,"/home/belle2/yuanmk/analysis/B2KKpi/sigMC/ana_B2KKpi_signalMC_final.root","B0","Mbc", false);


    mbc->cd();
    //gPad->SetLogy();
    h_sigMC->Draw("");
    h_scf->Draw("same");
    //hsigMC->Draw("HIST");
    //hmisid->Draw("HIST same");

    //hsigMC->Scale(1/hsigMC->GetEntries());
    //hmisid->Scale(hsigMC->GetEntries()/hmisid->GetEntries());

    h_sigMC->GetXaxis()->SetTitle("#Delta#theta/(rad)");
    h_sigMC->GetYaxis()->SetTitle("P_{#pi}/(GeV/c)");
    h_sigMC->GetXaxis()->SetTitleOffset(1.0);
    h_sigMC->GetYaxis()->SetTitleOffset(1.0);

    //h_sigMC->SetMaximum(10000000);
    
    //TLegend *legend = new TLegend(0.20,0.75,0.35,0.89);
    //legend->AddEntry("hsigMC","Signal MC","l");
    //legend->AddEntry("hmisid","misconstructed","lf");

    TLine* line1 = new TLine(0.5, ymin, 0.5, ymax);
    line1->SetLineColor(kRed);line1->SetLineStyle(9);
    line1->Draw();


    //legend->SetBorderSize(0);
    //legend->SetFillColor(0);
    //legend->Draw();
    //mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/sigMC/plots/png/dis_Mbc.png");
    //mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/sigMC/plots/pdf/dis_Mbc.pdf");
    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/sigMC/plots/pdf/dis2_pion_deltatheta_mom.pdf");

    //cout<<"truely identified:"<<hsigMC->GetEntries()<<endl;
    cout<<"truth matched events:"<<h_sigMC->GetEntries()<<endl;
    cout<<"scf events:"<<h_scf->GetEntries()<<endl;
}