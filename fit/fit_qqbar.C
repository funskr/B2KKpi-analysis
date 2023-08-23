#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
using namespace RooFit;

void fit_truth_matched_signal(){

    //***************pdf of deltaE****************
    RooRealVar de("deltaE", "deltaE", -0.3, 0.15);

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
    RooRealVar cp("ContProb_trans", "ContProb_trans", -4, 4);

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
    cuts += "(!(InvM_KpKm>1.8484&&InvM_KpKm<1.8806))&&";
    cuts += "(!(InvM_KmPip > 1.8408 && InvM_KmPip < 1.8875))&&";
    cuts += "(!(InvM_KpPim > 1.8408 && InvM_KpPim < 1.8875))&&";
    cuts += "isSignal==1";

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

    // Draw all frames on a canvas
    TCanvas *c = new TCanvas("c1", "c1", 800, 600);
    c->Divide(2);
    c->cd(1);
    gPad->SetLeftMargin(0.15);
    frame_de->GetYaxis()->SetTitleOffset(1.6);
    frame_de->GetXaxis()->SetTitle("#Delta E");
    frame_de->GetYaxis()->SetTitle("Events");
    frame_de->Draw(); 

    c->cd(2);
    frame_cp->GetYaxis()->SetTitleOffset(1.6);
    frame_cp->GetXaxis()->SetTitle("ContProb_trans");
    frame_cp->GetYaxis()->SetTitle("Events");
    frame_cp->Draw(); 

    c->SaveAs("./test.pdf");
}