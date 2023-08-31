
struct count_pair{
    Double_t before_cut;
    Double_t after_cut;
};

count_pair get_count(string address, string branch){

    TChain* chain = new TChain(branch.c_str());

    chain->Add(address.c_str());

    Double_t ContProb;
    chain->SetBranchAddress("ContProb", &ContProb);

    Int_t flag_candidate;
    chain->SetBranchAddress("flag_candidate", &flag_candidate);

    Int_t nentries = chain->GetEntries();

    count_pair result;
    result.before_cut = 0;
    result.after_cut = 0;
    for(Int_t i=0; i < nentries; i++){
        chain->GetEntry(i);
        if(flag_candidate==0) continue;
        result.before_cut++;
        if(ContProb > 0.4) continue;
        result.after_cut++;
    }

    return result;
}


void count_eff(){

    count_pair sigMC;
    sigMC=get_count("/home/belle2/yuanmk/data/B2KKpi/sample/ana/sigMC_sample.root","B0");

    count_pair qqbarMC[4];
    qqbarMC[0] = get_count("/home/belle2/yuanmk/data/B2KKpi/sample/ana/uubar_sample.root","B0");
    qqbarMC[1] = get_count("/home/belle2/yuanmk/data/B2KKpi/sample/ana/ddbar_sample.root","B0");
    qqbarMC[2] = get_count("/home/belle2/yuanmk/data/B2KKpi/sample/ana/ssbar_sample.root","B0");
    qqbarMC[3] = get_count("/home/belle2/yuanmk/data/B2KKpi/sample/ana/ccbar_sample.root","B0");


    count_pair cs;
    for(int i = 0; i < 4; i++){
        cs.before_cut += qqbarMC[i].before_cut;
        cs.after_cut += qqbarMC[i].after_cut;
    }
    Double_t cs_eff = (cs.before_cut-cs.after_cut)/cs.before_cut;

    cout<<"signal remain efficiency:"<<sigMC.after_cut/sigMC.before_cut<<endl;
    cout<<"continuum suppression efficiency: "<<cs_eff<<endl;
}