#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
//#include <RooStats/CorrelationHistFactory.h>
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
using namespace RooFit;
using namespace RooStats;

void fit_truth_matched_signal(){

    const Double_t xmin_de = -0.3; const Double_t xmax_de = 0.15;
    const Double_t xmin_cp = -4; const Double_t xmax_cp = 4;

    //***************pdf of deltaE****************
    RooRealVar de("deltaE", "deltaE", xmin_de, xmax_de);

    //crystal ball function
    RooRealVar mean_de("mean_de", "mean for the ", -1.84397e-02, -100, 100);
    RooRealVar CBSigma_de("CBSigma_de", " ", 2.59071e-02, -10, 10);
    RooRealVar CBAlpha_de("CBAlpha_de", " ", 7.43213e-01, -10, 10);
    RooRealVar CBN_de("CBN_de", "dimension for CB", 1.72161e+00, -10, 10);

    RooCrystalBall CB_de("CB_de", "crystall component for deltaE", de, mean_de, CBSigma_de, CBAlpha_de, CBN_de);
    
    //bifurcated gaussian
    RooRealVar sigmaL_de("sigmaL_de", " ", 4.63373e-02, -10, 10);
    RooRealVar sigmaR_de("sigmaR_de", " ", 9.29283e-02, -10, 10);

    RooBifurGauss CBbfgauss("CBbfgauss", "bf gauss for deltaE", de, mean_de, sigmaL_de, sigmaR_de);

    RooRealVar f_de("f_de", "", 8.94584e-01, 0., 1);
    RooAddPdf model_de("model_de", "model for deltaE", RooArgList(CB_de, CBbfgauss), f_de);

    //*******************pdf of FBDT' *******************
    RooRealVar cp("ContProb_trans", "ContProb_trans", xmin_cp, xmax_cp);

    RooRealVar mean1("mean1", "mean of first bifurgauss", -1.07054e+00, -10, 10);
    RooRealVar sigmaL1("sigmaL1", " ", 1.90164e-06, -10, 10);
    RooRealVar sigmaR1("sigmaR1", " ", 1.44071e+00, -10, 10);
    RooBifurGauss bfgauss1("bfgauss1", "bf gauss 1 for ContProb_trans", cp, mean1, sigmaL1, sigmaR1);

    RooRealVar mean2("mean2", "mean of second bifurgauss", -1.04308e+00, -10, 10);
    RooRealVar sigmaL2("sigmaL2", " ", 6.73310e-01, -10, 10);
    RooRealVar sigmaR2("sigmaR2", " ", 8.89630e-01, -10, 10);
    RooBifurGauss bfgauss2("bfgauss2", "bf gauss 2 for ContProb_trans", cp, mean2, sigmaL2, sigmaR2);

    RooRealVar mean3("mean3", "mean of third bifurgauss", -1.77573e+00, -10, 10);
    RooRealVar sigmaL3("sigmaL3", " ", 7.10922e-01, -10, 10);
    RooRealVar sigmaR3("sigmaR3", " ", 3.85614e-01, -10, 10);
    RooBifurGauss bfgauss3("bfgauss3", "bf gauss 3 for ContProb_trans", cp, mean3, sigmaL3, sigmaR3);

    RooRealVar f_cp1("f_cp1", "", 5.30805e-02, 0., 1);
    RooRealVar f_cp2("f_cp2", "", 7.39322e-01, 0., 1);
    RooAddPdf model_cp("model_cp", "model for ContProb_trans", RooArgList(bfgauss1, bfgauss2, bfgauss3), RooArgList(f_cp1, f_cp2));

    //2 D dimensional model
    RooProdPdf model_final("model_final", "model_final", RooArgSet(model_de, model_cp));

    //load data
    TChain* chain = new TChain("B0");
    chain->Add("/home/belle2/yuanmk/data/B2KKpi/sample/ana/sigMC_sample.root");

    string cuts;
    cuts = "ContProb<0.4&&flag_candidate==1&&";
    cuts += "FBDT_qrCombined<0.89&&";
    cuts += "(!(InvM_KpKm>1.8484&&InvM_KpKm<1.8806))&&";
    cuts += "(!(InvM_KmPip > 1.8408 && InvM_KmPip < 1.8875))&&";
    cuts += "(!(InvM_KpPim > 1.8408 && InvM_KpPim < 1.8875))&&";
    cuts += "isSignal==1";

    TTree* tree = chain->CopyTree(cuts.c_str());

    //RooDataSet* data = model_final.generate(RooArgSet(de, cp), 10000);
    RooDataSet data("data"," ", tree, RooArgSet(de, cp));

    //CorrelationHistFactory factory(data);

    model_final.fitTo(data);
    cout<<"Correlation between deltaE and ContProb: "<<data.correlation(de, cp)<<endl;

    RooPlot *frame_de = de.frame(Title(" "));
    data.plotOn(frame_de);
    model_de.plotOn(frame_de);
    model_de.plotOn(frame_de, Components(CB_de), LineStyle(kDashed), LineColor(kRed));
    model_de.plotOn(frame_de, Components(CBbfgauss), LineStyle(kDashed), LineColor(kGreen));

    RooPlot *frame_cp = cp.frame(Title(" "));
    data.plotOn(frame_cp);
    model_cp.plotOn(frame_cp);
    model_cp.plotOn(frame_cp, Components(bfgauss1), LineStyle(kDashed), LineColor(kRed));
    model_cp.plotOn(frame_cp, Components(bfgauss2), LineStyle(kDashed), LineColor(kGreen));
    model_cp.plotOn(frame_cp, Components(bfgauss3), LineStyle(kDashed), LineColor(kViolet));

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
    TPaveText *pt_de = new TPaveText(xmin_de+0.02*(xmax_de-xmin_de), ymax_de*0.65, xmin_de+0.3*(xmax_de-xmin_de), ymax_de*0.95);
    string str_mean_de = "mean_de = " + to_string(mean_de.getValV()) + " #pm " + to_string(mean_de.getError());
    string str_CBSigma_de = "CBSigma_de = " + to_string(CBSigma_de.getValV()) + " #pm " + to_string(CBSigma_de.getError());
    string str_CBAlpha_de = "CBAlpha_de = " + to_string(CBAlpha_de.getValV()) + " #pm " + to_string(CBAlpha_de.getError());
    string str_CBN_de = "CBN_de = " + to_string(CBN_de.getValV()) + " #pm " + to_string(CBN_de.getError());

    string str_sigmaL_de = "sigmaL_de = " + to_string(sigmaL_de.getValV()) + " #pm " + to_string(sigmaL_de.getError());
    string str_sigmaR_de = "sigmaR_de = " + to_string(sigmaR_de.getValV()) + " #pm " + to_string(sigmaR_de.getError());

    string str_f_de = "f_de = " + to_string(f_de.getValV()) + " #pm " + to_string(f_de.getError());

    pt_de->AddText(str_mean_de.c_str());
    pt_de->AddText(str_CBSigma_de.c_str());
    pt_de->AddText(str_CBAlpha_de.c_str());
    pt_de->AddText(str_CBN_de.c_str());
    pt_de->AddText(str_sigmaL_de.c_str());
    pt_de->AddText(str_sigmaR_de.c_str());
    pt_de->Draw();

    c->cd(2);
    frame_cp->GetYaxis()->SetTitleOffset(1.6);
    frame_cp->GetXaxis()->SetTitle("ContProb_trans");
    frame_cp->GetYaxis()->SetTitle("Events");
    frame_cp->Draw(); 

    //draw the output
    Double_t ymax_cp = frame_cp->GetMaximum();
    TPaveText *pt_cp = new TPaveText(xmin_cp+0.60*(xmax_cp-xmin_cp), ymax_cp*0.65, xmin_cp+0.9*(xmax_cp-xmin_cp), ymax_cp*0.95);
    
    Double_t diff1 = mean2.getValV() - mean1.getValV();
    Double_t diff1_err = sqrt(pow(mean1.getError(),2) + pow(mean2.getError(),2));
    string str_diff1 = "diff1 = " + to_string(diff1) + " #pm " + to_string(diff1_err);

    Double_t diff2 = mean3.getValV() - mean1.getValV();
    Double_t diff2_err = sqrt(pow(mean3.getError(),2) + pow(mean1.getError(),2));
    string str_diff2 = "diff2 = " + to_string(diff2) + " #pm " + to_string(diff2_err);

    Double_t ratio1 = sigmaL2.getValV() / sigmaL1.getValV();
    Double_t ratio1_err = sqrt(pow(ratio1/sigmaL2.getValV()*sigmaL2.getError(),2) + pow(ratio1/sigmaL1.getValV()*sigmaL1.getError(),2));
    string str_ratio1 = "ratio1 = " + to_string(ratio1) + " #pm " + to_string(ratio1_err);

    Double_t ratio2 = sigmaR2.getValV() / sigmaR1.getValV();
    Double_t ratio2_err = sqrt(pow(ratio2/sigmaR2.getValV()*sigmaR2.getError(),2) + pow(ratio2/sigmaR1.getValV()*sigmaR1.getError(),2));
    string str_ratio2 = "ratio2 = " + to_string(ratio2) + " #pm " + to_string(ratio2_err);

    Double_t ratio3 = sigmaL3.getValV() / sigmaL1.getValV();
    Double_t ratio3_err = sqrt(pow(ratio3/sigmaL1.getValV()*sigmaL1.getError(),2) + pow(ratio3/sigmaL3.getValV()*sigmaL3.getError(),2));
    string str_ratio3 = "ratio3 = " + to_string(ratio3) + " #pm " + to_string(ratio3_err);

    Double_t ratio4 = sigmaR3.getValV() / sigmaR1.getValV();
    Double_t ratio4_err = sqrt(pow(ratio4/sigmaR3.getValV()*sigmaR3.getError(),2) + pow(ratio4/sigmaR1.getValV()*sigmaR1.getError(),2));
    string str_ratio4 = "ratio4 = " + to_string(ratio4) + " #pm " + to_string(ratio4_err);

    string str_f_cp1 = "f_cp1 = " + to_string(f_cp1.getValV()) + " #pm " + to_string(f_cp1.getError());
    string str_f_cp2 = "f_cp2 = " + to_string(f_cp2.getValV()) + " #pm " + to_string(f_cp2.getError());

    pt_cp->AddText(str_diff1.c_str());
    pt_cp->AddText(str_diff2.c_str());
    pt_cp->AddText(str_ratio1.c_str());
    pt_cp->AddText(str_ratio2.c_str());
    pt_cp->AddText(str_ratio3.c_str());
    pt_cp->AddText(str_ratio4.c_str());
    pt_cp->AddText(str_f_cp1.c_str());
    pt_cp->AddText(str_f_cp2.c_str());
    pt_cp->Draw();

    c->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/fit_MC/pdf/fit_truth_matched_signal.pdf");
    c->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/fit_MC/png/fit_truth_matched_signal.png");
}