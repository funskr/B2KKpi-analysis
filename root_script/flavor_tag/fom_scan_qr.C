#include "TStyle.h"
#include "TCanvas.h"
#include "/home/belle2/yuanmk/useful/boot.h"

const double start = 0.5;
const double stop = 0.95;
const int n = 46;//scan step

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
    string stable_cuts = "dr<0.04 && fabs(dz)<0.2";
    string final_cuts = stable_cuts + "&& FBDT_qrCombined<";

    double var_array[n];
    double eff[n];
    double Nbackground[n];
    for(int i=0; i<n; i++){
        var_array[i] = start + (stop - start)/(n-1) * i;
    }

    cout<<"var:eff:Nbackground:FOM"<<endl;

    double Fom[n];
    const double a = 3;
    for(int i=0; i<n; i++){
        string cuts = final_cuts + to_string(var_array[i]);

        eff[i] = get_num("/home/belle2/yuanmk/data/B2KKpi/sigMC/root/B2KKpi_final.root","B0",cuts)/1e+6;

        Nbackground[i] = get_num("/home/belle2/yuanmk/data/B2KKpi/B2KKpi_bbar_20230716_night/sub00/*.root","B0",cuts);
        Nbackground[i] += get_num("/home/belle2/yuanmk/data/B2KKpi/B2KKpi_bbar_20230716_night/sub01/*.root","B0",cuts);
        Nbackground[i] += get_num("/home/belle2/yuanmk/data/B2KKpi/B2KKpi_bbar_20230716_night/sub02/*.root","B0",cuts);

        //calculate the fom value
        Fom[i] = eff[i]/(a/2+sqrt(Nbackground[i]));

        cout<<var_array[i]<<":"<<eff[i]<<":"<<Nbackground[i]<<":"<<Fom[i]<<endl;
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