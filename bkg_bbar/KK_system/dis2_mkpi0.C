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

    Double_t FBDT_qrCombined;
    chain->SetBranchAddress("FBDT_qrCombined", &FBDT_qrCombined);  
    
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

    Double_t pi0_px, pi0_py, pi0_pz, pi0_E;
    chain->SetBranchAddress("pi0_px", &pi0_px);
    chain->SetBranchAddress("pi0_py", &pi0_py);
    chain->SetBranchAddress("pi0_pz", &pi0_pz);
    chain->SetBranchAddress("pi0_E", &pi0_E);

    //draw on hist
    Double_t px_kppi0 = 0; Double_t py_kppi0 = 0; Double_t pz_kppi0 = 0; Double_t E_kppi0 = 0;
    Double_t px_kmpi0 = 0; Double_t py_kmpi0 = 0; Double_t pz_kmpi0 = 0; Double_t E_kmpi0 = 0;
    Double_t px_kk = 0; Double_t py_kk = 0; Double_t pz_kk = 0; Double_t E_kk = 0;
    Double_t InvM_kppi0 = 0; Double_t InvM_kmpi0 = 0; Double_t InvM_kk = 0;
    const Double_t mass_pi = 0.1349768;
    Int_t nentries = chain->GetEntries();

    for(Int_t i=0; i < nentries; i++){
        chain->GetEntry(i);
        if(dr>0.04) continue;
        if(fabs(dz)>0.2) continue;
        if(FBDT_qrCombined>0.72) continue;

        //pip_E = sqrt(Kp_px*Kp_px + Kp_py*Kp_py + Kp_pz*Kp_pz + mass_pi * mass_pi);
        //pim_E = sqrt(Km_px*Km_px + Km_py*Km_py + Km_pz*Km_pz + mass_pi * mass_pi);

        E_kppi0 = Kp_E + pi0_E;
        E_kmpi0 = Km_E + pi0_E;
        E_kk = Kp_E + Km_E;

        px_kppi0 = Kp_px + pi0_px;
        py_kppi0 = Kp_py + pi0_py;
        pz_kppi0 = Kp_pz + pi0_pz;

        px_kmpi0 = Km_px + pi0_px;
        py_kmpi0 = Km_py + pi0_py;
        pz_kmpi0 = Km_pz + pi0_pz;

        px_kk = Km_px + Kp_px;
        py_kk = Km_py + Kp_py;
        pz_kk = Km_pz + Kp_pz;

        InvM_kppi0 = sqrt(E_kppi0*E_kppi0 - (px_kppi0*px_kppi0 + py_kppi0*py_kppi0 + pz_kppi0*pz_kppi0));
        InvM_kmpi0 = sqrt(E_kmpi0*E_kmpi0 - (px_kmpi0*px_kmpi0 + py_kmpi0*py_kmpi0 + pz_kmpi0*pz_kmpi0));
        InvM_kk = sqrt(E_kk*E_kk - (px_kk*px_kk + py_kk*py_kk + pz_kk*pz_kk));

        if(InvM_kk > 1.849 && InvM_kk < 1.879) continue;

        hist->Fill(InvM_kppi0*InvM_kppi0, InvM_kmpi0*InvM_kmpi0);
    }
}

void dis2_mkpi0(){
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
    Double_t xmin=0, xmax=25;  Double_t xbins=100;
    //Double_t ymin=1.5, ymax=2.5;  Double_t ybins=100;

    TH2D *h_BBbar=new TH2D("h_BBbar", "h_BBbar", xbins, xmin, xmax, xbins, xmin, xmax);
    h_BBbar->SetMarkerColor(kBlue);
    draw_hist(h_BBbar,"/home/belle2/yuanmk/data/B2KKpi/B2KKpi_bbar_20230716_night/sub00/*.root", "B0");
    draw_hist(h_BBbar,"/home/belle2/yuanmk/data/B2KKpi/B2KKpi_bbar_20230716_night/sub01/*.root", "B0");
    draw_hist(h_BBbar,"/home/belle2/yuanmk/data/B2KKpi/B2KKpi_bbar_20230716_night/sub02/*.root", "B0");

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

    h_BBbar->GetXaxis()->SetTitle("M^{2}_{#pi^{0}K^{+}}/(GeV^{2}/c^{4})");
    h_BBbar->GetYaxis()->SetTitle("M^{2}_{#pi^{0}K^{-}}/(GeV^{2}/c^{4})");
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
    mbc->SaveAs("./dis2_mkpi0_mkk_veto.pdf");

    //cout<<"truely identified:"<<hsigMC->GetEntries()<<endl;
    cout<<"Total events:"<<h_BBbar->GetEntries()<<endl;
}