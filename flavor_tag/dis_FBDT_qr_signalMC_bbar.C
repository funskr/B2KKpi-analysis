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
    for(Int_t i=0; i < nentries; i++){
        chain->GetEntry(i);
        if(flag_candidate==0) continue;
        if(ContProb>0.4) continue;
        
        if(InvM_KpKm > 1.8484 && InvM_KpKm < 1.8806) continue;

        if(InvM_KmPip > 1.8408 && InvM_KmPip < 1.8875) continue;
        if(InvM_KpPim > 1.8408 && InvM_KpPim < 1.8875) continue;

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
        else if(flag == 4){
            if((abs(mcPDG) == 511 && pi0_mcPDG == 111 && Kp_mcPDG == 321 && Km_mcPDG == -321)) continue;
            hist->Fill(var);
        }
    }
}

void dis_FBDT_qr_signalMC_bbar(){
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

    THStack* hc = new THStack("hc","Signal MC");

    TH1D *h_sigMC_B0=new TH1D("h_sigMC_B0", "h_sigMC_B0", xbins, xmin, xmax);
    h_sigMC_B0->SetLineColor(kBlack);h_sigMC_B0->SetFillColor(kRed);
    h_sigMC_B0->SetLineWidth(1);
    draw_hist(h_sigMC_B0,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/sigMC_sample.root","B0","FBDT_qrCombined", 1);

    TH1D *h_sigMC_B0bar=new TH1D("h_sigMC_B0bar", "h_sigMC_B0bar", xbins, xmin, xmax);
    h_sigMC_B0bar->SetLineColor(kBlack);h_sigMC_B0bar->SetFillColor(kBlue);
    h_sigMC_B0bar->SetLineWidth(1);
    draw_hist(h_sigMC_B0bar,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/sigMC_sample.root","B0","FBDT_qrCombined", 2);

    TH1D *h_sigMC_other=new TH1D("h_sigMC_other", "h_sigMC_other", xbins, xmin, xmax);
    h_sigMC_other->SetLineColor(kBlack);h_sigMC_other->SetFillColor(kGreen);
    h_sigMC_other->SetLineWidth(1);
    draw_hist(h_sigMC_other,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/sigMC_sample.root","B0","FBDT_qrCombined", 3);

    TH1D *h_BBbar=new TH1D("h_BBbar", "h_BBbar", xbins, xmin, xmax);
    h_BBbar->SetLineColor(kBlack);h_BBbar->SetFillColorAlpha(kBlack,0.4);h_BBbar->SetFillStyle(3144);
    h_BBbar->SetLineWidth(2);
    draw_hist(h_BBbar,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/mixed_sample.root","B0","FBDT_qrCombined", 4);
    draw_hist(h_BBbar,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/charged_sample.root","B0","FBDT_qrCombined", 4);
    h_BBbar->Scale(1./3.);
    draw_hist(h_BBbar,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/uubar_sample.root","B0","FBDT_qrCombined", 4);
    draw_hist(h_BBbar,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/ddbar_sample.root","B0","FBDT_qrCombined", 4);
    draw_hist(h_BBbar,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/ssbar_sample.root","B0","FBDT_qrCombined", 4);
    draw_hist(h_BBbar,"/home/belle2/yuanmk/data/B2KKpi/sample/ana/ccbar_sample.root","B0","FBDT_qrCombined", 4);


    hc->Add(h_sigMC_other);
    hc->Add(h_sigMC_B0bar);
    hc->Add(h_sigMC_B0);

    mbc->cd();
    //gPad->SetLogy();
    //h_sigMC_B0->Draw("HIST");
    //h_sigMC_B0bar->Draw("HIST same");
    //h_sigMC_other->Draw("HIST same");
    hc->Draw("HIST");
    h_BBbar->Draw("HIST same");

    //hc->SetMaximum(1.1*hc->GetMaximum());
    Float_t rightmax = 3*h_BBbar->GetMaximum();
    if(rightmax <= 0) rightmax = 1;
    Float_t scale = hc->GetMaximum()/rightmax;
    h_BBbar->Scale(scale);

    hc->GetXaxis()->SetTitle("qr_{FBDT}");
    hc->GetYaxis()->SetTitle(ytitle);
    hc->GetXaxis()->SetTitleOffset(0.90);
    hc->GetYaxis()->SetTitleOffset(1.20);

    auto axis_x = hc->GetXaxis(); 
    TGaxis* axis_right = new TGaxis(axis_x->GetXmax(),0,
                                axis_x->GetXmax(), hc->GetMaximum(),
                                0,rightmax,510,"+L");
    axis_right->SetLineColor(kBlue);
    axis_right->SetLabelColor(kBlue);
    axis_right->Draw();
    //h_sigMC->SetMaximum(10000000);
    
    TLegend *legend = new TLegend(0.30,0.60,0.50,0.84);
    legend->AddEntry("h_sigMC_B0","SigMC B_{0}","f");
    legend->AddEntry("h_sigMC_B0bar","SigMC #bar{B}_{0}","f");
    legend->AddEntry("h_sigMC_other", "SigMC qrMC=0", "f");
    legend->AddEntry("h_BBbar", "generic bkg", "f");

    legend->SetBorderSize(0);
    legend->SetFillColor(0);
    legend->Draw();

    TArrow *xy1 = new TArrow(0.89, 0, 0.89, hc->GetMaximum()*0.3,0.015,"<");
    xy1->SetLineColor(kRed);xy1->SetLineWidth(3);
    xy1->Draw();

    TArrow *xy2 = new TArrow(0.145, 0, 0.145, hc->GetMaximum()*0.3,0.015,"<");
    xy2->SetLineColor(kBlue);xy2->SetLineWidth(2);
    //xy2->Draw();

    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/flavor_tag/png/dis_FBDT_qr_signMC_bbar.png");
    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/flavor_tag/pdf/dis_FBDT_qr_signMC_bbar.pdf");

    //cout<<h_B0->GetEntries()+h_B0bar->GetEntries()+h_other->GetEntries()<<endl;
}
