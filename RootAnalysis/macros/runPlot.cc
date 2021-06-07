#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <math.h>

#include "TROOT.h"
#include "TRint.h"
#include "TChain.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TString.h"
#include "TRandom.h"
#include "TUnixSystem.h"
#include "TTimeStamp.h"
#include "plotFake.hh"
//#include "plotReducedOverlays.hh"

#include "plotResultsnew.hh"

#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

using namespace std;

int TMVAClassification( TString myMethodList);

// example: bin/runPlot -y 2016GH -f 101 -p tmva1 -m :NTrees=1000:nCuts=25:MaxDepth=3:MinNodeSize=0.500000:BoostType=AdaBoost:AdaBoostBeta=0.40 -s fls3d:alpha:pvips:iso:chi2dof:docatrk:closetrk:m1iso:m2iso:eta -r /scratch/ursl/bmm4/bdt/tmva-trees-41-2016.root > & ! TMVA-101.log

// ----------------------------------------------------------------------
int main(int argc, char *argv[]) {

  TTimeStamp ts0;
  cout << "start time: " << ts0.AsString("lc") << endl;

  string progName  = argv[0];

  string dir("nada"), cuts("nada"), files("nada"), plot("nada"), mode("nada"), setup("nada"), rootfilename("nada"), syear("0");
  int year(0);

  // -- command line arguments
  for (int i = 0; i < argc; i++){                             // tmva1:               trainingfiles:
    if (!strcmp(argv[i], "-c"))  {cuts  = argv[++i];}         //
    if (!strcmp(argv[i], "-d"))  {dir   = argv[++i];}         //                      data input filename
    if (!strcmp(argv[i], "-f"))  {files = argv[++i];}         // offset
    if (!strcmp(argv[i], "-m"))  {mode  = argv[++i];}         // BDT parameters
    if (!strcmp(argv[i], "-p"))  {plot  = argv[++i];}         //
    if (!strcmp(argv[i], "-r"))  {rootfilename  = argv[++i];} // input rootfilename   output rootfilename
    if (!strcmp(argv[i], "-s"))  {setup = argv[++i];}         // vars                 signal input filename
    if (!strcmp(argv[i], "-w"))  {mode  = argv[++i];}         // BDT parameters
    if (!strcmp(argv[i], "-y"))  {syear = argv[++i];}         //
  }

  if ("2017" == syear) {
    year = 2017;
    if ("nada" == files) files = "plotResults.2017.files";
    if ("nada" == cuts)  cuts  = "baseCuts.2017.cuts";
    if ("nada" == dir)   dir   = "results";
    if ("nada" == setup) setup = "";
  }
  if ("2018" == syear) {
    year = 2018;
    if ("nada" == files) files = "plotResults.2018.files";
    if ("nada" == cuts)  cuts  = "baseCuts.2018.cuts";
    if ("nada" == dir)   dir   = "results";
    if ("nada" == setup) setup = "";
  }

  if ("2016" == syear) {
    cout << "you should not run with -y 2016, choose -y 2016BF or -y 2016GH instead!" << endl;
    exit(0);
  }


  cout << "syear: " << syear << " year: " << year << " setup: " << setup << " dir: " << dir << " cuts: " << cuts << endl;

  // -- run everything

  // -- results
  if (string::npos != plot.find("results")) {
    gROOT->Clear();  gROOT->DeleteAll();
    plotResultsnew a(dir, files, cuts, setup, year);
    if (rootfilename != "nada") a.changeSetup(dir, rootfilename, setup);
    if (mode != "nada") {
      a.makeAll(mode);
    } else {
      a.makeAll();
    }
  }
  //  //Fake 
  if (string::npos != plot.find("fake")) {
    gROOT->Clear();  gROOT->DeleteAll();
    plotFake a(dir, files, cuts, setup, year);
    if (rootfilename != "nada") a.changeSetup(dir, rootfilename, setup);
    if (mode != "nada") {
      a.makeAll(mode);
    } else {
      a.makeAll();
    }
  }

  // -- TMVA create training files
  TTimeStamp ts1;
  cout << "end time: " << ts1.AsString("lc") << ", this is the end, my friend." << endl;

  return 0;
}
