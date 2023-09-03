
void cut_flow(){

    TChain* sigMC = new TChain("B0");
    sigMC->Add("/home/belle2/yuanmk/data/B2KKpi/sample/ana/sigMC_sample.root");

    TChain* BBbar = new TChain("B0");
    BBbar->Add("/home/belle2/yuanmk/data/B2KKpi/sample/ana/mixed_sample.root");
    BBbar->Add("/home/belle2/yuanmk/data/B2KKpi/sample/ana/mixed_sample.root");

    TChain* qqbar = new TChain("B0");
    qqbar->Add("/home/belle2/yuanmk/data/B2KKpi/sample/ana/uubar_sample.root");
    qqbar->Add("/home/belle2/yuanmk/data/B2KKpi/sample/ana/ddbar_sample.root");
    qqbar->Add("/home/belle2/yuanmk/data/B2KKpi/sample/ana/ssbar_sample.root");
    qqbar->Add("/home/belle2/yuanmk/data/B2KKpi/sample/ana/ccbar_sample.root");

    cout<<"cut flow for signal MC"<<endl;
    cout<<"cut:eff:scf:bkg"<<endl;
    const int cut_num = 4;
    string cut[cut_num] = {
    "flag_candidate==1",
    "&&ContProb<0.4",
    "&&!(InvM_KpKm > 1.8484 && InvM_KpKm < 1.8806)&&!(InvM_KmPip > 1.8408 && InvM_KmPip < 1.8875)&&!(InvM_KpPim > 1.8408 && InvM_KpPim < 1.8875)",
    "&&FBDT_qrCombined<0.89"
    };
    string name[cut_num] = {"fundemental", "continuum suppression", "charm veto", "flavor tag"};
    double eff[cut_num]; double scf[cut_num]; double bkg[cut_num];
    string cuts = "";
    double gennum = 1e+5;
    for(int i=0; i<cut_num; i++){
        cuts += cut[i];
        string cuts_true = cuts + "&&isSignal==1";

        eff[i] = ((double)sigMC->GetEntries(cuts.c_str()))/gennum;
        //cout<<ture->GetEntries()<<":"<<tot->GetEntries()<<endl;
        scf[i] = 1. - ((double)sigMC->GetEntries(cuts_true.c_str()))/((double)sigMC->GetEntries(cuts.c_str()));
        bkg[i] = double(BBbar->GetEntries(cuts.c_str()))/3. + double(qqbar->GetEntries(cuts.c_str()));

        cout<<name[i]<<":"<<eff[i]<<":"<<scf[i]<<":"<<bkg[i]<<endl;
    }
}