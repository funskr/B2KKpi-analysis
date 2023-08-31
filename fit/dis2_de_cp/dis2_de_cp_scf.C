#include "/home/belle2/yuanmk/useful/boot.h"

void dis2_de_cp_scf(){
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

    TChain* chain = new TChain("B0");
    chain->Add("/home/belle2/yuanmk/data/B2KKpi/sample/ana/sigMC_sample.root");

    Double_t xmin = -0.3; Double_t xmax = 0.15; Int_t xbins = 100;
    Double_t ymin = -4; Double_t ymax = 4; Int_t ybins = 100;
    TH2D* h2_de_cp = new TH2D("h2_de_cp", "h2_de_cp", xbins, xmin, xmax, ybins, ymin, ymax);

    string cuts;
    cuts = "ContProb<0.4&&flag_candidate==1&&";
    cuts += "(!(InvM_KpKm>1.8484&&InvM_KpKm<1.8806))&&";
    cuts += "(!(InvM_KmPip > 1.8408 && InvM_KmPip < 1.8875))&&";
    cuts += "(!(InvM_KpPim > 1.8408 && InvM_KpPim < 1.8875))&&";
    cuts += "isSignal==0";

    chain->Draw("ContProb_trans:deltaE>>h2_de_cp", cuts.c_str());

    mbc->cd();
    h2_de_cp->Draw("colz");

    h2_de_cp->GetXaxis()->SetTitle("#Delta E/(GeV)");
    h2_de_cp->GetYaxis()->SetTitle("ContProb_{TRAN}");
    h2_de_cp->GetXaxis()->SetTitleOffset(1.0);
    h2_de_cp->GetYaxis()->SetTitleOffset(1.0);

    //draw the output
    TPaveText *pt = new TPaveText(xmin+0.02*(xmax-xmin), ymax*0.70, xmin+0.3*(xmax-xmin), ymax*0.97);
    pt->AddText("SCF signal MC");
    pt->Draw();


    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/correlation_deltaE_contProb_trans/pdf/dis2_de_cp_scf.pdf");
    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/correlation_deltaE_contProb_trans/png/dis2_de_cp_scf.png");
}