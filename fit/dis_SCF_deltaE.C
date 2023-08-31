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

    Double_t ContProb;
    chain->SetBranchAddress("ContProb", &ContProb);

    Double_t InvM_KpKm;
    chain->SetBranchAddress("InvM_KpKm", &InvM_KpKm);
    Double_t InvM_KpPim;
    chain->SetBranchAddress("InvM_KpPim", &InvM_KpPim);
    Double_t InvM_KmPip;
    chain->SetBranchAddress("InvM_KmPip", &InvM_KmPip);

    Int_t flag_candidate;
    chain->SetBranchAddress("flag_candidate", &flag_candidate);

    Double_t isSignal;
    chain->SetBranchAddress("isSignal", &isSignal);

    Double_t Kp_mcPDG;
    chain->SetBranchAddress("Kp_mcPDG", &Kp_mcPDG);
    Double_t Km_mcPDG;
    chain->SetBranchAddress("Km_mcPDG", &Km_mcPDG);
    Double_t pi0_mcPDG;
    chain->SetBranchAddress("pi0_mcPDG", &pi0_mcPDG);
    Double_t mcPDG;
    chain->SetBranchAddress("mcPDG", &mcPDG);

    //draw on hist
    Int_t nentries = chain->GetEntries();
    Int_t count_signal = 0;
    Int_t count_peaking = 0;
    for(Int_t i=0; i < nentries; i++){
        chain->GetEntry(i);
        if(flag_candidate==0) continue;
        if(ContProb>0.4) continue;
        
        if(InvM_KpKm > 1.8484 && InvM_KpKm < 1.8806) continue;

        if(InvM_KmPip > 1.8408 && InvM_KmPip < 1.8875) continue;
        if(InvM_KpPim > 1.8408 && InvM_KpPim < 1.8875) continue;

        if(isSignal==1)continue;

        if(abs(mcPDG) == 511 && pi0_mcPDG == 111 && Kp_mcPDG == 321 && Km_mcPDG == -321){
            //cout<<"signal"<<endl;
            count_signal++;
            //continue;
        }

        if(abs(mcPDG) == 511 && pi0_mcPDG == 111 && (Kp_mcPDG == 211 || Km_mcPDG == -211)){
            //cout<<"peeking background"<<endl;
            count_peaking++;
            //continue;
        }
        //cout<<mcPDG<<":"<<Kp_mcPDG<<" "<<Km_mcPDG<<" "<<pi0_mcPDG<<endl;

        hist->Fill(var);

    }
    cout<<count_signal<<endl;
    cout<<count_peaking<<endl;
}

void dis_SCF_deltaE(){
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
    Double_t xmin=-0.30, xmax=0.15;  Double_t xbins=100;
    TH1::SetDefaultSumw2(1);
    TString a("Events/"); char b[100];  sprintf(b, "(%.4f)",(xmax-xmin)/xbins);
    TString ytitle = a + b;

    TH1D *h_sigMC_SCF=new TH1D("h_sigMC_SCF", "h_sigMC_SCF", xbins, xmin, xmax);
    h_sigMC_SCF->SetLineColor(kBlue);
    h_sigMC_SCF->SetLineWidth(2);
    draw_hist(h_sigMC_SCF,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/sigMC_sample.root","B0","deltaE");

    mbc->cd();
    h_sigMC_SCF->Draw("HIST");

    h_sigMC_SCF->GetXaxis()->SetTitle("#Delta E");
    h_sigMC_SCF->GetYaxis()->SetTitle(ytitle);
    h_sigMC_SCF->GetXaxis()->SetTitleOffset(0.90);
    h_sigMC_SCF->GetYaxis()->SetTitleOffset(1.20);

    
    TLegend *legend = new TLegend(0.64,0.75,0.88,0.88);
    legend->AddEntry("h_sigMC_SCF","SCF signal component","l");

    legend->SetBorderSize(0);
    legend->SetFillColor(0);
    legend->Draw();

    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/fit_MC/pdf/dis_SCF_deltaE.pdf");
    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/fit_MC/png/dis_SCF_deltaE.png");
}
