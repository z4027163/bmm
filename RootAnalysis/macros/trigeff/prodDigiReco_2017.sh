#! /bin/bash

export RECREL=CMSSW_10_6_8
export SCRAM_ARCH=slc7_amd64_gcc700

# ----------------------------------------------------------------------
# -- The Basics
# ----------------------------------------------------------------------
export YEAR=$1
export SEED=$2
export ITER=$3


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

tar -zxvf ../../Bmm2_trig.tar.gz 
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

cd RootAnalysis/macros/trigeff/
cp ../../../../../trigeff .

#make -j8
./trigeff fYear=$YEAR iterations=$ITER seed=$SEED

#cd 2017/
ls -ltr
#"%iBuJpsiKp_trigg_%i_toyset%i.root",fYear,iterations, seed)
xrdcp $1BuJpsiKp_trigg_$3_toyset$2.root root://cmseos.fnal.gov///eos/uscms/store/user/ckar/$1BuJpsiKp_trigg_$3_toyset$2.root

ls *.pdf *.tex *.root >filecp.log
tar zcvf trigeff_$1_toy$2_$3.tar.gz ../trigeff/
xrdcp trigeff_$1_toy$2_$3.tar.gz root://cmseos.fnal.gov///eos/uscms/store/user/ckar/trigeff_$1_toy$2_$3.tar.gz
# for lf in $(cat filecp.log);
# do xrdcp $lf root://cmseos.fnal.gov///eos/uscms/store/user/ckar/2017/$lf;
# done

# BATCH END

echo "run: This is the end, my friend"
cd ${_CONDOR_SCRATCH_DIR}
rm -rf CMSSW_10_6_8
