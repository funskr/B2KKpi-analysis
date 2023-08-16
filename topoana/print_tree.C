void print_tree(){

    TChain* chain = new TChain("B0");

    chain->Add("bbar_MC.root");

    chain->Print();

}