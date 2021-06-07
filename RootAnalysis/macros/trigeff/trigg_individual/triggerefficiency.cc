

#include <iomanip>
#include "TTree.h"
#include "TH2.h"
#include "TH1.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TRandom3.h"
#include "TTimeStamp.h"

#include "TCanvas.h"
#include "TLatex.h"
#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include "TChain.h"
#include "TMath.h"
#include "TString.h"
#include "TGraphErrors.h"
using namespace std;
double errorbin(double bin1, double bin2){
  //return sqrt(((bin1+1)*(bin2-bin1+1))/((bin2+3)*(bin2+2)*(bin2+2)));    
  return sqrt(pow(1/sqrt(bin1),2) +pow(1/sqrt(bin2),2));
}
void triggerefficiency(TString command = ""){
  int seed = 1234;
  int iterations = 3;
  int fYear= 2017;
  TString mode = "bupsikMcComb";
  int channel = 0;
  TString tok;

  Ssiz_t from = 0;
  while(command.Tokenize(tok, from, "[ \t;=:]")) {
    if (tok=="seed") {                                 // set the random seed                                                                                     
      command.Tokenize(tok, from, "[ \t;=:]");
      seed = tok.Atoi();
    }else if (tok=="channel") {                          // set # of iterations                                                 
      command.Tokenize(tok, from, "[ \t;=:]");
      channel = tok.Atoi();
    }
    else if (tok=="fYear") {
      command.Tokenize(tok, from, "[ \t;=:]");
      fYear = tok.Atoi();
    }
    else if (tok=="mode") {
      command.Tokenize(tok, from, "[ \t;=:]");
      mode = tok;
    }
  }


  TH1D* Htrigeff0= new TH1D("Htrigeff0",Form("Year: %d, Chan %d;;Efficiency",fYear,channel),7,0,7);
  TH1D* Htrigeff1= new TH1D("Htrigeff1",";;Efficiency",7,0,7);
  TFile * fileMC =  TFile::Open(Form("../../%d/plotResultsnew.%d.root",fYear,fYear),"READ");
  cout<<"it is opening "<<mode.Data()<<endl;
  TH1D* hmass_muonID =(TH1D*)fileMC->Get(Form("%s/hMassWithMuonCuts_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  TH1D* hmass_trigg  =(TH1D*)fileMC->Get(Form("%s/hMassWithTriggerCuts_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  TH1D* hmass_trigg2 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_DoubleMu4_3_Jpsi_Displaced_Cuts_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  TH1D* hmass_trigg3 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_Dimuon0_Jpsi_Cuts_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  TH1D* hmass_trigg4 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_Dimuon0_Jpsi_NoVertexing_Cuts_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  TH1D* hmass_trigg5 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_DoubleMu4_Jpsi_Displaced_Cuts_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  TH1D* hmass_trigg6 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_DoubleMu4_Jpsi_NoVertexing_Cuts_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  TH1D* hmass_trigg_sig =(TH1D*)fileMC->Get(Form("bsmmMcComb/hMassWithTriggerCuts_bdt%d_bsmmMcComb_chan%d",fYear,channel));
  TH1D* hmass_muonID_sig =(TH1D*)fileMC->Get(Form("bsmmMcComb/hMassWithMuonCuts_bdt%d_bsmmMcComb_chan%d",fYear,channel));

  TH1D* hmass_wm_muonID =(TH1D*)fileMC->Get(Form("%s/hMassWithAnaCuts_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  TH1D* hmass_wm_trigg  =(TH1D*)fileMC->Get(Form("%s/hMassWithTriggerCuts_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  TH1D* hmass_wm_trigg2 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_DoubleMu4_3_Jpsi_Displaced_Cuts_womuid_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  TH1D* hmass_wm_trigg3 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_Dimuon0_Jpsi_Cuts_womuid_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  TH1D* hmass_wm_trigg4 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_Dimuon0_Jpsi_NoVertexing_Cuts_womuid_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  TH1D* hmass_wm_trigg5 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_DoubleMu4_Jpsi_Displaced_Cuts_womuid_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  TH1D* hmass_wm_trigg6 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_DoubleMu4_Jpsi_NoVertexing_Cuts_womuid_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  TH1D* hmass_wm_trigg_sig =(TH1D*)fileMC->Get(Form("bsmmMcComb/hMassWithTriggerCuts_bdt%d_bsmmMcComb_chan%d",fYear,channel));
  TH1D* hmass_wm_muonID_sig =(TH1D*)fileMC->Get(Form("bsmmMcComb/hMassWithAnaCuts_bdt%d_bsmmMcComb_chan%d",fYear,channel));


  hmass_muonID->Sumw2();
  hmass_trigg->Sumw2();
  hmass_trigg2->Sumw2();
  hmass_trigg3->Sumw2();
  hmass_trigg4->Sumw2();
  hmass_trigg5->Sumw2();
  hmass_trigg6->Sumw2();
  hmass_trigg_sig->Sumw2();
  double muonID_n(0), triggeff_n(0), triggeff2_n(0), triggeff3_n(0), triggeff4_n(0), triggeff5_n(0),triggeff6_n(0);
  hmass_wm_muonID->Sumw2();
  hmass_wm_trigg->Sumw2();
  hmass_wm_trigg2->Sumw2();
  hmass_wm_trigg3->Sumw2();
  hmass_wm_trigg4->Sumw2();
  hmass_wm_trigg5->Sumw2();
  hmass_wm_trigg6->Sumw2();
  hmass_wm_trigg_sig->Sumw2();
  double muonID_wm_n(0), triggeff_wm_n(0), triggeff2_wm_n(0), triggeff3_wm_n(0), triggeff4_wm_n(0), triggeff5_wm_n(0),triggeff6_wm_n(0);

  if(hmass_muonID) muonID_n   = hmass_muonID->Integral();
  cout<<"muonID "<<muonID_n<<endl;
  if(hmass_trigg) triggeff_n  = hmass_trigg->Integral();
  cout<<"muonID "<<triggeff_n<<endl;
  if(hmass_trigg2)triggeff2_n = hmass_trigg2->Integral();
  cout<<"muonID "<<triggeff2_n<<endl;
  if(hmass_trigg3)triggeff3_n = hmass_trigg3->Integral();
  cout<<"muonID "<<triggeff3_n<<endl;
  if(hmass_trigg4)triggeff4_n = hmass_trigg4->Integral();
  cout<<"muonID "<<triggeff4_n<<endl;
  if(hmass_trigg5)triggeff5_n = hmass_trigg5->Integral();
  cout<<"muonID "<<triggeff5_n<<endl;
  if(hmass_trigg6)triggeff6_n = hmass_trigg6->Integral();
  cout<<"muonID "<<triggeff6_n<<endl;
  double triggeffsig = hmass_trigg_sig->Integral();
  double muonidsig = hmass_muonID_sig->Integral();

  if(hmass_wm_muonID) muonID_wm_n   = hmass_wm_muonID->Integral();
  cout<<"muonID "<<muonID_wm_n<<endl;
  if(hmass_wm_trigg) triggeff_wm_n  = hmass_wm_trigg->Integral();
  cout<<"muonID "<<triggeff_wm_n<<endl;
  if(hmass_wm_trigg2)triggeff2_wm_n = hmass_wm_trigg2->Integral();
  cout<<"muonID "<<triggeff2_wm_n<<endl;
  if(hmass_wm_trigg3)triggeff3_wm_n = hmass_wm_trigg3->Integral();
  cout<<"muonID "<<triggeff3_wm_n<<endl;
  if(hmass_wm_trigg4)triggeff4_wm_n = hmass_wm_trigg4->Integral();
  cout<<"muonID "<<triggeff4_wm_n<<endl;
  if(hmass_wm_trigg5)triggeff5_wm_n = hmass_wm_trigg5->Integral();
  cout<<"muonID "<<triggeff5_wm_n<<endl;
  if(hmass_wm_trigg6)triggeff6_wm_n = hmass_wm_trigg6->Integral();
  cout<<"muonID "<<triggeff6_wm_n<<endl;
  double triggeffsig_wm = hmass_wm_trigg_sig->Integral();
  double muonidsig_wm = hmass_wm_muonID_sig->Integral();

  cout<<errorbin(triggeff_n,muonID_n)<<endl;

  Htrigeff0->SetBinContent(1,triggeff_n/muonID_n);
  Htrigeff0->SetBinError(1,errorbin(triggeff_n,muonID_n));
  Htrigeff0->SetBinContent(2,triggeff2_n/muonID_n);
  Htrigeff0->SetBinError(2,errorbin(triggeff2_n,muonID_n));
  Htrigeff0->SetBinContent(3,triggeff3_n/muonID_n);
  Htrigeff0->SetBinError(3,errorbin(triggeff3_n,muonID_n));
  Htrigeff0->SetBinContent(4,triggeff4_n/muonID_n);
  Htrigeff0->SetBinError(4,errorbin(triggeff4_n,muonID_n));
  Htrigeff0->SetBinContent(5,triggeff5_n/muonID_n);
  Htrigeff0->SetBinError(5,errorbin(triggeff5_n,muonID_n));
  Htrigeff0->SetBinContent(6,triggeff6_n/muonID_n);
  Htrigeff0->SetBinError(6,errorbin(triggeff6_n,muonID_n));
  Htrigeff0->SetBinContent(7,triggeffsig/muonidsig);
  Htrigeff0->SetBinError(7,errorbin(triggeffsig,muonidsig));

  Htrigeff0->SetMinimum(0.);
  Htrigeff0->SetMaximum(1.0);
  Htrigeff0->SetMarkerStyle(20);

  Htrigeff1->SetBinContent(1,triggeff_wm_n/muonID_wm_n);
  Htrigeff1->SetBinError(1,errorbin(triggeff_wm_n,muonID_wm_n));
  Htrigeff1->SetBinContent(2,triggeff2_wm_n/muonID_wm_n);
  Htrigeff1->SetBinError(2,errorbin(triggeff2_wm_n,muonID_wm_n));
  Htrigeff1->SetBinContent(3,triggeff3_wm_n/muonID_wm_n);
  Htrigeff1->SetBinError(3,errorbin(triggeff3_wm_n,muonID_wm_n));
  Htrigeff1->SetBinContent(4,triggeff4_wm_n/muonID_wm_n);
  Htrigeff1->SetBinError(4,errorbin(triggeff4_wm_n,muonID_wm_n));
  Htrigeff1->SetBinContent(5,triggeff5_wm_n/muonID_wm_n);
  Htrigeff1->SetBinError(5,errorbin(triggeff5_wm_n,muonID_wm_n));
  Htrigeff1->SetBinContent(6,triggeff6_wm_n/muonID_wm_n);
  Htrigeff1->SetBinError(6,errorbin(triggeff6_wm_n,muonID_wm_n));
  Htrigeff1->SetBinContent(7,triggeffsig_wm/muonidsig_wm);
  Htrigeff1->SetBinError(7,errorbin(triggeffsig_wm,muonidsig_wm));
  
  Htrigeff1->SetMinimum(0.);
  Htrigeff1->SetMaximum(1.0);
  Htrigeff1->SetMarkerStyle(20);
  Htrigeff1->SetMarkerColor(kRed);
  TLatex tex;
  tex.SetTextFont(42);
  tex.SetTextSize(0.025);
  tex.SetTextAlign(11);
  tex.SetTextAngle(90);
  tex.SetNDC();
  TString title  ="#color[4]{HLT_DoubleMu4_3_Jpsi_Displaced}";
  TString title2 ="#color[4]{HLT_DoubleMu4_3_Jpsi_Displaced}";
  TString title3 ="#color[4]{HLT_Dimuon0_Jpsi}";
  TString title4 ="#color[4]{HLT_Dimuon0_Jpsi_NoVertexing}";
  TString title5 ="#color[4]{HLT_DoubleMu4_Jpsi_Displaced}";
  TString title6 ="#color[4]{HLT_DoubleMu4_Jpsi_Displaced_NoVertexing}";
  TString title7 ="#color[4]{HLT_DoubleMu4_3_Bs}";
  
  gStyle->SetOptStat(0);
  TCanvas* c =new TCanvas("c", "", 600,600);
  Htrigeff0->Draw();
  Htrigeff1->Draw("same");
			    
  tex.DrawLatex(0.15,0.14,title);
  tex.DrawLatex(0.27,0.14,title2);
  tex.DrawLatex(0.40,0.14,title3);
  tex.DrawLatex(0.52,0.14,title4);
  tex.DrawLatex(0.64,0.14,title5);
  tex.DrawLatex(0.75,0.14,title6);
  tex.DrawLatex(0.87,0.14,title7);
  c->SaveAs(Form("Trigger_efficiency_%d_%s_%d.pdf",fYear,mode.Data(),channel));

  
}

int main(int argc, char *argv[]){
  TString cmd = "";
  for (int i=1; i<argc; i++) {
    cmd += TString(argv[i])+";";
  }
  triggerefficiency(cmd);
}

