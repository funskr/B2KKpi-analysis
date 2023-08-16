
void print_tree(){
    TChain* chain = new TChain("B0");

    chain->Add("/home/belle2/yuanmk/data/B2KKpi/sigMC/root/B2KKpi_all.root");

    chain->Print();
}