#include "candAnaBu2JpsiK.hh"
#include <cmath>
#include <string>

#include "common/HFMasses.hh"
#include "common/ana.hh"

#include "common/AnalysisDistribution.hh"

using namespace std;
#define NEW_MATCHING

// ----------------------------------------------------------------------
candAnaBu2JpsiK::candAnaBu2JpsiK(bmmReader *pReader, std::string name, std::string cutsFile) : candAna(pReader, name, cutsFile) {
  fAnaCuts.setAcName("candAnaBu2JpsiK");
  fGenK1Tmi = fRecK1Tmi = -1;
  BLIND = 0;
  cout << "==> candAnaBu2JpsiK: name = " << name << ", reading cutsfile " << cutsFile << endl;
  readCuts(cutsFile, 1);
}


// ----------------------------------------------------------------------
candAnaBu2JpsiK::~candAnaBu2JpsiK() {
  cout << "==> candAnaBu2JpsiK: destructor..." << endl;
}


// ----------------------------------------------------------------------
void candAnaBu2JpsiK::candAnalysis() {

  fGoodTracks     = false;
  fGoodTracksPt   = false;
  fGoodTracksEta  = false;

  fGoodAcceptance = false;
  fGoodJpsiMass   = false;
  fGoodJpsiCuts   = false;

  fHelicity = -99.;

  if (0 == fpCand) return;

  TAnaTrack *p0, *pk(0);
  for (int it = fpCand->fSig1; it <= fpCand->fSig2; ++it) {
    p0 = fpEvt->getSigTrack(it);
    if (321 == TMath::Abs(p0->fMCID)) {
      pk = p0;
    }
    if (211 == TMath::Abs(p0->fMCID)) {
      pk = p0;
    }
  }

  // -- check for overlap with a Bd -> J/psi Kstar (300511) candidate
  vector<int> idx0, idx1;
  getSigTracks(idx0, fpCand);
  int overlap(0);
  fBdJpsiKstarMass = -99.;
  for (int iC = 0; iC < fpEvt->nCands(); ++iC) {
    TAnaCand *pC = fpEvt->getCand(iC);
    if (300511 != pC->fType) continue;
    idx1.clear();
    getSigTracks(idx1, pC);
    // -- check for the same tracks
    overlap = 0;
    for (unsigned int i0 = 0; i0 < idx0.size(); ++i0) {
      for (unsigned int i1 = 0; i1 < idx1.size(); ++i1) {
	if (idx0[i0] == idx1[i1]) {
	  ++overlap;
	}
      }
    }
    // -- if all 3 track overlap, get kstar mass of the other cand
    if (3 == overlap) {
      fBdJpsiKstarMass = pC->fMass;
    }
  }


  if (0 == pk) {
    cout << "candAnaBu2JpsiK::candAnalysis:  no kaon found " << endl;
    fCandM = -98.;
    return;
  }

  fKaonPt        = pk->fRefPlab.Perp();
  fKaonEta       = pk->fRefPlab.Eta();
  fKaonPhi       = pk->fRefPlab.Phi();
  fKaonTkQuality = highPurity(pk);
  fKaonPtNrf     = pk->fPlab.Perp();
  fKaonEtaNrf    = pk->fPlab.Eta();

  fHitsBpix      = numberOfBPixLayers(pk);
  fHitsPix       = numberOfPixLayers(pk);
  fHitsStrip     = numberOfTrackerLayers(pk);

  // -- START for tracking studies
  fKaontkqual         = pk->fTrackQuality;
  fKaonalg            = pk->fAlgorithm;
  fKaonvalhits        = pk->fValidHits;
  fKaonvalhitfraction = pk->fValidHitFraction;
  fKaonlayerswithhits = pk->fLayersWithHits;
  fKaonchi2           = pk->fChi2;
  fKaondz             = pk->fdz;
  fKaondzE            = pk->fdzE;
  fKaond0             = pk->fd0;
  fKaond0E            = pk->fd0E;
  fKaondsz            = pk->fdsz;
  fKaondszE           = pk->fdszE;
  fKaondxy            = pk->fdxy;
  fKaondxyE           = pk->fdxyE;
  fKaonPtE            = pk->fPtE;
  fKaonEtaE           = pk->fEtaE;
  fKaonPhiE           = pk->fPhiE;
  // -- END for tracking studies


  if (fCandTmi > -1 && fCandTmi == fpCand->fIndex) {
    TGenCand *pg1 = fpEvt->getGenTWithIndex(fpEvt->getSimpleTrack(pk->fIndex)->getGenIndex());
    fKPtGen     = pg1->fP.Perp();
    fKEtaGen    = pg1->fP.Eta();
  } else {
    fKPtGen     = -99.;
    fKEtaGen    = -99.;
  }

  // -- Check for J/psi mass
  //cout<<" check jpsi "<<endl;

  TAnaCand *pD = 0;
  double chi2(0.);
  double ndof(0.);
  for (int i = fpCand->fDau1; i <= fpCand->fDau2; ++i) {
    if (i < 0) break;
    pD = fpEvt->getCand(i);
    //cout<<i<<" "<<pD->fType<<" "<<JPSITYPE<<endl;

    if (pD->fType == JPSITYPE) {
      if ((JPSIMASSLO < pD->fMass) && (pD->fMass < JPSIMASSHI)) fGoodJpsiMass = true;
      fJpsiMass = pD->fMass;
      fJpsiPt   = pD->fPlab.Perp();
      fJpsiEta  = pD->fPlab.Eta();
      fJpsiPhi  = pD->fPlab.Phi();
      fJpsiCosA = TMath::Cos(pD->fAlpha);
      fJpsiMaxDoca = pD->fMaxDoca;
      fJpsiFLSxy   = pD->fVtx.fDxy/pD->fVtx.fDxyE;
      fJpsiVtxProb = pD->fVtx.fProb;

      chi2 = pD->fVtx.fChi2;
      ndof = pD->fVtx.fNdof;
      break;
    }
  }

  candAna::candAnalysis();

  // -- overwrite some variables
  fCandChi2  = chi2;
  fCandDof   = ndof;
  fCandChi2Dof = chi2/ndof;

  fGoodTracks    = fGoodTracks    && fKaonTkQuality;
  fGoodTracksPt  = fGoodTracksPt  && ((TRACKPTLO < fKaonPt)   && (fKaonPt < TRACKPTHI));
  fGoodTracksEta = fGoodTracksEta && ((TRACKETALO < fKaonEta) && (fKaonEta < TRACKETAHI));

  fGoodAcceptance = fGoodAcceptance /*&& fGoodTracks*/     && fGoodTracksPt && fGoodTracksEta;
  fGoodJpsiCuts   = fGoodJpsiMass;
  //  fGoodJpsiCuts   = fGoodJpsiCuts   && (fJpsiPt > 7.) && (fJpsiCosA > 0.9) &&  (fJpsiFLSxy > 3.) && (fJpsiVtxProb > 0.1);
  fGoodJpsiCuts   = fGoodJpsiCuts   && (fJpsiPt > 7.) && (fJpsiVtxProb > 0.1);

  ((TH1D*)fHistDir->Get(Form("mon%s", fName.c_str())))->Fill(10);
  ((TH1D*)fHistDir->Get("../monEvents"))->Fill(3);



  // -- helicity
  TLorentzVector p4B; p4B.SetVectM(fpCand->fPlab, MBPLUS);
  TLorentzVector p4M1; p4M1.SetVectM(fpMuon1->fPlab, MMUON);
  TLorentzVector p4M2; p4M2.SetVectM(fpMuon2->fPlab, MMUON);
  TLorentzVector p4K;  p4K.SetVectM(pk->fPlab, MKAON);
  TLorentzVector p4Psi; p4Psi.SetVectM(pD->fPlab, MJPSI);

  TLorentzVector p4Mneg = (fpMuon1->fQ < 0? p4M1: p4M2);
  TVector3 boostToB = p4B.BoostVector();

  TLorentzVector p4PsiinB = p4Psi;
  p4PsiinB.Boost(-boostToB);

  TLorentzVector p4MneginB = p4Mneg;
  p4MneginB.Boost(-boostToB);

  TLorentzVector p4KinB = p4K;
  p4KinB.Boost(-boostToB);

  TVector3 boostToPsi = p4PsiinB.BoostVector();

  TLorentzVector p4KinPsi = p4KinB;
  p4KinPsi.Boost(-boostToPsi);

  TLorentzVector p4MneginPsi = p4MneginB;
  p4MneginPsi.Boost(-boostToPsi);

  fHelicity = p4MneginPsi.Angle(p4KinPsi.Vect());

}

// ----------------------------------------------------------------------
void candAnaBu2JpsiK::moreBasicCuts() {
  cout << "*** candAnaBu2JpsiK: more basic cuts" << endl;
  fAnaCuts.addCut("fGoodJpsiMass", "m(J/psi)", fGoodJpsiMass);
}


// ----------------------------------------------------------------------
// -- version for PYTHIA8 and new-style EvtGen daughter insertion
void candAnaBu2JpsiK::genMatch() {

  fGenM1Tmi = fGenM2Tmi = fGenK1Tmi = -1;
  fNGenPhotons = 0;

  TGenCand *pC(0), *pM1(0), *pM2(0), *pK(0), *pB(0), *pPsi(0), *pTmp(0);
  int nb(0), nphotons(0);
  bool goodMatch(false);
  for (int i = 0; i < fpEvt->nGenT(); ++i) {
    pC = fpEvt->getGenT(i);
    if (521 == TMath::Abs(pC->fID)) {
      // cout << " found B+ " << TYPE << endl;
      pB = pC;
      nb = 0;
      for (int id = pB->fDau1; id <= pB->fDau2; ++id) {
	pC = fpEvt->getGenTWithIndex(id);
	if (443 == TMath::Abs(pC->fID)) {
	  // cout << " found JPsi " << endl;
	  ++nb;
	  pPsi = pC;
	  pM1 = pM2 = 0;
	  for (int idd = pPsi->fDau1; idd <= pPsi->fDau2; ++idd) {
	    pC = fpEvt->getGenTWithIndex(idd);
	    if (22 == TMath::Abs(pC->fID)) ++nphotons;
	    if (13 == TMath::Abs(pC->fID)) {
	      if (0 == pM1) {
		pM1 = fpEvt->getGenTWithIndex(idd);
	      } else {
		pM2 = fpEvt->getGenTWithIndex(idd);
	      }
	      // cout << " mu " << pM1 << " " << pM2 << endl;
	    }
	  }
	} else if ((211 == TMath::Abs(pC->fID)) && (66 == (TYPE%1000))) {
	  // Bu2JpsiPi
	  ++nb;
	  pK = fpEvt->getGenTWithIndex(id);
	} else if (321 == TMath::Abs(pC->fID)) {
	  ++nb;
	  pK = fpEvt->getGenTWithIndex(id);
	} else 	if (22 == TMath::Abs(pC->fID)) {
	  ++nphotons;
	} else {
	  ++nb;
	}
      }
      if (0 != pM1 && 0 != pM2 && 0 != pK && (pPsi->fMom1 == pK->fMom1)) {
	// -- check that there are no other direct daughters than J/psi K (plus possibly photons)
	int nDaughters(0);
	for (int ij = 0; ij < fpEvt->nGenT(); ++ij) {
	  pTmp = fpEvt->getGenT(ij);
	  if (pTmp->fMom1 == pB->fNumber) {
	    if (pTmp->fID != 22) ++nDaughters;
	  }
	}
	if (2 == nDaughters) {
	  goodMatch = true;
	  fNGenPhotons = nphotons;
	  break;
	}
      }
    }
  }

  fGenBTmi = -1;
  if (goodMatch) {
    fMu1GenID = pM1->fID;
    fMu2GenID = pM2->fID;
    fKGenID = pK->fID;
    fGenBTmi = pB->fNumber;
    double m = pB->fP.Mag();
    double p = pB->fP.P();
    // Meson pointer
    TGenCand *pM = pB;
    double x = (pM1->fV - pM->fV).Mag();
    fGenFl3d = x;
    fGenLifeTime = x*m/p/TMath::Ccgs();
    if (pM1->fP.Perp() > pM2->fP.Perp()) {
      fGenM1Tmi = pM1->fNumber;
      fGenM2Tmi = pM2->fNumber;
    } else {
      fGenM1Tmi = pM2->fNumber;
      fGenM2Tmi = pM1->fNumber;
    }
    fGenK1Tmi = pK->fNumber;
  } else {
    fGenM1Tmi = -1;
    fGenM2Tmi = -1;
    fGenK1Tmi = -1;
  }

  if (fVerbose > 10) {
    cout << "fGenM1Tmi = " << fGenM1Tmi << endl;
    cout << "fGenM2Tmi = " << fGenM2Tmi << endl;
    cout << "fGenK1Tmi = " << fGenK1Tmi << endl;
  }

}


// ----------------------------------------------------------------------
// -- version for PYTHIA6 and old-style EvtGen daughter insertion
void candAnaBu2JpsiK::genMatchOld() {

  fGenM1Tmi = fGenM2Tmi = fGenK1Tmi = -1;
  fNGenPhotons = 0;

  TGenCand *pC(0), *pM1(0), *pM2(0), *pK(0), *pB(0), *pPsi(0);
  int nb(0), nphotons(0);
  bool goodMatch(false);
  for (int i = 0; i < fpEvt->nGenT(); ++i) {
    pC = fpEvt->getGenT(i);
    if (521 == TMath::Abs(pC->fID)) {
      //cout<<" found B0 "<<TYPE<<endl;
      pB = pC;
      nb = 0;
      for (int id = pB->fDau1; id <= pB->fDau2; ++id) {
	pC = fpEvt->getGenTWithIndex(id);
	if (443 == TMath::Abs(pC->fID)) {
	  // cout<<" found JPsi "<<endl;
	  ++nb;
	  pPsi = pC;
	  pM1 = pM2 = 0;
	  for (int idd = pPsi->fDau1; idd <= pPsi->fDau2; ++idd) {
	    pC = fpEvt->getGenTWithIndex(idd);
	    if (22 == TMath::Abs(pC->fID)) ++nphotons;
	    if (13 == TMath::Abs(pC->fID)) {
	      if (0 == pM1) {
		pM1 = fpEvt->getGenTWithIndex(idd);
	      } else {
		pM2 = fpEvt->getGenTWithIndex(idd);
	      }
	      //cout<<" mu "<<pM1<<" "<<pM2<<endl;
	    }
	  }
	} else if ( (TYPE%1000)==66 && 211 == TMath::Abs(pC->fID)) { // get Bu2JpsiPi
	  ++nb;
	  pK = fpEvt->getGenTWithIndex(id);
	  //cout<<" pi "<<pK<<" "<<nb<<endl;
	} else if (321 == TMath::Abs(pC->fID)) {
	  ++nb;
	  pK = fpEvt->getGenTWithIndex(id);
	  //cout<<" K "<<pK<<" "<<nb<<endl;
	} else 	if (22 == TMath::Abs(pC->fID)) {
	  ++nphotons;
	} else {
	  ++nb;
	}
      }
      if (nb > 2) {
	pM1 = pM2 = pK = pPsi = 0;
	continue; // skip B decays where more than J/psi and kaon came from B
      }
      if (0 != pM1 && 0 != pM2 && 0 != pK && (pPsi->fMom1 == pK->fMom1)) {
	goodMatch = true;
	//cout<<" goodMatch "<<goodMatch<<endl;
	fNGenPhotons = nphotons;
	break;
      }
    }
  }

  fGenBTmi = -1;
  if (goodMatch) {
    fMu1GenID = pM1->fID;
    fMu2GenID = pM2->fID;
    fKGenID = pK->fID;
    fGenBTmi = pB->fNumber;
    double m = pB->fP.Mag();
    double p = pB->fP.P();
    // Meson pointer
    TGenCand *pM = pB;
    double x = (pM1->fV - pM->fV).Mag();
    fGenFl3d = x;
    fGenLifeTime = x*m/p/TMath::Ccgs();
    if (pM1->fP.Perp() > pM2->fP.Perp()) {
      fGenM1Tmi = pM1->fNumber;
      fGenM2Tmi = pM2->fNumber;
    } else {
      fGenM1Tmi = pM2->fNumber;
      fGenM2Tmi = pM1->fNumber;
    }
    fGenK1Tmi = pK->fNumber;
  } else {
    fGenM1Tmi = -1;
    fGenM2Tmi = -1;
    fGenK1Tmi = -1;
  }

  if (fVerbose > 10) {
    cout << "fGenM1Tmi = " << fGenM1Tmi << endl;
    cout << "fGenM2Tmi = " << fGenM2Tmi << endl;
    cout << "fGenK1Tmi = " << fGenK1Tmi << endl;
  }
}



// ----------------------------------------------------------------------
void candAnaBu2JpsiK::recoMatch() {

  fRecM1Tmi = fRecM2Tmi = fRecK1Tmi = -1;
  TSimpleTrack *pT(0);
  for (int i = 0; i < fpEvt->nSimpleTracks(); ++i) {
    pT = fpEvt->getSimpleTrack(i);
    if (pT->getGenIndex() < 0) continue;
    // -- muon 1
    if (fGenM1Tmi > -1 && pT->getGenIndex() == fGenM1Tmi) {
      fRecM1Tmi = i;
    }

    // -- muon 2
    if (fGenM2Tmi > -1 && pT->getGenIndex() == fGenM2Tmi) {
      fRecM2Tmi = i;
    }

    // -- kaon
    if (fGenK1Tmi > -1 && pT->getGenIndex() == fGenK1Tmi) {
      fRecK1Tmi = i;
    }

    // -- skip rest if all matches found
    if (fRecM1Tmi > -1 && fRecM2Tmi > -1 && fRecK1Tmi > -1) break;
  }


  if (fVerbose > 10) {
    cout << "fRecM1Tmi = " << fRecM1Tmi << " matched to fGenM1Tmi = " << fGenM1Tmi << endl;
    cout << "fRecM2Tmi = " << fRecM2Tmi << " matched to fGenM2Tmi = " << fGenM2Tmi << endl;
    cout << "fRecK1Tmi = " << fRecK1Tmi << " matched to fGenK1Tmi = " << fGenK1Tmi << endl;
  }

}


// ----------------------------------------------------------------------
void candAnaBu2JpsiK::candMatch() {

  fCandTmi = -1;
  int idx(-1), type(-1);
  int d1Matched(0), d2Matched(0), d3Matched(0);
  TAnaCand *pCand(0);

  //cout<<TYPE<<endl;

  for (int iC = 0; iC < fpEvt->nCands(); ++iC) {
    pCand = fpEvt->getCand(iC);

    //cout<<TYPE<<" "<<pCand->fType<<" "<<iC<<endl;

    if (TYPE != pCand->fType) continue;

    d1Matched = d2Matched = d3Matched = 0;
    //cout<< TYPE;
    for (int i = pCand->fSig1; i <= pCand->fSig2; ++i) {
      idx = fpEvt->getSigTrack(i)->fIndex;
      type = TMath::Abs(fpEvt->getSigTrack(i)->fMCID);
      //cout<<" "<<idx<<" "<<type;
      if (fVerbose > 10) {
	cout << idx << " " << fRecM1Tmi << " " << fRecM2Tmi << " " << fRecK1Tmi << endl;
      }
      if (fRecM1Tmi > -1 && type == 13 && idx == fRecM1Tmi) {
	d1Matched = 1;
      }
      if (fRecM2Tmi > -1 && type == 13 && idx == fRecM2Tmi) {
	d2Matched = 1;
      }
      if (fRecK1Tmi > -1 && type == 321 && idx == fRecK1Tmi) {
	d3Matched = 1;
      }
    }

    //cout<<endl;

    if (d1Matched && d2Matched && d3Matched) {
      fCandTmi = iC;
      break;
    }
  }
  if (fVerbose > 10) {
    cout << "fCandTmi = " << fCandTmi << " matched to rec tracks " << fRecM1Tmi << " " << fRecM2Tmi << " " << fRecK1Tmi << endl;
  }

}


// ----------------------------------------------------------------------
void candAnaBu2JpsiK::bookHist() {
  cout << "==>candAnaBu2JpsiK: bookHist" << endl;
  candAna::bookHist();

  moreReducedTree(fTree);

  // -- Additional effTree variables
  fEffTree->Branch("kpt",    &fETk1pt,            "kpt/F");
  fEffTree->Branch("g3pt",   &fETg3pt,            "g3pt/F");
  fEffTree->Branch("keta",   &fETk1eta,           "keta/F");
  fEffTree->Branch("g3eta",  &fETg3eta,           "g3eta/F");
  fEffTree->Branch("kq",     &fETk1q,             "kq/I");
  fEffTree->Branch("kgt",    &fETk1gt,            "kgt/O");
  fEffTree->Branch("ghel",   &fETghel,            "ghel/D");
  fEffTree->Branch("nga",    &fNGenPhotons,       "nga/I");

}



// ----------------------------------------------------------------------
void candAnaBu2JpsiK::moreReducedTree(TTree *t) {
  // -- Additional reduced tree variables
  t->Branch("mpsi",        &fJpsiMass,    "mpsi/D");
  t->Branch("psipt",       &fJpsiPt,      "psipt/D");
  t->Branch("psieta",      &fJpsiEta,     "psieta/D");
  t->Branch("psiphi",      &fJpsiPhi,     "psiphi/D");
  t->Branch("psicosa",     &fJpsiCosA,    "psicosa/D");
  t->Branch("psimaxdoca",  &fJpsiMaxDoca, "psimaxdoca/D");
  t->Branch("psiflsxy",    &fJpsiFLSxy,   "psiflsxy/D");
  t->Branch("psiprob",     &fJpsiVtxProb, "psiprob/D");
  t->Branch("bdpsikstarmass", &fBdJpsiKstarMass, "bdpsikstarmass/D");

  t->Branch("kpt",  &fKaonPt,        "kpt/D");
  t->Branch("keta", &fKaonEta,       "keta/D");
  t->Branch("kphi", &fKaonPhi,       "kphi/D");
  t->Branch("kgt",  &fKaonTkQuality, "kgt/I");
  t->Branch("kbpix",&fHitsBpix,      "kbpix/I");
  t->Branch("kpix", &fHitsPix,       "kpix/I");
  t->Branch("ktrk", &fHitsStrip,     "ktrk/I");


  // -- START for tracking studies
  t->Branch("ktkqual",         &fKaontkqual,         "ktkqual/I");
  t->Branch("kalg",            &fKaonalg,            "kalg/I");
  t->Branch("kvalhits",        &fKaonvalhits,        "kvalhits/I");
  t->Branch("klayerswithhits", &fKaonlayerswithhits, "klayerswithhits/I");
  t->Branch("kvalhitfraction", &fKaonvalhitfraction, "kvalhitfraction/D");
  t->Branch("kchi2",           &fKaonchi2,           "kchi2/D");
  t->Branch("kdz",             &fKaondz,             "kdz/D");
  t->Branch("kdzE",            &fKaondzE,            "kdzE/D");
  t->Branch("kd0",             &fKaond0,             "kd0/D");
  t->Branch("kd0E",            &fKaond0E,            "kd0E/D");
  t->Branch("kdsz",            &fKaondsz,            "kdsz/D");
  t->Branch("kdszE",           &fKaondszE,           "kdszE/D");
  t->Branch("kdxy",            &fKaondxy,            "kdxy/D");
  t->Branch("kdxyE",           &fKaondxyE,           "kdxyE/D");
  t->Branch("kptE",            &fKaonPtE,            "kptE/D");
  t->Branch("ketaE",           &fKaonEtaE,           "ketaE/D");
  t->Branch("kphiE",           &fKaonPhiE,           "kphiE/D");
  // -- END for tracking studies


  t->Branch("t3pt", &fKaonPtNrf, "t3pt/D");
  t->Branch("t3eta",&fKaonEtaNrf,"t3eta/D");
  t->Branch("g3pt", &fKPtGen,    "g3pt/D");
  t->Branch("g3eta",&fKEtaGen,   "g3eta/D");
  t->Branch("g3id", &fKGenID,    "g3id/I");
  t->Branch("k1missid",  &fKa1Missid,    "k1missid/O");
  t->Branch("k1mumatch", &fKa1MuMatch,    "k1mumatch/O");
  t->Branch("k1mumatchr",  &fKa1MuMatchR,    "k1mumatchr/F");
  t->Branch("k1mumatchr2", &fKa1MuMatchR2,    "k1mumatchr2/F");

  t->Branch("hel", &fHelicity, "hel/D");

}

// ----------------------------------------------------------------------
void candAnaBu2JpsiK::fillCandidateHistograms(int offset) {
  candAna::fillCandidateHistograms(offset);
}



// ----------------------------------------------------------------------
void candAnaBu2JpsiK::efficiencyCalculation() {
  // -- gen level
  TGenCand *pB(0), *pM1(0), *pM2(0), *pK(0);
  if (-1 == fGenM1Tmi || -1 == fGenM2Tmi || -1 == fGenK1Tmi) {
    if (fVerbose > 2 ) cout << "--------------------> No matched signal decay found" << endl;
    return;
  }
  pB  = fpEvt->getGenTWithIndex(fGenBTmi);
  pM1 = fpEvt->getGenTWithIndex(fGenM1Tmi);
  pM2 = fpEvt->getGenTWithIndex(fGenM2Tmi);
  pK  = fpEvt->getGenTWithIndex(fGenK1Tmi);

  // -- helicity
  TLorentzVector p4B   = pB->fP;
  TLorentzVector p4M1  = pM1->fP;
  TLorentzVector p4M2  = pM2->fP;
  TLorentzVector p4K   = pK->fP;
  TLorentzVector p4Psi = pM1->fP + pM2->fP;

  TLorentzVector p4Mneg = (pM1->fQ < 0? pM1->fP: pM2->fP);
  TVector3 boostToB = p4B.BoostVector();

  TLorentzVector p4PsiinB = p4Psi;
  p4PsiinB.Boost(-boostToB);

  TLorentzVector p4MneginB = p4Mneg;
  p4MneginB.Boost(-boostToB);

  TLorentzVector p4KinB = p4K;
  p4KinB.Boost(-boostToB);

  TVector3 boostToPsi = p4PsiinB.BoostVector();

  TLorentzVector p4KinPsi = p4KinB;
  p4KinPsi.Boost(-boostToPsi);

  TLorentzVector p4MneginPsi = p4MneginB;
  p4MneginPsi.Boost(-boostToPsi);

  fETghel = p4MneginPsi.Angle(p4KinPsi.Vect());

  // -- reco level
  TSimpleTrack *prM1(0), *prM2(0), *prK(0);
  double bla(0);
  int m1Matched(0), m2Matched(0), kMatched(0), m1ID(0), m1mvaID(0), m2ID(0), m2mvaID(0), m1GT(0), m2GT(0), kGT(0);
  if (fRecM1Tmi > -1) {
    m1Matched = 1;
    prM1 = fpEvt->getSimpleTrack(fRecM1Tmi);
    if (mvaMuon(prM1, bla)) m1mvaID = 1;
    if (prM1->getHighPurity()) {
      m1GT = 1;
    } else {
      m1GT = 0;
    }
  }

  if (fRecM2Tmi > -1) {
    m2Matched = 1;
    prM2 = fpEvt->getSimpleTrack(fRecM2Tmi);
    if (mvaMuon(prM2, bla)) m2mvaID = 1;
    if (prM2->getHighPurity()) {
      m2GT = 1;
    } else {
      m2GT = 0;
    }
  }

  if (fRecK1Tmi > -1) {
    kMatched = 1;
    prK = fpEvt->getSimpleTrack(fRecK1Tmi);
    if (prK->getHighPurity()) {
      kGT = 1;
    } else {
      kGT = 0;
    }
  }

  m1ID = m1mvaID;
  m2ID = m2mvaID;

  // -- cand level
  TAnaCand *pCand(0);
  if (fCandTmi > -1) {
    pCand = fpEvt->getCand(fCandTmi);
  }

  // -- EffTree filling for all events with a signal decay
  fETgm    = pB->fP.M();
  fETgpt   = pB->fP.Perp();
  fETgtau  = fGenLifeTime;
  fETgeta  = pB->fP.Eta();
  fETg1pt  = pM1->fP.Perp();
  fETg1eta = pM1->fP.Eta();
  fETg2pt  = pM2->fP.Perp();
  fETg2eta = pM2->fP.Eta();
  fETg3pt  = pK->fP.Perp();
  fETg3eta = pK->fP.Eta();
  if (m1Matched) {
    fETm1pt  = prM1->getP().Perp();
    fETm1eta = prM1->getP().Eta();
    fETm1q   = prM1->getCharge();
    fETm1gt  = (m1GT>0?true:false);
    fETm1id  = (m1ID>0?true:false);
    fETm1mvaid = (m1mvaID>0?true:false);
  } else {
    fETm1pt  = -99.;
    fETm1eta = -99.;
    fETm1q   = -99;
    fETm1gt  = false;
    fETm1id  = false;
    fETm1mvaid = false;
  }
  if (m2Matched) {
    fETm2pt  = prM2->getP().Perp();
    fETm2eta = prM2->getP().Eta();
    fETm2q   = prM2->getCharge();
    fETm2gt  = (m2GT>0?true:false);
    fETm2id  = (m2ID>0?true:false);
    fETm2mvaid = (m2mvaID>0?true:false);
  } else {
    fETm2pt  = -99.;
    fETm2eta = -99.;
    fETm2q   = -99;
    fETm2gt  = false;
    fETm2id  = false;
    fETm2mvaid = false;
  }
  if (m1Matched && m2Matched) {
    fETchan = detChan(fETm1eta, fETm2eta);
  } else {
    fETchan = -1;
  }
  if (kMatched) {
    fETk1pt  = prK->getP().Perp();
    fETk1eta = prK->getP().Eta();
    fETk1q   = prK->getCharge();
    fETk1gt  = (kGT>0?true:false);
  } else {
    fETk1pt  = -99.;
    fETk1eta = -99.;
    fETk1q   = -99;
    fETk1gt  = false;
  }
  if (pCand) {
    fETcandMass = pCand->fMass;
    fETtau      = pCand->fTau3d;
  } else {
    fETcandMass = -99.;
    fETtau      = -99.;
  }

  fEffTree->Fill();

}


// ----------------------------------------------------------------------
void candAnaBu2JpsiK::readCuts(string filename, int dump) {
  candAna::readCuts(filename, dump);

  fCutFile = filename;

  if (dump) cout << "==> candAnaBu2JpsiK: Reading " << fCutFile << " for cut settings" << endl;
  vector<string> cutLines;
  readFile(fCutFile, cutLines);

  char CutName[100];
  float CutValue;

  char  buffer[200];
  fHistDir->cd();
  TH1D *hcuts = (TH1D*)fHistDir->Get("hcuts");
  hcuts->GetXaxis()->SetBinLabel(200, fCutFile.c_str());
  int ibin;
  string cstring = "B cand";

  for (unsigned int i = 0; i < cutLines.size(); ++i) {
    sprintf(buffer, "%s", cutLines[i].c_str());

    if (buffer[0] == '#') {continue;}
    if (buffer[0] == '/') {continue;}
    sscanf(buffer, "%s %f", CutName, &CutValue);

    if (!strcmp(CutName, "JPSITYPE")) {
      JPSITYPE = static_cast<int>(CutValue);
      if (dump) cout << "JPSITYPE:      " << JPSITYPE << endl;
      ibin = 210;
      hcuts->SetBinContent(ibin, JPSITYPE);
      hcuts->GetXaxis()->SetBinLabel(ibin, Form("%s :: J/#psi ID :: %d", CutName, JPSITYPE));
    }

    if (!strcmp(CutName, "JPSIMASSLO")) {
      JPSIMASSLO = CutValue;
      if (dump) cout << "JPSIMASSLO:      " << JPSIMASSLO << endl;
      ibin = 211;
      hcuts->SetBinContent(ibin, JPSIMASSLO);
      hcuts->GetXaxis()->SetBinLabel(ibin, Form("%s :: m(J/#psi) :: %3.1f", CutName, JPSIMASSLO));
    }

    if (!strcmp(CutName, "JPSIMASSHI")) {
      JPSIMASSHI = CutValue;
      if (dump) cout << "JPSIMASSLO:      " << JPSIMASSHI << endl;
      ibin = 212;
      hcuts->SetBinContent(ibin, JPSIMASSHI);
      hcuts->GetXaxis()->SetBinLabel(ibin, Form("%s :: m(J/#psi) :: %3.1f", CutName, JPSIMASSHI));
    }

  }

}
