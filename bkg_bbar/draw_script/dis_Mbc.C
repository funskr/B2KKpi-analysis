#include "TStyle.h"
#include "TCanvas.h"
#include "/home/belle2/yuanmk/useful/boot.h"

void draw_hist(TH1D* hist,string address, string file, string variable, bool flag=true, Int_t iDcy=0){
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
    Int_t iCascDcyBrP_B0_0;
    chain->SetBranchAddress("iCascDcyBrP_B0_0", &iCascDcyBrP_B0_0);

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
    for(Int_t i=0; i < nentries; i++){
        chain->GetEntry(i);
        if(flag_candidate==0) continue;
        if(ContProb>0.4) continue;
        
        if(InvM_KpKm > 1.8484 && InvM_KpKm < 1.8806) continue;

        if(InvM_KmPip > 1.8408 && InvM_KmPip < 1.8875) continue;
        if(InvM_KpPim > 1.8408 && InvM_KpPim < 1.8875) continue;

        if((abs(mcPDG) == 511 || abs(mcPDG) == 521) && pi0_mcPDG == 111 && Kp_mcPDG == 321 && Km_mcPDG == -321){
            //cout<<"signal"<<endl;
            count_signal++;
            continue;
        }

        if((abs(mcPDG) == 511 || abs(mcPDG) == 521) && pi0_mcPDG == 111 && (Kp_mcPDG == 211 || Km_mcPDG == -211)){
            cout<<"peeking background"<<endl;
            //count_signal++;
            continue;
        }

        cout<<mcPDG<<":"<<Kp_mcPDG<<" "<<Km_mcPDG<<" "<<pi0_mcPDG<<endl;

        if(flag){
            hist->Fill(var);
        }
        else{
            if(iCascDcyBrP_B0_0 == iDcy) hist->Fill(var);
        }
    }
    cout<<count_signal<<endl;
}

void dis_Mbc(){
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
    Double_t xmin=5.23, xmax=5.29;  Double_t xbins=100;
    TH1::SetDefaultSumw2(1);
    TString a("Events/"); char b[100];  sprintf(b, "(%.4f)",(xmax-xmin)/xbins);
    TString ytitle = a + b;

    TH1D *hBBbar=new TH1D("hBBbar", "hBBbar", xbins, xmin, xmax);
    hBBbar->SetLineColor(kBlue);
    hBBbar->SetLineWidth(2);
    draw_hist(hBBbar,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/mixed_sample.root","B0","Mbc");
    draw_hist(hBBbar,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/charged_sample.root","B0","Mbc");
    //draw_hist(hBBbar,"/home/belle2/yuanmk/data/B2KKpi/B2KKpi_bbar_20230716_night/sub01/*.root","B0","Mbc");
    //draw_hist(hBBbar,"/home/belle2/yuanmk/data/B2KKpi/B2KKpi_bbar_20230716_night/sub02/*.root","B0","Mbc");

    /*
    THStack* hc = new THStack("hc", "big channel");
    TH1D* hc_ind[10];
    int iCaseDcyBrP[10] = {27, 26, 3, 15, 35, 46, 100, 135, 101, 58};
    for(int i=0; i<10; i++){
        string hc_ind_name = "hc_ind_" + to_string(i);
        hc_ind[i] = new TH1D(hc_ind_name.c_str(), hc_ind_name.c_str(), xbins, xmin, xmax);
        hc_ind[i]->SetLineColor(kBlack);
        hc_ind[i]->SetFillColorAlpha(i+2,0.5);

        draw_hist(hc_ind[i], "/home/belle2/yuanmk/analysis/B2KKpi/topoana/bbar_MC.root", "B0", "Mbc", false, iCaseDcyBrP[i]);
        //cout<<hc_ind[i]->GetEntries()
        hc->Add(hc_ind[i]);
    }
    
    */



    mbc->cd();
    //gPad->SetLogy();
    hBBbar->Draw("HIST");
    //hc->Draw("HIST same");
    //hsigMC->Draw("HIST");
    //hmisid->Draw("HIST same");

    //hsigMC->Scale(1/hsigMC->GetEntries());
    //hmisid->Scale(hsigMC->GetEntries()/hmisid->GetEntries());

    hBBbar->GetXaxis()->SetTitle("M_{bc}/(GeV/c^{2})");
    hBBbar->GetYaxis()->SetTitle(ytitle);
    hBBbar->GetXaxis()->SetTitleOffset(0.90);
    hBBbar->GetYaxis()->SetTitleOffset(1.20);

    //h_sigMC->SetMaximum(10000000);
    
    TLegend *legend = new TLegend(0.30,0.60,0.50,0.84);
    legend->AddEntry("hBBbar","B#bar{B} MC sample","l");

    /*
    for(int i=0; i<10; i++){
        string hc_ind_name = "hc_ind_" + to_string(i);
        string legend_name = "No." + to_string(i+1) + " channel";
        legend->AddEntry(hc_ind_name.c_str(), legend_name.c_str(),"f");
    }
    */

    legend->SetBorderSize(0);
    legend->SetFillColor(0);
    legend->Draw();

    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/bkg_bbar/png/dis_Mbc.png");
    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/bkg_bbar/pdf/dis_Mbc.pdf");

}
