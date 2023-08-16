#include "TStyle.h"
#include "TCanvas.h"
#include "/home/belle2/yuanmk/useful/boot.h"

void draw_hist(TH2D* hist,string file, string branch){
    //if flag is true, the histogram is signal
    TChain* chain = new TChain(branch.c_str());

    chain -> Add(file.c_str());
    //set variables
    Double_t dr, dz;
    chain->SetBranchAddress("dr", &dr);
    chain->SetBranchAddress("dz", &dz);

    Double_t Kp_px, Kp_py, Kp_pz, Kp_E;
    chain->SetBranchAddress("Kp_px", &Kp_px);
    chain->SetBranchAddress("Kp_py", &Kp_py);
    chain->SetBranchAddress("Kp_pz", &Kp_pz);
    chain->SetBranchAddress("Kp_E", &Kp_E);

    Double_t Km_px, Km_py, Km_pz, Km_E;
    chain->SetBranchAddress("Km_px", &Km_px);
    chain->SetBranchAddress("Km_py", &Km_py);
    chain->SetBranchAddress("Km_pz", &Km_pz);
    chain->SetBranchAddress("Km_E", &Km_E);

    Double_t pi0_daughterAngle_0_1;
    chain->SetBranchAddress("pi0_daughterAngle_0_1", &pi0_daughterAngle_0_1);

    //draw on hist
    Double_t pip_E = 0; Double_t pim_E = 0;
    Double_t px_tot = 0; Double_t py_tot = 0; Double_t pz_tot = 0;
    Double_t E_Kppim = 0; Double_t E_Kmpip = 0; Double_t InvM_Kppim = 0; Double_t InvM_Kmpip = 0;
    Double_t InvM_KK = 0; Double_t E_tot = 0;
    const Double_t mass_pi = 0.1349768;
    Int_t nentries = chain->GetEntries();

    for(Int_t i=0; i < nentries; i++){
        chain->GetEntry(i);
        if(dr>0.04) continue;
        if(fabs(dz)>0.2) continue;
        if(pi0_daughterAngle_0_1>0.4) continue;

        pip_E = sqrt(Kp_px*Kp_px + Kp_py*Kp_py + Kp_pz*Kp_pz + mass_pi * mass_pi);
        pim_E = sqrt(Km_px*Km_px + Km_py*Km_py + Km_pz*Km_pz + mass_pi * mass_pi);

        E_Kppim = Kp_E + pim_E;
        E_Kmpip = Km_E + pip_E;
        E_tot = Km_E + Kp_E;

        px_tot = Kp_px + Km_px;
        py_tot = Kp_py + Km_py;
        pz_tot = Kp_pz + Km_pz;

        InvM_Kppim = sqrt(E_Kppim*E_Kppim - (px_tot*px_tot + py_tot*py_tot + pz_tot*pz_tot));
        InvM_Kmpip = sqrt(E_Kmpip*E_Kmpip - (px_tot*px_tot + py_tot*py_tot + pz_tot*pz_tot));
        InvM_KK = sqrt(E_tot*E_tot - (px_tot*px_tot + py_tot*py_tot + pz_tot*pz_tot));

        if(InvM_KK > 1.849 && InvM_KK < 1.879) continue;

        hist->Fill(InvM_Kmpip, InvM_Kppim);
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
    Double_t xmin=1.5, xmax=2.5;  Double_t xbins=100;
    //Double_t ymin=1.5, ymax=2.5;  Double_t ybins=100;

    TH2D *h_BBbar=new TH2D("h_BBbar", "h_BBbar", xbins, xmin, xmax, xbins, xmin, xmax);
    h_BBbar->SetMarkerColor(kBlue);
    draw_hist(h_BBbar,"/home/belle2/yuanmk/data/B2KKpi/BBbar/BBar_sample.root", "B0");

    //TH1D *hmisid=new TH1D("hmisid", "hmisid", xbins, xmin, xmax);
    //hmisid->SetLineColor(kBlue);hmisid->SetFillColor(kBlue);
    //hmisid->SetLineWidth(2);
    //draw_hist(hmisid,"/home/belle2/yuanmk/analysis/B2KKpi/sigMC/ana_B2KKpi_signalMC_final.root","B0","Mbc", false);


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
    //mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/sigMC/plots/png/dis_Mbc.png");
    //mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/sigMC/plots/pdf/dis_Mbc.pdf");
    mbc->SaveAs("./dis2_mkpi.pdf");

    //cout<<"truely identified:"<<hsigMC->GetEntries()<<endl;
    cout<<"Total events:"<<h_BBbar->GetEntries()<<endl;
}