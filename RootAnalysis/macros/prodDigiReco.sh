#! /bin/bash

export RECREL=CMSSW_10_6_8
export SCRAM_ARCH=slc7_amd64_gcc700

# ----------------------------------------------------------------------
# -- The Basics
# ----------------------------------------------------------------------
echo "--> Environment"
date
hostname
uname -a
df -kl
limit coredumpsize 0

source /cvmfs/cms.cern.ch/cmsset_default.sh
echo "-> which root"
which root

pwd
echo "--> End of env testing"

# ----------------------------------------------------------------------
# -- Setup CMSSW
# ----------------------------------------------------------------------
echo "--> Setup CMSSW"
pwd
date
eval `scramv1 project CMSSW CMSSW_10_6_8`

cd CMSSW_10_6_8/src/
eval `scramv1 runtime -sh` 

tar -zxvf ../../Bmm.tar.gz

ls -ltr
cd Bmm/
pwd
export BMMBASE=$(pwd)

cd RootAnalysis/common/
make -j8 all links

cd ../rootio/
make -j8 all links

cd ../macros/

make -j8

cd ../../

ls -ltr
scramv1 b ProjectRename
scramv1 b

cd RootAnalysis/macros/
bin/runPlot -y 2018 -s 2018 -d 2018 -c baseCuts.2018.cuts -f plotResults.2018.files -p results -m bdtopt

cd 2018/

ls *.pdf *.tex *.root >filecp.log

for lf in $(cat filecp.log);
do xrdcp $lf root://cmseos.fnal.gov///eos/uscms/store/user/ckar/2018/$lf;
done

# BATCH END

echo "run: This is the end, my friend"
cd ${_CONDOR_SCRATCH_DIR}
rm -rf CMSSW_10_6_8

