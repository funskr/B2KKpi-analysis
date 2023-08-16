void mkk_fit_sample(){
    TChain* chain = new TChain("B0");

    chain->Add("/home/belle2/yuanmk/data/B2KKpi/B2KKpi_bbar_20230716_night/sub00/*.root");
    chain->Add("/home/belle2/yuanmk/data/B2KKpi/B2KKpi_bbar_20230716_night/sub01/*.root");
    chain->Add("/home/belle2/yuanmk/data/B2KKpi/B2KKpi_bbar_20230716_night/sub02/*.root");

    //set variables
    Double_t dr, dz;
    chain->SetBranchAddress("dr", &dr);
    chain->SetBranchAddress("dz", &dz);

    Double_t FBDT_qrCombined;
    chain->SetBranchAddress("FBDT_qrCombined", &FBDT_qrCombined);  

    Double_t Kp_px, Kp_py, Kp_pz, Kp_E;
    chain->SetBranchAddress("Kp_px", &Kp_px);
    chain->SetBranchAddress("Kp_py", &Kp_py);
    chain->SetBranchAddress("Kp_pz", &Kp_pz);
    chain->SetBranchAddress("Kp_E", &Kp_E);

    Double_t Km_px, Km_py, Km_pz, Km_E;
    chain->SetBranchAddress("Km_px", &Km_px);
    chain->SetBranchAddress("Km_py", &Km_py);
    chain->SetBranchAddress("Km_pz", &Km_pz);
    chain->SetBranchAddress("Km_E", &Km_E);

    Double_t pi0_daughterAngle_0_1;
    chain->SetBranchAddress("pi0_daughterAngle_0_1", &pi0_daughterAngle_0_1);

    TFile* file = new TFile("./mkk_fit_sample.root","recreate");

    TTree* tree =  new TTree("B0", "mkk");
    Double_t InvM_KK;
    tree->Branch("InvM_KK", &InvM_KK, "InvM_KK/D");

    //Double_t Kp_E_s = 0; Double_t Km_E_s = 0;
    Double_t E_tot = 0; Double_t px_tot = 0; Double_t py_tot = 0; Double_t pz_tot = 0;

    const Double_t mass_K = 0.493677;
    Int_t nentries = chain->GetEntries();
    for(int i=0; i<nentries; i++){
        chain->GetEntry(i);
        if(dr>0.04) continue;
        if(fabs(dz)>0.2) continue;
        if(FBDT_qrCombined>0.72) continue;
        if(pi0_daughterAngle_0_1>0.4) continue;

        //Kp_E_s = sqrt(Kp_px*Kp_px + Kp_px*Kp_px + Kp_px*Kp_px + mass_K * mass_K);
        //Km_E_s = sqrt(Km_px*Km_px + Km_px*Km_px + Km_px*Km_px + mass_K * mass_K);
        E_tot = Km_E + Kp_E;

        px_tot = Kp_px + Km_px;
        py_tot = Kp_py + Km_py;
        pz_tot = Kp_pz + Km_pz;

        InvM_KK = sqrt(E_tot*E_tot - (px_tot*px_tot + py_tot*py_tot + pz_tot*pz_tot));
        
        tree->Fill();
    }

    tree->Write();
    file->Close();
}