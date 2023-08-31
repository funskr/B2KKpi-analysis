void combine_sample(vector<string> list, string branch, string output, vector<string> var_str){
    TChain* chain = new TChain(branch.c_str());

    for(int i=0; i<list.size(); i++){
        cout<<"load file: "<<list[i]<<endl;
        chain->Add(list[i].c_str());
    }

    //copy trees
    const int num_var = var_str.size();
    vector<Double_t> var; 
    var.resize(var_str.size());
    for(int i=0; i<num_var; i++){
        //var.push_back(0);
        chain->SetBranchAddress(var_str[i].c_str(), &var[i]);
    }

    //cuts
    Int_t run, event;
    chain->SetBranchAddress("__run__", &run);
    chain->SetBranchAddress("__event__", &event);
    
    Int_t ncandidates, candidate;
    chain->SetBranchAddress("__ncandidates__", &ncandidates);
    chain->SetBranchAddress("__candidate__", &candidate);

    Double_t vertex_chi2,chiSqrd;
    chain->SetBranchAddress("vertex_chi2", &vertex_chi2);
    chain->SetBranchAddress("chiSqrd", &chiSqrd);

    Double_t dr, dz;
    chain->SetBranchAddress("dr", &dr);
    chain->SetBranchAddress("dz", &dz);

    Double_t gamma_coshelicity;
    chain->SetBranchAddress("gamma_coshelicity", &gamma_coshelicity);

    Double_t Kp_kaonID, Km_kaonID;
    chain->SetBranchAddress("Kp_kaonID", &Kp_kaonID);
    chain->SetBranchAddress("Km_kaonID", &Km_kaonID);

    //the variables used here will not be stored in the new tree
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

    Double_t pi0_px, pi0_py, pi0_pz, pi0_E;
    chain->SetBranchAddress("pi0_px", &pi0_px);
    chain->SetBranchAddress("pi0_py", &pi0_py);
    chain->SetBranchAddress("pi0_pz", &pi0_pz);
    chain->SetBranchAddress("pi0_E", &pi0_E);

    Double_t ContProb;
    chain->SetBranchAddress("ContProb", &ContProb);

    //*****************************
    TTree* tree =  new TTree("B0", " ");
    //copyed trees
    vector<Double_t> var_new;
    var_new.resize(var_str.size());
    for(int i=0; i<num_var; i++){
        //var_new.push_back(0);
        string var_str_tmp = var_str[i]+"/D";
        tree->Branch(var_str[i].c_str(), &var_new[i], var_str_tmp.c_str());
    }

    Double_t InvM_KpKm;
    tree->Branch("InvM_KpKm", &InvM_KpKm, "InvM_KpKm/D");

    Double_t InvM_KpPim, InvM_KmPip;
    tree->Branch("InvM_KpPim", &InvM_KpPim, "InvM_KpPim/D");
    tree->Branch("InvM_KmPip", &InvM_KmPip, "InvM_KmPip/D");

    Double_t InvM_KpPi0, InvM_KmPi0;
    tree->Branch("InvM_KpPi0", &InvM_KpPi0, "InvM_KpPi0/D");
    tree->Branch("InvM_KmPi0", &InvM_KmPi0, "InvM_KmPi0/D");

    Double_t ContProb_trans;
    tree->Branch("ContProb_trans", &ContProb_trans, "ContProb_trans/D");
    Double_t ContProb_new;
    tree->Branch("ContProb", &ContProb_new, "ContProb/D");

    Int_t flag_candidate;
    tree->Branch("flag_candidate", &flag_candidate, "flag_candidate/I"); 

    //Double_t Kp_E_s = 0; Double_t Km_E_s = 0;
    Double_t pip_E = 0; Double_t pim_E = 0;
    Double_t px_KpKm = 0; Double_t py_KpKm = 0; Double_t pz_KpKm = 0;
    Double_t px_KpPi0 = 0; Double_t py_KpPi0 = 0; Double_t pz_KpPi0 = 0;
    Double_t px_KmPi0 = 0; Double_t py_KmPi0 = 0; Double_t pz_KmPi0 = 0;

    Double_t E_KpPim = 0; Double_t E_KmPip = 0; Double_t E_KpKm = 0;
    Double_t E_KpPi0 = 0; Double_t E_KmPi0 = 0;

    const Double_t mass_pi = 0.1349768;

    //entrys to be deleted
    Int_t nentries = chain->GetEntries();
    cout<<nentries<<endl;
    vector<Int_t> vec_entrys;
    vec_entrys.clear();
    //vec_entrys.reserve(nentries);

    double remain_vertex_chi2, remain_chiSqrd;
    int remain_entry;

    for(int i=0; i<nentries; i++){
        chain->GetEntry(i);
        if( ncandidates > 1 ){
            if( candidate == 0 ){
                remain_vertex_chi2 = vertex_chi2;
                remain_chiSqrd = chiSqrd;
                remain_entry = i;
            }
            else{
                if(chiSqrd < remain_chiSqrd){
                    vec_entrys.push_back(remain_entry);
                    //vec_entrys[remain_entry] = 0;

                    remain_vertex_chi2 = vertex_chi2;
                    remain_chiSqrd = chiSqrd;
                    remain_entry = i;
                }
                else if(chiSqrd > remain_chiSqrd){
                    vec_entrys.push_back(i);
                    //vec_entrys[i] = 0;
                }
                else if(chiSqrd == remain_chiSqrd){
                    if(vertex_chi2 > remain_vertex_chi2){
                        vec_entrys.push_back(remain_entry);
                        //vec_entrys[remain_entry] = 0;
                        
                        remain_vertex_chi2 = vertex_chi2;
                        remain_chiSqrd = chiSqrd;
                        remain_entry = i;
                    }
                    else if(vertex_chi2 < remain_vertex_chi2){
                        vec_entrys.push_back(i);
                        //vec_entrys[i] = 0;
                    }
                }
            }
        }
        //cout<<vec_entrys.size()<<endl;
    }

    for(int i=0; i<nentries; i++){
        chain->GetEntry(i);
        if(dr>0.04) continue;
        if(fabs(dz)>0.2) continue;
        if(fabs(gamma_coshelicity)>0.95) continue;
        //if(Kp_kaonID < 0.2 || Km_kaonID < 0.2) continue;
        //if(pi0_daughterAngle_0_1>0.5) continue;

        //give a flag forn candidate remained
        flag_candidate = 1;
        for(int j=0; j<vec_entrys.size(); j++){
            if(i == vec_entrys[j]) flag_candidate = 0;
        }
        //flag_candidate = vec_entrys[i];

        pip_E = sqrt(Kp_px*Kp_px + Kp_py*Kp_py + Kp_pz*Kp_pz + mass_pi * mass_pi);
        pim_E = sqrt(Km_px*Km_px + Km_py*Km_py + Km_pz*Km_pz + mass_pi * mass_pi);

        E_KpPim = Kp_E + pim_E;
        E_KmPip = Km_E + pip_E;
        E_KpPi0 = Kp_E + pi0_E;
        E_KmPi0 = Km_E + pi0_E;
        E_KpKm = Km_E + Kp_E;

        px_KpKm = Kp_px + Km_px;
        py_KpKm = Kp_py + Km_py;
        pz_KpKm = Kp_pz + Km_pz;

        px_KpPi0 = Kp_px + pi0_px;
        py_KpPi0 = Kp_py + pi0_py;
        pz_KpPi0 = Kp_pz + pi0_pz;

        px_KmPi0 = Km_px + pi0_px;
        py_KmPi0 = Km_py + pi0_py;
        pz_KmPi0 = Km_pz + pi0_pz;

        InvM_KpPim = sqrt(E_KpPim*E_KpPim - (px_KpKm*px_KpKm + py_KpKm*py_KpKm + pz_KpKm*pz_KpKm));
        InvM_KmPip = sqrt(E_KmPip*E_KmPip - (px_KpKm*px_KpKm + py_KpKm*py_KpKm + pz_KpKm*pz_KpKm));
        InvM_KpKm = sqrt(E_KpKm*E_KpKm - (px_KpKm*px_KpKm + py_KpKm*py_KpKm + pz_KpKm*pz_KpKm));
        InvM_KpPi0 = sqrt(E_KpPi0*E_KpPi0 - (px_KpPi0*px_KpPi0 + py_KpPi0*py_KpPi0 + pz_KpPi0*pz_KpPi0));
        InvM_KmPi0 = sqrt(E_KmPi0*E_KmPi0 - (px_KmPi0*px_KmPi0 + py_KmPi0*py_KmPi0 + pz_KmPi0*pz_KmPi0));

        for(int j=0; j<num_var; j++){
            var_new[j] = var[j];
        }

        ContProb_new = ContProb;
        if(ContProb<0.4)
            ContProb_trans = log10((ContProb-0)/(0.4-ContProb));
        else
            ContProb_trans = -9;

        tree->Fill();
    }

    TFile* file = new TFile(output.c_str(), "recreate");
    tree->Write();
    file->Close();
    cout<<output<<": file outputed"<<endl;
    delete tree; delete chain;
}

void load_vec_string(string file, vector<string>& list){
    ifstream f;
    f.open(file.c_str(), ios::in);

    //vector<string> list;
    //list.clear();

    if(f.is_open()){
        cout<<file<<": file load successfully"<<endl;
        string buff;
        while(getline(f,buff)){
            //cout<<buff<<endl;
            if(buff[0]=='#') continue;
            list.push_back(buff);
        }  
    }
    else{
        cout<<file<<":configure file load error"<<endl;
        //return false;
    }
    
    f.close();
    return 0;
}

void load_config(vector<vector<string>>& config){
    ifstream f;
    f.open("config", ios::in);

    vector<string> list;
    list.clear();

    if(f.is_open()){
        string buff;
        vector<int> split;
        vector<string> config_tmp;
        char split_str=':';
        while(getline(f,buff)){
            //cout<<buff<<endl;
            if(buff[0]=='#') continue;
            split.push_back(0);
            for(int j=0; j<buff.length(); j++){
                if(buff[j]==split_str){
                        split.push_back(j);
                        //cout<<j<<endl;
                }
            }
            split.push_back(buff.length());

            config_tmp.clear();
            for(int j=0; j<split.size()-1; j++){
                if(j==0){
                    config_tmp.push_back(buff.substr(split[j],split[j+1]-split[j]));
                }
                else{
                    config_tmp.push_back(buff.substr(split[j]+1,split[j+1]-split[j]-1));
                }
            }
            config.push_back(config_tmp);

            split.clear();
        }
        cout<<"config file load successfully"<<endl;
        //f.close();
    }
    else{
        cout<<"configure file load error!!!"<<endl;
    }
    f.close();
    return 0;
}

void sample(){
    //list:branch:output
    vector<vector<string>> config;
    load_config(config);

    string root_list_tmp;
    vector<string> var_list_combine;

    vector<string> root_list;
    vector<string> var_list;
    
    for(int i=0; i<config.size(); i++){
        for(auto &x : config[i]) cout<<x<<endl;
        //cout<<config[i][0]<<endl<<config[i][1]<<endl<<config[i][2]<<endl;
        
        //load root files list
        root_list_tmp = "./list/" + config[i][0];
        root_list.clear();
        load_vec_string(root_list_tmp, root_list);

        //load var list
        var_list_combine.clear();
        var_list.clear();
        load_vec_string("var.config", var_list_combine);

        string var_list_tmp;
        for(int j=0; j<var_list_combine.size(); j++){
            var_list_tmp = "./var/" + var_list_combine[j];
            load_vec_string(var_list_tmp, var_list);
        }
        //cout<<var_list_tmp<<endl;
        //test
        for(auto &x : root_list) cout<<x<<endl;
        //for(auto &x : var_list) cout<<x<<endl;

        combine_sample(root_list, config[i][1], config[i][2], var_list);

    }

}