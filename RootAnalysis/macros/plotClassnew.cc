#include "plotClassnew.hh"

#include <fstream>
#include <iostream>
#include <sstream>

#include "TROOT.h"
#include "TStyle.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TString.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TF1.h"

#include "common/util.hh"

#include "preselection.hh"
#include "setupReader.hh"
//#include "ROOT/RDataFrame.hxx"

ClassImp(plotClassnew)

using namespace std;
using namespace ROOT;
// ----------------------------------------------------------------------
plotClassnew::plotClassnew(string dir, string files, string cuts, string setup, int year) {

  setTdrStyle();

  fCutFile = cuts;
  fFilesFile = files;

  // without this line the calling of fReaderEvents2[fChan]->EvaluateMVA("BDT") fill crash?!?!
  fMvaMethod = TString("BDT");

  gStyle->SetHatchesSpacing(2);

  fDBX = true;
  fDoUseBDT = false;
  fVerbose = true;

  fDirectory = dir;
  fSetup = setup;
  fYear = year;
  fMode = UNSET;

  delete gRandom;
  gRandom = (TRandom*) new TRandom3;

  fEpsilon = 0.00001;
  fLumi = 20.;

  legg = 0;
  c0 = c1 = c2 = c3 = c4 = c5 =0;
  tl = new TLatex();
  box = new TBox();
  pa = new TArrow();
  pl = new TLine();
  legge = 0;

  fAccPt = 4.0;
  fAccEtaGen = 2.5;
  fAccEtaRec = 2.4;

  c0 = (TCanvas*)gROOT->FindObject("c0");
  // if (!c0) c0 = new TCanvas("c0","--c0--",0,0,656,700);
  if (!c0) c0 = new TCanvas("c0","--c0--",0,0,1200,700);


  fHistFile = 0; // this must be opened in a derived class!

  fStampString = "preliminary";
  if (fDoUseBDT) {
    fStampString = "BDT";
  } else {
    fStampString = "CNC";
  }
  fStampCms = "BMM4";
  fStampLumi = "XXX fb^{-1}";

  fCrossSection  = 5.679e+01; // mb
  fCrossSection *= 1.0e12;

  fFsfu.val  = 0.250;
  fFsfu.name = "fsfu";
  fFsfu.setErrors(0.0001, 0.012, 0.012);

  fBfPsiMuMu     = 0.05961;
  fBfPsiMuMuE    = 0.00033;
  fBfPhiKpKm     = 0.492;
  fBfPhiKpKmE    = 0.005;

  // L6134 of /cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw-patch/CMSSW_8_0_19_patch2/src/GeneratorInterface/EvtGenInterface/data/DECAY_NOLONGLIFE.DEC
  fBfKstarKpPim  = 0.6657;
  fBfKstarKpPimE = 0.007;   // assume 1% error as for phi -> K+K-

  string sfiles(files);
  if (string::npos != sfiles.find("2011")) {
    fStampLumi = "L = 5 fb^{-1} (#sqrt{s} = 7 TeV)";
  }
  if (string::npos != sfiles.find("2012")) {
    fStampLumi = "L = 20 fb^{-1} (#sqrt{s} = 8 TeV)";
  }
  if (string::npos != sfiles.find("2015")) {
    fStampLumi = "L = 2.5 fb^{-1} (#sqrt{s} = 13 TeV)";
  }
  if (string::npos != sfiles.find("2016")) {
    fStampLumi = "L = 36.7 fb^{-1} (#sqrt{s} = 13 TeV)";
    if (string::npos != fSetup.find("BF")) {
      fStampLumi = "L = 20 fb^{-1} (#sqrt{s} = 13 TeV)";
    }
    if (string::npos != fSetup.find("G")) {
      fStampLumi = "L = 7.6 fb^{-1} (#sqrt{s} = 13 TeV)";
    }
    if (string::npos != fSetup.find("H")) {
      fStampLumi = "L = 8.7 fb^{-1} (#sqrt{s} = 13 TeV)";
    }
    if (string::npos != fSetup.find("GH")) {
      fStampLumi = "L = 16 fb^{-1} (#sqrt{s} = 13 TeV)";
    }
  }
  if (string::npos != sfiles.find("2017")) {
    fStampLumi = "L = 40.X fb^{-1} (#sqrt{s} = 13 TeV)";
  }
  if (string::npos != sfiles.find("2018")) {
    fStampLumi = "L = 60.X fb^{-1} (#sqrt{s} = 13 TeV)";
  }


  //  if (setup == "") fSuffix = Form("%d", fYear);
  if (string::npos != fSetup.find(Form("%d", fYear))) {
    fSuffix = Form("%s", fSetup.c_str());
  } else {
    fSuffix = Form("%d%s", fYear, fSetup.c_str());
  }

  fIF = new initFunc();

  fCncCuts.addCut("fGoodAcceptance", "acceptance", fGoodAcceptance);
  fCncCuts.addCut("fGoodJpsiCuts", "J/psi", fGoodJpsiCuts);
  fCncCuts.addCut("fGoodHLT", "HLT", fGoodHLT);
  fCncCuts.addCut("fGoodPvAveW8", "<w8>", fGoodPvAveW8);
  fCncCuts.addCut("fGoodDcand", "Dcand", fGoodDcand);
  fCncCuts.addCut("fGoodMuonsPt", "p_{T,#mu} [GeV]", fGoodMuonsPt);
  fCncCuts.addCut("fGoodMuonsEta", "#eta_{#mu} ", fGoodMuonsEta);
  fCncCuts.addCut("fGoodGlobalMuons", "lepton ID", fGoodGlobalMuons);
  fCncCuts.addCut("fGoodGlobalMuonsKin", "lepton ID kin", fGoodGlobalMuonsKin);
  fCncCuts.addCut("fGoodMuonsID", "lepton ID", fGoodMuonsID);
  fCncCuts.addCut("fGoodTracks", "good tracks", fGoodTracks);
  fCncCuts.addCut("fGoodTracksPt", "p_{T,trk} [GeV]", fGoodTracksPt);
  fCncCuts.addCut("fGoodTracksEta", "#eta_{trk} ", fGoodTracksEta);

  fCncCuts.addCut("fGoodQ", "q_{1} q_{2}", fGoodQ);
  fCncCuts.addCut("fGoodPt", "p_{T,B}", fGoodPt);
  fCncCuts.addCut("fGoodPhi", "#phi_{B}", fGoodPhi);
  fCncCuts.addCut("fGoodEta", "#eta_{B}", fGoodEta);

  fCncCuts.addCut("fGoodChi2", "#chi^{2}", fGoodChi2);
  fCncCuts.addCut("fGoodMaxDoca", "MAXDOCA", fGoodMaxDoca);

  fCncCuts.addCut("fGoodAlpha", "#alpha", fGoodAlpha);
  fCncCuts.addCut("fGoodFLS", "l/#sigma(l)", fGoodFLS);

  fCncCuts.addCut("fGoodIp", "IP", fGoodIp);
  fCncCuts.addCut("fGoodIpS", "IPS", fGoodIpS);
  fCncCuts.addCut("fGoodLip", "LIP", fGoodLip);
  fCncCuts.addCut("fGoodLipS", "LIPS", fGoodLipS);

  fCncCuts.addCut("fGoodDocaTrk", "d_{ca}(trk)", fGoodDocaTrk);
  fCncCuts.addCut("fGoodIso", "I_{trk}", fGoodIso);
  fCncCuts.addCut("fGoodM1Iso", "I_{trk}^{#mu,1}", fGoodM1Iso);
  fCncCuts.addCut("fGoodM2Iso", "I_{trk}^{#mu,2}", fGoodM2Iso);
  fCncCuts.addCut("fGoodCloseTrack", "close track veto", fGoodCloseTrack);

  fBdtCuts.addCut("fGoodAcceptance", "acceptance", fGoodAcceptance);
  fBdtCuts.addCut("fGoodJpsiCuts", "J/psi", fGoodJpsiCuts);
  fBdtCuts.addCut("fGoodHLT", "HLT", fGoodHLT);
  fBdtCuts.addCut("fGoodPvAveW8", "<w8>", fGoodPvAveW8);
  fBdtCuts.addCut("fGoodDcand", "Dcand", fGoodDcand);
  fBdtCuts.addCut("fGoodMuonsPt", "p_{T,#mu} [GeV]", fGoodMuonsPt);
  fBdtCuts.addCut("fGoodMuonsEta", "#eta_{#mu} ", fGoodMuonsEta);
  fBdtCuts.addCut("fGoodGlobalMuons", "lepton ID", fGoodGlobalMuons);
  fBdtCuts.addCut("fGoodGlobalMuonsKin", "lepton ID kin", fGoodGlobalMuonsKin);
  fBdtCuts.addCut("fGoodMuonsID", "lepton ID", fGoodMuonsID);
  fBdtCuts.addCut("fGoodTracks", "good tracks", fGoodTracks);
  fBdtCuts.addCut("fGoodTracksPt", "p_{T,trk} [GeV]", fGoodTracksPt);
  fBdtCuts.addCut("fGoodTracksEta", "#eta_{trk} ", fGoodTracksEta);

  fBdtCuts.addCut("fGoodQ", "q_{1} q_{2}", fGoodQ);
  fBdtCuts.addCut("fGoodPt", "p_{T,B} [GeV]", fGoodPt);
  fBdtCuts.addCut("fGoodPhi", "#phi_{B} ", fGoodPhi);
  fBdtCuts.addCut("fGoodEta", "#eta_{B} ", fGoodEta);

  fBdtCuts.addCut("fGoodBdtPresel", "BDTPresel", fGoodBdtPresel);
  fBdtCuts.addCut("fGoodBDT", "bdt", fGoodBDT);


  // -- NOTE: This should be synchronized to AN-16-178/trunk/symbols.tex
  fVarToTex.insert(make_pair("pt", "p_{T_{B}} #it{[GeV]}"));
  fVarToTexSymbol.insert(make_pair("pt", "\\ptb"));
  fVarToTex.insert(make_pair("eta", "#eta_{B}"));
  fVarToTexSymbol.insert(make_pair("eta", "\\etab"));

  fVarToTex.insert(make_pair("mpt", "p_{T_{#mu}} #it{[GeV]}"));
  fVarToTexSymbol.insert(make_pair("mpt", "\\ptmu"));
  fVarToTex.insert(make_pair("m1pt", "p_{T_{#mu,1}} #it{[GeV]}"));
  fVarToTexSymbol.insert(make_pair("m1pt", "\\ptmuone"));
  fVarToTex.insert(make_pair("m2pt", "p_{T_{#mu,2}} #it{[GeV]}"));
  fVarToTexSymbol.insert(make_pair("m2pt", "\\ptmutwo"));
  fVarToTex.insert(make_pair("meta", "#eta_{#mu}"));
  fVarToTexSymbol.insert(make_pair("meta", "\\etamu"));
  fVarToTex.insert(make_pair("m1eta", "#eta_{#mu,1}"));
  fVarToTexSymbol.insert(make_pair("m1eta", "\\etamuone"));
  fVarToTex.insert(make_pair("m2eta", "#eta_{#mu,2}"));
  fVarToTexSymbol.insert(make_pair("m2eta", "\\etamutwo"));
  fVarToTex.insert(make_pair("fls3d", "l_{3D}/#sigma(l_{3D})"));
  fVarToTexSymbol.insert(make_pair("fls3d", "\\fls"));
  fVarToTex.insert(make_pair("alpha", "#alpha_{3D}"));
  fVarToTexSymbol.insert(make_pair("alpha", "\\alpha"));
  fVarToTex.insert(make_pair("chi2dof", "#chi^{2}/dof"));
  fVarToTexSymbol.insert(make_pair("chi2dof", "\\chidof"));

  fVarToTex.insert(make_pair("iso", "isolation"));
  fVarToTexSymbol.insert(make_pair("iso", "\\iso"));
  fVarToTex.insert(make_pair("m1iso", "#mu_{1} isolation"));
  fVarToTexSymbol.insert(make_pair("m1iso", "\\isomuone"));
  fVarToTex.insert(make_pair("m2iso", "#mu_{2} isolation"));
  fVarToTexSymbol.insert(make_pair("m2iso", "\\isomutwo"));
  fVarToTex.insert(make_pair("docatrk", "d_{ca}^{0} #it{[cm]}"));
  fVarToTexSymbol.insert(make_pair("docatrk", "\\docatrk"));
  fVarToTex.insert(make_pair("closetrk", "N_{trk}^{close}"));
  fVarToTexSymbol.insert(make_pair("closetrk", "\\closetrk"));
  fVarToTex.insert(make_pair("closetrks1", "N_{trk}^{close, 1#sigma}"));
  fVarToTexSymbol.insert(make_pair("closetrks1", "\\closetrkA"));
  fVarToTex.insert(make_pair("closetrks2", "N_{trk}^{close, 2#sigma}"));
  fVarToTexSymbol.insert(make_pair("closetrks2", "\\closetrkB"));
  fVarToTex.insert(make_pair("closetrks3", "N_{trk}^{close, 3#sigma}"));
  fVarToTexSymbol.insert(make_pair("closetrks3", "\\closetrkC"));

  fVarToTex.insert(make_pair("maxdoca", "d_{ca}^{max} #it{[cm]}"));
  fVarToTexSymbol.insert(make_pair("maxdoca", "\\maxdoca"));
  fVarToTex.insert(make_pair("pvip", "#delta_{3D} #it{[cm]}"));
  fVarToTexSymbol.insert(make_pair("pvip", "\\pvip"));
  fVarToTex.insert(make_pair("pvips", "#delta_{3D}/#sigma(#delta_{3D})"));
  fVarToTexSymbol.insert(make_pair("pvips", "\\pvips"));

  // -- initialize cuts
  if (cuts == "nada") {
    cuts = Form("baseCuts.%d.cuts", fYear);
  }
  cout << "==> Reading cuts from " << Form("%s", cuts.c_str()) << endl;
  readCuts(Form("%s/%s", fDirectory.c_str(), cuts.c_str()));
  fNchan = fCuts.size();
}

// ----------------------------------------------------------------------
plotClassnew::~plotClassnew() {
  cout << "plotClassnew destructor" << endl;
  for (map<string, dataset*>::iterator imap = fDS.begin(); imap != fDS.end(); ++imap) {
    if (fDS[imap->first]->fF) {
      cout << "    => closing " << imap->first;
      cout << ": " << fDS[imap->first]->fF->GetName() << endl;
      imap->second->fF->Close();
    }
  }
}

// ----------------------------------------------------------------------
void plotClassnew::changeSetup(string dir, string name, string setup) {
  // if (setup == "") {
  //   fHistFileName = Form("%s/%s.%d.root, dir.c_str(), name.c_str(), fYear);
  //   fNumbersFileName = fDirectory + Form("/%s.%d.txt", name.c_str(), fYear);
  // } else {
  //   fHistFileName = Form("%s/%s.%d%s.root", dir.c_str(), name.c_str(), fYear, setup.c_str());
  //   fNumbersFileName = fDirectory + Form("/%s.%d%s.txt", name.c_str(), fYear, setup.c_str());
  // }

  fHistFileName = Form("%s/%s.%s.root", dir.c_str(), name.c_str(), fSuffix.c_str());
  fNumbersFileName = fDirectory + Form("/%s.%s.txt", name.c_str(), fSuffix.c_str());


  fTexFileName = fNumbersFileName;
  replaceAll(fTexFileName, ".txt", ".tex");
  // string old = fTexFileName;
  // old += ".old";
  // cout << "old: " << old << endl;
  // system(Form("/bin/mv %s %s", fTexFileName.c_str(), old.c_str()));

  cout << "plotClassnew::changeSetup: " << endl
       << "  name             = " << name << endl
       << "  setup            = " << setup << endl
       << "  fSetup           = " << fSetup << endl
       << "  fSuffix          = " << fSuffix << endl
       << "  fYear            = " << fYear << endl
       << "  fHistFileName    = " << fHistFileName << endl
       << "  fNumbersFileName = " << fNumbersFileName << endl
       << "  fTexFileName     = " << fTexFileName << endl
       << endl;
}


// ----------------------------------------------------------------------
string plotClassnew::era(int run) {
  // -- 2011
  if (160325 <= run && run <= 173692) return "A";
  if (175832 <= run && run <= 180252) return "B";
  // -- 2012
  if (190456 <= run && run <= 193621) return "A";
  if (193833 <= run && run <= 196531) return "B";
  if (198022 <= run && run <= 203742) return "C";
  if (203777 <= run && run <= 208686) return "D";
  // -- 2015
  if (250985 <= run && run <= 253620) return "A";
  if (254227 <= run && run <= 255031) return "B";
  if (256630 <= run && run <= 260627) return "C";
  // -- 2016
  if (271036 <= run && run <= 271658) return "A";
  if (272007 <= run && run <= 275376) return "B";
  if (275657 <= run && run <= 276283) return "C";
  if (276315 <= run && run <= 276811) return "D";
  if (276831 <= run && run <= 277420) return "E";
  if (277772 <= run && run <= 278808) return "F";
  if (278820 <= run && run <= 280385) return "G";
  if (280919 <= run && run <= 284044) return "H";
  // -- 2017
  if (294927 <= run && run <= 297019) return "A";
  if (297046 <= run && run <= 299329) return "B";
  if (299368 <= run && run <= 302029) return "C";
  if (302031 <= run && run <= 305000) return "D";

  return "X";

}

// ----------------------------------------------------------------------
int plotClassnew::iera(int run) {
  // -- 2011
  if (160325 <= run && run <= 173692) return 1; // A
  if (175832 <= run && run <= 180252) return 2; // B
  // -- 2012
  if (190456 <= run && run <= 193621) return 1; // A
  if (193833 <= run && run <= 196531) return 2; // B
  if (198022 <= run && run <= 203742) return 3; // C
  if (203777 <= run && run <= 208686) return 4;	// D
  // -- 2015
  if (250985 <= run && run <= 253620) return 1; // A
  if (254227 <= run && run <= 255031) return 2; // B
  if (256630 <= run && run <= 260627) return 3; // C
  // -- 2016
  if (271036 <= run && run <= 271658) return 1; // A
  if (272007 <= run && run <= 275376) return 2; // B
  if (275657 <= run && run <= 276283) return 3; // C
  if (276315 <= run && run <= 276811) return 4; // D
  if (276831 <= run && run <= 277420) return 5; // E
  if (277772 <= run && run <= 278808) return 6; // F
  if (278820 <= run && run <= 280385) return 7; // G
  if (280919 <= run && run <= 284044) return 8; // H
  // -- 2017
  if (294927 <= run && run <= 297019) return 1; // A
  if (297046 <= run && run <= 299999) return 2; // B

  return 0;

}



// ----------------------------------------------------------------------
void plotClassnew::setup(string ds) {
  fSample = ds;
  string dir = "candAnaMuMu";
  fMode = BMM;
  if (string::npos != ds.find("bupsik")) {
    fMode = BU2JPSIKP;
    dir = "candAnaBu2JpsiK";
  } else if (string::npos != ds.find("bspsiphi")) {
    dir = "candAnaBs2JpsiPhi";
    fMode = BS2JPSIPHI;
  } else if (string::npos != ds.find("bspsif")) {
    dir = "candAnaBs2Jpsif0";
    fMode = BS2JPSIF;
  } else if (string::npos != ds.find("bdpsikstar")) {
    dir = "candAnaBd2JpsiKstar";
    fMode = BD2JPSIKSTAR;
  } else if (string::npos != ds.find("bsmm")) {
    dir = "candAnaMuMu";
    fMode = BSMM;
  } else if (string::npos != ds.find("bdmm")) {
    dir = "candAnaMuMu";
    fMode = BDMM;
  } else if (string::npos != ds.find("Bg")) {
    dir = "candAnaMuMu";
    fMode = RARE;
  }

  fTreeDir = dir;
}

// ----------------------------------------------------------------------
void plotClassnew::init() {
}

// ----------------------------------------------------------------------
// see http://root.cern.ch/phpBB3/viewtopic.php?f=3&t=15054
void plotClassnew::closeHistFile() {
  fHistFile->Write();
}

// ----------------------------------------------------------------------
void plotClassnew::cd(std::string dataset, std::string dir) {
  if (0 == fDS.count(dataset)) {
    cout << "unknown dataset: " << dataset << endl;
  } else {
    fDS[dataset]->cd(dir.c_str());
  }
}

// ----------------------------------------------------------------------
void plotClassnew::bookHist(string name) {
  cout << "==> plotClassnew: bookHist " << name << endl;
}

// ----------------------------------------------------------------------
void plotClassnew::makeAll(int bitmask) {
  cout << "==> plotClassnew: makeAll " << bitmask << endl;
}

// ----------------------------------------------------------------------
void plotClassnew::treeAnalysis() {
  cout << "==> plotClassnew: treeAnalysis " << endl;
}

// ----------------------------------------------------------------------
void plotClassnew::normHist(TH1 *h, string ds, int method) {
  double scale(1.);
  string smethod("");
  // -- normalize to 1
  if (method == UNITY) {
    smethod = "unity";
    scale = (h->Integral() > 0 ? 1./h->Integral() : 1.);
    h->GetYaxis()->SetTitle("normalized to 1");
  } else if (method == SOMETHING) {
    smethod = "something";
    scale = fNorm * (h->Integral() > 0 ? fNorm/h->Integral() : 1.);
    h->GetYaxis()->SetTitle("weighted events");
  } else if (method == XSECTION) {
    smethod = "xsection";
    // -- normalize to EFFECTIVE xsec*bf (EFFECTIVE to account for cuts)
    //    the cross section is known for ds
    //    ds corresponds to know lumi
    //
    //    n = xsec * L
    //    "integral" over histogram should be EFFECTIVE xsec
    scale = (h->Integral() > 0 ? fDS[ds]->fXsec*fDS[ds]->fBf/h->Integral() : 1.);
    h->GetYaxis()->SetTitle("pb");
  } else if (method == LUMI) {
    smethod = "lumi";
    // -- normalize to xsec*bf
    //    n = xsec * L
    //    "integral" over histogram should be events expected in fLumi
    scale = (h->Integral() > 0 ? fLumi/fDS[ds]->fLumi : 1.);
    h->GetYaxis()->SetTitle(Form("events in %4.0f/fb", fLumi));
  } else if (method == NONORM) {
    smethod = "nonorm";
    scale = 1.;
  } else {
    scale = 1.;
  }

  cout << "==>plotClassnew:  normHist scaling by " << scale << ", based on method " << smethod << endl;

  double c(0.), e(0.);
  for (int i = 0; i <= h->GetNbinsX(); ++i) {
    c = h->GetBinContent(i);
    e = h->GetBinError(i);
    h->SetBinContent(i, c*scale);
    h->SetBinError(i, e*scale);
  }
}


// ----------------------------------------------------------------------
void plotClassnew::overlay(TH1* h1, string f1, TH1* h2, string f2, TH1* h3, string f3,
 			int method, bool loga, bool legend, double xleg, double yleg) {
  const bool verbose(true);

  showOverflow(h1);
  showOverflow(h2);
  if (h3) showOverflow(h3);

  normHist(h1, f1, method);
  normHist(h2, f2, method);
  if (h3) normHist(h3, f3, method);
  double ymin(0.0001);
  double h1max(h1->GetBinContent(h1->GetMaximumBin()));
  double h2max(h2->GetBinContent(h2->GetMaximumBin()));
  double h3max(h3?h3->GetBinContent(h3->GetMaximumBin()):-1);
  double hmax(h1max);
  int imax(1);
  if (h2max > h1max) {
    hmax = h2max;
    imax = 2;
  }
  if (h3max > h2max) {
    hmax = h3max;
    imax = 3;
  }
  hmax *= 1.2;
  if (verbose)  {
    cout << "hmax = " << hmax << " from imax = " << imax;
    if (1 == imax) cout << " bin " << h1->GetMaximumBin() << " with maximum " << h1->GetBinContent(h1->GetMaximumBin()) << endl;
    if (2 == imax) cout << " bin " << h2->GetMaximumBin() << " with maximum " << h2->GetBinContent(h2->GetMaximumBin()) << endl;
    if (3 == imax) cout << " bin " << h3->GetMaximumBin() << " with maximum " << h3->GetBinContent(h3->GetMaximumBin()) << endl;
  }
  if (loga) {
    gPad->SetLogy(1);
    hmax *= 2.;
    double hmin(h1->GetMinimum(ymin));
    if (verbose) cout << "hmin1 = " << hmin << endl;
    if (h2->GetMinimum(ymin) < hmin) {
      hmin = h2->GetMinimum(ymin);
      if (verbose) cout << "hmin2 = " << hmin << endl;
    }
    if (h3 && h3->GetMinimum(ymin) < hmin) {
      hmin = h3->GetMinimum(ymin);
      if (verbose) cout << "hmin3 = " << hmin << endl;
    }
    h1->SetMinimum(0.1*hmin);
    if (verbose) cout << "hmin = " << hmin << endl;
  } else {
    gPad->SetLogy(0);
    h1->SetMinimum(0.);
  }

  h1->SetMaximum(hmax);

  TH1* h0 = h1->DrawCopy("hist");
  setTitles(h1, h0);
  h2->DrawCopy("histsame");
  if (h3) h3->DrawCopy("histsame");

  if (legend) {
    newLegend(xleg, yleg, xleg+0.25, yleg+0.15);
    legg->SetTextSize(0.03);
    string text;
    text = fDS[f1]->fName.c_str();
    if (fDBX) {
      text = Form("%s: %4.3f#pm%4.3f, %4.3f", fDS[f1]->fName.c_str(), h1->GetMean(), h1->GetMeanError(), h1->GetRMS());
    }
    legg->AddEntry(h1, text.c_str(), "f");

    text = fDS[f2]->fName.c_str();
    if (fDBX) {
      text = Form("%s: %4.3f#pm%4.3f, %4.3f", fDS[f2]->fName.c_str(), h2->GetMean(), h2->GetMeanError(), h2->GetRMS());
    }
    legg->AddEntry(h2, text.c_str(), "f");

    if (h3) {
      text = fDS[f3]->fName.c_str();
      if (fDBX) {
	text = Form("%s: %4.3f#pm%4.3f, %4.3f", fDS[f3]->fName.c_str(), h3->GetMean(), h3->GetMeanError(), h3->GetRMS());
      }
      legg->AddEntry(h3, text.c_str(), "f");
    }

    legg->Draw();
  }


  if (verbose) cout << "==>plotClassnew: overlay(" << f1 << ", " << h1->GetName() << " integral= " << h1->Integral()
		    << ", " << f2 << ", " << h2->GetName() << " integral= " << h2->Integral()
		    << (h3? Form(", %s, %s, %f", f3.c_str(), h3->GetName(), h3->Integral()) : "")
		    << ")  log: " << loga << " legend = " << legend
		    << endl;
}

// ----------------------------------------------------------------------
void plotClassnew::overlay(string h1name, string f1, string h2name, string f2, string h3name, string f3, int method, bool loga,
			bool legend, double xleg, double yleg) {

  cout << h1name << " from " << f1 << " vs. " << h2name << " from " << f2 << " vs. " << h3name << " from " << f3 << endl;

  TH1D *h1 = fDS[f1]->getHist(Form("%s", h1name.c_str()), true);
  TH1D *h2 = fDS[f2]->getHist(Form("%s", h2name.c_str()), true);
  TH1D *h3(0);
  if (h3name != "") {
    h3 = fDS[f3]->getHist(Form("%s", h3name.c_str()), true);
  } else {
    h3 = 0;
  }

  overlay(h1, f1, h2, f2, h3, f3, method, loga, legend, xleg, yleg);
}


// ----------------------------------------------------------------------
void plotClassnew::loopFunction1() {
}


// ----------------------------------------------------------------------
void plotClassnew::loopFunction2() {
}

// ----------------------------------------------------------------------
void plotClassnew::loopOverTree(TTree *t, int ifunc, int nevts, int nstart) {
  cout << "wrong function" << endl;
}


// ----------------------------------------------------------------------
void plotClassnew::setupTree(TTree *t, string mode) {

  fCds = fDS[fSample];

  if (string::npos != mode.find("Mc")) {
    fIsMC = true;
  } else {
    fIsMC = false;
  }
  if (string::npos != mode.find("bupsik")){
    
    t->SetBranchAddress("nbkmm",    &fb.nbkmm);
    // t->SetBranchAddress("bkmm_jpsimc_mu1pt",    &fb.m1pt);
    // t->SetBranchAddress("bkmm_jpsimc_mu2pt",    &fb.m2pt);
    // t->SetBranchAddress("bkmm_jpsimc_mu1eta",    &fb.m1eta);
    // t->SetBranchAddress("bkmm_jpsimc_mu2eta",    &fb.m2eta);
    // t->SetBranchAddress("bkmm_jpsimc_mu1phi",    &fb.m1phi);
    // t->SetBranchAddress("bkmm_jpsimc_mu2phi",    &fb.m2phi);
    
    t->SetBranchAddress("bkmm_jpsimc_sl3d",    &fb.fls3d);
    t->SetBranchAddress("bkmm_jpsimc_slxy",    &fb.flsxy);
    t->SetBranchAddress("bkmm_jpsimc_mass",       &fb.m);
    t->SetBranchAddress("bkmm_jpsimc_massErr",       &fb.me);
    t->SetBranchAddress("bkmm_jpsimc_vtx_chi2dof",  &fb.chi2dof); 
    t->SetBranchAddress("bkmm_jpsimc_pvip", &fb.pvip);
    t->SetBranchAddress("bkmm_jpsimc_pvipErr", &fb.pvipErr);
    
    t->SetBranchAddress("bkmm_jpsimc_pt",    &fb.pt);
    t->SetBranchAddress("bkmm_jpsimc_eta",    &fb.eta);
    //t->SetBranchAddress("bkmm_jpsimc_phi",    &fb.phi);
    t->SetBranchAddress("bkmm_bmm_bdt",&fb.bdt); 
    
    t->SetBranchAddress("bkmm_jpsimc_pv2lip",  &fb.pv2lip);
    t->SetBranchAddress("bkmm_jpsimc_pvlip",  &fb.pvlip);
    t->SetBranchAddress("bkmm_jpsimc_pvlipErr",  &fb.pvlipErr);
    t->SetBranchAddress("bkmm_jpsimc_alpha",  &fb.alpha);
    t->SetBranchAddress("bkmm_jpsimc_cosAlphaXY",    &fb.cosa);
    t->SetBranchAddress("bkmm_bmm_docatrk", &fb.docatrk);
    t->SetBranchAddress("bkmm_bmm_closetrk", &fb.closetrk);
    t->SetBranchAddress("bkmm_bmm_closetrks1", &fb.closetrks1);
    t->SetBranchAddress("bkmm_bmm_closetrks2", &fb.closetrks2);
    t->SetBranchAddress("bkmm_bmm_closetrks3", &fb.closetrks3);

    t->SetBranchAddress("bkmm_jpsimc_vtx_prob", &fb.bkmm_jpsimc_vtx_prob);
    t->SetBranchAddress("bkmm_kaon_sdxy_bs", &fb.bkmm_kaon_sdxy_bs);
    t->SetBranchAddress("bkmm_bmm_iso", &fb.iso);
    t->SetBranchAddress("bkmm_bmm_m1iso", &fb.m1iso);
    t->SetBranchAddress("bkmm_bmm_m2iso", &fb.m2iso);
    t->SetBranchAddress("bkmm_kaon_pt", &fb.k1pt);
    t->SetBranchAddress("bkmm_kaon_eta", &fb.k1eta);
    t->SetBranchAddress("bkmm_kaon_dxy_bs", &fb.bkmm_kaon_dxy_bs);
    t->SetBranchAddress("bkmm_kaon_sdxy_bs", &fb.bkmm_kaon_sdxy_bs);
    t->SetBranchAddress("bkmm_kaon_mu1_doca", &fb.bkmm_kaon_mu1_doca);
    t->SetBranchAddress("bkmm_kaon_mu2_doca", &fb.bkmm_kaon_mu2_doca);
    t->SetBranchAddress("bkmm_kaon_phi", &fb.k1phi);
    t->SetBranchAddress("bkmm_mm_index", &fb.bkmm_mm_index);
  }
  if (string::npos != mode.find("bspsiphi")){
    
    t->SetBranchAddress("nbkkmm",    &fb.nbkkmm);
    
    t->SetBranchAddress("bkkmm_jpsikk_sl3d",    &fb.fls3d);
    t->SetBranchAddress("bkkmm_jpsikk_slxy",    &fb.flsxy);
    t->SetBranchAddress("bkkmm_jpsikk_mass",       &fb.m);
    t->SetBranchAddress("bkkmm_jpsikk_massErr",       &fb.me);
    t->SetBranchAddress("bkkmm_jpsikk_vtx_chi2dof",  &fb.chi2dof); 
    t->SetBranchAddress("bkkmm_jpsikk_pvip", &fb.pvip);
    t->SetBranchAddress("bkkmm_jpsikk_pvipErr", &fb.pvipErr);
    
    t->SetBranchAddress("bkkmm_jpsikk_pt",    &fb.pt);
    t->SetBranchAddress("bkkmm_jpsikk_eta",    &fb.eta);
    //t->SetBranchAddress("bkkmm_bmm_bdt",&fb.bdt); 
    t->SetBranchAddress("bkkmm_kk_mass",&fb.bkkmm_kk_mass);
    t->SetBranchAddress("bkkmm_jpsikk_pv2lip",  &fb.pv2lip);
    t->SetBranchAddress("bkkmm_jpsikk_pvlip",  &fb.pvlip);
    t->SetBranchAddress("bkkmm_jpsikk_pvlipErr",  &fb.pvlipErr);
    t->SetBranchAddress("bkkmm_jpsikk_alpha",  &fb.alpha);
    t->SetBranchAddress("bkkmm_jpsikk_cosAlphaXY",    &fb.cosa);
    // t->SetBranchAddress("bkkmm_bmm_docatrk", &fb.docatrk);
    // t->SetBranchAddress("bkkmm_bmm_closetrk", &fb.closetrk);
    // t->SetBranchAddress("bkkmm_bmm_closetrks1", &fb.closetrks1);
    // t->SetBranchAddress("bkkmm_bmm_closetrks2", &fb.closetrks2);
    // t->SetBranchAddress("bkkmm_bmm_closetrks3", &fb.closetrks3);

    t->SetBranchAddress("bkkmm_jpsikk_vtx_prob", &fb.bkkmm_jpsimc_vtx_prob);
    t->SetBranchAddress("bkkmm_kaon1_sdxy_bs", &fb.bkkmm_kaon1_sdxy_bs);
    t->SetBranchAddress("bkkmm_kaon2_sdxy_bs", &fb.bkkmm_kaon2_sdxy_bs);
    // t->SetBranchAddress("bkkmm_bmm_iso", &fb.iso);
    // t->SetBranchAddress("bkkmm_bmm_m1iso", &fb.m1iso);
    // t->SetBranchAddress("bkkmm_bmm_m2iso", &fb.m2iso);

    t->SetBranchAddress("bkkmm_jpsikk_kaon1pt", &fb.k1pt);
    t->SetBranchAddress("bkkmm_jpsikk_kaon1eta", &fb.k1eta);
    t->SetBranchAddress("bkkmm_kaon1_dxy_bs", &fb.bkkmm_kaon1_dxy_bs);
    t->SetBranchAddress("bkkmm_kaon1_sdxy_bs", &fb.bkkmm_kaon1_sdxy_bs);
    //t->SetBranchAddress("bkkmm_kaon1_mu1_doca", &fb.bkkmm_kaon1_mu1_doca);
    //t->SetBranchAddress("bkkmm_kaon1_mu2_doca", &fb.bkkmm_kaon1_mu2_doca);
    t->SetBranchAddress("bkkmm_jpsikk_kaon1phi", &fb.k1phi);

    t->SetBranchAddress("bkkmm_jpsikk_kaon2pt", &fb.k2pt);
    t->SetBranchAddress("bkkmm_jpsikk_kaon2eta", &fb.k2eta);
    t->SetBranchAddress("bkkmm_kaon2_dxy_bs", &fb.bkkmm_kaon2_dxy_bs);
    t->SetBranchAddress("bkkmm_kaon2_sdxy_bs", &fb.bkkmm_kaon2_sdxy_bs);
    //t->SetBranchAddress("bkkmm_kaon2_mu1_doca", &fb.bkkmm_kaon2_mu1_doca);
    //t->SetBranchAddress("bkkmm_kaon2_mu2_doca", &fb.bkkmm_kaon2_mu2_doca);
    t->SetBranchAddress("bkkmm_jpsikk_kaon2phi", &fb.k2phi);

    t->SetBranchAddress("bkkmm_mm_index", &fb.bkkmm_mm_index);
  }

  t->SetBranchAddress("run",     &fb.run);
  t->SetBranchAddress("event",     &fb.evt);
  if(2018 == fYear){
    t->SetBranchAddress("HLT_DoubleMu4_3_Jpsi", &fb.hlt1); }
  else if(2017 == fYear){
    t->SetBranchAddress("HLT_DoubleMu4_3_Jpsi_Displaced", &fb.hlt1); 
  }
  t->SetBranchAddress("HLT_Dimuon0_Jpsi", &fb.hlt_2);
  t->SetBranchAddress("HLT_Dimuon0_Jpsi_NoVertexing", &fb.hlt_3);
  t->SetBranchAddress("HLT_DoubleMu4_Jpsi_Displaced", &fb.hlt_4);
  t->SetBranchAddress("HLT_DoubleMu4_Jpsi_NoVertexing", &fb.hlt_5);
  
  t->SetBranchAddress("HLT_DoubleMu4_3_Bs", &fb.hlt3);     
  t->SetBranchAddress("nmm", &fb.nmm);
  t->SetBranchAddress("mm_bdt", &fb.mm_bdt);
  t->SetBranchAddress("mm_doca", &fb.mm_doca);
  t->SetBranchAddress("mm_docatrk", &fb.mm_docatrk);
  t->SetBranchAddress("mm_iso", &fb.mm_iso);
  t->SetBranchAddress("mm_kin_eta", &fb.mm_kin_eta);
  t->SetBranchAddress("mm_kin_pt", &fb.mm_kin_pt);
  t->SetBranchAddress("mm_kin_l3d", &fb.mm_kin_l3d);
  t->SetBranchAddress("mm_kin_lxy", &fb.mm_kin_lxy);
  t->SetBranchAddress("mm_kin_sl3d", &fb.mm_kin_sl3d);
  t->SetBranchAddress("mm_kin_slxy", &fb.mm_kin_slxy);
  t->SetBranchAddress("mm_kal_mass", &fb.mm_kal_mass);
  t->SetBranchAddress("mm_kal_lxy", &fb.mm_kal_lxy);
  t->SetBranchAddress("mm_kal_slxy", &fb.mm_kal_slxy);
  t->SetBranchAddress("mm_kal_vtx_prob", &fb.mm_kal_vtx_prob);
  t->SetBranchAddress("mm_kin_mass", &fb.mm_kin_mass);
  t->SetBranchAddress("mm_kin_massErr", &fb.mm_kin_massErr);
  t->SetBranchAddress("mm_kin_alpha", &fb.mm_kin_alpha);
  t->SetBranchAddress("mm_kin_cosAlphaXY", &fb.mm_kin_cosAlphaXY);

  t->SetBranchAddress("mm_kin_mu1eta", &fb.mm_kin_mu1eta);
  t->SetBranchAddress("mm_kin_mu1pt", &fb.mm_kin_mu1pt);
  t->SetBranchAddress("mm_kin_mu1phi", &fb.mm_kin_mu1phi);
  t->SetBranchAddress("mm_kin_mu2eta", &fb.mm_kin_mu2eta);
  t->SetBranchAddress("mm_kin_mu2pt",& fb.mm_kin_mu2pt);
  t->SetBranchAddress("mm_kin_mu2phi", &fb.mm_kin_mu2phi);
  t->SetBranchAddress("mm_kin_pvip", &fb.mm_kin_pvip);
  t->SetBranchAddress("mm_kin_pvipErr", &fb.mm_kin_pvipErr);
  t->SetBranchAddress("mm_kin_pv2lip", &fb.mm_kin_pv2lip);
  t->SetBranchAddress("mm_kin_pv2lipErr", &fb.mm_kin_pv2lipErr);
  t->SetBranchAddress("mm_kin_pvlip", &fb.mm_kin_pvlip);
  t->SetBranchAddress("mm_kin_pvlipErr", &fb.mm_kin_pvlipErr);
  t->SetBranchAddress("mm_kin_pvlip", &fb.mm_kin_pvlip);

  t->SetBranchAddress("mm_kin_vtx_chi2dof", &fb.mm_kin_vtx_chi2dof);
  t->SetBranchAddress("mm_kin_vtx_prob", &fb.mm_kin_vtx_prob);
  t->SetBranchAddress("mm_m1iso", &fb.mm_m1iso);
  t->SetBranchAddress("mm_m2iso", &fb.mm_m2iso);
  t->SetBranchAddress("mm_mass", &fb.mm_mass);
  t->SetBranchAddress("mm_closetrk", &fb.mm_closetrk);
  t->SetBranchAddress("mm_closetrks1", &fb.mm_closetrk1);
  t->SetBranchAddress("mm_closetrks2", &fb.mm_closetrk2);
  t->SetBranchAddress("mm_closetrks3", &fb.mm_closetrk3);

  //  t->SetBranchAddress("nMuon", &fb.nMuon);
  t->SetBranchAddress("Muon1_dxy", &fb.Muon1_dxy);
  t->SetBranchAddress("Muon1_dxyErr", &fb.Muon1_dxyErr);
  t->SetBranchAddress("Muon1_dz", &fb.Muon1_dz);
  t->SetBranchAddress("Muon1_dzErr", &fb.Muon1_dzErr);

  t->SetBranchAddress("Muon1_eta", &fb.Muon1_eta);
  t->SetBranchAddress("Muon1_ip3d", &fb.Muon1_ip3d);
  t->SetBranchAddress("Muon1_mass", &fb.Muon1_mass);
  t->SetBranchAddress("Muon1_phi", &fb.Muon1_phi);
  t->SetBranchAddress("Muon1_pt", &fb.Muon1_pt);
  t->SetBranchAddress("Muon1_ptErr", &fb.Muon1_ptErr);
  t->SetBranchAddress("Muon1_sip3d", &fb.Muon1_sip3d);  
  t->SetBranchAddress("Muon1_softMva", &fb.Muon1_softMva);
  t->SetBranchAddress("Muon1_mvaLowPt", &fb.Muon1_mvaLowPt);
  t->SetBranchAddress("Muon1_charge", &fb.Muon1_Charge);
  t->SetBranchAddress("Muon1_pdgId", &fb.Muon1_pdgId);
  t->SetBranchAddress("Muon1_tightCharge", &fb.Muon1_tightCharge);
  t->SetBranchAddress("Muon1_highPtId", &fb.Muon1_highPtId);
  t->SetBranchAddress("Muon1_inTimeMuon", &fb.Muon1_inTimeMuon);
  t->SetBranchAddress("Muon1_isGlobal", &fb.Muon1_isGlobal);
  t->SetBranchAddress("Muon1_isTracker", &fb.Muon1_isTracker);
  t->SetBranchAddress("Muon1_looseId", &fb.Muon1_looseId);
  t->SetBranchAddress("Muon1_mediumId", &fb.Muon1_mediumId);
  t->SetBranchAddress("Muon1_mediumPromptId", &fb.Muon1_mediumPromptId);
  t->SetBranchAddress("Muon1_mvaId", &fb.Muon1_mvaId);
  t->SetBranchAddress("Muon1_softId", &fb.Muon1_softId);
  t->SetBranchAddress("Muon1_softMvaId", &fb.Muon1_softMvaId);
  t->SetBranchAddress("Muon1_tightId", &fb.Muon1_tightId);
  
  t->SetBranchAddress("Muon2_dxy", &fb.Muon2_dxy);
  t->SetBranchAddress("Muon2_dxyErr", &fb.Muon2_dxyErr);
  t->SetBranchAddress("Muon2_dz", &fb.Muon2_dz);
  t->SetBranchAddress("Muon2_dzErr", &fb.Muon2_dzErr);

  t->SetBranchAddress("Muon2_eta", &fb.Muon2_eta);
  t->SetBranchAddress("Muon2_ip3d", &fb.Muon2_ip3d);
  t->SetBranchAddress("Muon2_mass", &fb.Muon2_mass);
  t->SetBranchAddress("Muon2_phi", &fb.Muon2_phi);
  t->SetBranchAddress("Muon2_pt", &fb.Muon2_pt);
  t->SetBranchAddress("Muon2_ptErr", &fb.Muon2_ptErr);
  t->SetBranchAddress("Muon2_sip3d", &fb.Muon2_sip3d);  
  t->SetBranchAddress("Muon2_softMva", &fb.Muon2_softMva);
  t->SetBranchAddress("Muon2_mvaLowPt", &fb.Muon2_mvaLowPt);
  t->SetBranchAddress("Muon2_charge", &fb.Muon2_Charge);
  t->SetBranchAddress("Muon2_pdgId", &fb.Muon2_pdgId);
  t->SetBranchAddress("Muon2_tightCharge", &fb.Muon2_tightCharge);
  t->SetBranchAddress("Muon2_highPtId", &fb.Muon2_highPtId);
  t->SetBranchAddress("Muon2_inTimeMuon", &fb.Muon2_inTimeMuon);
  t->SetBranchAddress("Muon2_isGlobal", &fb.Muon2_isGlobal);
  t->SetBranchAddress("Muon2_isTracker", &fb.Muon2_isTracker);
  t->SetBranchAddress("Muon2_looseId", &fb.Muon2_looseId);
  t->SetBranchAddress("Muon2_mediumId", &fb.Muon2_mediumId);
  t->SetBranchAddress("Muon2_mediumPromptId", &fb.Muon2_mediumPromptId);
  t->SetBranchAddress("Muon2_mvaId", &fb.Muon2_mvaId);
  t->SetBranchAddress("Muon2_softId", &fb.Muon2_softId);
  t->SetBranchAddress("Muon2_softMvaId", &fb.Muon2_softMvaId);
  t->SetBranchAddress("Muon2_tightId", &fb.Muon2_tightId);
  t->SetBranchAddress("mm_mu1_index", &fb.mm_mu1_index);
  t->SetBranchAddress("mm_mu2_index", &fb.mm_mu2_index);
  
  t->SetBranchAddress("ps",&fb.psw8);
  t->SetBranchAddress("m1pt",    &fb.m1pt);
  t->SetBranchAddress("m2pt",    &fb.m2pt);
  t->SetBranchAddress("m1eta",    &fb.m1eta);
  t->SetBranchAddress("m2eta",    &fb.m2eta);
  t->SetBranchAddress("m1phi",    &fb.m1phi);
  t->SetBranchAddress("m2phi",    &fb.m2phi);
  t->SetBranchAddress("m1id",    &fb.m1id);
  t->SetBranchAddress("m1q",    &fb.m1q);
  t->SetBranchAddress("m2id",    &fb.m2id);
  t->SetBranchAddress("m2q",    &fb.m2q);
  t->SetBranchAddress("m",      &fb.m_s);
  t->SetBranchAddress("chan",      &fb.chan_s);

  t->SetBranchAddress("w_sl3d",   &fb.w_sl3d);
  t->SetBranchAddress("w_PU",   &fb.w_PU);
  t->SetBranchAddress("w_trigJpsi",   &fb.w_trigJpsi);
  t->SetBranchAddress("w_trigJpsi_err",   &fb.w_trigJpsi_err);

  if(fIsMC){
    //t->SetBranchAddress("nPSWeight", &fb.nPS);
    //t->SetBranchAddress("PSWeight", &fb.PSW8);
    if (string::npos != mode.find("bupsik")) {                                                                                                                                 
      t->SetBranchAddress("bkmm_gen_mass", &fb.genmass);
      t->SetBranchAddress("bkmm_gen_kaon_pdgId", &fb.bkmm_k1_pdgId);
      t->SetBranchAddress("bkmm_gen_kaon_mpdgId", &fb.bkmm_pdgId);
      
      t->SetBranchAddress("bkmm_gen_l3d", &fb.bkmm_gen_l3d);
      t->SetBranchAddress("bkmm_gen_lxy", &fb.bkmm_gen_lxy);
      t->SetBranchAddress("bkmm_gen_kaon_pt", &fb.bkmm_gen_kaon_pt);
      t->SetBranchAddress("bkmm_gen_pt", &fb.bkmm_gen_pt);
      
    }else if (string::npos != mode.find("bspsiphi")) {
      t->SetBranchAddress("bkkmm_gen_mass", &fb.genmass);
      t->SetBranchAddress("bkkmm_gen_kaon1_pdgId", &fb.bkkmm_k1_pdgId);
      t->SetBranchAddress("bkkmm_gen_kaon2_pdgId", &fb.bkkmm_k2_pdgId);
      t->SetBranchAddress("bkkmm_gen_pdgId", &fb.bkkmm_pdgId);

  
    }
    t->SetBranchAddress("mm_gen_l3d", &fb.mm_gen_l3d);
    t->SetBranchAddress("mm_gen_lxy", &fb.mm_gen_lxy);
    t->SetBranchAddress("mm_gen_mass", &fb.mm_gen_mass);
    t->SetBranchAddress("mm_gen_mu1_pt", &fb.mm_gen_mu1_pt);
    t->SetBranchAddress("mm_gen_mu2_pt", &fb.mm_gen_mu2_pt);
    t->SetBranchAddress("mm_gen_pt", &fb.mm_gen_pt);
    
    t->SetBranchAddress("mm_gen_mpdgId", &fb.mm_gen_mpdgId);
    t->SetBranchAddress("mm_gen_mu1_mpdgId", &fb.mm_gen_mu1_mpdgId);
    t->SetBranchAddress("mm_gen_mu1_pdgId", &fb.mm_gen_mu1_pdgId);
    t->SetBranchAddress("mm_gen_mu2_mpdgId", &fb.mm_gen_mu2_mpdgId);
    t->SetBranchAddress("mm_gen_mu2_pdgId", &fb.mm_gen_mu2_pdgId);
    t->SetBranchAddress("mm_gen_pdgId", &fb.mm_gen_pdgId);

  }


}

void plotClassnew::setupTreeChain(TChain *t, string mode) {

  fCds = fDS[fSample];

  if (string::npos != mode.find("Mc")) {
    fIsMC = true;
  } else {
    fIsMC = false;
  }
  if (string::npos != mode.find("bupsik")) {                                                                                                                                 
    t->SetBranchAddress("nbkmm",    &fb.nbkmm);
    t->SetBranchAddress("bkmm_jpsimc_mu1pt",    &fb.m1pt);
    t->SetBranchAddress("bkmm_jpsimc_mu2pt",    &fb.m2pt);
    t->SetBranchAddress("bkmm_jpsimc_mu1eta",    &fb.m1eta);
    t->SetBranchAddress("bkmm_jpsimc_mu2eta",    &fb.m2eta);
    t->SetBranchAddress("bkmm_jpsimc_mu1phi",    &fb.m1phi);
    t->SetBranchAddress("bkmm_jpsimc_mu2phi",    &fb.m2phi);
    
    t->SetBranchAddress("bkmm_jpsimc_sl3d",    &fb.fls3d);
    t->SetBranchAddress("bkmm_jpsimc_slxy",    &fb.flsxy);
    t->SetBranchAddress("bkmm_jpsimc_mass",       &fb.m);
    t->SetBranchAddress("bkmm_jpsimc_massErr",       &fb.me);
    t->SetBranchAddress("bkmm_jpsimc_vtx_chi2dof",  &fb.chi2dof); 
    t->SetBranchAddress("bkmm_jpsimc_pvip", &fb.pvip);
    t->SetBranchAddress("bkmm_jpsimc_pvipErr", &fb.pvipErr);
    
    t->SetBranchAddress("bkmm_jpsimc_pt",    &fb.pt);
    t->SetBranchAddress("bkmm_jpsimc_eta",    &fb.eta);
    //t->SetBranchAddress("bkmm_jpsimc_phi",    &fb.phi);
    t->SetBranchAddress("bkmm_bmm_bdt",&fb.bdt); 
    
    t->SetBranchAddress("bkmm_jpsimc_pv2lip",  &fb.pv2lip);
    t->SetBranchAddress("bkmm_jpsimc_pvlip",  &fb.pvlip);
    t->SetBranchAddress("bkmm_jpsimc_pvlipErr",  &fb.pvlipErr);
    t->SetBranchAddress("bkmm_jpsimc_alpha",  &fb.alpha);
    t->SetBranchAddress("bkmm_jpsimc_cosAlphaXY",    &fb.cosa);
    t->SetBranchAddress("bkmm_bmm_docatrk", &fb.docatrk);
    t->SetBranchAddress("bkmm_bmm_closetrk", &fb.closetrk);
    t->SetBranchAddress("bkmm_bmm_closetrks1", &fb.closetrks1);
    t->SetBranchAddress("bkmm_bmm_closetrks2", &fb.closetrks2);
    t->SetBranchAddress("bkmm_bmm_closetrks3", &fb.closetrks3);
    
    t->SetBranchAddress("bkmm_jpsimc_vtx_prob", &fb.bkmm_jpsimc_vtx_prob);
    t->SetBranchAddress("bkmm_kaon_sdxy_bs", &fb.bkmm_kaon_sdxy_bs);
    t->SetBranchAddress("bkmm_bmm_iso", &fb.iso);
    t->SetBranchAddress("bkmm_bmm_m1iso", &fb.m1iso);
    t->SetBranchAddress("bkmm_bmm_m2iso", &fb.m2iso);
    t->SetBranchAddress("bkmm_kaon_pt", &fb.k1pt);
    t->SetBranchAddress("bkmm_kaon_eta", &fb.k1eta);
    t->SetBranchAddress("bkmm_kaon_dxy_bs", &fb.bkmm_kaon_dxy_bs);
    t->SetBranchAddress("bkmm_kaon_sdxy_bs", &fb.bkmm_kaon_sdxy_bs);
    t->SetBranchAddress("bkmm_kaon_mu1_doca", &fb.bkmm_kaon_mu1_doca);
    t->SetBranchAddress("bkmm_kaon_mu2_doca", &fb.bkmm_kaon_mu2_doca);
    t->SetBranchAddress("bkmm_kaon_phi", &fb.k1phi);  
    t->SetBranchAddress("bkmm_mm_index", &fb.bkmm_mm_index);
  }
  t->SetBranchAddress("run",     &fb.run);
  t->SetBranchAddress("event",     &fb.evt);
  if(2018 == fYear){
    t->SetBranchAddress("HLT_DoubleMu4_3_Jpsi", &fb.hlt1); }
  else if(2017 == fYear){
    t->SetBranchAddress("HLT_DoubleMu4_3_Jpsi_Displaced", &fb.hlt1);
  }
  t->SetBranchAddress("HLT_DoubleMu4_3_Bs", &fb.hlt3);
  

  t->SetBranchAddress("nmm", &fb.nmm);
  t->SetBranchAddress("mm_bdt", &fb.mm_bdt);
  t->SetBranchAddress("mm_doca", &fb.mm_doca);
  t->SetBranchAddress("mm_docatrk", &fb.mm_docatrk);
  t->SetBranchAddress("mm_iso", &fb.mm_iso);
  t->SetBranchAddress("mm_kin_eta", &fb.mm_kin_eta);
  t->SetBranchAddress("mm_kin_pt", &fb.mm_kin_pt);
  t->SetBranchAddress("mm_kin_l3d", &fb.mm_kin_l3d);
  t->SetBranchAddress("mm_kin_lxy", &fb.mm_kin_lxy);
  t->SetBranchAddress("mm_kin_sl3d", &fb.mm_kin_sl3d);
  t->SetBranchAddress("mm_kin_slxy", &fb.mm_kin_slxy);
  t->SetBranchAddress("mm_kal_mass", &fb.mm_kal_mass);
  t->SetBranchAddress("mm_kal_lxy", &fb.mm_kal_lxy);
  t->SetBranchAddress("mm_kal_slxy", &fb.mm_kal_slxy);
  t->SetBranchAddress("mm_kal_vtx_prob", &fb.mm_kal_vtx_prob);
  t->SetBranchAddress("mm_kin_mass", &fb.mm_kin_mass);
  t->SetBranchAddress("mm_kin_massErr", &fb.mm_kin_massErr);
  t->SetBranchAddress("mm_kin_alpha", &fb.mm_kin_alpha);
  t->SetBranchAddress("mm_kin_cosAlphaXY", &fb.mm_kin_cosAlphaXY);

  t->SetBranchAddress("mm_kin_mu1eta", &fb.mm_kin_mu1eta);
  t->SetBranchAddress("mm_kin_mu1pt", &fb.mm_kin_mu1pt);
  t->SetBranchAddress("mm_kin_mu1phi", &fb.mm_kin_mu1phi);
  t->SetBranchAddress("mm_kin_mu2eta", &fb.mm_kin_mu2eta);
  t->SetBranchAddress("mm_kin_mu2pt",& fb.mm_kin_mu2pt);
  t->SetBranchAddress("mm_kin_mu2phi", &fb.mm_kin_mu2phi);
  t->SetBranchAddress("mm_kin_pvip", &fb.mm_kin_pvip);
  t->SetBranchAddress("mm_kin_pvipErr", &fb.mm_kin_pvipErr);
  t->SetBranchAddress("mm_kin_pv2lip", &fb.mm_kin_pv2lip);
  t->SetBranchAddress("mm_kin_pv2lipErr", &fb.mm_kin_pv2lipErr);
  t->SetBranchAddress("mm_kin_pvlip", &fb.mm_kin_pvlip);
  t->SetBranchAddress("mm_kin_pvlipErr", &fb.mm_kin_pvlipErr);
  t->SetBranchAddress("mm_kin_pvlip", &fb.mm_kin_pvlip);

  t->SetBranchAddress("mm_kin_vtx_chi2dof", &fb.mm_kin_vtx_chi2dof);
  t->SetBranchAddress("mm_kin_vtx_prob", &fb.mm_kin_vtx_prob);
  t->SetBranchAddress("mm_m1iso", &fb.mm_m1iso);
  t->SetBranchAddress("mm_m2iso", &fb.mm_m2iso);
  t->SetBranchAddress("mm_mass", &fb.mm_mass);
  t->SetBranchAddress("mm_closetrk", &fb.mm_closetrk);
  t->SetBranchAddress("mm_closetrks1", &fb.mm_closetrk1);
  t->SetBranchAddress("mm_closetrks2", &fb.mm_closetrk2);
  t->SetBranchAddress("mm_closetrks3", &fb.mm_closetrk3);

  //  t->SetBranchAddress("nMuon", &fb.nMuon);
  t->SetBranchAddress("Muon1_dxy", &fb.Muon1_dxy);
  t->SetBranchAddress("Muon1_dxyErr", &fb.Muon1_dxyErr);
  t->SetBranchAddress("Muon1_dz", &fb.Muon1_dz);
  t->SetBranchAddress("Muon1_dzErr", &fb.Muon1_dzErr);

  t->SetBranchAddress("Muon1_eta", &fb.Muon1_eta);
  t->SetBranchAddress("Muon1_ip3d", &fb.Muon1_ip3d);
  t->SetBranchAddress("Muon1_mass", &fb.Muon1_mass);
  t->SetBranchAddress("Muon1_phi", &fb.Muon1_phi);
  t->SetBranchAddress("Muon1_pt", &fb.Muon1_pt);
  t->SetBranchAddress("Muon1_ptErr", &fb.Muon1_ptErr);
  t->SetBranchAddress("Muon1_segmentComp", &fb.Muon1_segmentComp);
  t->SetBranchAddress("Muon1_sip3d", &fb.Muon1_sip3d);  
  t->SetBranchAddress("Muon1_softMva", &fb.Muon1_softMva);
  t->SetBranchAddress("Muon1_mvaLowPt", &fb.Muon1_mvaLowPt);
  t->SetBranchAddress("Muon1_charge", &fb.Muon1_Charge);
  t->SetBranchAddress("Muon1_nTrackerLayers", &fb.Muon1_nTrackerLayers);
  t->SetBranchAddress("Muon1_nStations", &fb.Muon1_nStations);
  t->SetBranchAddress("Muon1_pdgId", &fb.Muon1_pdgId);
  t->SetBranchAddress("Muon1_tightCharge", &fb.Muon1_tightCharge);
  t->SetBranchAddress("Muon1_highPtId", &fb.Muon1_highPtId);
  t->SetBranchAddress("Muon1_inTimeMuon", &fb.Muon1_inTimeMuon);
  t->SetBranchAddress("Muon1_isGlobal", &fb.Muon1_isGlobal);
  t->SetBranchAddress("Muon1_isPFcand", &fb.Muon1_isPFcand);
  t->SetBranchAddress("Muon1_isTracker", &fb.Muon1_isTracker);
  t->SetBranchAddress("Muon1_looseId", &fb.Muon1_looseId);
  t->SetBranchAddress("Muon1_mediumId", &fb.Muon1_mediumId);
  t->SetBranchAddress("Muon1_mediumPromptId", &fb.Muon1_mediumPromptId);
  t->SetBranchAddress("Muon1_mvaId", &fb.Muon1_mvaId);
  t->SetBranchAddress("Muon1_softId", &fb.Muon1_softId);
  t->SetBranchAddress("Muon1_softMvaId", &fb.Muon1_softMvaId);
  t->SetBranchAddress("Muon1_tightId", &fb.Muon1_tightId);
  
  t->SetBranchAddress("Muon2_dxy", &fb.Muon2_dxy);
  t->SetBranchAddress("Muon2_dxyErr", &fb.Muon2_dxyErr);
  t->SetBranchAddress("Muon2_dz", &fb.Muon2_dz);
  t->SetBranchAddress("Muon2_dzErr", &fb.Muon2_dzErr);

  t->SetBranchAddress("Muon2_eta", &fb.Muon2_eta);
  t->SetBranchAddress("Muon2_ip3d", &fb.Muon2_ip3d);
  t->SetBranchAddress("Muon2_mass", &fb.Muon2_mass);
  t->SetBranchAddress("Muon2_phi", &fb.Muon2_phi);
  t->SetBranchAddress("Muon2_pt", &fb.Muon2_pt);
  t->SetBranchAddress("Muon2_ptErr", &fb.Muon2_ptErr);
  t->SetBranchAddress("Muon2_segmentComp", &fb.Muon2_segmentComp);
  t->SetBranchAddress("Muon2_sip3d", &fb.Muon2_sip3d);  
  t->SetBranchAddress("Muon2_softMva", &fb.Muon2_softMva);
  t->SetBranchAddress("Muon2_mvaLowPt", &fb.Muon2_mvaLowPt);
  t->SetBranchAddress("Muon2_charge", &fb.Muon2_Charge);
  t->SetBranchAddress("Muon2_nTrackerLayers", &fb.Muon2_nTrackerLayers);
  t->SetBranchAddress("Muon2_nStations", &fb.Muon2_nStations);
  t->SetBranchAddress("Muon2_pdgId", &fb.Muon2_pdgId);
  t->SetBranchAddress("Muon2_tightCharge", &fb.Muon2_tightCharge);
  t->SetBranchAddress("Muon2_highPtId", &fb.Muon2_highPtId);
  t->SetBranchAddress("Muon2_inTimeMuon", &fb.Muon2_inTimeMuon);
  t->SetBranchAddress("Muon2_isGlobal", &fb.Muon2_isGlobal);
  t->SetBranchAddress("Muon2_isPFcand", &fb.Muon2_isPFcand);
  t->SetBranchAddress("Muon2_isTracker", &fb.Muon2_isTracker);
  t->SetBranchAddress("Muon2_looseId", &fb.Muon2_looseId);
  t->SetBranchAddress("Muon2_mediumId", &fb.Muon2_mediumId);
  t->SetBranchAddress("Muon2_mediumPromptId", &fb.Muon2_mediumPromptId);
  t->SetBranchAddress("Muon2_mvaId", &fb.Muon2_mvaId);
  t->SetBranchAddress("Muon2_softId", &fb.Muon2_softId);
  t->SetBranchAddress("Muon2_softMvaId", &fb.Muon2_softMvaId);
  t->SetBranchAddress("Muon2_tightId", &fb.Muon2_tightId);


  t->SetBranchAddress("mm_mu1_index", &fb.mm_mu1_index);
  t->SetBranchAddress("mm_mu2_index", &fb.mm_mu2_index);  

  if(fIsMC){
    //t->SetBranchAddress("nPSWeight", &fb.nPS);
    //t->SetBranchAddress("PSWeight", &fb.PSW8);
    if (string::npos != mode.find("bupsik")) {                                                                                                                                 
        t->SetBranchAddress("bkmm_gen_mass", &fb.genmass);
	t->SetBranchAddress("bkmm_gen_kaon_pdgId", &fb.bkmm_k1_pdgId);
	t->SetBranchAddress("bkmm_gen_kaon_mpdgId", &fb.bkmm_pdgId);
	
	t->SetBranchAddress("bkmm_gen_l3d", &fb.bkmm_gen_l3d);
	t->SetBranchAddress("bkmm_gen_lxy", &fb.bkmm_gen_lxy);
	t->SetBranchAddress("bkmm_gen_kaon_pt", &fb.bkmm_gen_kaon_pt);
	t->SetBranchAddress("bkmm_gen_pt", &fb.bkmm_gen_pt);
    }
    
    t->SetBranchAddress("mm_gen_l3d", &fb.mm_gen_l3d);
    t->SetBranchAddress("mm_gen_lxy", &fb.mm_gen_lxy);
    t->SetBranchAddress("mm_gen_mass", &fb.mm_gen_mass);
    t->SetBranchAddress("mm_gen_mu1_pt", &fb.mm_gen_mu1_pt);
    t->SetBranchAddress("mm_gen_mu2_pt", &fb.mm_gen_mu2_pt);
    t->SetBranchAddress("mm_gen_pt", &fb.mm_gen_pt);
    
    t->SetBranchAddress("mm_gen_mpdgId", &fb.mm_gen_mpdgId);
    t->SetBranchAddress("mm_gen_mu1_mpdgId", &fb.mm_gen_mu1_mpdgId);
    t->SetBranchAddress("mm_gen_mu1_pdgId", &fb.mm_gen_mu1_pdgId);
    t->SetBranchAddress("mm_gen_mu2_mpdgId", &fb.mm_gen_mu2_mpdgId);
    t->SetBranchAddress("mm_gen_mu2_pdgId", &fb.mm_gen_mu2_pdgId);
    t->SetBranchAddress("mm_gen_pdgId", &fb.mm_gen_pdgId);

  }
  cout<<"plotClassnew::  this is to set chain "<<endl;

}


// ----------------------------------------------------------------------
void plotClassnew::candAnalysis(){

  cuts *pCuts(0);

  // if(fb.m1pt>fb.m2pt){
  //   if(fabs(fb.m2eta)>0 && fabs(fb.m2eta)<0.7)fChan=0;
  //   else if(fabs(fb.m2eta)>0.7 && fabs(fb.m2eta)<1.4)fChan=1;
  //   else if(fabs(fb.m2eta)>1.4 && fabs(fb.m2eta)<2.0)fChan=2;
  //   else if(fabs(fb.m2eta)>2.0 && fabs(fb.m2eta)<2.4)fChan=3;
  // }
  // else if(fb.m1pt<fb.m2pt){
  //   if(fabs(fb.m2eta)>0 && fabs(fb.m2eta)<0.7)fChan=0;
  //   else if(fabs(fb.m2eta)>0.7 && fabs(fb.m2eta)<1.4)fChan=1;
  //   else if(fabs(fb.m2eta)>1.4 && fabs(fb.m2eta)<2.0)fChan=2;
  //   else if(fabs(fb.m2eta)>2.0 && fabs(fb.m2eta)<2.4)fChan=3;
  // }
  // //cout<<"channel in plotClassnew "<<fChan<<endl;
  // if (fChan < 0 || fChan >= fNchan) {
  //   if (0) cout << "plotClassnew::candAnalysis: " << fb.run << " " << fb.evt
  // 		<< " could not determine channel: " << fb.m1eta << " " << fb.m2eta << endl;
  // 	     //fBDT = -99.;
  //   fGoodHLT = fGoodMuonsID = fGoodAmsID = fGoodGlobalMuons = fGoodGlobalMuonsKin = false;
  // //   fGoodRealMuonsID = fGoodRealAmsID = false;
  // //   fGoodCmssw = fGoodCandAna = false;
  // //   fGoodQ = fGoodPvAveW8 = fGoodMaxDoca = fGoodIp = fGoodIpS = fGoodPt = fGoodEta = fGoodPhi = fGoodAlpha =  fGoodChi2 = fGoodFLS = false;
  // //   fGoodCloseTrack = fGoodCloseTrackS1 = fGoodCloseTrackS2 = fGoodCloseTrackS3 = false;
  // //   fGoodIso = fGoodM1Iso = fGoodM2Iso = fGoodDocaTrk = fGoodCNC = fGoodBDT = fGoodBdtPresel = fPreselection = false;
  // //   fGoodAcceptance = fGoodBdtPt = fGoodMuonsPt = fGoodMuonsEta = fGoodTracks =  fGoodTracksPt = fGoodTracksEta = false;
  // //   fGoodDcand= false;
  //   return;
  // }
  // pCuts = fCuts[fChan];
  // bool bp2jpsikp(false), bs2jpsiphi(false), bd2jpsikstar(false);
  // if (BU2JPSIKP == fMode)  {
  //   bp2jpsikp = true;
  // }
  // if (BS2JPSIPHI == fMode)  {
  //   bs2jpsiphi = true;
  // }
  // if (BD2JPSIKSTAR == fMode)  {
  //   bd2jpsikstar = true;
  // }

  // // -- reset all
  // fBDT = -99.;
  // fGoodHLT = fGoodMuonsID = fGoodAmsID = fGoodGlobalMuons = fGoodGlobalMuonsKin = fGoodDcand = false;
  // fGoodRealMuonsID = fGoodRealAmsID = false;
  // fGoodQ = fGoodPvAveW8 = fGoodMaxDoca = fGoodIp = fGoodIpS = fGoodPt = fGoodEta = fGoodPhi = fGoodAlpha =  fGoodChi2 = fGoodFLS = false;
  // fGoodIso = fGoodM1Iso = fGoodM2Iso = fGoodDocaTrk = fGoodCNC = fGoodBDT = fGoodBdtPresel = fPreselection = fPreselectionBDT = false;
  // fGoodCloseTrack = fGoodCloseTrackS1 = fGoodCloseTrackS2 = fGoodCloseTrackS3 = false;

  // // fGoodJpsiCuts = true;

  // // // -- ensure that the underlying cuts are also applied/applicable at the plot* level
  

  // fGoodAcceptance = true;
  // // fGoodBdtPt      = true;
  // fGoodMuonsPt    = true;
  // fGoodMuonsEta   = true;
  // // fGoodTracks     = fb.gtqual;
  // fGoodTracksPt   = true;
  // fGoodTracksEta  = true;
  // // fGoodQ          = (fb.m1q*fb.m2q < 0);

  // // fIsCowboy = fb.cb;

  // if (fIsMC) {
  // //   if (fb.dcand) fGoodDcand = true;
  //   if (fb.m1pt < fAccPt) fGoodAcceptance = false;
  //   if (fb.m2pt < fAccPt) fGoodAcceptance = false;
  //   if (TMath::Abs(fb.m1eta) > fAccEtaGen) fGoodAcceptance = false;
  //   if (TMath::Abs(fb.m2eta) > fAccEtaGen) fGoodAcceptance = false;
  // } else {
  // //   fGoodDcand = true;
  // //   static int runComplained(-1);
  // //   if (!fb.json) {
  // //     if (fb.run != runComplained) {
  // // 	cout << "json failure for run " << fb.run << " LS " << fb.ls << endl;
  // // 	runComplained = fb.run;
  // //     }
  // //     return;
  // //   }
  // }

  // // // -- insist on channel boundaries
  // double m1 = TMath::Abs(fb.m1eta);
  // double m2 = TMath::Abs(fb.m2eta);
  // if (m2 > m1) m1 = m2;
  // if (m1 > pCuts->metaMax) fGoodMuonsEta = false;
  // if (m1 < pCuts->metaMin) fGoodMuonsEta = false;

  // //if (bp2jpsikp) {
  // //   if (fIsMC) {
  // //     // gen-level cuts for Bu2JpsiKp
  // //     if (fb.g1pt < fAccPt) fGoodAcceptance = false; // FIXME?
  // //     if (fb.g2pt < fAccPt) fGoodAcceptance = false; // FIXME?
  // //     if (TMath::Abs(fb.g3eta) > fAccEtaGen) fGoodAcceptance = false;
  // //     if (fb.g3pt < 0.6) fGoodAcceptance = false;   // FIXME!!
  // //   }
  // //   if (TMath::Abs(fb.keta) > fAccEtaRec) {
  // //     fGoodAcceptance = false;
  // //     fGoodTracksEta = false;
  // //   }
  // //   if (fb.kpt < 0.6) {
  // //     fGoodAcceptance = false;
  // //     fGoodTracksPt = false;
  // //   }
  // //   if (0 == fb.kgt)  {
  // //     fGoodAcceptance = false;
  // //     fGoodTracks = false;
  // //   }
  // // }


  // // if (bs2jpsiphi || bp2jpsikp || bd2jpsikstar) {
  // //   if (fb.mpsi > 3.15) fGoodJpsiCuts = false;
  // //   if (fb.mpsi < 3.04) fGoodJpsiCuts = false;
  // //   if (fb.psipt < 7.0) fGoodJpsiCuts = false;
  // //   if (fb.psiprob < 0.1) fGoodJpsiCuts = false;
  // // }

  // // fGoodGlobalMuons = (fb.m1mvabdt > -2.5) && (fb.m2mvabdt > -2.5);
  // // fGoodGlobalMuonsKin = fGoodGlobalMuons && fGoodMuonsPt && fGoodMuonsEta ;
  // // fGoodMuonsID     = (fGoodGlobalMuonsKin && (fb.m1mvabdt > fCuts[0]->muonbdt) && (fb.m2mvabdt > fCuts[0]->muonbdt));
  // // fGoodAmsID       = (fGoodGlobalMuonsKin && (fb.m1mvabdt < fCuts[0]->muonbdt) && (fb.m2mvabdt < fCuts[0]->muonbdt));

  // // fGoodRealMuonsID = ((fb.m1rmvabdt > -2.5) && (fb.m2rmvabdt > -2.5) && fGoodMuonsPt && fGoodMuonsEta && (fb.m1rmvabdt > fCuts[0]->muonbdt) && (fb.m2rmvabdt > fCuts[0]->muonbdt));
  // // fGoodRealAmsID   = ((fb.m1rmvabdt > -2.5) && (fb.m2rmvabdt > -2.5) && fGoodMuonsPt && fGoodMuonsEta && (fb.m1rmvabdt < fCuts[0]->muonbdt) && (fb.m2rmvabdt < fCuts[0]->muonbdt));

  // // if (RARE == fMode) {
  // //   fGoodMuonsID = true;
  // //   fGoodAmsID   = true;
  // //   fGoodGlobalMuons = true;
  // //   fGoodGlobalMuonsKin = true;
  // //   fGoodDcand = true;
  // // }

  // fW8 = 1.;
  // fW8MmuID = fW8Mtrig = fW8DmuID = fW8Dtrig = -1.;
  // double w1(-1.), w2(-1.);

  // // if (fIsMC) {
  // //   // FIXME REMOVE    PidTable *pT, *pT1, *pT2;

  // //   double am1eta = TMath::Abs(fb.m1eta);
  // //   double am2eta = TMath::Abs(fb.m2eta);

  // //   //FIXME REMOVE    pT  = fptM;
  // //   if ((RARE == fMode) || (BDMM == fMode) || (BSMM == fMode)) {

  // //     // -- normal muon ID
  // //     w1 = w2 = -1.;
  // //     // -- track 1
  // //     if (  13 == fb.g1id) w1 = fptNegMuons->effD(fb.m1pt, am1eta, 1.);
  // //     if ( -13 == fb.g1id) w1 = fptPosMuons->effD(fb.m1pt, am1eta, 1.);

  // //     if (  321 == fb.g1id) w1 = fptFakePosKaons->effD(fb.m1pt, am1eta, 1.);
  // //     if ( -321 == fb.g1id) w1 = fptFakeNegKaons->effD(fb.m1pt, am1eta, 1.);

  // //     if (  211 == fb.g1id) w1 = fptFakePosPions->effD(fb.m1pt, am1eta, 1.);
  // //     if ( -211 == fb.g1id) w1 = fptFakeNegPions->effD(fb.m1pt, am1eta, 1.);

  // //     if ( 2212 == fb.g1id) w1 = fptFakePosProtons->effD(fb.m1pt, am1eta, 1.);
  // //     if (-2212 == fb.g1id) w1 = fptFakeNegProtons->effD(fb.m1pt, am1eta, 1.);

  // //     // -- track 2
  // //     if (  13 == fb.g2id) w2 = fptNegMuons->effD(fb.m2pt, am2eta, 1.);
  // //     if ( -13 == fb.g2id) w2 = fptPosMuons->effD(fb.m2pt, am2eta, 1.);

  // //     if (  321 == fb.g2id) w2 = fptFakePosKaons->effD(fb.m2pt, am2eta, 1.);
  // //     if ( -321 == fb.g2id) w2 = fptFakeNegKaons->effD(fb.m2pt, am2eta, 1.);

  // //     if (  211 == fb.g2id) w2 = fptFakePosPions->effD(fb.m2pt, am2eta, 1.);
  // //     if ( -211 == fb.g2id) w2 = fptFakeNegPions->effD(fb.m2pt, am2eta, 1.);

  // //     if ( 2212 == fb.g2id) w2 = fptFakePosProtons->effD(fb.m2pt, am2eta, 1.);
  // //     if (-2212 == fb.g2id) w2 = fptFakeNegProtons->effD(fb.m2pt, am2eta, 1.);

  // //     fW8MisId = w1*w2;
  // //     if (0) cout << "fW8MisId = " << fW8MisId << " w1 = " << w1 << " w2 = " << w2
  // // 		  << " m1eta =  " << fb.m1eta << " m2eta =  " << fb.m2eta
  // // 		  << " m1pt =  " << fb.m1pt << " m2pt =  " << fb.m2pt
  // // 		  << " g1id =  " << fb.g1id << " g2id =  " << fb.g2id
  // // 		  << endl;

  // //     // -- anti-muon ID
  // //     w1 = w2 = -1.;
  // //     // -- track 1
  // //     if (  13 == fb.g1id) w1 = famNegMuons->effD(fb.m1pt, am1eta, 1.);
  // //     if ( -13 == fb.g1id) w1 = famPosMuons->effD(fb.m1pt, am1eta, 1.);

  // //     if (  321 == fb.g1id) w1 = famFakePosKaons->effD(fb.m1pt, am1eta, 1.);
  // //     if ( -321 == fb.g1id) w1 = famFakeNegKaons->effD(fb.m1pt, am1eta, 1.);

  // //     if (  211 == fb.g1id) w1 = famFakePosPions->effD(fb.m1pt, am1eta, 1.);
  // //     if ( -211 == fb.g1id) w1 = famFakeNegPions->effD(fb.m1pt, am1eta, 1.);

  // //     if ( 2212 == fb.g1id) w1 = famFakePosProtons->effD(fb.m1pt, am1eta, 1.);
  // //     if (-2212 == fb.g1id) w1 = famFakeNegProtons->effD(fb.m1pt, am1eta, 1.);

  // //     // -- track 2
  // //     if (  13 == fb.g2id) w2 = famNegMuons->effD(fb.m2pt, am2eta, 1.);
  // //     if ( -13 == fb.g2id) w2 = famPosMuons->effD(fb.m2pt, am2eta, 1.);

  // //     if (  321 == fb.g2id) w2 = famFakePosKaons->effD(fb.m2pt, am2eta, 1.);
  // //     if ( -321 == fb.g2id) w2 = famFakeNegKaons->effD(fb.m2pt, am2eta, 1.);

  // //     if (  211 == fb.g2id) w2 = famFakePosPions->effD(fb.m2pt, am2eta, 1.);
  // //     if ( -211 == fb.g2id) w2 = famFakeNegPions->effD(fb.m2pt, am2eta, 1.);

  // //     if ( 2212 == fb.g2id) w2 = famFakePosProtons->effD(fb.m2pt, am2eta, 1.);
  // //     if (-2212 == fb.g2id) w2 = famFakeNegProtons->effD(fb.m2pt, am2eta, 1.);

  // //     fW8AmMisId = w1*w2;

  // //     if (0) cout << "fW8AmMisId = " << fW8AmMisId << " w1 = " << w1 << " w2 = " << w2
  // // 		  << " m1eta =  " << fb.m1eta << " m2eta =  " << fb.m2eta
  // // 		  << " m1pt =  " << fb.m1pt << " m2pt =  " << fb.m2pt
  // // 		  << " g1id =  " << fb.g1id << " g2id =  " << fb.g2id
  // // 		  << endl;
  // //   }

  // // }
  
  // // fGoodPvAveW8    = (fb.pvw8 > 0.6);
  // //fGoodDoca    = (TMath::Abs(fb.doca) < pCuts->docatrk);
  // fGoodIp         = (TMath::Abs(fb.pvip) < pCuts->pvip);
  // fGoodIpS        = (TMath::Abs(fb.pvip/fb.pvipErr) < pCuts->pvips);

  // fGoodLip        = (TMath::Abs(fb.pvlip) < pCuts->pvlip);
  // fGoodLipS       = (TMath::Abs(fb.pvlip/fb.pvlipErr) < pCuts->pvlips);

  // fGoodPt         = (fb.pt > pCuts->pt);
  // // fGoodPhi        = ((fb.phi > pCuts->phiMin) && (fb.phi < pCuts->phiMax));
  // fGoodEta        = ((fb.eta > -fAccEtaRec) && (fb.eta < fAccEtaRec));
  // fGoodAlpha      = (fb.alpha < pCuts->alpha);
  // fGoodChi2       = (fb.chi2dof < pCuts->chi2dof);
  // fGoodFLS        = (fb.fls3d > pCuts->fls3d);
  // if (TMath::IsNaN(fb.fls3d)) fGoodFLS = false;

  // fGoodCloseTrack   = (fb.closetrk < pCuts->closetrk);
  // fGoodCloseTrackS1 = (fb.closetrks1 < pCuts->closetrks1);
  // fGoodCloseTrackS2 = (fb.closetrks2 < pCuts->closetrks2);
  // fGoodCloseTrackS3 = (fb.closetrks3 < pCuts->closetrks3);
  // fGoodIso          = (fb.iso > pCuts->iso);
  // fGoodM1Iso        = (fb.m1iso > pCuts->m1iso);
  // fGoodM2Iso        = (fb.m2iso > pCuts->m2iso);
  // fGoodDocaTrk      = (fb.docatrk > pCuts->docatrk);

  // fGoodBDT        = (fb.bdt > pCuts->bdtCut);
  // fGoodHLT        = fb.hlt1 ;
  // fGoodCmssw   =  (fb.m1pt>4.) && (fb.m2pt>4.)   && (fb.flsxy > 10.) && (fb.docatrk < 0.08) && (fb.chi2dof < 5.);
  // fGoodCandAna =  (fb.alpha<0.2) && (fb.fls3d>4.) && (TMath::Abs(fb.pvip/fb.pvipErr) < 4.) && (TMath::Abs(fb.pvip) < 0.02);
  // // -- no trigger matching for rare decays!
  // if (RARE == fMode) fGoodHLT = true;

  // fPreselection    = (fGoodHLT && fGoodBDT && fGoodCmssw && fGoodCandAna && fGoodAcceptance && fGoodMuonsEta) ;
  
  // //&& fGoodPt && fGoodEta && fGoodAlpha && fGoodChi2 && fGoodFLS && fGoodCloseTrack && fGoodDocaTrk && fGoodIso && fGoodM1Iso  && fGoodM2Iso && 


  // fCncCuts.update();
  // fBdtCuts.update();
  //  cout<<"After candanalysis calculation"<<endl;
}



// ----------------------------------------------------------------------
TTree* plotClassnew::getTree(string ds, string dir, string tree) {
  if (!fDS[ds]) {
    cout << "xx> plotClassnew::getTree: dataset ->" << ds << "<- not found" << endl;
    return 0;
  }
  TTree *t(0);
  // if (!dir.compare("")) {
  //   t = (TTree*)fDS[ds]->fF->Get(tree.c_str());
  // } else {

  //t = (TTree*)fDS[ds]->fF->Get(Form("%s/Events", dir.c_str()));
  //  t = (TTree*)fDS[ds]->fF->Get("Events");
  t = (TTree*)fDS[ds]->fF->Get("Candidates");
  //}
  cout << "plotClassnew::getTree(" << ds << ", " << dir << ", " << tree << "): " << t << endl;
  cout << "File name " << fDS[ds]->fF << endl;
  return t;
}

TChain* plotClassnew::getChain(string ds, string dir, string mode){
  if (!fDS[ds]) {
    cout << "xx> plotClassnew::getChain: dataset ->" << ds << "<- not found" << endl;
    return 0;
  }
  //TChain *t=new TChain("candAnaBu2JpsiK/Events");  
  TChain *t=new TChain("Candidates");  
  //TChain *t=new TChain("tree"); 
  
  if(string::npos != mode.find("bupsikData")){
    t->Add("/eos/home-c/ckar/Bmm5_code/nanoaodsample/bmm-reduced_BuToJpsiK_Data2018A_nano_v2.root");
    t->Add("/eos/home-c/ckar/Bmm5_code/nanoaodsample/bmm-reduced_BuToJpsiK_Data2018B_nano_v2.root");
    t->Add("/eos/home-c/ckar/Bmm5_code/nanoaodsample/bmm-reduced_BuToJpsiK_Data2018C_nano_v2.root");
    cout<<"using mode "<<mode<<endl;
  }

  else if(string::npos != mode.find("bmmData")){
    t->Add("/eos/home-c/ckar/Bmm5_code/nanoaodsample/bmm-reduced_BToMuMu_Data2018A_nano_v2.root");
    t->Add("/eos/home-c/ckar/Bmm5_code/nanoaodsample/bmm-reduced_BToMuMu_Data2018B_nano_v2.root");
    t->Add("/eos/home-c/ckar/Bmm5_code/nanoaodsample/bmm-reduced_BToMuMu_Data2018C_nano_v2.root");
    t->Add("/eos/home-c/ckar/Bmm5_code/nanoaodsample/bmm-reduced_BToMuMu_Data2018D_nano_v2.root");

  }
  cout << "plotClassnew::getChain(" << ds << ", " << dir << ", " <<mode<<"): " << t->GetEntries() << endl;
  return t;
}

// ----------------------------------------------------------------------
TFile* plotClassnew::loadFile(string file) {
  TFile *f = TFile::Open(file.c_str());
  return f;
}

TChain* plotClassnew::loadChain(string file) {
  TChain* f =new TChain();
  f -> AddFile(file.c_str());
  return f;
}



// ----------------------------------------------------------------------
void plotClassnew::replaceAll(string &sInput, const string &oldString, const string &newString) {
  string::size_type foundpos = sInput.find(oldString);
  while (foundpos != string::npos)  {
    sInput.replace(sInput.begin() + foundpos, sInput.begin() + foundpos + oldString.length(), newString);
    foundpos = sInput.find(oldString);
  }
}

// ----------------------------------------------------------------------
void plotClassnew::newLegend(double x1, double y1, double x2, double y2, string title) {
  //  if (legg) delete legg;
  legg = new TLegend(x1, y1, x2, y2, title.c_str());
  legg->SetFillStyle(0);
  legg->SetBorderSize(0);
  legg->SetTextSize(0.04);
  legg->SetFillColor(0);
  legg->SetTextFont(42);
}

// ----------------------------------------------------------------------
void plotClassnew::makeCanvas(int i) {
  if (i & 16) {
    c5 = new TCanvas("c5", "c5", 210,   0, 800, 900);
    c5->ToggleEventStatus();
  }
  if (i & 8) {
    c4 = new TCanvas("c4", "c4", 210,   0, 800, 600);
    c4->ToggleEventStatus();
  }
  if (i & 4) {
    c3 = new TCanvas("c3", "c3", 200,  20, 800, 800);
    c3->ToggleEventStatus();
  }
  if (i & 1) {
    //    c1 = new TCanvas("c1", "c1", 20,  60, 1200, 400);
    c1 = new TCanvas("c1", "c1", 20,  60, 1000, 400);
    c1->ToggleEventStatus();
  }
  if (i & 2) {
    c2 = new TCanvas("c2", "c2", 300, 200, 400, 800);
    c2->ToggleEventStatus();
  }
}


// ----------------------------------------------------------------------
// void plotClassnew::calcBDT() {
//   fBDT = -99.;
//   if (!preselection(fb)) return;

//   fBDT = -98.;
//   if (fChan < 0) return;

//   fBDT = -97.;
//   if (fGoodAcceptance
//     && fGoodQ
//       //NO!!    && fGoodMuonsGmID
//     && fGoodMuonsPt
//     && fGoodMuonsEta
//     && fGoodJpsiCuts
//     && fGoodPvAveW8
//       ) {
//     // do nothing
//   } else {
//     return;
//   }

//   fBDT = -90.;
//   if (fGoodCandAna && fGoodCmssw) {
//     // do nothing
//   } else {
//     return;
//   }

//   frd.pt = fb.pt;
//   frd.eta = fb.eta;
//   frd.m1eta = fb.m1eta;
//   frd.m2eta = fb.m2eta;
//   frd.m1pt = fb.m1pt;
//   frd.m2pt = fb.m2pt;
//   frd.m1phi = fb.m1phi;
//   frd.m2phi = fb.m2phi;

//   frd.fls3d = fb.fls3d;
//   frd.fl3d = fb.fl3d;
//   frd.flsxy = fb.flsxy;
//   frd.alpha = fb.alpha;
//   frd.maxdoca = fb.maxdoca;
//   frd.pvip = fb.pvip;
//   frd.pvips = fb.pvips;
//   frd.iso = fb.iso;
//   frd.docatrk = fb.docatrk;
//   frd.chi2dof = fb.chi2dof;
//   frd.closetrk = fb.closetrk;

//   frd.closetrks1 = fb.closetrks1;
//   frd.closetrks2 = fb.closetrks2;
//   frd.closetrks3 = fb.closetrks3;

//   frd.m1iso = fb.m1iso;
//   frd.m2iso = fb.m2iso;

//   frd.pvdchi2 = fb.pvdchi2;
//   frd.othervtx = fb.othervtx;

//   frd.pvlip  = fb.pvlip;
//   frd.pvlips = fb.pvlips;

//   frd.pv2lip  = fb.pv2lip;
//   frd.pv2lips = fb.pv2lips;

//   frd.m  = fb.m;
//   int remainder = TMath::Abs(fb.evt%3);
//   if (0 == remainder) {
//     fBDT   = fReaderEvents0[fChan]->EvaluateMVA("BDT");
//   } else if (1 == remainder) {
//     fBDT   = fReaderEvents1[fChan]->EvaluateMVA("BDT");
//   } else if (2 == remainder) {
//     fBDT   = fReaderEvents2[fChan]->EvaluateMVA("BDT");
//   } else {
//     cout << "all hell break loose" << endl;
//   }

//   if (0 && TMath::Abs(fBDT - fb.bdt) > 1.e-6) {
//     cout << "calculated fBDT = " << fBDT
// 	 << " from reduced tree: fb.bdt = " << fb.bdt
// 	 << " mode = " << fMode << "bd2psikstar(" << BD2JPSIKSTAR << ") fChan = " << fChan
// 	 << endl;
//     printRedTreeEvt(fb);
//     if (fMode == BU2JPSIKP || fMode == BS2JPSIPHI || fMode == BD2JPSIKSTAR) {

//       cout << "jpsi: pt = " << fb.psipt << " prob = " << fb.psiprob << endl;

//       cout << "kaons: pt eta gt = ";
//       if (fMode == BU2JPSIKP) {
// 	cout << fb.kpt << " " << fb.keta << " " << fb.kgt;
//       }
//       if (fMode == BS2JPSIPHI) {
// 	cout << fb.k1pt << "/" << fb.k2pt << " " << fb.k1eta << "/" << fb.k2eta << " " << fb.k1gt << "/" << fb.k2gt;
//       }
//       if (fMode == BD2JPSIKSTAR) {
// 	cout << fb.kpt << "/" << fb.pipt << " " << fb.keta << "/" << fb.pieta << " " << fb.kgt << "/" << fb.pigt;
//       }
//     }
//     cout << endl;
//   }
// }


// ----------------------------------------------------------------------
void plotClassnew::readFile(string filename, vector<string> &lines) {
  cout << "    readFile " << filename << endl;
  char  buffer[200];
  ifstream is(filename.c_str());
  if (!is) {
    cout << "file ->" << filename << "<- not found, exit(1)" << endl;
    exit(1);
  }
  char input[1000];
  while (is.getline(buffer, 200, '\n')) {
    if (buffer[0] != '+') {
      lines.push_back(string(buffer));
    } else {
      sscanf(buffer, "+input %s", input);
      readFile(input, lines);
    }
  }
}

// ----------------------------------------------------------------------
void plotClassnew::readCuts(string filename) {
  cout << "==> plotClassnew: Reading " << filename << " for cut settings" << endl;
  vector<string> cutLines;
  readFile(filename, cutLines);

  float cutvalue;
  int dump(0), ok(0);
  string cutname("nada");

  cuts *a = 0;

  fCuts.clear();

  for (unsigned int i = 0; i < cutLines.size(); ++i) {
    if (string::npos != cutLines[i].find("nchan")) {
      cleanupString(cutLines[i]);
      vector<string> lineItems = split(cutLines[i], ' ');
      fNchan = atoi(lineItems[1].c_str());
    }
  }

  if (fNchan < 1) {
    cout << "no analysis channels found?!" << endl;
  } else {
    cout << "creating " << fNchan << " analysis channels" << endl;
  }

  fReaderEvents0.reserve(fNchan);
  fReaderEvents1.reserve(fNchan);
  fReaderEvents2.reserve(fNchan);

  for (int i = 0; i < fNchan; ++i) {
    a = new cuts;
    a->index = i;
    fCuts.push_back(a);
  }

  for (unsigned int i = 0; i < cutLines.size(); ++i) {
    cleanupString(cutLines[i]);
    vector<string> lineItems = split(cutLines[i], ' ');
    if (lineItems.size() < 2) {
      //      cout << "no complete cut line provided ->" << cutLines[i] << "<-" << endl;
      continue;
    }
    cutname  = lineItems[0];

    for (unsigned int j = 1; j < lineItems.size(); ++j) {
      a = fCuts[j-1];

      cutvalue = atof(lineItems[j].c_str());
      if (cutname == "nchan") {
	ok = 1;
      }

      if (cutname == "index") {
	a->index = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "index:                " << cutvalue << endl;
      }

      if (cutname == "metaMin") {
	a->metaMin = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "metaMin:              " << cutvalue << endl;
      }

      if (cutname == "metaMax") {
	a->metaMax = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "metaMax:              " << cutvalue << endl;
      }

      if (cutname == "muonbdt") {
	a->muonbdt = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "muonbdt:              " << cutvalue << endl;
      }

      if (cutname == "mBdLo") {
	a->mBdLo = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "mBdLo:                " << cutvalue << endl;
      }

      if (cutname == "mBdLo") {
	a->mBdLo = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "mBdLo:                " << cutvalue << endl;
      }

      if (cutname == "mBdHi") {
	a->mBdHi = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "mBdHi:                " << cutvalue << endl;
      }

      if (cutname == "mBsLo") {
	a->mBsLo = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "mBsLo:                " << cutvalue << endl;
      }

      if (cutname == "mBsHi") {
	a->mBsHi = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "mBsHi:                " << cutvalue << endl;
      }

      if (cutname == "mBuLo") {
	a->mBuLo = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "mBuLo:                " << cutvalue << endl;
      }

      if (cutname == "mBuHi") {
	a->mBuHi = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "mBuHi:                " << cutvalue << endl;
      }

      if (cutname == "etaMin") {
	a->etaMin = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "etaMin:               " << cutvalue << endl;
      }

      if (cutname == "etaMax") {
	a->etaMax = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "etaMax:               " << cutvalue << endl;
      }

      if (cutname == "pt") {
	a->pt = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "pt:                   " << cutvalue << endl;
      }

      if (cutname == "phiMax") {
	a->phiMax = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "phimax:               " << cutvalue << endl;
      }

      if (cutname == "phiMin") {
	a->phiMin = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "phimin:               " << cutvalue << endl;
      }

      if (cutname == "m1pt") {
	a->m1pt = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "m1pt:                 " << cutvalue << endl;
      }

      if (cutname == "m2pt") {
	a->m2pt = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "m2pt:                 " << cutvalue << endl;
      }

      if (cutname == "iso") {
	a->iso = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "iso:                  " << cutvalue << endl;
      }
      if (cutname == "m1iso") {
	a->m1iso = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "m1iso:                " << cutvalue << endl;
      }
      if (cutname == "m2iso") {
	a->m2iso = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "m2iso:                " << cutvalue << endl;
      }

      if (cutname == "chi2dof") {
	a->chi2dof = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "chi2dof:              " << cutvalue << endl;
      }

      if (cutname == "alpha") {
	a->alpha = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "alpha:                " << cutvalue << endl;
      }

      if (cutname == "fls3d") {
	a->fls3d = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "fls3d:                " << cutvalue << endl;
      }

      if (cutname == "flsxy") {
	a->flsxy = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "flsxy:                " << cutvalue << endl;
      }

      if (cutname == "flxyLo") {
	a->flxyLo = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "flxyLo:               " << cutvalue << endl;
      }

      if (cutname == "flxyHi") {
	a->flxyHi = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "flxyHi:               " << cutvalue << endl;
      }

      if (cutname == "docatrk") {
	a->docatrk = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "docatrk:              " << cutvalue << endl;
      }

      if (cutname == "maxdoca") {
	a->maxdoca = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "maxdoca:              " << cutvalue << endl;
      }

      if (cutname == "closetrk") {
	a->closetrk = static_cast<int>(cutvalue); ok = 1;
	if (dump) cout << j-1 << " " << "closetrk:             " << cutvalue << endl;
      }
      if (cutname == "closetrks1") {
	a->closetrks1 = static_cast<int>(cutvalue); ok = 1;
	if (dump) cout << j-1 << " " << "closetrks1:           " << cutvalue << endl;
      }
      if (cutname == "closetrks2") {
	a->closetrks2 = static_cast<int>(cutvalue); ok = 1;
	if (dump) cout << j-1 << " " << "closetrks2:           " << cutvalue << endl;
      }
      if (cutname == "closetrks3") {
	a->closetrks3 = static_cast<int>(cutvalue); ok = 1;
	if (dump) cout << j-1 << " " << "closetrks3:           " << cutvalue << endl;
      }


      if (cutname == "pvip") {
	a->pvip = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "pvip:                 " << cutvalue << endl;
      }

      if (cutname == "pvips") {
	a->pvips = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "pvips:                " << cutvalue << endl;
      }


      if (cutname == "pvlip") {
	a->pvlip = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "pvlip:                " << cutvalue << endl;
      }

      if (cutname == "pvlips") {
	a->pvlips = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "pvlips:               " << cutvalue << endl;
      }

      if (cutname == "pv2lip") {
	a->pv2lip = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "pv2lip:               " << cutvalue << endl;
      }

      if (cutname == "pv2lips") {
	a->pv2lips = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "pv2lips:              " << cutvalue << endl;
      }

      if (cutname == "bdtxml") {
	a->bdtXml = lineItems[j]; ok = 1;
	if (dump) cout << j-1 << " " << "bdtxml:              " << a->bdtXml << endl;

	string sXmlName = "weights/" + a->bdtXml + "-Events0_BDT.weights.xml";
	cout << "call setupReader(" << sXmlName << ") for chan " << a->index << endl;
	TMVA::Reader *ar = setupReader(sXmlName, frd, fPresel);
	fReaderEvents0[a->index] = ar;

	sXmlName = "weights/" + a->bdtXml + "-Events1_BDT.weights.xml";
	cout << "call setupReader(" << sXmlName << ") for chan " << a->index << endl;
	ar = setupReader(sXmlName, frd, fPresel);
	fReaderEvents1[a->index] = ar;
	if (dump) cout << "xml:                   " << sXmlName << endl;

	sXmlName = "weights/" + a->bdtXml + "-Events2_BDT.weights.xml";
	cout << "call setupReader(" << sXmlName << ") for chan " << a->index << endl;
	ar = setupReader(sXmlName, frd, fPresel);
	fReaderEvents2[a->index] = ar;
	if (dump) cout << "xml:                   " << sXmlName << endl;
      }

      if (cutname == "bdtcut") {
	a->bdtCut = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "bdtcut:              " << cutvalue << endl;
      }

      if (cutname == "bdtmupt") {
	a->bdtMuPt = cutvalue; ok = 1;
	if (dump) cout << j-1 << " " << "bdtmupt:              " << cutvalue << endl;
      }

      if (cutname == "l1seeds") {
	vector<string> vl1seeds = split(lineItems[j], ',');
	for (unsigned int is = 0; is < vl1seeds.size(); ++is) {
	  fCuts[j-1]->l1seeds.push_back(atoi(vl1seeds[is].c_str()));
	}
      }


    }

    if (!ok) cout << "==> what about " << cutname << endl;
  }
  cout << "Preselection: " << endl;
  cout << fPresel.preselection() << endl;

  cout << "==> finished reading cut setting, fCuts.size() =  " << fCuts.size() << endl;

}


// ----------------------------------------------------------------------
void plotClassnew::printCuts(ostream &OUT) {

  OUT << "----------------------------------------------------------------------" << endl;
  OUT << "channel    ";
  for (unsigned int i = 0; i < fCuts.size(); ++i)  OUT << Form("%10d", fCuts[i]->index);
  OUT << endl;

  OUT << "metaMin    ";
  fTEX << Form("\\vdef{%s:etamuf:var}  {\\ensuremath{{|\\eta(\\mu_f)| } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->metaMin);
    fTEX <<  Form("\\vdef{%s:metaMin:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->metaMin) << endl;
  }
  OUT << endl;

  OUT << "metaMax    ";
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->metaMax);
    fTEX <<  Form("\\vdef{%s:metaMax:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->metaMax) << endl;
  }
  OUT << endl;

  OUT << "muonbdt    ";
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->muonbdt);
    fTEX <<  Form("\\vdef{%s:muonbdt:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->muonbdt) << endl;
  }
  OUT << endl;

  OUT << "l1seeds     ";
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    for (int is = fCuts[i]->l1seeds.size()*2; is < 10; ++is) OUT << " ";
    for (unsigned is = 0; is < fCuts[i]->l1seeds.size(); ++is) OUT << Form("%d ", fCuts[i]->l1seeds[is]);
    for (unsigned is = 0; is < fCuts[i]->l1seeds.size(); ++is)
      fTEX <<  Form("\\vdef{%s:l1seeds:%d}   {\\ensuremath{{%d } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->l1seeds[is]) << endl;
  }
  OUT << endl;

  OUT << "mBdLo      ";
  fTEX << Form("\\vdef{%s:mBd:var}  {\\ensuremath{{m(\\Bz) } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i) {
    OUT << Form("%10.3f", fCuts[i]->mBdLo);
    fTEX << Form("\\vdef{%s:mBdLo:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->mBdLo) << endl;
  }
  OUT << endl;

  OUT << "mBdHi      ";
  for (unsigned int i = 0; i < fCuts.size(); ++i) {
    OUT << Form("%10.3f", fCuts[i]->mBdHi);
    fTEX << Form("\\vdef{%s:mBdHi:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->mBdHi) << endl;
  }
  OUT << endl;

  OUT << "mBsLo      ";
  fTEX << Form("\\vdef{%s:mBs:var}  {\\ensuremath{{m(\\Bs) } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->mBsLo);
    fTEX <<  Form("\\vdef{%s:mBsLo:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->mBsLo) << endl;
  }
  OUT << endl;

  OUT << "mBsHi      ";
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->mBsHi);
    fTEX <<  Form("\\vdef{%s:mBsHi:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->mBsHi) << endl;
  }
  OUT << endl;

  OUT << "mBuLo      ";
  fTEX << Form("\\vdef{%s:mBu:var}  {\\ensuremath{{m(\\Bs) } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->mBuLo);
    fTEX <<  Form("\\vdef{%s:mBuLo:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->mBuLo) << endl;
  }
  OUT << endl;

  OUT << "mBuHi      ";
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->mBuHi);
    fTEX <<  Form("\\vdef{%s:mBuHi:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->mBuHi) << endl;
  }
  OUT << endl;


  OUT << "m1pt       ";
  fTEX << Form("\\vdef{%s:ptmuone:var}  {\\ensuremath{{\\ptmuone } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->m1pt);
    fTEX <<  Form("\\vdef{%s:m1pt:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->m1pt) << endl;
  }
  OUT << endl;

  OUT << "m2pt       ";
  fTEX << Form("\\vdef{%s:ptmutwo:var}  {\\ensuremath{{\\ptmutwo } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->m2pt);
    fTEX <<  Form("\\vdef{%s:m2pt:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->m2pt) << endl;
  }
  OUT << endl;

  OUT << "etaMin     ";
  fTEX << Form("\\vdef{%s:etaB:var}  {\\ensuremath{{|\\eta| } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->etaMin);
    fTEX <<  Form("\\vdef{%s:etaMin:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->etaMin) << endl;
  }
  OUT << endl;

  OUT << "etaMax     ";
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->etaMax);
    fTEX <<  Form("\\vdef{%s:etaMax:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->etaMax) << endl;
  }
  OUT << endl;

  OUT << "pt         ";
  fTEX << Form("\\vdef{%s:ptb:var}  {\\ensuremath{{\\ptb } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->pt);
    fTEX <<  Form("\\vdef{%s:pt:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->pt) << endl;
  }
  OUT << endl;

  OUT << "phiMin     ";
  fTEX << Form("\\vdef{%s:phiMin:var}  {\\ensuremath{{\\phi } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->phiMin);
    fTEX <<  Form("\\vdef{%s:phiMin:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->phiMin) << endl;
  }
  OUT << endl;

  OUT << "phiMax     ";
  fTEX << Form("\\vdef{%s:phiMax:var}  {\\ensuremath{{\\phi } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->phiMax);
    fTEX <<  Form("\\vdef{%s:phiMax:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->phiMax) << endl;
  }
  OUT << endl;

  OUT << "iso        ";
  fTEX << Form("\\vdef{%s:iso:var}  {\\ensuremath{{\\iso } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->iso);
    fTEX <<  Form("\\vdef{%s:iso:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->iso) << endl;
  }
  OUT << endl;

  OUT << "m1iso      ";
  fTEX << Form("\\vdef{%s:m1iso:var}  {\\ensuremath{{\\isomuone } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->m1iso);
    fTEX <<  Form("\\vdef{%s:m1iso:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->m1iso) << endl;
  }
  OUT << endl;

  OUT << "m2iso      ";
  fTEX << Form("\\vdef{%s:m2iso:var}  {\\ensuremath{{\\isomutwo } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->m2iso);
    fTEX <<  Form("\\vdef{%s:m2iso:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->m2iso) << endl;
  }
  OUT << endl;

  OUT << "chi2dof    ";
  fTEX << Form("\\vdef{%s:chidof:var}  {\\ensuremath{{\\chidof } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->chi2dof);
    fTEX <<  Form("\\vdef{%s:chi2dof:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->chi2dof) << endl;
  }
  OUT << endl;

  OUT << "alpha      ";
  fTEX << Form("\\vdef{%s:alpha:var}  {\\ensuremath{{\\alpha } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->alpha);
    fTEX <<  Form("\\vdef{%s:alpha:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->alpha) << endl;
  }
  OUT << endl;

  OUT << "fls3d      ";
  fTEX << Form("\\vdef{%s:fls:var}  {\\ensuremath{{\\fls } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->fls3d);
    fTEX <<  Form("\\vdef{%s:fls3d:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->fls3d) << endl;
  }
  OUT << endl;

  OUT << "flsxy      ";
  fTEX << Form("\\vdef{%s:flsxy:var}  {\\ensuremath{{\\flsyx } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->flsxy);
    fTEX <<  Form("\\vdef{%s:flsxy:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->flsxy) << endl;
  }
  OUT << endl;

  OUT << "docatrk    ";
  fTEX << Form("\\vdef{%s:docatrk:var}  {\\ensuremath{{\\docatrk } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->docatrk);
    fTEX <<  Form("\\vdef{%s:docatrk:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->docatrk) << endl;
  }
  OUT << endl;

  OUT << "closetrk   ";
  fTEX << Form("\\vdef{%s:closetrk:var}  {\\ensuremath{{\\closetrk } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->closetrk);
    fTEX <<  Form("\\vdef{%s:closetrk:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->closetrk) << endl;
  }
  OUT << endl;

  OUT << "closetrks1 ";
  fTEX << Form("\\vdef{%s:closetrks1:var}  {\\ensuremath{{\\closetrksI } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->closetrks1);
    fTEX <<  Form("\\vdef{%s:closetrks1:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->closetrks1) << endl;
  }
  OUT << endl;

  OUT << "closetrks2 ";
  fTEX << Form("\\vdef{%s:closetrks2:var}  {\\ensuremath{{\\closetrksII } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->closetrks2);
    fTEX <<  Form("\\vdef{%s:closetrks2:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->closetrks2) << endl;
  }
  OUT << endl;

  OUT << "closetrks3 ";
  fTEX << Form("\\vdef{%s:closetrks3:var}  {\\ensuremath{{\\closetrksIII } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->closetrks3);
    fTEX <<  Form("\\vdef{%s:closetrks3:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->closetrks3) << endl;
  }
  OUT << endl;

  OUT << "maxdoca    ";
  fTEX << Form("\\vdef{%s:maxdoca:var}  {\\ensuremath{{\\maxdoca } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->maxdoca);
    fTEX <<  Form("\\vdef{%s:maxdoca:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->maxdoca) << endl;
  }
  OUT << endl;

  OUT << "pvip       ";
  fTEX << Form("\\vdef{%s:pvip:var}  {\\ensuremath{{\\pvip } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->pvip);
    fTEX <<  Form("\\vdef{%s:pvip:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->pvip) << endl;
  }
  OUT << endl;

  OUT << "pvips      ";
  fTEX << Form("\\vdef{%s:pvips:var}  {\\ensuremath{{\\pvips } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->pvips);
    fTEX <<  Form("\\vdef{%s:pvips:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->pvips) << endl;
  }
  OUT << endl;

  OUT << "pvlip      ";
  fTEX << Form("\\vdef{%s:lip:var}  {\\ensuremath{{\\pvlip } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->pvlip);
    fTEX <<  Form("\\vdef{%s:pvlip:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->pvlip) << endl;
  }
  OUT << endl;

  OUT << "pvlips     ";
  fTEX << Form("\\vdef{%s:lips:var}  {\\ensuremath{{\\pvlips } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->pvlips);
    fTEX <<  Form("\\vdef{%s:pvlips:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->pvlips) << endl;
  }
  OUT << endl;

  OUT << "pv2lip     ";
  fTEX << Form("\\vdef{%s:liptwo:var}  {\\ensuremath{{\\pvliptwo } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->pv2lip);
    fTEX <<  Form("\\vdef{%s:pv2lip:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->pv2lip) << endl;
  }
  OUT << endl;

  OUT << "pv2lips    ";
  fTEX << Form("\\vdef{%s:lipstwo:var}  {\\ensuremath{{\\pvlipstwo } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->pv2lips);
    fTEX <<  Form("\\vdef{%s:pv2lips:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->pv2lips) << endl;
  }
  OUT << endl;

  OUT << "bdtXml     ";
  fTEX << Form("\\vdef{%s:bdtxml:var}  {\\tt{{\\bdtxml } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10s", fCuts[i]->bdtXml.c_str());
    fTEX <<  Form("\\vdef{%s:bdtxml:%d}   {\\tt{{%s } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->bdtXml.c_str()) << endl;
  }
  OUT << endl;

  OUT << "bdtCut     ";
  fTEX << Form("\\vdef{%s:bdtcut:var}  {\\tt{{\\bdtcut } } }", fSuffix.c_str()) << endl;
  for (unsigned int i = 0; i < fCuts.size(); ++i)  {
    OUT << Form("%10.3f", fCuts[i]->bdtCut);
    fTEX <<  Form("\\vdef{%s:bdtcut:%d}   {\\ensuremath{{%4.3f } } }", fSuffix.c_str(), fCuts[i]->index, fCuts[i]->bdtCut) << endl;
  }
  OUT << endl;

  OUT << "----------------------------------------------------------------------" << endl;

  OUT.flush();

  return;
}



// ----------------------------------------------------------------------
void plotClassnew::setItalic() {
  tl->SetTextFont(52);
}


// ----------------------------------------------------------------------
void plotClassnew::setRoman() {
  tl->SetTextFont(42);
}


// ----------------------------------------------------------------------
void plotClassnew::savePad(string name, TCanvas *c) {
  if (0 == c) {
    gPad->SaveAs(Form("%s/%s", fDirectory.c_str(), name.c_str()));
  } else {
    c->SaveAs(Form("%s/%s", fDirectory.c_str(), name.c_str()));
  }
}



// ----------------------------------------------------------------------
void plotClassnew::insertDataset(std::string dsname, dataset *ds) {
  if (fDS.find(dsname) != fDS.end()) {
    cout << "######## Error: " << dsname  << " already present in fDS, NOT inserting again" << endl;
  } else {
    //    cout << "     inserting: " << dsname  << " " << endl;
    fDS.insert(make_pair(dsname, ds));
  }
}


// ----------------------------------------------------------------------
void plotClassnew::loadFiles(string afiles) {

  string files = fDirectory + string("/") + afiles;
  cout << "==> plotClassnew::loadFiles loading files listed in " << files << endl;

  char buffer[1000];
  ifstream is(files.c_str());
  while (is.getline(buffer, 1000, '\n')) {
    if (buffer[0] == '#') {continue;}
    if (buffer[0] == '/') {continue;}

    string sbuffer = string(buffer);
    replaceAll(sbuffer, " ", "");
    replaceAll(sbuffer, "\t", "");
    if (sbuffer.size() < 1) continue;

    string::size_type m1 = sbuffer.find("lumi=");
    string::size_type m2 = sbuffer.find("eff=");
    string::size_type m3 = sbuffer.find("file=");
    string stype("nada");
    bool useBf(false);
    if (m1 > sbuffer.size()) {
      m1 = sbuffer.find("bf=");
      useBf = true;
    }
    double eff(0.), effE(0.);
    if (m2 > sbuffer.size()) {
      m2 = sbuffer.find("file=");
    } else {
      string seff = sbuffer.substr(m2+4, m3-m2-4);
      float val, err;
      int expo;
      sscanf(seff.c_str(), "(%f,%f)e%d", &val, &err, &expo);
      eff = val*TMath::Power(10., expo);
      effE = err*TMath::Power(10., expo);
    }
    stype = sbuffer.substr(5, m1-5);
    string slumi("nada"), sbf("nada");
    if (useBf) {
      sbf = sbuffer.substr(m1+3, m2-m1-3);
    } else {
      slumi = sbuffer.substr(m1+5, m3-m1-5);
    }
    string sfile = sbuffer.substr(m3+5);

    string sname("nada"), sdecay("nada"), ldecay("");

    double bf(0.), bfE(0.);
    if (useBf) {
      //cout << "sbf = " << sbf  << endl;
      float val, err;
      int expo;
      sscanf(sbf.c_str(), "(%f,%f)e%d", &val, &err, &expo);
      bf = val*TMath::Power(10., expo);
      bfE = err*TMath::Power(10., expo);
    } else {
      bf = 0.;
      bfE = 0.;
    }

    //    if (useBf) cout << " -> BF = " << bf << " +/- " << bfE << endl;

    TFile *pF(0);
    //TChain *pC(0);

    dataset *ds(0);

    if (string::npos != stype.find("data")) {
      // -- DATA
      pF = loadFile(sfile);
      //pC = loadChain(sfile);
      ds = new dataset();
      ds->fSize = 1.2;
      ds->fWidth = 2;
      if (string::npos != stype.find("bmm,")) {
        sname = "bmmData";
	if (string::npos != stype.find("legacy")) sname += "Legacy";
        sdecay = "dimuon";
	ldecay = "dimuon";
	ds->fColor = kBlack;
	ds->fSymbol = 20;
	ds->fF      = pF;
	//ds->fC      = pC;
	ds->fBf     = bf;
	ds->fBfE    = bfE;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	ds->fLumi   = atof(slumi.c_str());
      }

       if (string::npos != stype.find("bupsik,")) {
        sname = "bupsikData";
	if (string::npos != stype.find("legacy")) sname += "Legacy";
        sdecay = "B^{+} #rightarrow J/#kern[-0.2]{#it{#psi}}K^{+}";
        ldecay = "\\bupsik";
	ds->fColor = kBlack;
	ds->fSymbol = 20;
	ds->fF      = pF;
	//ds->fC      = pC;
	cout<<"Bf in plotclassnew "<<bf<<endl;
	ds->fBf     = bf;
	ds->fBfE    = bfE;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	ds->fLumi   = atof(slumi.c_str());
      }

      if (string::npos != stype.find("bspsiphi,")) {
        sname = "bspsiphiData";
	if (string::npos != stype.find("legacy")) sname += "Legacy";
        sdecay = "B^{0}_{s} #rightarrow J/#kern[-0.2]{#it{#psi}}#it{#phi}";
	ldecay = "\\bspsiphi";
	ds->fColor = kBlack;
	ds->fSymbol = 20;
	ds->fF      = pF;
	ds->fBf     = bf;
	ds->fBfE    = bfE;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	ds->fLumi   = atof(slumi.c_str());
      }

      if (string::npos != stype.find("bdpsikstar,")) {
        sname = "bdpsikstarData";
	if (string::npos != stype.find("legacy")) sname += "Legacy";
        sdecay = "B^{0} #rightarrow J/#kern[-0.2]{#it{#psi}}K^{*0}";
	ldecay = "\\bdpsikstar";
	ds->fColor = kBlack;
	ds->fSymbol = 20;
	ds->fF      = pF;
	ds->fBf     = bf;
	ds->fBfE    = bfE;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	ds->fLumi   = atof(slumi.c_str());
      }

      if (string::npos != stype.find("fake,")) {
        sname = "fakeData";
	if (string::npos != stype.find("legacy")) sname += "Legacy";
        sdecay = "fake";
	ldecay = "fake";
	ds->fColor = kBlack;
	ds->fSymbol = 20;
	ds->fF      = pF;
	ds->fBf     = bf;
	ds->fBfE    = bfE;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	ds->fLumi   = atof(slumi.c_str());
      }

    } else if (string::npos != stype.find("mc")) {
      // -- MC
      pF = loadFile(sfile);

      ds = new dataset();
      ds->fSize = 0.1;
      ds->fWidth = 2.;

      double bfbsmmVal = 3.57e-9;
      double bfbsmmErr = 0.17e-9;


      if (string::npos != stype.find("fake,")) {
        sname = "fakeMc";
        sdecay = "fake";
	ldecay = "fake";
	ds->fColor = kMagenta;
	ds->fSymbol = 25;
	ds->fF      = pF;
	ds->fBf     = bf;
	ds->fBfE    = bfE;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	ds->fLumi   = atof(slumi.c_str());
      }

      if (string::npos != stype.find("fakePions,")) {
        sname = "fakePionsMc";
        sdecay = "fake";
	ldecay = "fake";
	ds->fColor = kMagenta;
	ds->fSymbol = 25;
	ds->fF      = pF;
	ds->fBf     = bf;
	ds->fBfE    = bfE;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	ds->fLumi   = atof(slumi.c_str());
      }

      if (string::npos != stype.find("fakeKaons,")) {
        sname = "fakeKaonsMc";
        sdecay = "fake";
	ldecay = "fake";
	ds->fColor = kMagenta;
	ds->fSymbol = 25;
	ds->fF      = pF;
	ds->fBf     = bf;
	ds->fBfE    = bfE;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	ds->fLumi   = atof(slumi.c_str());
      }

      if (string::npos != stype.find("bupsik,")) {
        sname = "bupsikMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
	sdecay = "#it{B^{+}} #rightarrow #it{J}/#kern[-0.2]{#it{#psi}}K^{+}";
        ldecay = "\\bupsik";
	ds->fColor = kGreen-2;
	ds->fSymbol = 24;
	ds->fWidth  = 2.;
	ds->fF      = pF;
	ds->fBf     = bf;
	ds->fBfE    = bfE;
	// ds->fBf     = 6.021E-05;
	// ds->fBfE    = 1.761E-06;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3354;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bspsiphi,")) {
        sname = "bspsiphiMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "#it{B^{0}_{s}} #rightarrow #it{J}/#kern[-0.2]{#it{#psi}}#it{#phi}";
        ldecay = "\\bspsiphi";
	ds->fColor = kRed;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = 3.167E-05;
	ds->fBfE    = 2.375E-06;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bspsif,")) {
        sname = "bspsifMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "#it{B^{0}_{s}} #rightarrow #it{J}/#kern[-0.2]{#it{#psi}}f_{0}";
        ldecay = "\\bspsif";
	ds->fColor = kRed;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = 0.1; // FIXME DUMMY
	ds->fBfE    = 0.1;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bsmm,")) {
        sname = "bsmmMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
	if (string::npos != stype.find("Run1")) sname += "Run1";
        sdecay = "#it{B^{0}_{s}} #rightarrow #it{#mu^{+}#mu^{-}}";
        ldecay = "\\bsmm";
	ds->fColor = kRed;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fBf     = bfbsmmVal;
	ds->fBfE    = bfbsmmErr;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bymm,")) {
        sname = "bymmMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "B^{0}_{s}(5.7GeV) #rightarrow #it{#mu#mu}";
        ldecay = "\\bymm";
	ds->fColor = kRed;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = bfbsmmVal;
	ds->fBfE    = bfbsmmErr;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bxmm,")) {
        sname = "bxmmMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "B^{0}_{s}(5.1GeV) #rightarrow #it{#mu#mu}";
        ldecay = "\\bxmm";
	ds->fColor = kRed;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = bfbsmmVal;
	ds->fBfE    = bfbsmmErr;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bsmm80,")) {
        sname = "bsmm80Mc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "B^{0}_{s}(1.80ps) #rightarrow #it{#mu#mu}";
        ldecay = "\\bsmm";
	ds->fColor = kRed;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = bfbsmmVal;
	ds->fBfE    = bfbsmmErr;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bsmm75,")) {
        sname = "bsmm75Mc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "B^{0}_{s}(1.75ps) #rightarrow #it{#mu#mu}";
        ldecay = "\\bsmm";
	ds->fColor = kRed;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = bfbsmmVal;
	ds->fBfE    = bfbsmmErr;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bsmm70,")) {
        sname = "bsmm70Mc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "B^{0}_{s}(1.70ps) #rightarrow #it{#mu#mu}";
        ldecay = "\\bsmm";
	ds->fColor = kRed;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = bfbsmmVal;
	ds->fBfE    = bfbsmmErr;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bsmm69,")) {
        sname = "bsmm69Mc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "B^{0}_{s}(1.69ps) #rightarrow #it{#mu#mu}";
        ldecay = "\\bsmm";
	ds->fColor = kRed;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = bfbsmmVal;
	ds->fBfE    = bfbsmmErr;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bsmm68,")) {
        sname = "bsmm68Mc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "B^{0}_{s}(1.68ps) #rightarrow #it{#mu#mu}";
        ldecay = "\\bsmm";
	ds->fColor = kRed;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = bfbsmmVal;
	ds->fBfE    = bfbsmmErr;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bsmm67,")) {
        sname = "bsmm67Mc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "B^{0}_{s}(1.67ps) #rightarrow #it{#mu#mu}";
        ldecay = "\\bsmm";
	ds->fColor = kRed;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = bfbsmmVal;
	ds->fBfE    = bfbsmmErr;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bsmm66,")) {
        sname = "bsmm66Mc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "B^{0}_{s}(1.66ps) #rightarrow #it{#mu#mu}";
        ldecay = "\\bsmm";
	ds->fColor = kRed;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = bfbsmmVal;
	ds->fBfE    = bfbsmmErr;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bsmm65,")) {
        sname = "bsmm65Mc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "B^{0}_{s}(1.65ps) #rightarrow #it{#mu#mu}";
        ldecay = "\\bsmm";
	ds->fColor = kRed;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = bfbsmmVal;
	ds->fBfE    = bfbsmmErr;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bsmm60,")) {
        sname = "bsmm60Mc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "B^{0}_{s}(1.60ps) #rightarrow #it{#mu#mu}";
        ldecay = "\\bsmm";
	ds->fColor = kRed;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = bfbsmmVal;
	ds->fBfE    = bfbsmmErr;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bsmm55,")) {
        sname = "bsmm55Mc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "B^{0}_{s}(1.55ps) #rightarrow #it{#mu#mu}";
        ldecay = "\\bsmm";
	ds->fColor = kRed;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = bfbsmmVal;
	ds->fBfE    = bfbsmmErr;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bsmm50,")) {
        sname = "bsmm50Mc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "B^{0}_{s}(1.50ps) #rightarrow #it{#mu#mu}";
        ldecay = "\\bsmm";
	ds->fColor = kRed;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = bfbsmmVal;
	ds->fBfE    = bfbsmmErr;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bsmm45,")) {
        sname = "bsmm45Mc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "B^{0}_{s}(1.45ps) #rightarrow #it{#mu#mu}";
        ldecay = "\\bsmm";
	ds->fColor = kRed;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = bfbsmmVal;
	ds->fBfE    = bfbsmmErr;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bsmm40,")) {
        sname = "bsmm40Mc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "B^{0}_{s}(1.40ps) #rightarrow #it{#mu#mu}";
        ldecay = "\\bsmm";
	ds->fColor = kRed;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = bfbsmmVal;
	ds->fBfE    = bfbsmmErr;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bsmm35,")) {
        sname = "bsmm35Mc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "B^{0}_{s}(1.35ps) #rightarrow #it{#mu#mu}";
        ldecay = "\\bsmm";
	ds->fColor = kRed;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = bfbsmmVal;
	ds->fBfE    = bfbsmmErr;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bdpsikstar,")) {
        sname = "bdpsikstarMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "#it{B^{0}} #rightarrow J/#kern[-0.2]{#it{#psi}}K^{*0}";
        ldecay = "\\bdpsikstar";
	ds->fColor = kBlue;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = 5.040E-05;
	ds->fBfE    = 2.004E-06;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bdmm,")) {
        sname = "bdmmMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
        sdecay = "#it{B^{0}} #rightarrow #it{#mu^{+}#mu^{-}}";
        ldecay = "\\bdmm";
	ds->fColor = kBlue;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = bf;
	ds->fBfE    = bfE;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 3365;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bdkpi,")) {
        sname = "bdkpiMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
	if (string::npos != stype.find("bg")) sname += "Bg";
        sdecay = "#it{B^{0}} #rightarrow #it{K^{+}}#it{#pi^{-}}";
        ldecay = "\\bdkpi";
	ds->fColor = kBlue-2;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = 1.96e-5;
	ds->fBfE    = 0.05e-5;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 1000;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bdkk,")) {
        sname = "bdkkMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
	if (string::npos != stype.find("bg")) sname += "Bg";
        sdecay = "#it{B^{0}} #rightarrow #it{K^{+}K^{-}}";
        ldecay = "\\bdkk";
	ds->fColor = kBlue-10;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = 7.8e-8;
	ds->fBfE    = 1.5e-8;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 1000;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bdpipi,")) {
        sname = "bdpipiMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
	if (string::npos != stype.find("bg")) sname += "Bg";
        sdecay = "#it{B^{0}} #rightarrow #it{#pi^{+}#pi^{-}}";
        ldecay = "\\bdpipi";
	ds->fColor = kBlue-7;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = 5.12e-6;
	ds->fBfE    = 0.19e-6;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 1000;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bdpimunu,")) {
        sname = "bdpimunuMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
	if (string::npos != stype.find("bg")) sname += "Bg";
        sdecay = "#it{B^{0}} #rightarrow #it{#pi^{-}#mu^{+}#nu}";
        ldecay = "\\bdpimunu";
	ds->fColor = kBlue-9;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = 1.50e-4;
	ds->fBfE    = 0.06e-4;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 1000;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bdpimumu,")) {
        sname = "bdpimumuMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
	if (string::npos != stype.find("bg")) sname += "Bg";
        sdecay = "#it{B^{0}} #rightarrow #it{#pi^{0}#mu^{+}#mu^{-}}";
        ldecay = "\\bdpimumu";
	ds->fColor = kBlue-8;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = 5.3e-8;
	ds->fBfE    = 0.53e-8;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 1000;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }



      if (string::npos != stype.find("bspipi,")) {
        sname = "bspipiMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
	if (string::npos != stype.find("bg")) sname += "Bg";
        sdecay = "#it{B_{s}} #rightarrow #it{#pi^{+}#pi^{-}}";
        ldecay = "\\bspipi";
	ds->fColor = kRed-10;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = 7.0e-7;
	ds->fBfE    = 0.8e-7;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 1000;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bskpi,")) {
        sname = "bskpiMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
	if (string::npos != stype.find("bg")) sname += "Bg";
        sdecay = "#it{B_{s}} #rightarrow #it{K^{-}}#it{#pi^{+}}";
        ldecay = "\\bskpi";
	ds->fColor = kRed-7;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = 5.7e-6;
	ds->fBfE    = 0.6e-6;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 1000;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bskk,")) {
        sname = "bskkMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
	if (string::npos != stype.find("bg")) sname += "Bg";
        sdecay = "#it{B_{s}} #rightarrow #it{K^{+}K^{-}}";
        ldecay = "\\bskk";
	ds->fColor = kRed-2;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = 2.59e-5;
	ds->fBfE    = 0.17e-5;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 1000;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bskmunu,")) {
        sname = "bskmunuMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
	if (string::npos != stype.find("bg")) sname += "Bg";
        sdecay = "#it{B_{s}} #rightarrow #it{K^{-}}#it{#mu^{+}}#it{#nu}";
        ldecay = "\\bskmunu";
	ds->fColor = kRed-9;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = 9.4e-5;
	ds->fBfE    = 2.4e-5;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 1000;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("lbppi,")) {
        sname = "lbppiMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
	if (string::npos != stype.find("bg")) sname += "Bg";
        sdecay = "#it{#Lambda_{b}} #rightarrow #it{p}#it{#pi^{-}}";
        ldecay = "\\lbppi";
	ds->fColor = kGreen-7;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = 4.2e-6;
	ds->fBfE    = 0.8e-6;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 1000;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("lbpk,")) {
        sname = "lbpkMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
	if (string::npos != stype.find("bg")) sname += "Bg";
        sdecay = "#it{#Lambda_{b}} #rightarrow #it{pK^{-}}";
        ldecay = "\\lbpk";
	ds->fColor = kGreen-2;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = 5.1e-6;
	ds->fBfE    = 0.9e-6;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 1000;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("lbpmunu,")) {
	sname = "lbpmunuMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
	if (string::npos != stype.find("bg")) sname += "Bg";
        sdecay = "#it{#Lambda_{b}} #rightarrow #it{p}#it{#mu^{-}}#it{#nu}";
        ldecay = "\\lbpmunu";
	ds->fColor = kGreen-9;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = 4.1e-4;
	ds->fBfE    = 1.0e-4;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 1000;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bcpsimunu,")) {
	sname = "bcpsimunuMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
	if (string::npos != stype.find("bg")) sname += "Bg";
        sdecay = "#it{B_{c}} #rightarrow #it{J}/#it{#psi}#it{#mu}#it{#nu}";
        ldecay = "\\bcpsimunu";
	ds->fColor = kMagenta-3;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = 2.96e-2;
	ds->fBfE    = 1.06e-2;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 1000;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bupimumu,")) {
        sname = "bupimumuMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
	if (string::npos != stype.find("bg")) sname += "Bg";
        sdecay = "#it{B^{+}} #rightarrow #it{#pi^{+}#mu#mu}";
        ldecay = "\\bupimumu";
	ds->fColor = kYellow+3;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = 1.76e-8;
	ds->fBfE    = 0.23e-8;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 1000;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

      if (string::npos != stype.find("bupsipi,")) {
	sname = "bupsipiMc";
	if (string::npos != stype.find("mcOff")) sname += "Off";
	if (string::npos != stype.find("mcComb")) sname += "Comb";
	if (string::npos != stype.find("acc")) sname += "Acc";
	if (string::npos != stype.find("Run1")) sname += "Run1";
        sdecay = "#it{B^{+}} #rightarrow #it{J}/#it{#psi}#it{#pi}^{+}";
        ldecay = "\\bcpsimunu";
	ds->fColor = kYellow+2;
	ds->fSymbol = 24;
	ds->fF      = pF;
	ds->fBf     = 2.313e-6;
	ds->fBfE    = 7.3e-8;
	ds->fFilterEff = eff;
	ds->fFilterEffE = effE;
	ds->fMass   = 1.;
	ds->fFillStyle = 1000;
	TH1D *h1     = ds->getHist("monEvents", false);
	int nEvtFile = static_cast<int>((h1?h1->GetBinContent(1):1));
	ds->fLumi    = nEvtFile/fCrossSection/ds->fBf/ds->fFilterEff;
      }

    }
    // -- insert if with a "valid" sname
    if (sname != "nada") {
      ds->fLcolor    = ds->fColor;
      ds->fFcolor    = ds->fColor;
      ds->fName      = sdecay;
      ds->fLatexName = ldecay;
      ds->fFullName  = sname;
      insertDataset(sname, ds);
    } else {
      delete ds;
    }


  }

  cout << "    plotClassnew::loadFiles loaded " << fDS.size() << " datasets" << endl;

  is.close();


  if (1) {
    string directory("../common/pidtables/");
    string name("");
    if (2011 == fYear) {
      directory = string("weights/pidtables/");
      string prefixB("bmm4-42");
      name = directory + Form("%d-321Pos-%s.dat", fYear, prefixB.c_str());  fptFakePosKaons   = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-321Neg-%s.dat", fYear, prefixB.c_str());  fptFakeNegKaons   = new PidTable(Form(name.c_str()));

      name = directory + Form("%d-211Pos-%s.dat", fYear, prefixB.c_str());  fptFakePosPions   = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-211Neg-%s.dat", fYear, prefixB.c_str());  fptFakeNegPions   = new PidTable(Form(name.c_str()));

      name = directory + Form("%d-2212Pos-%s.dat", fYear, prefixB.c_str()); fptFakePosProtons = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-2212Neg-%s.dat", fYear, prefixB.c_str()); fptFakeNegProtons = new PidTable(Form(name.c_str()));

      name = directory + Form("%d-13Pos-%s.dat", fYear, prefixB.c_str()); fptPosMuons = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-13Neg-%s.dat", fYear, prefixB.c_str()); fptNegMuons = new PidTable(Form(name.c_str()));

      // -- antimuon ID
      prefixB = "am-bmm4-42";
      name = directory + Form("%d-321Pos-%s.dat", fYear, prefixB.c_str());  famFakePosKaons   = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-321Neg-%s.dat", fYear, prefixB.c_str());  famFakeNegKaons   = new PidTable(Form(name.c_str()));

      name = directory + Form("%d-211Pos-%s.dat", fYear, prefixB.c_str());  famFakePosPions   = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-211Neg-%s.dat", fYear, prefixB.c_str());  famFakeNegPions   = new PidTable(Form(name.c_str()));

      name = directory + Form("%d-2212Pos-%s.dat", fYear, prefixB.c_str()); famFakePosProtons = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-2212Neg-%s.dat", fYear, prefixB.c_str()); famFakeNegProtons = new PidTable(Form(name.c_str()));

      name = directory + Form("%d-13Pos-%s.dat", fYear, prefixB.c_str()); famPosMuons = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-13Neg-%s.dat", fYear, prefixB.c_str()); famNegMuons = new PidTable(Form(name.c_str()));

    } else if (2012 == fYear) {
      directory = string("weights/pidtables/");
      string prefixB("bmm4-42");
      name = directory + Form("%d-321Pos-%s.dat", fYear, prefixB.c_str());  fptFakePosKaons   = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-321Neg-%s.dat", fYear, prefixB.c_str());  fptFakeNegKaons   = new PidTable(Form(name.c_str()));

      name = directory + Form("%d-211Pos-%s.dat", fYear, prefixB.c_str());  fptFakePosPions   = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-211Neg-%s.dat", fYear, prefixB.c_str());  fptFakeNegPions   = new PidTable(Form(name.c_str()));

      name = directory + Form("%d-2212Pos-%s.dat", fYear, prefixB.c_str()); fptFakePosProtons = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-2212Neg-%s.dat", fYear, prefixB.c_str()); fptFakeNegProtons = new PidTable(Form(name.c_str()));

      name = directory + Form("%d-13Pos-%s.dat", fYear, prefixB.c_str()); fptPosMuons = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-13Neg-%s.dat", fYear, prefixB.c_str()); fptNegMuons = new PidTable(Form(name.c_str()));

      // -- anti-muon ID
      prefixB = "am-bmm4-42";
      name = directory + Form("%d-321Pos-%s.dat", fYear, prefixB.c_str());  famFakePosKaons   = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-321Neg-%s.dat", fYear, prefixB.c_str());  famFakeNegKaons   = new PidTable(Form(name.c_str()));

      name = directory + Form("%d-211Pos-%s.dat", fYear, prefixB.c_str());  famFakePosPions   = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-211Neg-%s.dat", fYear, prefixB.c_str());  famFakeNegPions   = new PidTable(Form(name.c_str()));

      name = directory + Form("%d-2212Pos-%s.dat", fYear, prefixB.c_str()); famFakePosProtons = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-2212Neg-%s.dat", fYear, prefixB.c_str()); famFakeNegProtons = new PidTable(Form(name.c_str()));

      name = directory + Form("%d-13Pos-%s.dat", fYear, prefixB.c_str()); famPosMuons = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-13Neg-%s.dat", fYear, prefixB.c_str()); famNegMuons = new PidTable(Form(name.c_str()));

    } else if (2016 == fYear) {
      directory = string("weights/pidtables/");
      string prefixB("bmm4-25");
      name = directory + Form("%d-321Pos-%s.dat", fYear, prefixB.c_str());  fptFakePosKaons   = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-321Neg-%s.dat", fYear, prefixB.c_str());  fptFakeNegKaons   = new PidTable(Form(name.c_str()));

      name = directory + Form("%d-211Pos-%s.dat", fYear, prefixB.c_str());  fptFakePosPions   = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-211Neg-%s.dat", fYear, prefixB.c_str());  fptFakeNegPions   = new PidTable(Form(name.c_str()));

      name = directory + Form("%d-2212Pos-%s.dat", fYear, prefixB.c_str()); fptFakePosProtons = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-2212Neg-%s.dat", fYear, prefixB.c_str()); fptFakeNegProtons = new PidTable(Form(name.c_str()));

      name = directory + Form("%d-13Pos-%s.dat", fYear, prefixB.c_str()); fptPosMuons = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-13Neg-%s.dat", fYear, prefixB.c_str()); fptNegMuons = new PidTable(Form(name.c_str()));

      prefixB = "am-bmm4-25";
      name = directory + Form("%d-321Pos-%s.dat", fYear, prefixB.c_str());  famFakePosKaons   = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-321Neg-%s.dat", fYear, prefixB.c_str());  famFakeNegKaons   = new PidTable(Form(name.c_str()));

      name = directory + Form("%d-211Pos-%s.dat", fYear, prefixB.c_str());  famFakePosPions   = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-211Neg-%s.dat", fYear, prefixB.c_str());  famFakeNegPions   = new PidTable(Form(name.c_str()));

      name = directory + Form("%d-2212Pos-%s.dat", fYear, prefixB.c_str()); famFakePosProtons = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-2212Neg-%s.dat", fYear, prefixB.c_str()); famFakeNegProtons = new PidTable(Form(name.c_str()));

      name = directory + Form("%d-13Pos-%s.dat", fYear, prefixB.c_str()); famPosMuons = new PidTable(Form(name.c_str()));
      name = directory + Form("%d-13Neg-%s.dat", fYear, prefixB.c_str()); famNegMuons = new PidTable(Form(name.c_str()));

    }
  }


}


// ----------------------------------------------------------------------
// downloaded on 2016/06/6 from https://ghm.web.cern.ch/ghm/plots/
TStyle * plotClassnew::setTdrStyle() {

  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  // For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

  // For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

  // For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

  // For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

  tdrStyle->SetEndErrorSize(2);
  // tdrStyle->SetErrorMarker(20);
  //tdrStyle->SetErrorX(0.);

  tdrStyle->SetMarkerStyle(20);

  //For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

  //For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

  // For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

  // Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadRightMargin(0.02);

  // For the Global title:
  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

  // For the axis titles:
  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.25);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

  // For the axis labels:
  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

  // For the axis:
  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

  // Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

  // Postscript options:
  tdrStyle->SetPaperSize(20.,20.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->SetHatchesLineWidth(1);
  tdrStyle->SetHatchesSpacing(0.05);

  tdrStyle->cd();

  return tdrStyle;
}


// ----------------------------------------------------------------------
void plotClassnew::muonBdtSetup(TH1D *hcuts, std::string &prefixB, double &cutB, std::string &prefixE, double &cutE) {
  string prefix("");
  for (int i = 1; i < hcuts->GetNbinsX(); ++i) {
    prefix = hcuts->GetXaxis()->GetBinLabel(i);
    if (string::npos != prefix.find("MUBDTXML ::")) {
      replaceAll(prefix, "MUBDTXML :: ", "");
      replaceAll(prefix, "muonid-", "");
      replaceAll(prefix, "-B", "");
      replaceAll(prefix, "-E", "");
      prefixB = prefix;
      break;
    }
  }

  for (int i = 1; i < hcuts->GetNbinsX(); ++i) {
    prefix = hcuts->GetXaxis()->GetBinLabel(i);
    if (string::npos != prefix.find("MUBDTXML1 ::")) {
      replaceAll(prefix, "MUBDTXML1 :: ", "");
      replaceAll(prefix, "muonid-", "");
      replaceAll(prefix, "-B", "");
      replaceAll(prefix, "-E", "");
      prefixE = prefix;
      break;
    }
  }

  for (int i = 1; i < hcuts->GetNbinsX(); ++i) {
    prefix = hcuts->GetXaxis()->GetBinLabel(i);
    if (string::npos != prefix.find("MUBDT ::")) {
      replaceAll(prefix, "MUBDT :: BDT(#mu) :: ", "");
      cutB = atof(prefix.c_str());
      break;
    }
  }

  for (int i = 1; i < hcuts->GetNbinsX(); ++i) {
    prefix = hcuts->GetXaxis()->GetBinLabel(i);
    if (string::npos != prefix.find("MUBDT1 ::")) {
      replaceAll(prefix, "MUBDT1 :: BDT(#mu) :: ", "");
      cutE = atof(prefix.c_str());
      break;
    }
  }

}


// // ----------------------------------------------------------------------
// TMVA::Reader* plotClassnew::setupMuonMvaReader(string xmlFile, mvaMuonIDData &d) {
//   TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );
//   TString dir    = "weights/";
//   TString methodNameprefix = "BDT";
//   // -- read in variables from weight file
//   vector<string> allLines;
//   char  buffer[2000];
//   string weightFile = "weights/TMVA-" + xmlFile + ".weights.xml";
//   cout << "setupMuonMvaReader, open file " << weightFile << endl;
//   ifstream is(weightFile.c_str());
//   while (is.getline(buffer, 2000, '\n')) allLines.push_back(string(buffer));
//   int nvars(-1);
//   string::size_type m1, m2;
//   string stype;
//   cout << "  read " << allLines.size() << " lines " << endl;
//   for (unsigned int i = 0; i < allLines.size(); ++i) {
//     // -- parse and add variables
//     if (string::npos != allLines[i].find("Variables NVar")) {
//       m1 = allLines[i].find("=\"");
//       stype = allLines[i].substr(m1+2, allLines[i].size()-m1-2-2);
//       cout << "  " << stype << " variables" << endl;
//       nvars = atoi(stype.c_str());
//       if (-1 == nvars) continue;
//       for (unsigned int j = i+1; j < i+nvars+1; ++j) {
// 	m1 = allLines[j].find("Expression=\"")+10;
// 	m2 = allLines[j].find("\" Label=\"");
// 	stype = allLines[j].substr(m1+2, m2-m1-2);
// 	if (stype == "segComp") {
// 	  //	  cout << "  adding segComp" << endl;
// 	  reader->AddVariable("segComp", &d.segComp);
// 	  continue;
// 	}
// 	if (stype == "chi2LocMom") {
// 	  //	  cout << "  adding chi2LocMom" << endl;
// 	  reader->AddVariable("chi2LocMom", &d.chi2LocMom);
// 	  continue;
// 	}
// 	if (stype == "chi2LocPos") {
// 	  //	  cout << "  adding chi2LocPos" << endl;
// 	  reader->AddVariable("chi2LocPos", &d.chi2LocPos);
// 	  continue;
// 	}
// 	if (stype == "NTrkVHits") {
// 	  //	  cout << "  adding NTrkVHits" << endl;
// 	  reader->AddVariable("NTrkVHits", &d.NTrkVHits);
// 	  continue;
// 	}

// 	if (stype == "trkValidFract") {
// 	  //	  cout << "  adding trkValidFract" << endl;
// 	  reader->AddVariable( "trkValidFract", &d.trkValidFract);
// 	  continue;
// 	}
// 	if (stype == "glbNChi2") {
// 	  reader->AddVariable("glbNChi2", &d.glbNChi2);
// 	  //	  cout << "  adding glbNChi2" << endl;
// 	  continue;
// 	}
// 	if (stype == "eta") {
// 	  //	  cout << "  adding eta" << endl;
// 	  reader->AddVariable("eta", &d.eta);
// 	  continue;
// 	}
// 	if (stype == "pt") {
// 	  //	  cout << "  adding pt" << endl;
// 	  reader->AddVariable("pt", &d.pt);
// 	  continue;
// 	}
// 	if (stype == "glbTrackProb") {
// 	  //	  cout << "  adding glbTrackProb" << endl;
// 	  reader->AddVariable("glbTrackProb", &d.glbTrackProb);
// 	  continue;
// 	}
// 	if (stype == "NTrkEHitsOut") {
// 	  //	  cout << "  adding NTrkEHitsOut" << endl;
// 	  reader->AddVariable("NTrkEHitsOut", &d.NTrkEHitsOut);
// 	  continue;
// 	}

// 	// -- variables for SW's BDT
// 	if (stype == "glbTrackTailProb") {
// 	  //	  cout << "  adding glbTrackTailProb" << endl;
// 	  reader->AddVariable("glbTrackTailProb", &d.glbTrackTailProb);
// 	  continue;
// 	}
// 	if (stype == "glbDeltaEtaPhi") {
// 	  //	  cout << "  adding glbDeltaEtaPhi" << endl;
// 	  reader->AddVariable("glbDeltaEtaPhi", &d.glbDeltaEtaPhi);
// 	  continue;
// 	}
// 	if (stype == "iValFrac") {
// 	  //	  cout << "  adding iValFrac (aka trkValidFract)" << endl;
// 	  reader->AddVariable("iValFrac", &d.trkValidFract);
// 	  continue;
// 	}
// 	if (stype == "LWH") {
// 	  //	  cout << "  adding LWH" << endl;
// 	  reader->AddVariable("LWH", &d.LWH);
// 	  continue;
// 	}
// 	if (stype == "dxyRef") {
// 	  //	  cout << "  adding dxyRef" << endl;
// 	  reader->AddVariable("dxyRef", &d.dxyRef);
// 	  continue;
// 	}
// 	if (stype == "kinkFinder") {
// 	  //	  cout << "  adding kinkFinder" << endl;
// 	  reader->AddVariable("kinkFinder", &d.kinkFinder);
// 	  continue;
// 	}
// 	if (stype == "dzRef") {
// 	  //	  cout << "  adding dzRef" << endl;
// 	  reader->AddVariable("dzRef", &d.dzRef);
// 	  continue;
// 	}
// 	if (stype == "glbKinkFinder") {
// 	  //	  cout << "  adding glbKinkFinder" << endl;
// 	  reader->AddVariable("glbKinkFinder", &d.glbKinkFinder);
// 	  continue;
// 	}
// 	if (stype == "TMath::Log(2+glbKinkFinder)") {
// 	  //	  cout << "  adding TMath::Log(2+glbKinkFinder)" << endl;
// 	  reader->AddVariable("TMath::Log(2+glbKinkFinder)", &d.glbKinkFinderLOG);
// 	  continue;
// 	}
// 	if (stype == "timeAtIpInOutErr") {
// 	  //	  cout << "  adding timeAtIpInOutErr" << endl;
// 	  reader->AddVariable("timeAtIpInOutErr", &d.timeAtIpInOutErr);
// 	  continue;
// 	}
// 	if (stype == "outerChi2") {
// 	  //	  cout << "  adding outerChi2" << endl;
// 	  reader->AddVariable("outerChi2", &d.outerChi2);
// 	  continue;
// 	}
// 	if (stype == "valPixHits") {
// 	  //	  cout << "  adding valPixHits" << endl;
// 	  reader->AddVariable("valPixHits", &d.valPixHits);
// 	  continue;
// 	}
// 	if (stype == "TMTrkMult100") {
// 	  //	  cout << "  adding TMTrkMult100" << endl;
// 	  reader->AddVariable("TMTrkMult100", &d.TMTrkMult100);
// 	  continue;
// 	}
// 	if (stype == "innerChi2") {
// 	  //	  cout << "  adding innerChi2" << endl;
// 	  reader->AddVariable("innerChi2", &d.innerChi2);
// 	  continue;
// 	}
// 	if (stype == "trkRelChi2") {
// 	  //	  cout << "  adding trkRelChi2" << endl;
// 	  reader->AddVariable("trkRelChi2", &d.trkRelChi2);
// 	  continue;
// 	}
// 	if (stype == "vMuonHitComb") {
// 	  //	  cout << "  adding vMuonHitComb" << endl;
// 	  reader->AddVariable("vMuonHitComb", &d.vMuonHitComb);
// 	  continue;
// 	}
// 	if (stype == "Qprod") {
// 	  //	  cout << "  adding Qprod" << endl;
// 	  reader->AddVariable("Qprod", &d.Qprod);
// 	  continue;
// 	}
//       }
//       break;
//     }
//   }

//   nvars = -1;
//   for (unsigned int i = 0; i < allLines.size(); ++i) {
//     // -- parse and add spectators
//     if (string::npos != allLines[i].find("Spectators NSpec")) {
//       m1 = allLines[i].find("=\"");
//       stype = allLines[i].substr(m1+2, allLines[i].size()-m1-2-2);
//       //      cout << "==> " << stype << endl;
//       nvars = atoi(stype.c_str());
//       if (-1 == nvars) continue;
//       for (unsigned int j = i+1; j < i+nvars+1; ++j) {
// 	m1 = allLines[j].find("Expression=\"")+10;
// 	m2 = allLines[j].find("\" Label=\"");
// 	stype = allLines[j].substr(m1+2, m2-m1-2);
// 	//	cout << "ivar " << j-i << " spectator string: ->" << stype << "<-" << endl;
// 	//	cout << " adding " << stype << " as a spectator dummy" << endl;
// 	reader->AddSpectator( stype.c_str(), &d.spectatorDummy);
//       }
//       break;
//     }
//   }

//   reader->BookMVA("BDT", TString(weightFile.c_str()));
//   return reader;
// }
