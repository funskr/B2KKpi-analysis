#include "TStyle.h"
#include "TCanvas.h"
#include "/home/belle2/yuanmk/useful/boot.h"

const double start = 0.7;
const double stop = 0.99;
const int n = 30;//scan step

double get_num(string files, string branch, string cuts){

    TChain* chain = new TChain(branch.c_str());
    chain->Add(files.c_str());
    double number = chain->GetEntries(cuts.c_str());

    delete chain;
    return number;
}

void fom_scan_qr(){
    setStyle();

    TCanvas *mbc=new TCanvas("mbc","mbc",720,600);
    mbc->SetFillColor(0);
    mbc->SetFrameFillColor(0);
    gStyle->SetPadColor(0);
    gStyle->SetCanvasColor(0);
    gStyle->SetOptStat(0);
    mbc->SetLeftMargin(0.15);
    mbc->SetTopMargin(0.1);
    mbc->SetRightMargin(0.1);
    mbc->SetBottomMargin(0.15);


    //stable cuts
    vector<string> cuts{
    "flag_candidate==1",
    "&&ContProb<0.4",
    "&&!(InvM_KpKm > 1.8484 && InvM_KpKm < 1.8806)&&!(InvM_KmPip > 1.8408 && InvM_KmPip < 1.8875)&&!(InvM_KpPim > 1.8408 && InvM_KpPim < 1.8875)",
    };

    string stable_cuts = "";
    for(string x : cuts) stable_cuts += x;
    string final_cuts = stable_cuts + "&& FBDT_qrCombined<";

    double var_array[n];
    double Nsig[n];
    double Nbackground[n];
    for(int i=0; i<n; i++){
        var_array[i] = start + (stop - start)/(n-1) * i;
    }

    cout<<"var:Nsig:Nbackground:FOM"<<endl;

    double Fom[n];
    const double a = 5;
    for(int i=0; i<n; i++){
        string cut_sig = final_cuts + to_string(var_array[i]);

        Nsig[i] = get_num("/home/belle2/yuanmk/data/B2KKpi/sample/ana/sigMC_sample.root","B0",cut_sig);

        string cut_bkg = cut_sig + "&&(!(abs(mcPDG) == 511 && pi0_mcPDG == 111 && Kp_mcPDG == 321 && Km_mcPDG == -321))";
        //cut_bkg += "(!(abs(mcPDG) == 511 && pi0_mcPDG == 111 && (Kp_mcPDG == 211 || Km_mcPDG == -211)))";

        Nbackground[i] = get_num("/home/belle2/yuanmk/data/B2KKpi/sample/ana/mixed_sample.root", "B0", cut_bkg)/3.;
        Nbackground[i] += get_num("/home/belle2/yuanmk/data/B2KKpi/sample/ana/charged_sample.root", "B0", cut_bkg)/3.;
        Nbackground[i] += get_num("/home/belle2/yuanmk/data/B2KKpi/sample/ana/uubar_sample.root", "B0", cut_bkg);
        Nbackground[i] += get_num("/home/belle2/yuanmk/data/B2KKpi/sample/ana/ddbar_sample.root", "B0", cut_bkg);
        Nbackground[i] += get_num("/home/belle2/yuanmk/data/B2KKpi/sample/ana/ssbar_sample.root", "B0", cut_bkg);
        Nbackground[i] += get_num("/home/belle2/yuanmk/data/B2KKpi/sample/ana/ccbar_sample.root", "B0", cut_bkg);

        //calculate the fom value
        Fom[i] = Nsig[i]/(sqrt(Nbackground[i]));

        cout<<var_array[i]<<":"<<Nsig[i]<<":"<<Nbackground[i]<<":"<<Fom[i]<<endl;
    }

	//for(int i=0; i<n; i++) 

    //get the max and min value of Fom list
    double Fom_max=Fom[0],Fom_min=Fom[0];
    int index_max=0;int index_min=0;

    for(int i=0; i<n; i++){
        if(Fom[i]>Fom_max){
            Fom_max = Fom[i];
            index_max = i;
        }
        if(Fom[i]<Fom_min){
			Fom_min = Fom[i];
			index_min = i;
		} 	
        cout<<var_array[i]<<":"<<Fom[i]<<endl;
    }

    cout<<"optimized result:"<<var_array[index_max]<<endl;

    mbc->cd();
    auto g_fom = new TGraph(n, var_array, Fom);
    g_fom->SetTitle("");
    g_fom->Draw("AC*");
    
    g_fom->GetXaxis()->SetTitle("qr_{FBDT}");
    g_fom->GetYaxis()->SetTitle("FOM");
    g_fom->GetXaxis()->SetTitleOffset(0.90);
    g_fom->GetYaxis()->SetTitleOffset(1.20);

    TArrow *xy1 = new TArrow(var_array[index_max], Fom_min+(Fom_max - Fom_min)*0.3, var_array[index_max], Fom_max-(Fom_max - Fom_min)*0.02,0.015,">");
    xy1->SetLineColor(kRed);xy1->SetLineWidth(2);
    xy1->Draw();

    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/flavor_tag/pdf/fom_scan_final.pdf");
    mbc->SaveAs("/home/belle2/yuanmk/analysis/B2KKpi/plots/flavor_tag/png/fom_scan_final.png");
}