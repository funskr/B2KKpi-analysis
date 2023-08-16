
void cut_flow(){

    TChain* chain = new TChain("B0");

    chain->Add("/home/belle2/yuanmk/data/B2KKpi/sigMC/root/sigMC_sample.root");

    cout<<"cut flow for signal MC"<<endl;
    cout<<"cut:eff:scf"<<endl;
    string cut[3] = {"dr<0.04&&fabs(dz)<0.2&&pi0_daughterAngle_0_1<0.5&&flag_candidate==1", "&&FBDT_qrCombined<0.72", "&&!(InvM_KK>1.849&&InvM_KK<1.879)"};
    string name[3] = {"fundemental", "FBDT", "mkk veto"};
    double eff[3]; double scf[3];
    string cuts = "";
    double gennum = 1e+5;
    for(int i=0; i<3; i++){
        cuts += cut[i];
        string cuts_true = cuts + "&&isSignal==1";

        TTree *tot = chain->CopyTree(cuts.c_str());
        TTree *ture = chain->CopyTree(cuts_true.c_str());
        //TTree *mix = chain->CopyTree(cuts_true.c_str());

        eff[i] = ((double)tot->GetEntries())/gennum;
        //cout<<ture->GetEntries()<<":"<<tot->GetEntries()<<endl;
        scf[i] = 1. - ((double)ture->GetEntries())/((double)tot->GetEntries());

        cout<<name[i]<<":"<<eff[i]<<":"<<scf[i]<<endl;
    }
}