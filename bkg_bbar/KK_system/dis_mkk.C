#include "TStyle.h"
#include "TCanvas.h"
#include "/home/belle2/yuanmk/useful/boot.h"

void draw_hist(TH1D* hist,string file, string branch, bool flag_sig=true, bool split=false){
    //if flag is true, the histogram is signal
    TChain* chain = new TChain(branch.c_str());

    chain -> Add(file.c_str());
    //set variables
    Double_t isSignal;
    chain->SetBranchAddress("isSignal", &isSignal);

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

    Double_t InvM_KK;
    chain->SetBranchAddress("InvM_KK", &InvM_KK);
    Double_t InvM_KpPim;
    chain->SetBranchAddress("InvM_KpPim", &InvM_KpPim);
    Double_t InvM_KmPip;
    chain->SetBranchAddress("InvM_KmPip", &InvM_KmPip);

    Double_t pi0_daughterAngle_0_1;
    chain->SetBranchAddress("pi0_daughterAngle_0_1", &pi0_daughterAngle_0_1);

    Int_t flag_candidate;
    chain->SetBranchAddress("flag_candidate", &flag_candidate);


    Int_t nentries = chain->GetEntries();
    //cout<<"Km_E:Km_E_s"<<endl;
    for(Int_t i=0; i < nentries; i++){
        chain->GetEntry(i);
        if(dr>0.04) continue;
        if(fabs(dz)>0.2) continue;
        if(FBDT_qrCombined>0.72) continue;
        if(pi0_daughterAngle_0_1>0.5) continue;
        if(flag_candidate==0) continue;

        
        if(InvM_KK > 1.849 && InvM_KK < 1.879) continue;
        //if(InvM_KpPim > 1.849 && InvM_KpPim < 1.879) continue;
        //if(InvM_KmPip > 1.849 && InvM_KmPip < 1.879) continue;

        if(split){
            if(flag_sig){
                if(isSignal == 1){
                    //cout<<"ture mc:"<<isSignal<<endl;
                    hist->Fill(InvM_KK);
                }
            }
            else{
                if(isSignal == 0 || isnan(isSignal)){
                    //cout<<"false mc:"<<isSignal<<endl;
                    hist->Fill(InvM_KK);
                }
            }
        }
        else{
            hist->Fill(InvM_KK);
        }
    }
}

void dis_mkk(){
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

    THStack* hc = new THStack("hc","total");

    TH1D *hsigMC_true=new TH1D("hsigMC_true", "hsigMC_true", xbins, xmin, xmax);
    hsigMC_true->SetLineColor(kBlack);hsigMC_true->SetFillColorAlpha(kBlue,0.5);
    hsigMC_true->SetLineWidth(2);
    draw_hist(hsigMC_true,"/home/belle2/yuanmk/data/B2KKpi/sigMC/root/sigMC_sample.root", "B0", true, true);

    TH1D *hsigMC_mis=new TH1D("hsigMC_mis", "hsigMC_mis", xbins, xmin, xmax);
    hsigMC_mis->SetLineColor(kBlack);hsigMC_mis->SetFillColorAlpha(kRed,0.5);
    hsigMC_mis->SetLineWidth(2);
    draw_hist(hsigMC_mis,"/home/belle2/yuanmk/data/B2KKpi/sigMC/root/sigMC_sample.root", "B0", false, true);
    
    hc->Add(hsigMC_mis);
    hc->Add(hsigMC_true);

    TH1D *hbbbar=new TH1D("hbbbar", "hbbbar", xbins, xmin, xmax);
    hbbbar->SetLineColor(kRed);
    hbbbar->SetLineWidth(2);
    draw_hist(hbbbar,"/home/belle2/yuanmk/data/B2KKpi/BBbar/BBar_sample.root","B0");
    //draw_hist(hbbbar,"/home/belle2/yuanmk/data/B2KKpi/B2KKpi_bbar_20230716_night/sub01/*.root","B0");
    //draw_hist(hbbbar,"/home/belle2/yuanmk/data/B2KKpi/B2KKpi_bbar_20230716_night/sub02/*.root","B0");

    mbc->cd();
    //gPad->SetLogy();
    //hsigMC->Draw("HIST");
    hc->Draw("HIST");
    hbbbar->Draw("HIST same");
    //hmisid->Draw("HIST same");

    Float_t rightmax = 3*hbbbar->GetMaximum();
    if(rightmax <= 0) rightmax = 1;
    Float_t scale = hc->GetMaximum()/rightmax;
    //hsigMC->Scale(1/hsigMC->GetEntries());
    hbbbar->Scale(scale);

    //hbbbar->Scale(hsigMC->GetMaximum()/hbbbar->GetMaximum());

    hc->GetXaxis()->SetTitle("M_{K^{+}K^{-}}/(GeV/c^{2})");
    hc->GetYaxis()->SetTitle(ytitle);
    hc->GetXaxis()->SetTitleOffset(0.90);
    hc->GetYaxis()->SetTitleOffset(1.20);
    
    auto axis_x = hc->GetXaxis(); 
    TGaxis* axis_right = new TGaxis(axis_x->GetXmax(),0,
                                axis_x->GetXmax(), hc->GetMaximum()*1.05,
                                0,rightmax,510,"+L");
    axis_right->SetLineColor(kRed);
    axis_right->SetLabelColor(kRed);
    axis_right->Draw();
    
    TLegend *legend = new TLegend(0.20,0.75,0.35,0.89);
    legend->AddEntry("hsigMC_true", "SigMC isSignal=1", "f");
    legend->AddEntry("hsigMC_mis", "Signal isSignal!=1", "f");
    legend->AddEntry("hbbbar","BBbar MC","lf");

    legend->SetBorderSize(0);
    legend->SetFillColor(0);
    legend->Draw();

    TArrow *xy1 = new TArrow(0.112, 0, 0.112, hc->GetMaximum()*0.3,0.015,"<");
    xy1->SetLineColor(kBlue);xy1->SetLineWidth(2);
    //xy1->Draw();

    TArrow *xy2 = new TArrow(0.145, 0, 0.145, hc->GetMaximum()*0.3,0.015,"<");
    xy2->SetLineColor(kBlue);xy2->SetLineWidth(2);
    //xy2->Draw();

    //mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/sigMC/plots/png/dis_Mbc.png");
    //mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/sigMC/plots/pdf/dis_Mbc.pdf");
    mbc->SaveAs("./dis_mkk.pdf");

    //cout<<"truely identified:"<<hsigMC->GetEntries()<<endl;
    //cout<<"misidentified:"<<hmisid->GetEntries()<<endl;
}