#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
using namespace RooFit;

void fit_peaking_bbbar(){

    const Double_t xmin_de = -0.3; const Double_t xmax_de = 0.15;
    const Double_t xmin_cp = -4; const Double_t xmax_cp = 3;

    //***************pdf of deltaE****************
    RooRealVar de("deltaE", "deltaE", xmin_de, xmax_de);

    //double gaussian
    RooRealVar mean_de("mean_de", "mean of gaussians", 0.03, -0.3, 0.15);
    RooRealVar sigma_de("sigma_de", "width of gaussians", 0.1, 0, 10);
    RooGaussian gauss_de("gauss_de", " ", de, mean_de, sigma_de);

    RooRealVar c0("c0","coefficient #0", 0, -1, 1);
    RooRealVar c1("c1","coefficient #1", 0, -1, 1);
    RooChebychev ploy2("ploy2","background p.d.f.", de, RooArgSet(c0,c1));

    RooRealVar f_de("f_de", "", 0.3, 0., 1);
    RooAddPdf model_de("model_de", "model for deltaE", RooArgList(gauss_de, ploy2), f_de);

    //*******************pdf of FBDT' *******************
    RooRealVar cp("ContProb_trans", "ContProb_trans", xmin_cp, xmax_cp);

    RooRealVar mean1_cp("mean1_cp", "mean of first bifurgauss", 1.24365e-01, -10, 10);
    RooRealVar sigmaL1_cp("sigmaL1_cp", " ", 4.53023e-01, -10, 10);
    RooRealVar sigmaR1_cp("sigmaR1_cp", " ", 9.83091e-01, -10, 10);
    RooBifurGauss bfgauss1_cp("bfgauss1_cp", "bf gauss 1 for ContProb_trans", cp, mean1_cp, sigmaL1_cp, sigmaR1_cp);

    RooRealVar mean2_cp("mean2_cp", "mean of first bifurgauss", 1.24365e-01, -10, 10);
    RooRealVar sigmaL2_cp("sigmaL2_cp", " ", 4.53023e-01, -10, 10);
    RooRealVar sigmaR2_cp("sigmaR2_cp", " ", 9.83091e-01, -10, 10);
    RooBifurGauss bfgauss2_cp("bfgauss2_cp", "bf gauss 1 for ContProb_trans", cp, mean2_cp, sigmaL2_cp, sigmaR2_cp);

    RooRealVar f_cp("f_cp", "", 2.99761e-01, 0., 1);
    RooAddPdf model_cp("model_cp", "model for ContProb_trans", RooArgList(bfgauss1_cp, bfgauss2_cp), f_cp);

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
    cuts += "(abs(mcPDG) == 511 && pi0_mcPDG == 111 && (Kp_mcPDG == 211 || Km_mcPDG == -211))";
    //cuts += "isSignal==1";

    TTree* tree = chain->CopyTree(cuts.c_str());

    //RooDataSet* data = model_final.generate(RooArgSet(de, cp), 10000);
    RooDataSet data("data"," ", tree, RooArgSet(de, cp));
    model_final.fitTo(data);

    RooPlot *frame_de = de.frame(Title(" "));
    data.plotOn(frame_de);
    model_de.plotOn(frame_de);
    model_de.plotOn(frame_de, Components(gauss_de), LineStyle(kDashed), LineColor(kRed));
    model_de.plotOn(frame_de, Components(ploy2), LineStyle(kDashed), LineColor(kGreen));

    RooPlot *frame_cp = cp.frame(Title(" "));
    data.plotOn(frame_cp);
    model_cp.plotOn(frame_cp);
    model_cp.plotOn(frame_cp, Components(bfgauss1_cp), LineStyle(kDashed), LineColor(kRed));
    model_cp.plotOn(frame_cp, Components(bfgauss2_cp), LineStyle(kDashed), LineColor(kGreen));

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
    TPaveText *pt_de = new TPaveText(xmin_de+0.02*(xmax_de-xmin_de), ymax_de*0.70, xmin_de+0.3*(xmax_de-xmin_de), ymax_de*0.97);
    string str_mean_de = "mean_de = " + to_string(mean_de.getValV()) + " #pm " + to_string(mean_de.getError());
    string str_sigma_de = "sigma_de = " + to_string(sigma_de.getValV()) + " #pm " + to_string(sigma_de.getError());

    string str_c0 = "c0 = " + to_string(c0.getValV()) + " #pm " + to_string(c0.getError());
    string str_c1 = "c1 = " + to_string(c1.getValV()) + " #pm " + to_string(c1.getError());

    pt_de->AddText(str_mean_de.c_str());
    pt_de->AddText(str_sigma_de.c_str());
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
    
    string str_mean1_cp = "mean1_cp = " + to_string(mean1_cp.getValV()) + " #pm " + to_string(mean1_cp.getError());
    string str_sigmaL1_cp = "sigmaL1_cp = " + to_string(sigmaL1_cp.getValV()) + " #pm " + to_string(sigmaL1_cp.getError());
    string str_sigmaR1_cp = "sigmaR1_cp = " + to_string(sigmaR1_cp.getValV()) + " #pm " + to_string(sigmaR1_cp.getError());
    string str_mean2_cp = "mean2_cp = " + to_string(mean2_cp.getValV()) + " #pm " + to_string(mean2_cp.getError());
    string str_sigmaL2_cp = "sigmaL2_cp = " + to_string(sigmaL2_cp.getValV()) + " #pm " + to_string(sigmaL2_cp.getError());
    string str_sigmaR2_cp = "sigmaR2_cp = " + to_string(sigmaR2_cp.getValV()) + " #pm " + to_string(sigmaR2_cp.getError());

    pt_cp->AddText(str_mean1_cp.c_str());
    pt_cp->AddText(str_sigmaL1_cp.c_str());
    pt_cp->AddText(str_sigmaR1_cp.c_str());
    pt_cp->AddText(str_mean2_cp.c_str());
    pt_cp->AddText(str_sigmaL2_cp.c_str());
    pt_cp->AddText(str_sigmaR2_cp.c_str());    
    pt_cp->Draw();

    c->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/fit_MC/pdf/fit_peaking_bbbar.pdf");
    c->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/fit_MC/png/fit_peaking_bbbar.png");
}