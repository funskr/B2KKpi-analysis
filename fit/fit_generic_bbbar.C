#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
using namespace RooFit;

void fit_generic_bbbar(){

    const Double_t xmin_de = -0.3; const Double_t xmax_de = 0.15;
    const Double_t xmin_cp = -4; const Double_t xmax_cp = 4;

    //***************pdf of deltaE****************
    RooRealVar de("deltaE", "deltaE", xmin_de, xmax_de);

    //second-order polynomial function
    RooRealVar c0("c0","coefficient #0", -3.38584e-01, -10, 10);
    RooRealVar c1("c1","coefficient #1", 3.19969e-02, -10, 10);
    RooChebychev model_de("model_de", " ", de, RooArgSet(c0, c1));

    //RooAddPdf model_de("model_de", "model for deltaE", ploy2);

    //*******************pdf of FBDT' *******************
    RooRealVar cp("ContProb_trans", "ContProb_trans", xmin_cp, xmax_cp);

    RooRealVar mean1("mean1", "mean of first bifurgauss", 1.24365e-01, -10, 10);
    RooRealVar sigmaL1("sigmaL1", " ", 4.53023e-01, -10, 10);
    RooRealVar sigmaR1("sigmaR1", " ", 9.83091e-01, -10, 10);
    RooBifurGauss bfgauss1("bfgauss1", "bf gauss 1 for ContProb_trans", cp, mean1, sigmaL1, sigmaR1);

    RooRealVar mean2("mean2", "mean of first bifurgauss", 5.32730e-02, -10, 10);
    RooRealVar sigmaL2("sigmaL2", " ", 7.41844e-01, -10, 10);
    RooRealVar sigmaR2("sigmaR2", " ", 5.98606e-01, -10, 10);
    RooBifurGauss bfgauss2("bfgauss2", "bf gauss 1 for ContProb_trans", cp, mean2, sigmaL2, sigmaR2);

    RooRealVar f_cp("f_cp", "", 2.99761e-01, 0., 1);
    RooAddPdf model_cp("model_cp", "model for ContProb_trans", RooArgList(bfgauss1, bfgauss2), f_cp);

    //2 D dimensional model
    RooProdPdf model_final("model_final", "model_final", RooArgSet(model_de, model_cp));

    //load data
    TChain* chain = new TChain("B0");
    chain->Add("/home/belle2/yuanmk/data/B2KKpi/sample/ana/mixed_sample.root");
    chain->Add("/home/belle2/yuanmk/data/B2KKpi/sample/ana/charged_sample.root");

    string cuts;
    cuts = "ContProb<0.4&&flag_candidate==1&&";
    cuts += "(!(InvM_KpKm>1.8484&&InvM_KpKm<1.8806))&&";
    cuts += "(!(InvM_KmPip > 1.8408 && InvM_KmPip < 1.8875))&&";
    cuts += "(!(InvM_KpPim > 1.8408 && InvM_KpPim < 1.8875))&&";
    cuts += "(!(abs(mcPDG) == 511 && pi0_mcPDG == 111 && Kp_mcPDG == 321 && Km_mcPDG == -321))&&";
    cuts += "(!(abs(mcPDG) == 511 && pi0_mcPDG == 111 && (Kp_mcPDG == 211 || Km_mcPDG == -211)))";
    //cuts += "isSignal==1";

    TTree* tree = chain->CopyTree(cuts.c_str());

    //RooDataSet* data = model_final.generate(RooArgSet(de, cp), 10000);
    RooDataSet data("data"," ", tree, RooArgSet(de, cp));
    model_final.fitTo(data);

    RooPlot *frame_de = de.frame(Title(" "));
    data.plotOn(frame_de);
    model_de.plotOn(frame_de);

    RooPlot *frame_cp = cp.frame(Title(" "));
    data.plotOn(frame_cp);
    model_cp.plotOn(frame_cp);
    model_cp.plotOn(frame_cp, Components(bfgauss1), LineStyle(kDashed), LineColor(kRed));
    model_cp.plotOn(frame_cp, Components(bfgauss2), LineStyle(kDashed), LineColor(kGreen));

    // Draw all frames on a canvas
    TCanvas *c = new TCanvas("c1", "c1", 1600, 600);
    c->Divide(2);
    c->cd(1);
    gPad->SetLeftMargin(0.15);
    frame_de->GetYaxis()->SetTitleOffset(1.6);
    frame_de->GetXaxis()->SetTitle("#Delta E");
    frame_de->GetYaxis()->SetTitle("Events");
    frame_de->Draw(); 

    //draw the output
    Double_t ymax_de = frame_de->GetMaximum();
    TPaveText *pt_de = new TPaveText(xmin_de+0.02*(xmax_de-xmin_de), ymax_de*0.10, xmin_de+0.3*(xmax_de-xmin_de), ymax_de*0.40);
    string str_c0 = "c0 = " + to_string(c0.getValV()) + " #pm " + to_string(c0.getError());
    string str_c1 = "c1 = " + to_string(c1.getValV()) + " #pm " + to_string(c1.getError());

    pt_de->AddText(str_c0.c_str());
    pt_de->AddText(str_c1.c_str());
    pt_de->Draw();

    c->cd(2);
    frame_cp->GetYaxis()->SetTitleOffset(1.6);
    frame_cp->GetXaxis()->SetTitle("ContProb_trans");
    frame_cp->GetYaxis()->SetTitle("Events");
    frame_cp->Draw(); 

    Double_t ymax_cp = frame_cp->GetMaximum();
    TPaveText *pt_cp = new TPaveText(xmin_cp+0.02*(xmax_cp-xmin_cp), ymax_cp*0.65, xmin_cp+0.32*(xmax_cp-xmin_cp), ymax_cp*0.95);
    
    string str_mean1 = "mean1 = " + to_string(mean1.getValV()) + " #pm " + to_string(mean1.getError());
    string str_sigmaL1 = "sigmaL1 = " + to_string(sigmaL1.getValV()) + " #pm " + to_string(sigmaL1.getError());
    string str_sigmaR1 = "sigmaR1 = " + to_string(sigmaR1.getValV()) + " #pm " + to_string(sigmaR1.getError());
    string str_mean2 = "mean2 = " + to_string(mean2.getValV()) + " #pm " + to_string(mean2.getError());
    string str_sigmaL2 = "sigmaL2 = " + to_string(sigmaL2.getValV()) + " #pm " + to_string(sigmaL2.getError());
    string str_sigmaR2 = "sigmaR2 = " + to_string(sigmaR2.getValV()) + " #pm " + to_string(sigmaR2.getError());
    string str_f_cp = "f_cp = " + to_string(f_cp.getValV()) + " #pm " + to_string(f_cp.getError());

    pt_cp->AddText(str_mean1.c_str());
    pt_cp->AddText(str_sigmaL1.c_str());
    pt_cp->AddText(str_sigmaR1.c_str());
    pt_cp->AddText(str_mean2.c_str());
    pt_cp->AddText(str_sigmaL2.c_str());
    pt_cp->AddText(str_sigmaR2.c_str());
    pt_cp->AddText(str_f_cp.c_str());
    pt_cp->Draw();

    c->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/fit_MC/pdf/fit_generic_bbbar.pdf");
    c->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/fit_MC/png/fit_generic_bbbar.png");
}