void sigMC_sample(){
    TChain* chain = new TChain("B0");

    chain->Add("/home/belle2/yuanmk/data/B2KKpi/sigMC/root/B2KKpi_all.root");

    TTree* tree =  chain->CopyTree("");

    //set variables
    Int_t run, event;
    tree->SetBranchAddress("__run__", &run);
    tree->SetBranchAddress("__event__", &event);
    
    Int_t ncandidates, candidate;
    tree->SetBranchAddress("__ncandidates__", &ncandidates);
    tree->SetBranchAddress("__candidate__", &candidate);

    Double_t vertex_chi2,chiSqrd;
    tree->SetBranchAddress("vertex_chi2", &vertex_chi2);
    tree->SetBranchAddress("chiSqrd", &chiSqrd);

    Double_t dr, dz;
    tree->SetBranchAddress("dr", &dr);
    tree->SetBranchAddress("dz", &dz);

    Double_t FBDT_qrCombined;
    tree->SetBranchAddress("FBDT_qrCombined", &FBDT_qrCombined);  

    Double_t Kp_px, Kp_py, Kp_pz, Kp_E;
    tree->SetBranchAddress("Kp_px", &Kp_px);
    tree->SetBranchAddress("Kp_py", &Kp_py);
    tree->SetBranchAddress("Kp_pz", &Kp_pz);
    tree->SetBranchAddress("Kp_E", &Kp_E);

    Double_t Km_px, Km_py, Km_pz, Km_E;
    tree->SetBranchAddress("Km_px", &Km_px);
    tree->SetBranchAddress("Km_py", &Km_py);
    tree->SetBranchAddress("Km_pz", &Km_pz);
    tree->SetBranchAddress("Km_E", &Km_E);

    Double_t InvM_KK;
    TBranch* branch_InvM_KK = tree->Branch("InvM_KK", &InvM_KK, "InvM_KK/D");

    Double_t InvM_KpPim, InvM_KmPip;
    TBranch* branch_InvM_KpPim = tree->Branch("InvM_KpPim", &InvM_KpPim, "InvM_KpPim/D");
    TBranch* branch_InvM_KmPip = tree->Branch("InvM_KmPip", &InvM_KmPip, "InvM_KmPip/D");

    Int_t flag_candidate;
    TBranch* branch_flag_candidate = tree->Branch("flag_candidate", &flag_candidate, "flag_candidate/I"); 

    //Double_t Kp_E_s = 0; Double_t Km_E_s = 0;
    Double_t pip_E = 0; Double_t pim_E = 0;
    Double_t px_tot = 0; Double_t py_tot = 0; Double_t pz_tot = 0;
    Double_t E_KpPim = 0; Double_t E_KmPip = 0; Double_t E_tot = 0;
    const Double_t mass_pi = 0.1349768;

    //entrys to be deleted
    vector<int> vec_entrys;
    
    Int_t nentries = chain->GetEntries();

    double remain_vertex_chi2, remain_chiSqrd;
    int remain_entry;
    for(int i=0; i<nentries; i++){
        tree->GetEntry(i);

        if( ncandidates > 1 ){

            if( candidate == 0 ){
                remain_vertex_chi2 = vertex_chi2;
                remain_chiSqrd = chiSqrd;
                remain_entry = i;
            }
            else{
                if(chiSqrd < remain_chiSqrd){
                    vec_entrys.push_back(remain_entry);

                    remain_vertex_chi2 = vertex_chi2;
                    remain_chiSqrd = chiSqrd;
                    remain_entry = i;
                }
                else if(chiSqrd > remain_chiSqrd){
                    vec_entrys.push_back(i);
                }
                else if(chiSqrd == remain_chiSqrd){
                    if(vertex_chi2 > remain_vertex_chi2){
                        vec_entrys.push_back(remain_entry);
                        
                        remain_vertex_chi2 = vertex_chi2;
                        remain_chiSqrd = chiSqrd;
                        remain_entry = i;
                    }
                    else if(vertex_chi2 < remain_vertex_chi2){
                        vec_entrys.push_back(i);
                    }
                }
            }
        }
    }

    for(int i=0; i<nentries; i++){
        tree->GetEntry(i);
        //if(dr>0.04) continue;
        //if(fabs(dz)>0.2) continue;
        //if(FBDT_qrCombined>0.72) continue;

        //give a flag forn candidate remained
        flag_candidate = 1;
        for(int j=0; j<vec_entrys.size(); j++){
            if(i == vec_entrys[j]) flag_candidate = 0;
        }

        pip_E = sqrt(Kp_px*Kp_px + Kp_py*Kp_py + Kp_pz*Kp_pz + mass_pi * mass_pi);
        pim_E = sqrt(Km_px*Km_px + Km_py*Km_py + Km_pz*Km_pz + mass_pi * mass_pi);

        E_KpPim = Kp_E + pim_E;
        E_KmPip = Km_E + pip_E;
        E_tot = Km_E + Kp_E;

        px_tot = Kp_px + Km_px;
        py_tot = Kp_py + Km_py;
        pz_tot = Kp_pz + Km_pz;

        InvM_KpPim = sqrt(E_KpPim*E_KpPim - (px_tot*px_tot + py_tot*py_tot + pz_tot*pz_tot));
        InvM_KmPip = sqrt(E_KmPip*E_KmPip - (px_tot*px_tot + py_tot*py_tot + pz_tot*pz_tot));
        InvM_KK = sqrt(E_tot*E_tot - (px_tot*px_tot + py_tot*py_tot + pz_tot*pz_tot));

        branch_InvM_KK->Fill();
        branch_InvM_KpPim->Fill();
        branch_InvM_KmPip->Fill();
        branch_flag_candidate->Fill();
    }

    TFile* file = new TFile("/home/belle2/yuanmk/data/B2KKpi/sigMC/root/sigMC_sample.root","recreate");
    tree->Write();
    file->Close();
}