#ifndef REDTREEDATANEW
#define REDTREEDATANEW

#define NTRGMAX 40

struct redTreeDatanew{

  double corrW8;
  
  bool json, cb, tis, reftrg;
  Bool_t hlt1, hlt2, hlt3;
  int ntrgtos[NTRGMAX];
  bool dcand;
  double bdt2, pvw8, pvz;
  double rlumi;

  UInt_t          run;
  ULong64_t       evt;
  UInt_t nbkmm, nmm, nMuon, nPS;
  
  Float_t pt[15], eta[15], phi[15],  m[15], me[15],  cosa[15], alpha[15], iso[15];
  Float_t m1pt[15], m2pt[15], fls3d[15], flsxy[15], chi2dof[15], pvip[15], pvipErr[15], bdt[15], doca[15];
  Float_t pvlip[15], pv2lip[15], m1eta[15], m2eta[15], m1phi[15], m2phi[15],  pvlipErr[15];
  Float_t m1iso[15], m2iso[15], k1pt[15], k1eta[15], k1phi[15];

  Float_t docatrk[15], genmass[15];
  Float_t bkmm_kaon_dxy_bs[15], bkmm_kaon_eta[15], bkmm_kaon_mu1_doca[15], bkmm_kaon_mu2_doca[15], bkmm_kaon_phi[15], bkmm_kaon_pt[15], bkmm_kaon_sdxy_bs[15];
  Int_t bkmm_k1_pdgId[15], bkmm_pdgId[15];
  Float_t bkmm_gen_l3d[15], bkmm_gen_pt[15], bkmm_gen_kaon_pt[15], bkmm_gen_lxy[15];

  Int_t  closetrk[15], closetrks1[15], closetrks2[15], closetrks3[15];

  Float_t mm_bdt[12], mm_doca[12], mm_docatrk[12], mm_iso[12], mm_kin_eta[12], mm_kin_pt[12], mm_kin_l3d[12], mm_kin_lxy[12], mm_kin_sl3d[12], mm_kin_slxy[12];
  Float_t mm_kal_mass[12], mm_kal_lxy[12], mm_kal_slxy[12], mm_kal_vtx_prob[12], mm_kin_mass[12], mm_kin_massErr[12], mm_kin_alpha[12], mm_kin_cosAlphaXY[12];
  Float_t mm_kin_mu1pt[12], mm_kin_mu1eta[12], mm_kin_mu1phi[12], mm_kin_mu2pt[12], mm_kin_mu2eta[12], mm_kin_mu2phi[12];
  Float_t mm_kin_pvip[12], mm_kin_pvipErr[12], mm_kin_pv2lip[12], mm_kin_pv2lipErr[12], mm_kin_pvlip[12], mm_kin_pvlipErr[12]; 
  Float_t mm_kin_vtx_chi2dof[12], mm_kin_vtx_prob[12], mm_m1iso[12], mm_m2iso[12], mm_mass[12];
  Int_t mm_closetrk[12], mm_closetrk1[12], mm_closetrk2[12], mm_closetrk3[12];
  Float_t mm_gen_l3d[12], mm_gen_pt[12], mm_gen_mu1_pt[12], mm_gen_lxy[12], mm_gen_mu2_pt[12], mm_gen_mass[12];
  Int_t mm_gen_mpdgId[12], mm_gen_mu1_mpdgId[12], mm_gen_mu2_mpdgId[12], mm_gen_mu1_pdgId[12], mm_gen_mu2_pdgId[12], mm_gen_pdgId[12];
  
  Float_t Muon_dxy[12], Muon_dxyErr[12], Muon_dz[12], Muon_dzErr[12], Muon_eta[12], Muon_ip3d[12], Muon_sip3d[12];
  Float_t Muon_mass[12], Muon_phi[12], Muon_pt[12], Muon_ptErr[12], Muon_segmentComp[12], Muon_softMva[12], Muon_mvaLowPt[12];
  Int_t Muon_Charge[12], Muon_nTrackerLayers[12], Muon_nStations[12], Muon_pdgId[12], Muon_tightCharge[12];
  
  UInt_t Muon_highPtId[12];
  
  Bool_t          Muon_inTimeMuon[12], Muon_isGlobal[12], Muon_isPFcand[12], Muon_isTracker[12], Muon_looseId[12], Muon_mediumId[12], Muon_mediumPromptId[12], Muon_softId[12], Muon_softMvaId[12], Muon_tightId[12];
  UChar_t         Muon_mvaId[12];

  Int_t bkmm_mm_index[15], mm_mu1_index[15], mm_mu2_index[15];
  Float_t bkmm_jpsimc_vtx_prob[15];
  Float_t  PSW8;
  // Float_t pt, eta, phi,  m, me,  cosa, alpha, iso;
  // Float_t m1pt, m2pt, fls3d, flsxy, chi2dof, pvip, pvipErr, bdt, doca;
  // Float_t pvlip, pv2lip, m1eta, m2eta, m1phi, m2phi,  pvlipErr;
  // Float_t m1iso, m2iso, k1pt, k1eta;

  // Float_t docatrk, genmass;
  // Int_t bkmm_k1_pdgId, bkmm_pdgId;
  // Int_t  closetrk, closetrks1, closetrks2, closetrks3;

};

#endif
