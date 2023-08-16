#include "TStyle.h"
#include "TCanvas.h"
#include "/home/belle2/yuanmk/useful/boot.h"

void draw_hist(TH1D* hist,string file, string branch){
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

    //draw on hist
    Double_t Kp_E_s = 0; Double_t Km_E_s = 0;
    Double_t E_tot = 0; Double_t px_tot = 0; Double_t py_tot = 0; Double_t pz_tot = 0;
    Double_t InvM_KK = 0;
    const Double_t mass_K = 0.493677;
    Int_t nentries = chain->GetEntries();
    //cout<<"Km_E:Km_E_s"<<endl;
    for(Int_t i=0; i < nentries; i++){
        chain->GetEntry(i);
        if(dr>0.04) continue;
        if(fabs(dz)>0.2) continue;
        if(FBDT_qrCombined>0.72) continue;

        //Kp_E_s = sqrt(Kp_px*Kp_px + Kp_px*Kp_px + Kp_px*Kp_px + mass_K * mass_K);
        //Km_E_s = sqrt(Km_px*Km_px + Km_px*Km_px + Km_px*Km_px + mass_K * mass_K);
        E_tot = Km_E + Kp_E;

        px_tot = Kp_px + Km_px;
        py_tot = Kp_py + Km_py;
        pz_tot = Kp_pz + Km_pz;

        InvM_KK = sqrt(E_tot*E_tot - (px_tot*px_tot + py_tot*py_tot + pz_tot*pz_tot));
        
        if(InvM_KK > 1.849 && InvM_KK < 1.879) continue;

        hist->Fill(InvM_KK);
    }
}

void dis_mkk_sigMC(){
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
    Double_t xmin=0, xmax=6;  Double_t xbins=200;
    TH1::SetDefaultSumw2(1);
    TString a("Events/"); char b[100];  sprintf(b, "(%.2f)",(xmax-xmin)/xbins);
    TString ytitle = a + b;

    TH1D *hsigMC=new TH1D("hsigMC", "hsigMC", xbins, xmin, xmax);
    hsigMC->SetLineColor(kBlue);
    hsigMC->SetLineWidth(2);
    draw_hist(hsigMC,"/home/belle2/yuanmk/analysis/B2KKpi/bbar_MC/ana_B2KKpi_signalMC_flavor_tag.root", "B0");

    TH1D *hbbbar=new TH1D("hbbbar", "hbbbar", xbins, xmin, xmax);
    hbbbar->SetLineColor(kRed);
    hbbbar->SetLineWidth(2);
    draw_hist(hbbbar,"/home/belle2/yuanmk/data/B2KKpi/B2KKpi_bbar_20230716_night/sub00/*.root","B0");
    draw_hist(hbbbar,"/home/belle2/yuanmk/data/B2KKpi/B2KKpi_bbar_20230716_night/sub01/*.root","B0");
    draw_hist(hbbbar,"/home/belle2/yuanmk/data/B2KKpi/B2KKpi_bbar_20230716_night/sub02/*.root","B0");

    mbc->cd();
    //gPad->SetLogy();
    hsigMC->Draw("HIST");
    hbbbar->Draw("HIST same");
    //hmisid->Draw("HIST same");

    Float_t rightmax = 3*hbbbar->GetMaximum();
    if(rightmax <= 0) rightmax = 1;
    Float_t scale = hsigMC->GetMaximum()/rightmax;
    //hsigMC->Scale(1/hsigMC->GetEntries());
    hbbbar->Scale(scale);

    hsigMC->GetXaxis()->SetTitle("M_{K^{+}K^{-}}/(GeV/c^{2})");
    hsigMC->GetYaxis()->SetTitle(ytitle);
    hsigMC->GetXaxis()->SetTitleOffset(0.90);
    hsigMC->GetYaxis()->SetTitleOffset(1.20);


    auto axis_x = hc->GetXaxis(); 
    TGaxis* axis_right = new TGaxis(axis_x->GetXmax(),0,
                                axis_x->GetXmax(), hsigMC->GetMaximum(),
                                0,rightmax,510,"+L");
    axis_right->SetLineColor(kRed);
    axis_right->SetLabelColor(kRed);
    axis_right->Draw();
    //h_sigMC->SetMaximum(10000000);
    
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

    //mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/sigMC/plots/png/dis_Mbc.png");
    //mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/sigMC/plots/pdf/dis_Mbc.pdf");
    mbc->SaveAs("./dis_mkk_qr_cut_mkk_veto.pdf");

    //cout<<"truely identified:"<<hsigMC->GetEntries()<<endl;
    //cout<<"misidentified:"<<hmisid->GetEntries()<<endl;
}