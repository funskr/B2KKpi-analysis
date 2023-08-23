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

    //Double_t dr, dz;
    //chain->SetBranchAddress("dr", &dr);
    //chain->SetBranchAddress("dz", &dz);

    //Double_t FBDT_qrCombined;
    //chain->SetBranchAddress("FBDT_qrCombined", &FBDT_qrCombined);  

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


    Int_t nentries = chain->GetEntries();
    //cout<<"Km_E:Km_E_s"<<endl;
    for(Int_t i=0; i < nentries; i++){
        chain->GetEntry(i);
        if(flag_candidate==0) continue;
        if(ContProb>0.4) continue;
        
        if(InvM_KpKm > 1.8484 && InvM_KpKm < 1.8806) continue;

        if(InvM_KmPip > 1.8408 && InvM_KmPip < 1.8875) continue;
        if(InvM_KpPim > 1.8408 && InvM_KpPim < 1.8875) continue;

        if(split){
            if(flag_sig){
                if(isSignal == 1){
                    //cout<<"ture mc:"<<isSignal<<endl;
                    hist->Fill(InvM_KpKm);
                }
            }
            else{
                if(isSignal == 0 || isnan(isSignal)){
                    //cout<<"false mc:"<<isSignal<<endl;
                    hist->Fill(InvM_KpKm);
                }
            }
        }
        else{
            hist->Fill(InvM_KpKm);
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
    Double_t xmin=0.8, xmax=5.5;  Double_t xbins=200;
    TH1::SetDefaultSumw2(1);
    TString a("Events/"); char b[100];  sprintf(b, "(%.2f)",(xmax-xmin)/xbins);
    TString ytitle = a + b;

    THStack* hc_sigMC = new THStack("hc_sigMC","sigMC");

    TH1D *hsigMC_true=new TH1D("hsigMC_true", "hsigMC_true", xbins, xmin, xmax);
    hsigMC_true->SetLineColor(kBlack);hsigMC_true->SetFillColorAlpha(kBlue,0.5);
    hsigMC_true->SetLineWidth(1);
    draw_hist(hsigMC_true,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/sigMC_sample.root", "B0", true, true);

    TH1D *hsigMC_mis=new TH1D("hsigMC_mis", "hsigMC_mis", xbins, xmin, xmax);
    hsigMC_mis->SetLineColor(kBlack);hsigMC_mis->SetFillColorAlpha(kRed,0.5);
    hsigMC_mis->SetLineWidth(1);
    draw_hist(hsigMC_mis,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/sigMC_sample.root", "B0", false, true);
    
    hc_sigMC->Add(hsigMC_mis);
    hc_sigMC->Add(hsigMC_true);

    TH1D *h_mixed=new TH1D("h_mixed", "h_mixed", xbins, xmin, xmax);
    h_mixed->SetLineColor(kGreen);//h_mixed->SetFillColorAlpha(kGreen,0.4);h_mixed->SetFillStyle(3490);
    h_mixed->SetLineWidth(2);
    draw_hist(h_mixed,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/mixed_sample.root","B0");
    //draw_hist(hbbbar,"/home/belle2/yuanmk/data/B2KKpi/sample/charged_sample.root","B0");

    TH1D *h_charged=new TH1D("h_charged", "h_charged", xbins, xmin, xmax);
    h_charged->SetLineColor(kViolet);//h_charged->SetFillColorAlpha(kYellow,0.4);h_charged->SetFillStyle(3490);
    h_charged->SetLineWidth(2);
    draw_hist(h_charged,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/charged_sample.root","B0");

    mbc->cd();
    //gPad->SetLogy();
    //hsigMC->Draw("HIST");
    hc_sigMC->Draw("HIST");
    //hc_bbbar->Draw("HIST same");
    h_mixed->Draw("HIST same");
    h_charged->Draw("HIST same");

    //hbbbar->Scale(hsigMC->GetMaximum()/hbbbar->GetMaximum());
    Float_t rightmax = 5*h_mixed->GetMaximum();
    if(rightmax <= 0) rightmax = 1;
    Float_t scale = hc_sigMC->GetMaximum()/rightmax;
    h_mixed->Scale(scale);
    h_charged->Scale(scale);

    hc_sigMC->GetXaxis()->SetTitle("M_{K^{+}K^{-}}/(GeV/c^{2})");
    hc_sigMC->GetYaxis()->SetTitle(ytitle);
    hc_sigMC->GetXaxis()->SetTitleOffset(0.90);
    hc_sigMC->GetYaxis()->SetTitleOffset(1.20);
    
    auto axis_x = hc_sigMC->GetXaxis(); 
    TGaxis* axis_right = new TGaxis(axis_x->GetXmax(),0,
                                axis_x->GetXmax(), hc_sigMC->GetMaximum()*1.05,
                                0,rightmax,510,"+L");
    axis_right->SetLineColor(kRed);
    axis_right->SetLabelColor(kRed);
    axis_right->Draw();
    
    TLegend *legend = new TLegend(0.18,0.75,0.32,0.89);
    legend->AddEntry("hsigMC_true", "SigMC isSignal=1", "f");
    legend->AddEntry("hsigMC_mis", "Signal isSignal!=1", "f");
    legend->AddEntry("h_mixed","mixed MC","lf");
    legend->AddEntry("h_charged", "charged MC", "lf");

    legend->SetBorderSize(0);
    legend->SetFillColor(0);
    legend->Draw();

    TArrow *xy1 = new TArrow(0.112, 0, 0.112, hc_sigMC->GetMaximum()*0.3,0.015,"<");
    xy1->SetLineColor(kBlue);xy1->SetLineWidth(2);
    //xy1->Draw();

    TArrow *xy2 = new TArrow(0.145, 0, 0.145, hc_sigMC->GetMaximum()*0.3,0.015,"<");
    xy2->SetLineColor(kBlue);xy2->SetLineWidth(2);
    //xy2->Draw();

    //mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/bkg_bbar/pdf/dis_mkk.pdf");
    //mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/bkg_bbar/png/dis_mkk.png");

    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/bkg_bbar/pdf/dis_mkk_charmveto.pdf");
    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/bkg_bbar/png/dis_mkk_charmveto.png");

    //cout<<"truely identified:"<<hsigMC->GetEntries()<<endl;
    //cout<<"misidentified:"<<hmisid->GetEntries()<<endl;
}