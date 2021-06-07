#ifndef REDTREEDATANEW
#define REDTREEDATANEW

#define NTRGMAX 40

struct redTreeDatanew{

  double corrW8;
  
  bool json, cb, tis, reftrg;
  Bool_t hlt1, hlt2, hlt3;
  Bool_t hlt_1, hlt_2, hlt_3,hlt_4,hlt_5;
  int ntrgtos[NTRGMAX];
  bool dcand;
  double bdt2, pvw8, pvz;
  double rlumi;
  int psw8;

  UInt_t          run;
  ULong64_t       evt;
  UInt_t nbkmm, nmm, nbkkmm, nMuon, nPS;
  
  Float_t pt, eta, phi,  m, me,  cosa, alpha, iso;
  Float_t fls3d, flsxy, chi2dof, pvip, pvipErr, bdt, doca;
  Float_t pvlip, pv2lip, pvlipErr;
  Float_t m1iso, m2iso, k1pt, k1eta, k1phi, k2pt, k2eta, k2phi;

  Float_t docatrk, genmass;
  Float_t bkmm_kaon_dxy_bs, bkmm_kaon_eta, bkmm_kaon_mu1_doca, bkmm_kaon_mu2_doca, bkmm_kaon_phi, bkmm_kaon_pt, bkmm_kaon_sdxy_bs;
  Int_t bkmm_k1_pdgId, bkmm_pdgId;
  Float_t bkmm_gen_l3d, bkmm_gen_pt, bkmm_gen_kaon_pt, bkmm_gen_lxy;

  Float_t bkkmm_kaon1_dxy_bs, bkkmm_kaon1_eta, bkkmm_kaon1_mu1_doca, bkkmm_kaon1_mu2_doca, bkkmm_kaon1_phi, bkkmm_kaon1_pt, bkkmm_kaon1_sdxy_bs;
  Float_t bkkmm_kaon2_dxy_bs, bkkmm_kaon2_eta, bkkmm_kaon2_mu1_doca, bkkmm_kaon2_mu2_doca, bkkmm_kaon2_phi, bkkmm_kaon2_pt, bkkmm_kaon2_sdxy_bs;
  
  Float_t bkkmm_kk_mass;
  Int_t bkkmm_k1_pdgId, bkkmm_pdgId, bkkmm_k2_pdgId;
  Float_t bkkmm_gen_l3d, bkkmm_gen_pt, bkkmm_gen_kaon1_pt, bkkmm_gen_lxy;
  Float_t bkkmm_gen_kaon2_pt;

  Int_t  closetrk, closetrks1, closetrks2, closetrks3;

  Float_t mm_bdt, mm_doca, mm_docatrk, mm_iso, mm_kin_eta, mm_kin_pt, mm_kin_l3d, mm_kin_lxy, mm_kin_sl3d, mm_kin_slxy;
  Float_t mm_kal_mass, mm_kal_lxy, mm_kal_slxy, mm_kal_vtx_prob, mm_kin_mass, mm_kin_massErr, mm_kin_alpha, mm_kin_cosAlphaXY;
  Float_t mm_kin_mu1pt, mm_kin_mu1eta, mm_kin_mu1phi, mm_kin_mu2pt, mm_kin_mu2eta, mm_kin_mu2phi;
  Float_t mm_kin_pvip, mm_kin_pvipErr, mm_kin_pv2lip, mm_kin_pv2lipErr, mm_kin_pvlip, mm_kin_pvlipErr; 
  Float_t mm_kin_vtx_chi2dof, mm_kin_vtx_prob, mm_m1iso, mm_m2iso, mm_mass;
  Int_t mm_closetrk, mm_closetrk1, mm_closetrk2, mm_closetrk3;
  Float_t mm_gen_l3d, mm_gen_pt, mm_gen_mu1_pt, mm_gen_lxy, mm_gen_mu2_pt, mm_gen_mass;
  Int_t mm_gen_mpdgId, mm_gen_mu1_mpdgId, mm_gen_mu2_mpdgId, mm_gen_mu1_pdgId, mm_gen_mu2_pdgId, mm_gen_pdgId;
  
  Float_t Muon1_dxy, Muon1_dxyErr, Muon1_dz, Muon1_dzErr, Muon1_eta, Muon1_ip3d, Muon1_sip3d;
  Float_t Muon1_mass, Muon1_phi, Muon1_pt, Muon1_ptErr, Muon1_segmentComp, Muon1_softMva, Muon1_mvaLowPt;
  Int_t Muon1_Charge, Muon1_nTrackerLayers, Muon1_nStations, Muon1_pdgId, Muon1_tightCharge;
  
  UChar_t Muon1_highPtId;
  UChar_t Muon2_highPtId;
  
  Bool_t  Muon1_inTimeMuon, Muon1_isGlobal, Muon1_isPFcand, Muon1_isTracker, Muon1_looseId, Muon1_mediumId, Muon1_mediumPromptId, Muon1_softId, Muon1_softMvaId, Muon1_tightId;
  UChar_t Muon1_mvaId;

  Float_t Muon2_dxy, Muon2_dxyErr, Muon2_dz, Muon2_dzErr, Muon2_eta, Muon2_ip3d, Muon2_sip3d;
  Float_t Muon2_mass, Muon2_phi, Muon2_pt, Muon2_ptErr, Muon2_segmentComp, Muon2_softMva, Muon2_mvaLowPt;
  Int_t Muon2_Charge, Muon2_nTrackerLayers, Muon2_nStations, Muon2_pdgId, Muon2_tightCharge;
  
  Bool_t  Muon2_inTimeMuon, Muon2_isGlobal, Muon2_isPFcand, Muon2_isTracker, Muon2_looseId, Muon2_mediumId, Muon2_mediumPromptId, Muon2_softId, Muon2_softMvaId, Muon2_tightId;
  UChar_t Muon2_mvaId;

  Int_t bkmm_mm_index, mm_mu1_index, mm_mu2_index;
  Float_t bkmm_jpsimc_vtx_prob;
  Float_t  PSW8;

  Int_t bkkmm_mm_index;
  Float_t bkkmm_jpsimc_vtx_prob;

  Int_t m1q ,m2q;
  
  Double_t m2id, m1id, m_s;
  Int_t chan_s;
  // Float_t pt, eta, phi,  m, me,  cosa, alpha, iso;
  Double_t m1pt, m2pt, m1eta, m2eta, m1phi, m2phi;
  // Float_t pvlip, pv2lip, m1eta, m2eta, m1phi, m2phi,  pvlipErr;
  // Float_t m1iso, m2iso, k1pt, k1eta;
  Float_t  w_sl3d, w_PU , w_trigJpsi, w_trigJpsi_err;
  // Float_t docatrk, genmass;
  // Int_t bkmm_k1_pdgId, bkmm_pdgId;
  // Int_t  closetrk, closetrks1, closetrks2, closetrks3;

};

#endif
