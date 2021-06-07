
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
void mergetrig(){
  TH1D* Htrigeff0= new TH1D("Htrigeff0","Chan0",100,-1.,1.0);
  TH1D* Htrigeff1= new TH1D("Htrigeff1","Chan1",100,-1.,1.0);
  

  for(int iloop =0; iloop<1000; iloop++){
    TFile* file= TFile::Open("");
    TH1D* h1=(TH1D*)file->Get("Htrigeff0");
    TH1D* h2=(TH1D*)file->Get("Htrigeff1");
    for(int i=0; i<10;i++){
	Htrigeff0->Fill(h1->GetBinContent(i));
	Htrigeff1->Fill(h2->GetBinContent(i));
    }	  
  }
  cout<<"Htrigeff0- "<<Htrigeff0->GetMean()<<" RMS "<<Htrigeff0->GetRMS()<<endl;
  cout<<"Htrigeff1- "<<Htrigeff1->GetMean()<<" RMS "<<Htrigeff1->GetRMS()<<endl;
  
}

// int main(int argc, char *argv[]){
//   TString cmd = "";
//   for (int i=1; i<argc; i++) {
//     cmd += TString(argv[i])+";";
//   }
//   triggerefficiency(cmd);
// }


// TH2D* randomtoy(int fYear, int iloop){
//   //TH2D* randomtoy(TH2D* htoy, int iloop){

//   TFile * filetoy = NULL;
//   if(fYear == 2017)filetoy= TFile::Open("w_trigger_Jpsi_2017.root","READ");
//   if(fYear == 2018)filetoy= TFile::Open("w_trigger_Jpsi_2018.root","READ");

//   TH2D* h2D= (TH2D*)filetoy->Get("scalefactors");
//   //for(int iloop=0; iloop<10; iloop++){
//   //TH2D* htoy = (TH2D*) h2D->Clone("htoy");
//   TH2D* htoy = (TH2D*) h2D->Clone(Form("triggertoy_%i",iloop));
//   for(int wtx =1; wtx<=h2D->GetNbinsX(); wtx++)
//     for(int wty =1; wty<=h2D->GetNbinsY(); wty++){
//       double var_wt = h2D->GetBinContent(wtx,wty);
//       double var_wter = h2D->GetBinError(wtx,wty);
//       htoy->SetBinContent(wtx,wty,gRandom->Gaus(var_wt,var_wter));
//     }
//   return htoy;
// }
// double weightcal(TH2D* htoy, double x_new, double y_new){

//   double weight(0);
//   double x_range = htoy->GetXaxis()->GetXmax()-htoy->GetXaxis()->GetXmin();
//   double y_range = htoy->GetYaxis()->GetXmax()-htoy->GetYaxis()->GetXmin();
//   x_new = max (x_new,  htoy->GetXaxis()->GetXmin()+0.001*x_range);
//   y_new = max (y_new, htoy->GetYaxis()->GetXmin()+0.001*y_range);
//   x_new = min (x_new,  htoy->GetXaxis()->GetXmax()-0.001*x_range);
//   y_new = min (y_new, htoy->GetYaxis()->GetXmax()-0.001*y_range);
  
//   weight = htoy->Interpolate(x_new, y_new);
//   // h2Dnew->Delete();
//   return weight;
// }
  
// void filework(TH2D *htoy, TH1D* Htrigeff0, TH1D* Htrigeff1, int fYear,  int iloop){

//   TFile* file = NULL;
//   if(fYear==2017)
//     file=TFile::Open("root://cmseos.fnal.gov//eos/uscms/store/user/ckar/Bmm5/bmm-reduced_BuToJpsiK_BMuonFilter_MC2017_nano_v3.root","READ");
//   if(fYear==2018)
//     file=TFile::Open("root://cmseos.fnal.gov//eos/uscms/store/user/ckar/Bmm5/bmm-reduced_BuToJpsiK_BMuonFilter_MC2018_nano_v3.root","READ");
//   TTree* t = (TTree*)file->Get("Candidates");

//   int   nentries = t->GetEntries();
//   Float_t fls3d(0), vtx_prob(0), Muon1_pt(0), Muon2_pt(0), Muon1_eta(0), Muon2_eta(0), chi2dof(0);
//   bool hlt1(0), Muon1_softId(0), Muon2_softId(0);

//   t->SetBranchAddress("bkmm_jpsimc_sl3d",    &fls3d);
//   t->SetBranchAddress("bkmm_jpsimc_vtx_prob", &vtx_prob);
//   if( fYear == 2018){
//     t->SetBranchAddress("HLT_DoubleMu4_3_Jpsi", &hlt1); }
//   else if(fYear== 2017){
//     t->SetBranchAddress("HLT_DoubleMu4_3_Jpsi_Displaced", &hlt1);}

//   t->SetBranchAddress("Muon1_pt", &Muon1_pt);
//   t->SetBranchAddress("Muon2_pt", &Muon2_pt);
//   t->SetBranchAddress("Muon1_eta", &Muon1_eta);
//   t->SetBranchAddress("Muon2_eta", &Muon2_eta);
//   t->SetBranchAddress("bkmm_jpsimc_vtx_chi2dof", &chi2dof);
//   t->SetBranchAddress("Muon1_softId", &Muon1_softId);
//   t->SetBranchAddress("Muon2_softId", &Muon2_softId);
  
//   //}
//   int step(1000000);
//   if (nentries < 5000000)  step = 500000;
//   if (nentries < 1000000)  step = 100000;
//   if (nentries < 100000)   step = 10000;
//   if (nentries < 10000)    step = 1000;
//   if (nentries < 1000)     step = 100;

//   double ntotal0(0), npass0(0), npass0_wo(0);
//   double ntotal1(0), npass1(0), npass1_wo(0);
//   // ----------------------------                                                                                                                                            
//   // -- the real loop starts here
  
//   // ----------------------------                                                                                                                                            
//   for (int jentry = 0; jentry < nentries; jentry++) {
//     t->GetEntry(jentry);
//     if (jentry%step == 0) {
//       TTimeStamp ts;
//       cout << Form(" Here only trigger .. evt = %d", jentry)
// 	   << ", time now: " << ts.AsString("lc")
// 	   << endl;
//     }
//     if(Muon1_pt > 4. && Muon2_pt > 4. && fls3d > 4. && chi2dof < 5 && Muon1_softId==1 && Muon2_softId==1 ){
      
//       if(max(TMath::Abs(Muon1_eta), TMath::Abs(Muon2_eta)) < 0.7) ntotal0++;
//       else if(max(TMath::Abs(Muon1_eta),TMath::Abs(Muon2_eta)) > 0.7 && (max(TMath::Abs(Muon1_eta),TMath::Abs(Muon2_eta)) < 1.4)) ntotal1++;

//       if(hlt1){
// 	double weight= weightcal(htoy, fls3d,vtx_prob);
// 	if(max(TMath::Abs(Muon1_eta), TMath::Abs(Muon2_eta)) < 0.7) {
// 	  npass0_wo++; 
// 	  npass0 += weight;
// 	}
// 	else if(max(TMath::Abs(Muon1_eta),TMath::Abs(Muon2_eta)) > 0.7 && (max(TMath::Abs(Muon1_eta),TMath::Abs(Muon2_eta)) < 1.4)) {
// 	  npass1 += weight;
// 	  npass1_wo++;

// 	}
//       }
      
//     }
//   }
//   //  Htrigeff0->
//   cout<<"Wo Wght "<< npass0_wo<<" ntotal "<<ntotal0<<" ratio "<<(double)npass0_wo/ntotal0<<endl;
//   cout<<"W Wght "<< npass0<<" ntotal "<<ntotal0<<" ratio "<<(double)npass0/ntotal0<<endl;
//   cout<<"Wo Wght Chan1 "<< npass1_wo<<" ntotal "<<ntotal1<<" ratio "<<(double)npass1_wo/ntotal1<<endl;
//   cout<<"W Wght chan1 "<< npass1<<" ntotal "<<ntotal1<<" ratio "<<(double)npass1/ntotal1<<endl;
  
//   Htrigeff0->Fill(iloop, npass0/ntotal0);
//   Htrigeff1->Fill(iloop, npass1/ntotal1);
//   file->Close();
// }
// void triggerefficiency(TString command = ""){
//   int seed = 1234;
//   int iterations = 5;
//   int fYear= 2017;
//   TString tok;

//   Ssiz_t from = 0;
//   while(command.Tokenize(tok, from, "[ \t;=:]")) {
//     if (tok=="seed") {                                 // set the random seed                                                                                     
//       command.Tokenize(tok, from, "[ \t;=:]");
//       seed = tok.Atoi();
//     }else if (tok=="iterations") {                          // set # of iterations                                                                                  
//       command.Tokenize(tok, from, "[ \t;=:]");
//       iterations = tok.Atoi();
//     }
//     else if (tok=="fYear") {
//       command.Tokenize(tok, from, "[ \t;=:]");
//       fYear = tok.Atoi();
//     }
//   }

