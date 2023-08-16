

void mix_sample(){

    TChain* chain_mix = new TChain("B0");

    //signal MC
    chain_mix->Add("/home/belle2/yuanmk/analysis/B2KKpi/continuum/B2KKpi_cs_signalMC.root");

    //uubar MC
    chain_mix->Add("/home/belle2/yuanmk/analysis/B2KKpi/continuum/B2KKpi_cs_signalMC.root");

    //ddbar MC

    //ssbar MC

    //ccbar MC


    TFile *file = new TFile("./mix_sample.root");

    TTree* tree_mix =  chain_mix->CopyTree("");
    tree_mix->Write();
    file->Close();

}