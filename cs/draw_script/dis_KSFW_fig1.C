#include "TStyle.h"
#include "TCanvas.h"
#include "/home/belle2/yuanmk/useful/boot.h"


void draw_hist(TH1D* hist,string address, string tree, string variable){
    //if flag is true, the histogram is signal
    TChain* chain = new TChain(tree.c_str());

    chain -> Add(address.c_str());
    //set variables
    Double_t var;
    chain->SetBranchAddress(variable.c_str(), &var);

    //draw on hist
    Int_t nentries = chain->GetEntries();
    for(Int_t i=0; i < nentries; i++){
        chain->GetEntry(i);

        hist->Fill(var); 
    }
}


void dis_KSFW_fig1(){
    vector<string> name;
    vector<string> KSFW_variables;
    vector<string> legend_position;

    ifstream f;
    f.open("/home/belle2/yuanmk/analysis/B2KKpi/continuum/draw_script/KSFW_variables1.conf", ios::in);
    if(f.is_open()){
        cout<<"KSFW variable configure file load successfully"<<endl;
        string buff;
        vector<int> split;
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

            name.push_back(buff.substr(split[0],split[1]));
            KSFW_variables.push_back(buff.substr(split[1]+1,split[2]-split[1]-1));
            legend_position.push_back(buff.substr(split[2]+1,split[3]-split[2]-1));

            split.clear();
        }
    }
    else{
        cout<<"KSFW variable configure file load error"<<endl;
        return 0;
    }

    setStyle();

    TCanvas *mbc=new TCanvas("mbc","mbc",1080,800);
    mbc->SetFillColor(0);
    mbc->SetFrameFillColor(0);
    gStyle->SetPadColor(0);
    gStyle->SetCanvasColor(0);
    gStyle->SetOptStat(0);
    mbc->SetLeftMargin(0.15);
    mbc->SetTopMargin(0.1);
    mbc->SetRightMargin(0.1);
    mbc->SetBottomMargin(0.15);

    mbc->Divide(3,3);
   
    // set ytitle
    Double_t xmin=-20, xmax=20;  Int_t xbins=100;
    TH1::SetDefaultSumw2(1);
    TString a("Events/"); char b[100];  sprintf(b, "(%.2f)",(xmax-xmin)/xbins);
    TString ytitle = a + b;

    TH1D* hKSFW_sig[9]; TH1D* hKSFW_con[9];
    string str_hKSFW_sig = ""; string str_hKSFW_con = "";
    for(int i=0; i<9; i++){
        str_hKSFW_sig = "hKSFW_sig_"+to_string(i);
        str_hKSFW_con = "hKSFW_con_"+to_string(i);

        hKSFW_sig[i] =new TH1D(str_hKSFW_sig.c_str(), str_hKSFW_sig.c_str(), xbins, xmin, xmax);
        hKSFW_sig[i]->SetLineColor(kRed);
        hKSFW_sig[i]->SetLineWidth(2);
        draw_hist(hKSFW_sig[i], "/home/belle2/yuanmk/analysis/B2KKpi/continuum/B2KKpi_cs_signalMC.root", "B0", KSFW_variables[i].c_str());

        hKSFW_con[i] =new TH1D(str_hKSFW_con.c_str(), str_hKSFW_con.c_str(), xbins, xmin, xmax);
        hKSFW_con[i]->SetLineColor(kBlue);
        hKSFW_con[i]->SetLineWidth(2);
        draw_hist(hKSFW_con[i], "/home/belle2/yuanmk/data/B2KKpi/cs/B2KKpi_cs_ccbar_20230715/sub00/*.root", "B0", KSFW_variables[i].c_str());
        //draw_hist(hKSFW_con[i], "/home/belle2/yuanmk/analysis/B2KKpi/continuum/B2KKpi_cs_signalMC.root", "B0", KSFW_variables[i].c_str());
        //draw_hist(hKSFW_con[i], "/home/belle2/yuanmk/analysis/B2KKpi/continuum/B2KKpi_cs_signalMC.root", "B0", KSFW_variables[i].c_str());
        //draw_hist(hKSFW_con[i], "/home/belle2/yuanmk/analysis/B2KKpi/continuum/B2KKpi_cs_signalMC.root", "B0", KSFW_variables[i].c_str());


        mbc->cd(i+1);
        hKSFW_sig[i]->Draw("HIST");
        hKSFW_con[i]->Draw("HIST same");
        hKSFW_sig[i]->Scale(hKSFW_con[i]->GetMaximum()/hKSFW_sig[i]->GetMaximum());

        hKSFW_sig[i]->GetXaxis()->SetTitle(name[i].c_str());
        hKSFW_sig[i]->GetYaxis()->SetTitle(ytitle);
        hKSFW_sig[i]->GetXaxis()->SetTitleOffset(0.90);
        hKSFW_sig[i]->GetYaxis()->SetTitleOffset(1.2);

        //string legend_position="LEGEND"; 
        if(legend_position[i] == "left"){//left
            TLegend *legend = new TLegend(0.22,0.70,0.40,0.86);
            legend->AddEntry("hsigMC","signal MC","l");
            legend->AddEntry("huubar","uubar","l");
            legend->AddEntry("hccbar","ccbar","l");
            legend->AddEntry("hddbar","ddbar","l");
            legend->AddEntry("hssbar","ssbar","l");
            legend->SetBorderSize(0);
            legend->SetFillColor(0);
            legend->Draw();
        }
        else if(legend_position[i] == "right"){//right
            TLegend *legend = new TLegend(0.70,0.70,0.86,0.86);
            legend->AddEntry("hsigMC","signal MC","l");
            legend->AddEntry("huubar","uubar","l");
            legend->AddEntry("hccbar","ccbar","l");
            legend->AddEntry("hddbar","ddbar","l");
            legend->AddEntry("hssbar","ssbar","l");
            legend->SetBorderSize(0);
            legend->SetFillColor(0);
            legend->Draw();
        }
        //mbc->Update();
    }

    string pdf = "./dis_KSFW_fig1.pdf";
    string png = "./dis_KSFW_fig1.png";
    mbc->SaveAs(pdf.c_str());
    mbc->SaveAs(png.c_str());

    //cout<<hsigMC->GetEntries()<<endl;
}