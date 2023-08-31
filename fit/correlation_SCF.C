
void correlation_SCF(){

    const Double_t xmin_de = -0.3; const Double_t xmax_de = 0.15;
    const Double_t xmin_cp = -4; const Double_t xmax_cp = 4;

    RooRealVar de("deltaE", "deltaE", xmin_de, xmax_de);

    RooRealVar cp("ContProb_trans", "ContProb_trans", xmin_cp, xmax_cp);

    TChain* chain = new TChain("B0");
    chain->Add("/home/belle2/yuanmk/data/B2KKpi/sample/ana/sigMC_sample.root");
    string cuts;
    cuts = "ContProb<0.4&&flag_candidate==1&&";
    cuts += "FBDT_qrCombined<0.89&&";
    cuts += "(!(InvM_KpKm>1.8484&&InvM_KpKm<1.8806))&&";
    cuts += "(!(InvM_KmPip > 1.8408 && InvM_KmPip < 1.8875))&&";
    cuts += "(!(InvM_KpPim > 1.8408 && InvM_KpPim < 1.8875))&&";
    cuts += "isSignal==0";

    TTree* tree = chain->CopyTree(cuts.c_str());
    RooDataSet data("data"," ", tree, RooArgSet(de, cp));
    cout<<"Correlation between deltaE and ContProb: "<<data.correlation(de, cp)<<endl;

}