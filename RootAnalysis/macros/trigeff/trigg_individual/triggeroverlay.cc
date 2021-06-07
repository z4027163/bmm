

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
#include "TCut.h"
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
  return (bin1/bin2)*sqrt(pow(1/sqrt(bin1),2) +pow(1/sqrt(bin2),2));
}
void trigger(TString command = ""){
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


  TH1D* Htrigeff0= new TH1D("Htrigeff0",Form("Year: %d, Chan %d;;Efficiency",fYear,channel),5,0,5);
  TH1D* Htrigeff1= new TH1D("Htrigeff1",";;Efficiency",7,0,7);

  TFile *file=TFile::Open("/uscms/home/ckar/nobackup/BsToMuMu/minebmm/flatntuple/CMSSW_10_3_2/src/reduce_samples/JpsiK_2018_onlytest.root","READ");
  //TFile *file=TFile::Open("root://cmseos.fnal.gov//eos/uscms/store/user/ckar/Bmm5/NanoAOD/Merged/bmm-reduced_BuToJpsiK_BMuonFilter_MC2018_nano_v3.root","READ");
  TTree* Cand=(TTree*)file->Get("Candidates");

  TCut base_cut_mm="mm_mu1_pt >4.0 && mm_mu2_pt >4.0 && abs(mm_mu1_eta)<1.4  && abs(mm_mu2_eta)<1.4 && mm_kin_sl3d>4 && mm_kin_vtx_chi2dof<5 && m1id && m2id ";
  //TCut base_cut_bkmm="mm_mu1_pt >4.0 && mm_mu2_pt >4.0 && abs(mm_mu1_eta)<1.4  && abs(mm_mu2_eta)<1.4 && bkmm_jpsimc_sl3d>4 && bkmm_jpsimc_vtx_chi2dof<5 && m1id && m2id ";
  TCut base_cut_bkmm="mm_mu1_pt >4.0 && mm_mu2_pt >4.0 && abs(mm_mu1_eta)<1.4  && abs(mm_mu2_eta)<1.4 && bkmm_jpsimc_sl3d>4 && bkmm_jpsimc_vtx_chi2dof<5 && Muon1_softMvaId && Muon2_softMvaId ";//&&mm_kin_pt>6.0 ";

  TCut base_cut_bkmm1=" abs(mm_mu1_eta)<1.4  && abs(mm_mu2_eta)<1.4 && bkmm_jpsimc_sl3d>4 && bkmm_jpsimc_vtx_chi2dof<5 && Muon1_softMvaId && Muon2_softMvaId &&mm_kin_pt>6.0 ";
  //  TCut base_cut_bkmm="Muon1_softMvaId && Muon2_softMvaId ";//mm_mu1_pt >4.0 && mm_mu2_pt >4.0 && abs(mm_mu1_eta)<1.4  && abs(mm_mu2_eta)<1.4 && bkmm_jpsimc_sl3d>4 && bkmm_jpsimc_vtx_chi2dof<5 && Muon1_softMvaId && Muon2_softMvaId ";
  TCut base_cut_bkmm_soft="mm_mu1_pt >4.0 && mm_mu2_pt >4.0 && abs(mm_mu1_eta)<1.4  && abs(mm_mu2_eta)<1.4 && bkmm_jpsimc_sl3d>4 && bkmm_jpsimc_vtx_chi2dof<5 && Muon1_softId && Muon2_softId ";
  TCut cut_c;
  if(channel ==0)cut_c="chan==0";
  if(channel ==1)cut_c="chan==1";
  TCut trig1 = "HLT_DoubleMu4_3_Bs";
  TCut trig2 = "HLT_DoubleMu4_3_Jpsi_Displaced";
  TCut trig3 = "HLT_DoubleMu4_Jpsi_Displaced";
  TCut trig4 = "HLT_DoubleMu4_Jpsi_NoVertexing";
  TCut trig5 = "HLT_Dimuon0_Jpsi";
  TCut trig6 = "HLT_Dimuon0_Jpsi_NoVertexing";
  TCut trig_base = "HLT_Dimuon0_LowMass";
  double trig0_n= Cand->GetEntries(base_cut_bkmm && cut_c);
  double trig0_n_t= Cand->GetEntries(base_cut_bkmm1 && cut_c);
  double trig2_n= Cand->GetEntries(base_cut_bkmm && cut_c && trig2);
  double trig3_n= Cand->GetEntries(base_cut_bkmm && cut_c && trig3);
  double trig4_n= Cand->GetEntries(base_cut_bkmm && cut_c && trig4);
  double trig5_n= Cand->GetEntries(base_cut_bkmm1 && cut_c && trig5);
  double trig6_n= Cand->GetEntries(base_cut_bkmm1 && cut_c && trig6);
  double trign_base= Cand->GetEntries(base_cut_bkmm1 && cut_c && trig_base);

  cout<<"Efficiency trig HLT_DoubleMu4_3_Jpsi_Displaced "<<trig2_n<<"\t/"<<trig0_n<<"\t= "<<(double)trig2_n/trig0_n<<endl;
  cout<<"Efficiency trig HLT_DoubleMu4_Jpsi_Displaced  "<<trig3_n<<"\t/"<<trig0_n<<"\t= "<<(double)trig3_n/trig0_n<<endl;
  cout<<"Efficiency trigHLT_DoubleMu4_Jpsi_NoVertexing  "<<trig4_n<<"\t/"<<trig0_n<<"\t= "<<(double)trig4_n/trig0_n<<endl;
  cout<<"Efficiency trig HLT_Dimuon0_Jpsi "<<trig5_n<<"\t/ "<<trig0_n_t<<"\t= "<<(double)trig5_n/trig0_n_t<<endl;
  cout<<"Efficiency trig HLT_Dimuon0_Jpsi_NoVertexing "<<trig6_n<<"\t/ "<<trig0_n_t<<"\t= "<<(double)trig6_n/trig0_n_t<<endl;
  cout<<"Efficiency trig HLT_Dimuon0_Lowmass "<<trign_base<<"\t/ "<<trig0_n_t<<"\t= "<<(double)trign_base/trig0_n_t<<endl;

  //return;
  /*Cand->Draw("L1_DoubleMu0_SQ_OS>>h0","HLT_Dimuon0_Jpsi_NoVertexing");
  TH1D* h0=(TH1D*)gDirectory->Get("h0");
  Cand->Draw("L1_DoubleMu0er1p5_SQ_dR_Max1p4>>h1","HLT_Dimuon0_Jpsi_NoVertexing");
  TH1D* h1=(TH1D*)gDirectory->Get("h1");
  Cand->Draw("L1_DoubleMu0_SQ>>h2","HLT_Dimuon0_Jpsi_NoVertexing");
  TH1D* h2=(TH1D*)gDirectory->Get("h2");
  Cand->Draw("L1_DoubleMu0er1p4_SQ_OS_dR_Max1p4>>h3","HLT_Dimuon0_Jpsi_NoVertexing");
  TH1D* h3=(TH1D*)gDirectory->Get("h3");
  Cand->Draw("L1_DoubleMu0er1p4_dEta_Max1p8_OS>>h4","HLT_Dimuon0_Jpsi_NoVertexing");
  TH1D* h4=(TH1D*)gDirectory->Get("h4");
  Cand->Draw("L1_DoubleMu0er1p5_SQ_OS>>h5","HLT_Dimuon0_Jpsi_NoVertexing");
  TH1D* h5=(TH1D*)gDirectory->Get("h5");
  Cand->Draw("L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4>>h6","HLT_Dimuon0_Jpsi_NoVertexing");
  TH1D* h6=(TH1D*)gDirectory->Get("h6");
  Cand->Draw("L1_DoubleMu0er1p5_SQ_dR_Max1p4>>h7","HLT_Dimuon0_Jpsi_NoVertexing");
  TH1D* h7=(TH1D*)gDirectory->Get("h7");
  Cand->Draw("L1_DoubleMu0er2_SQ_dR_Max1p4>>h8","HLT_Dimuon0_Jpsi_NoVertexing");
  TH1D* h8=(TH1D*)gDirectory->Get("h8");
  Cand->Draw("L1_DoubleMu4_SQ_OS>>h9","HLT_Dimuon0_Jpsi_NoVertexing");
  TH1D* h9=(TH1D*)gDirectory->Get("h9");
  Cand->Draw("L1_DoubleMu4_SQ_OS_dR_Max1p2>>h10","HLT_Dimuon0_Jpsi_NoVertexing");
  TH1D* h10=(TH1D*)gDirectory->Get("h10");
  Cand->Draw("L1_DoubleMu4p5_SQ>>h11","HLT_Dimuon0_Jpsi_NoVertexing");
  TH1D* h11=(TH1D*)gDirectory->Get("h11");
  Cand->Draw("L1_DoubleMu4p5_SQ_OS>>h12","HLT_Dimuon0_Jpsi_NoVertexing");
  TH1D* h12=(TH1D*)gDirectory->Get("h12");
  Cand->Draw("L1_DoubleMu4p5_SQ_OS_dR_Max1p2>>h13","HLT_Dimuon0_Jpsi_NoVertexing");
  TH1D* h13=(TH1D*)gDirectory->Get("h13");
  Cand->Draw("L1_DoubleMu4p5er2p0_SQ_OS>>h14","HLT_Dimuon0_Jpsi_NoVertexing");
  TH1D* h14=(TH1D*)gDirectory->Get("h14");
  Cand->Draw("L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18>>h15","HLT_Dimuon0_Jpsi_NoVertexing");
  TH1D* h15=(TH1D*)gDirectory->Get("h15");

  // TFile * fileMC =  TFile::Open(Form("../../%d/plotResultsnew.%d.root",fYear,fYear),"READ");
  // cout<<"it is opening "<<mode.Data()<<endl;
  // TH1D* hmass_muonID =(TH1D*)fileMC->Get(Form("%s/hMassWithMuonCuts_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  // TH1D* hmass_trigg  =(TH1D*)fileMC->Get(Form("%s/hMassWithTriggerCuts_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  // TH1D* hmass_trigg2 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_DoubleMu4_3_Jpsi_Displaced_Cuts_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  // TH1D* hmass_trigg3 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_Dimuon0_Jpsi_Cuts_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  // TH1D* hmass_trigg4 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_Dimuon0_Jpsi_NoVertexing_Cuts_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  // TH1D* hmass_trigg5 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_DoubleMu4_Jpsi_Displaced_Cuts_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  // TH1D* hmass_trigg6 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_DoubleMu4_Jpsi_NoVertexing_Cuts_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  // TH1D* hmass_trigg_sig =(TH1D*)fileMC->Get(Form("bsmmMcComb/hMassWithTriggerCuts_bdt%d_bsmmMcComb_chan%d",fYear,channel));
  // TH1D* hmass_muonID_sig =(TH1D*)fileMC->Get(Form("bsmmMcComb/hMassWithMuonCuts_bdt%d_bsmmMcComb_chan%d",fYear,channel));

  // TH1D* hmass_wm_muonID =(TH1D*)fileMC->Get(Form("%s/hMassWithAnaCuts_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  // TH1D* hmass_wm_trigg  =(TH1D*)fileMC->Get(Form("%s/hMassWithTriggerCuts_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  // TH1D* hmass_wm_trigg2 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_DoubleMu4_3_Jpsi_Displaced_Cuts_womuid_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  // TH1D* hmass_wm_trigg3 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_Dimuon0_Jpsi_Cuts_womuid_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  // TH1D* hmass_wm_trigg4 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_Dimuon0_Jpsi_NoVertexing_Cuts_womuid_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  // TH1D* hmass_wm_trigg5 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_DoubleMu4_Jpsi_Displaced_Cuts_womuid_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  // TH1D* hmass_wm_trigg6 =(TH1D*)fileMC->Get(Form("%s/hMassWithTrigger_DoubleMu4_Jpsi_NoVertexing_Cuts_womuid_bdt%d_%s_chan%d",mode.Data(),fYear,mode.Data(),channel));
  // TH1D* hmass_wm_trigg_sig =(TH1D*)fileMC->Get(Form("bsmmMcComb/hMassWithTriggerCuts_bdt%d_bsmmMcComb_chan%d",fYear,channel));
  // TH1D* hmass_wm_muonID_sig =(TH1D*)fileMC->Get(Form("bsmmMcComb/hMassWithAnaCuts_bdt%d_bsmmMcComb_chan%d",fYear,channel));


  // hmass_muonID->Sumw2();
  // hmass_trigg->Sumw2();
  // hmass_trigg2->Sumw2();
  // hmass_trigg3->Sumw2();
  // hmass_trigg4->Sumw2();
  // hmass_trigg5->Sumw2();
  // hmass_trigg6->Sumw2();
  // hmass_trigg_sig->Sumw2();
  // double muonID_n(0), triggeff_n(0), triggeff2_n(0), triggeff3_n(0), triggeff4_n(0), triggeff5_n(0),triggeff6_n(0);
  // hmass_wm_muonID->Sumw2();
  // hmass_wm_trigg->Sumw2();
  // hmass_wm_trigg2->Sumw2();
  // hmass_wm_trigg3->Sumw2();
  // hmass_wm_trigg4->Sumw2();
  // hmass_wm_trigg5->Sumw2();
  // hmass_wm_trigg6->Sumw2();
  // hmass_wm_trigg_sig->Sumw2();
  // double muonID_wm_n(0), triggeff_wm_n(0), triggeff2_wm_n(0), triggeff3_wm_n(0), triggeff4_wm_n(0), triggeff5_wm_n(0),triggeff6_wm_n(0);

  // if(hmass_muonID) muonID_n   = hmass_muonID->Integral();
  // cout<<"muonID "<<muonID_n<<endl;
  // if(hmass_trigg) triggeff_n  = hmass_trigg->Integral();
  // cout<<"muonID "<<triggeff_n<<endl;
  // if(hmass_trigg2)triggeff2_n = hmass_trigg2->Integral();
  // cout<<"muonID "<<triggeff2_n<<endl;
  // if(hmass_trigg3)triggeff3_n = hmass_trigg3->Integral();
  // cout<<"muonID "<<triggeff3_n<<endl;
  // if(hmass_trigg4)triggeff4_n = hmass_trigg4->Integral();
  // cout<<"muonID "<<triggeff4_n<<endl;
  // if(hmass_trigg5)triggeff5_n = hmass_trigg5->Integral();
  // cout<<"muonID "<<triggeff5_n<<endl;
  // if(hmass_trigg6)triggeff6_n = hmass_trigg6->Integral();
  // cout<<"muonID "<<triggeff6_n<<endl;
  // double triggeffsig = hmass_trigg_sig->Integral();
  // double muonidsig = hmass_muonID_sig->Integral();

  // if(hmass_wm_muonID) muonID_wm_n   = hmass_wm_muonID->Integral();
  // cout<<"muonID "<<muonID_wm_n<<endl;
  // if(hmass_wm_trigg) triggeff_wm_n  = hmass_wm_trigg->Integral();
  // cout<<"muonID "<<triggeff_wm_n<<endl;
  // if(hmass_wm_trigg2)triggeff2_wm_n = hmass_wm_trigg2->Integral();
  // cout<<"muonID "<<triggeff2_wm_n<<endl;
  // if(hmass_wm_trigg3)triggeff3_wm_n = hmass_wm_trigg3->Integral();
  // cout<<"muonID "<<triggeff3_wm_n<<endl;
  // if(hmass_wm_trigg4)triggeff4_wm_n = hmass_wm_trigg4->Integral();
  // cout<<"muonID "<<triggeff4_wm_n<<endl;
  // if(hmass_wm_trigg5)triggeff5_wm_n = hmass_wm_trigg5->Integral();
  // cout<<"muonID "<<triggeff5_wm_n<<endl;
  // if(hmass_wm_trigg6)triggeff6_wm_n = hmass_wm_trigg6->Integral();
  // cout<<"muonID "<<triggeff6_wm_n<<endl;
  // double triggeffsig_wm = hmass_wm_trigg_sig->Integral();
  // double muonidsig_wm = hmass_wm_muonID_sig->Integral();

  // cout<<errorbin(triggeff_n,muonID_n)<<endl;
  */

  Htrigeff0->SetBinContent(1,trig2_n/trig0_n);
  Htrigeff0->SetBinError(1,errorbin(trig2_n,trig0_n));
  Htrigeff0->SetBinContent(2,trig3_n/trig0_n);
  Htrigeff0->SetBinError(2,errorbin(trig3_n,trig0_n));
  Htrigeff0->SetBinContent(3,trig4_n/trig0_n);
  Htrigeff0->SetBinError(3,errorbin(trig4_n,trig0_n));
  Htrigeff0->SetBinContent(4,trig5_n/trig0_n);
  Htrigeff0->SetBinError(4,errorbin(trig5_n,trig0_n));
  Htrigeff0->SetBinContent(5,trig6_n/trig0_n);
  Htrigeff0->SetBinError(5,errorbin(trig6_n,trig0_n));
  //  Htrigeff0->SetBinContent(6,trig2_n/trig0_n);
  //Htrigeff0->SetBinError(6,errorbin(trig2_n,trig0_n));
  // Htrigeff0->SetBinContent(7,triggeffsig/muonidsig);
  // Htrigeff0->SetBinError(7,errorbin(triggeffsig,muonidsig));

  Htrigeff0->SetMinimum(0.);
  Htrigeff0->SetMaximum(1.0);
  Htrigeff0->SetMarkerStyle(20);

  // Htrigeff1->SetBinContent(1,triggeff_wm_n/muonID_wm_n);
  // Htrigeff1->SetBinError(1,errorbin(triggeff_wm_n,muonID_wm_n));
  // Htrigeff1->SetBinContent(2,triggeff2_wm_n/muonID_wm_n);
  // Htrigeff1->SetBinError(2,errorbin(triggeff2_wm_n,muonID_wm_n));
  // Htrigeff1->SetBinContent(3,triggeff3_wm_n/muonID_wm_n);
  // Htrigeff1->SetBinError(3,errorbin(triggeff3_wm_n,muonID_wm_n));
  // Htrigeff1->SetBinContent(4,triggeff4_wm_n/muonID_wm_n);
  // Htrigeff1->SetBinError(4,errorbin(triggeff4_wm_n,muonID_wm_n));
  // Htrigeff1->SetBinContent(5,triggeff5_wm_n/muonID_wm_n);
  // Htrigeff1->SetBinError(5,errorbin(triggeff5_wm_n,muonID_wm_n));
  // Htrigeff1->SetBinContent(6,triggeff6_wm_n/muonID_wm_n);
  // Htrigeff1->SetBinError(6,errorbin(triggeff6_wm_n,muonID_wm_n));
  // Htrigeff1->SetBinContent(7,triggeffsig_wm/muonidsig_wm);
  // Htrigeff1->SetBinError(7,errorbin(triggeffsig_wm,muonidsig_wm));
  
  // Htrigeff1->SetMinimum(0.);
  // Htrigeff1->SetMaximum(1.0);
  // Htrigeff1->SetMarkerStyle(20);
  // Htrigeff1->SetMarkerColor(kRed);
  TLatex tex;
  tex.SetTextFont(42);
  tex.SetTextSize(0.025);
  tex.SetTextAlign(11);
  tex.SetTextAngle(90);
  tex.SetNDC();

  TString title  ="#color[4]{HLT_DoubleMu4_3_Jpsi_Displaced}";
  TString title2 ="#color[4]{HLT_DoubleMu4_3_Jpsi_Displaced}";
  TString title3 ="#color[4]{HLT_DoubleMu4_Jpsi_Displaced}";
  TString title4 ="#color[4]{HLT_DoubleMu4_Jpsi_NoVertexing}";
  TString title5 ="#color[4]{HLT_Dimuon0_Jpsi}";
  TString title6 ="#color[4]{HLT_Dimuon0_Jpsi_NoVertexing}";
  TString title7 ="#color[4]{HLT_DoubleMu4_3_Bs}";

  // Htrigeff0->TickX();  
  gStyle->SetOptStat(0);
  TCanvas* c =new TCanvas("c", "", 600,600);
  Htrigeff0->Draw();
  //Htrigeff1->Draw("same");
  // c->Divide(4,4);
  // c->cd(1);h0->Draw();
  // c->cd(2);h1->Draw();
  // c->cd(3);h2->Draw();
  // c->cd(4);h3->Draw();
  // c->cd(5);h4->Draw();
  // c->cd(6);h5->Draw();
  // c->cd(7);h6->Draw();
  // c->cd(8);h7->Draw();
  // c->cd(9);h8->Draw();
  // c->cd(10);h9->Draw();
  // c->cd(11);h10->Draw();
  // c->cd(12);h11->Draw();
  // c->cd(13);h12->Draw();
  // c->cd(14);h13->Draw();
  // c->cd(15);h14->Draw();
  // c->cd(16);h15->Draw();

  tex.DrawLatex(0.15,0.14,title2);
  tex.DrawLatex(0.32,0.14,title3);
  tex.DrawLatex(0.49,0.14,title4);
  tex.DrawLatex(0.67,0.14,title5);
  tex.DrawLatex(0.85,0.14,title6);
  //  tex.DrawLatex(0.75,0.14,title7);
  //tex.DrawLatex(0.87,0.14,title7);
  c->SaveAs(Form("Trigger_efficiency_dimuon_%d_%s_%d.pdf",fYear,mode.Data(),channel));
  //  c->SaveAs("Trigger_efficiency_L1seed.pdf");

  
}

int main(int argc, char *argv[]){
  TString cmd = "";
  for (int i=1; i<argc; i++) {
    cmd += TString(argv[i])+";";
  }
  trigger(cmd);
}

