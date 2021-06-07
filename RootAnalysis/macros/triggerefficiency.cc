
#include "TCanvas.h"
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

#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include "TChain.h"
#include "TMath.h"
#include "TString.h"
#include "TGraphErrors.h"
using namespace std;

TH2D* randomtoy(int fYear, int iloop){
  //TH2D* randomtoy(TH2D* htoy, int iloop){

  TFile * filetoy = NULL;
  if(fYear == 2017)filetoy= TFile::Open("w_trigger_Jpsi_2017.root","READ");
  if(fYear == 2018)filetoy= TFile::Open("w_trigger_Jpsi_2018.root","READ");

  TH2D* h2D= (TH2D*)filetoy->Get("scalefactors");
  //for(int iloop=0; iloop<10; iloop++){
  //TH2D* htoy = (TH2D*) h2D->Clone("htoy");
  TH2D* htoy = (TH2D*) h2D->Clone(Form("triggertoy_%i",iloop));
  for(int wtx =1; wtx<=h2D->GetNbinsX(); wtx++)
    for(int wty =1; wty<=h2D->GetNbinsY(); wty++){
      double var_wt = h2D->GetBinContent(wtx,wty);
      double var_wter = h2D->GetBinError(wtx,wty);
      htoy->SetBinContent(wtx,wty,gRandom->Gaus(var_wt,var_wter));
    }
  return htoy;
}
double weightcal(TH2D* htoy, double x_new, double y_new){

  double weight(0);
  double x_range = htoy->GetXaxis()->GetXmax()-htoy->GetXaxis()->GetXmin();
  double y_range = htoy->GetYaxis()->GetXmax()-htoy->GetYaxis()->GetXmin();
  //cout<<" xnew "<<x_new <<" ynew "<<y_new<<" htoy->GetXaxis()->GetXmin() "<<htoy->GetXaxis()->GetXmin()<< " htoy->GetYaxis()-> "<<htoy->GetYaxis()->GetXmin()<<endl;
  x_new = max (x_new,  htoy->GetXaxis()->GetXmin()+0.001*x_range);
  y_new = max (y_new, htoy->GetYaxis()->GetXmin()+0.001*y_range);
  x_new = min (x_new,  htoy->GetXaxis()->GetXmax()-0.001*x_range);
  y_new = min (y_new, htoy->GetYaxis()->GetXmax()-0.001*y_range);
  
  weight = htoy->Interpolate(x_new, y_new);
  //cout<<" xnew "<<x_new<<" y_new "<<y_new <<" weight "<<weight<<endl;
// h2Dnew->Delete();
  return weight;
}
  
void filework(TH2D *htoy, TH1D* Htrigeff0, TH1D* Htrigeff1,TH1D* w_Htrigeff0,TH1D* w_Htrigeff1, int fYear,  int iloop){

  TFile* file = NULL;
  if(fYear==2017)
    file=TFile::Open("root://cmseos.fnal.gov//eos/uscms/store/user/ckar/Bmm5/bmm-reduced_BuToJpsiK_BMuonFilter_MC2017_nano_v3.root","READ");
  if(fYear==2018)
    file=TFile::Open("root://cmseos.fnal.gov//eos/uscms/store/user/ckar/Bmm5/bmm-reduced_BuToJpsiK_BMuonFilter_MC2018_nano_v3.root","READ");
  TTree* t = (TTree*)file->Get("Candidates");

  int   nentries = t->GetEntries();
  Float_t fls3d(0), vtx_prob(0), Muon1_pt(0), Muon2_pt(0), Muon1_eta(0), Muon2_eta(0), chi2dof(0);
  bool hlt1(0), Muon1_softId(0), Muon2_softId(0);

  t->SetBranchAddress("bkmm_jpsimc_sl3d",    &fls3d);
  t->SetBranchAddress("bkmm_jpsimc_vtx_prob", &vtx_prob);
  if( fYear == 2018){
    t->SetBranchAddress("HLT_DoubleMu4_3_Jpsi", &hlt1); }
  else if(fYear== 2017){
    t->SetBranchAddress("HLT_DoubleMu4_3_Jpsi_Displaced", &hlt1);}

  t->SetBranchAddress("Muon1_pt", &Muon1_pt);
  t->SetBranchAddress("Muon2_pt", &Muon2_pt);
  t->SetBranchAddress("Muon1_eta", &Muon1_eta);
  t->SetBranchAddress("Muon2_eta", &Muon2_eta);
  t->SetBranchAddress("bkmm_jpsimc_vtx_chi2dof", &chi2dof);
  t->SetBranchAddress("Muon1_softId", &Muon1_softId);
  t->SetBranchAddress("Muon2_softId", &Muon2_softId);
  
  //}
  int step(1000000);
  if (nentries < 5000000)  step = 500000;
  if (nentries < 1000000)  step = 100000;
  if (nentries < 100000)   step = 10000;
  if (nentries < 10000)    step = 1000;
  if (nentries < 1000)     step = 100;

  double ntotal0(0), npass0(0), npass0_wo(0);
  double ntotal1(0), npass1(0), npass1_wo(0);
  // ----------------------------                                                                                                                                            
  // -- the real loop starts here
  
  // ----------------------------                                                                                                                                            
  //for (int jentry = 0; jentry < 1000; jentry++) {
  for (int jentry = 0; jentry < nentries; jentry++) {
    t->GetEntry(jentry);
    if (jentry%step == 0) {
      TTimeStamp ts;
      cout << Form(" Here only trigger .. evt = %d", jentry)
	   << ", time now: " << ts.AsString("lc")
	   << endl;
    }
    if(Muon1_pt > 4. && Muon2_pt > 4. && fls3d > 4. && chi2dof < 5 && Muon1_softId==1 && Muon2_softId==1 ){
      
      if(max(TMath::Abs(Muon1_eta), TMath::Abs(Muon2_eta)) < 0.7) ntotal0++;
      else if(max(TMath::Abs(Muon1_eta),TMath::Abs(Muon2_eta)) > 0.7 && (max(TMath::Abs(Muon1_eta),TMath::Abs(Muon2_eta)) < 1.4)) ntotal1++;

      if(hlt1){
	//double weight= weightcal(htoy, fls3d,vtx_prob);
	double weight= weightcal(htoy, vtx_prob, fls3d);
	//if(jentry<1000)cout<<"fls3d "<<fls3d<<" vtxprob "<<vtx_prob<<"weight "<<weight<<endl;
	if(max(TMath::Abs(Muon1_eta), TMath::Abs(Muon2_eta)) < 0.7) {
	  npass0_wo++; 
	  npass0 += weight;
	  w_Htrigeff0->Fill(weight);
	}
	else if(max(TMath::Abs(Muon1_eta),TMath::Abs(Muon2_eta)) > 0.7 && (max(TMath::Abs(Muon1_eta),TMath::Abs(Muon2_eta)) < 1.4)) {
	  npass1 += weight;
	  
	  npass1_wo++;
	  w_Htrigeff1->Fill(weight);

	}
      }
      
    }
  }
  //  Htrigeff0->
  cout<<"Wo Wght "<< npass0_wo<<" ntotal "<<ntotal0<<" ratio "<<(double)npass0_wo/ntotal0<<endl;
  cout<<"W Wght "<< npass0<<" ntotal "<<ntotal0<<" ratio "<<(double)npass0/ntotal0<<endl;
  cout<<"Wo Wght Chan1 "<< npass1_wo<<" ntotal "<<ntotal1<<" ratio "<<(double)npass1_wo/ntotal1<<endl;
  cout<<"W Wght chan1 "<< npass1<<" ntotal "<<ntotal1<<" ratio "<<(double)npass1/ntotal1<<endl;
  
  Htrigeff0->Fill(iloop, npass0/ntotal0);
  Htrigeff1->Fill(iloop, npass1/ntotal1);
  file->Close();
}
void triggerefficiency(TString command = ""){
  int seed = 1234;
  int iterations = 3;
  int fYear= 2017;
  TString mode =bupsikMcComb;
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


  TH1D* Htrigeff0= new TH1D("Htrigeff0",Form("Chan%d",channel),7,0,7);
  TH1D* Htrigeff1= new TH1D("Htrigeff1","Chan1",7,0,7);
  TFile * fileMC =  TFile::Open(Form("%d/plotResultsnew.%d.root",fYear,fYear),"READ");

  TH1D* hmass_muonID =( TH1D*)fileMC->Get(Form("hMassWithMuonCuts_bdt_%d_%s_chan%d",fYear,mode.Data(),channel));
  TH1D* hmass_trigg =( TH1D*)fileMC->Get(Form("hMassWithTriggerCuts_bdt_%d_%s_chan%d",fYear,mode.Data(),channel));
  TH1D* hmass_trigg2 =( TH1D*)fileMC->Get(Form("hMassWithTrigger_DoubleMu4_3_Jpsi_Displaced_Cuts_bdt_%d_%s_chan%d",fYear,mode.Data(),channel));
  TH1D* hmass_trigg3 =( TH1D*)fileMC->Get(Form("hMassWithTrigger_Dimuon0_Jpsi_Cuts_bdt_%d_%s_chan%d",fYear,mode.Data(),channel));
  TH1D* hmass_trigg4 =( TH1D*)fileMC->Get(Form("hMassWithTrigger_Dimuon0_Jpsi_NoVertexing_Cuts_bdt_%d_%s_chan%d",fYear,mode.Data(),channel));
  TH1D* hmass_trigg5 =( TH1D*)fileMC->Get(Form("hMassWithTrigger_DoubleMu4_Jpsi_Displaced_Cuts_bdt_%d_%s_chan%d",fYear,mode.Data(),channel));
  TH1D* hmass_trigg6 =( TH1D*)fileMC->Get(Form("hMassWithTrigger_DoubleMu4_Jpsi_NoVertexing_Cuts_bdt_%d_%s_chan%d",fYear,mode.Data(),channel));

  double muonID_n =hmass_muonID->Integral(4.9,5.9);
  double triggeff_n =hmass_trigg->Integral(4.9,5.9);
  double triggeff2_n =hmass_trigg2->Integral(4.9,5.9);
  double triggeff3_n =hmass_trigg3->Integral(4.9,5.9);
  double triggeff4_n =hmass_trigg4->Integral(4.9,5.9);
  double triggeff5_n =hmass_trigg5->Integral(4.9,5.9);
  double triggeff6_n =hmass_trigg6->Integral(4.9,5.9);
  
  Htrigeff0->SetBinContent(1,triggeff_n/muonID_n);
  Htrigeff0->SetBinContent(2,triggeff2_n/muonID_n);
  Htrigeff0->SetBinContent(3,triggeff3_n/muonID_n);
  Htrigeff0->SetBinContent(4,triggeff4_n/muonID_n);
  Htrigeff0->SetBinContent(5,triggeff5_n/muonID_n);
  Htrigeff0->SetBinContent(6,triggeff6_n/muonID_n);

  TCanvas* c =new TCanvas("c", "", 600,600);
  Htrigeff0->Draw();
  c->SaveAs(Form("Trigger_efficiency_%d_%s_%d.pdf",fYear,mode.Data(),channel));

  // TH1D* w_Htrigeff0[iterations];
  // TH1D* w_Htrigeff1[iterations];
  // for(int k=0 ; k<iterations; k++){
  //   w_Htrigeff0[k]= new TH1D(Form("w_Htrigeff0_toy%d",k),"Weight Chan0",1000,0,3);
  //   w_Htrigeff1[k]= new TH1D(Form("w_Htrigeff1_toy%d",k),"Weight Chan1",1000,0,3);
  // }
  // TH2D* htoy_s[iterations];
  // //for(int i= 0; i<iterations; i++){
  // // htoy_s[i]=new TH2D(Form("toy_%i",i));
  // //}
  // //TTimeStamp ts0;
  // gRandom = new TRandom3(seed);

  // for(int iloop =0; iloop<iterations; iloop++){
  //   cout<<" toy number "<<iloop<<" for year "<<fYear<<endl;
  //   TH2D* htoy= randomtoy(fYear, iloop);
  //   filework(htoy, Htrigeff0, Htrigeff1, w_Htrigeff0[iloop], w_Htrigeff1[iloop], fYear, iloop);
  //   htoy_s[iloop]=(TH2D*)htoy->Clone();
  //   //htoy->Delete();

  // }
  
  // TFile *output = TFile::Open(Form("%iBuJpsiKp_trigg_%i_toyset%i.root",fYear,iterations, seed),"RECREATE");
  // Htrigeff1->Write();
  // Htrigeff0->Write();
  
  // for(int i= 0; i<iterations; i++){                                                                                                                                 
  //   htoy_s[i]->Write();
  //   w_Htrigeff1[i]->Write();
  //   w_Htrigeff0[i]->Write();

  // }    
  // output->Close();
  
}

int main(int argc, char *argv[]){
  TString cmd = "";
  for (int i=1; i<argc; i++) {
    cmd += TString(argv[i])+";";
  }
  triggerefficiency(cmd);
}

