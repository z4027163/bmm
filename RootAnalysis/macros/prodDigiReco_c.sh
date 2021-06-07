
#! /bin/bash

export RECREL=CMSSW_10_6_8
export SCRAM_ARCH=slc7_amd64_gcc700

#export FILE1=file:root://cmseos.fnal.gov//eos/uscms/store/user/ckar/PYTHIA8_BsToMuMuPhi_2018_Gen/crab_crabjob_Genfile_2018nofilterMC_private/191223_064706/0000/PYTHIA8-BsToMuMuPhi-RunIIFall18GS-00134_nofilter_step0_$1.root
#export FILE1=/store/mc/RunIISummer16MiniAODv3/BsToJpsiPhi_BMuonFilter_SoftQCDnonD_TuneCUEP8M1_13TeV-pythia8-evtgen/MINIAODSIM/PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/60000/167E760C-0EEA-E911-80D6-0CC47AE2BAAE.root


#export FILE1=$2
#export FILE2="BsToMuMuPhi-JpsiMC_2016MINI_$1.root"


echo "========================"
echo "====> SGE  wrapper <===="
echo "========================"

echo "--> Running SGE digi-reco job wrapper"

#echo $FILE1
#echo $FILE2



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
echo "-> which edg-gridftp-ls"
which edg-gridftp-ls
echo "-> which globus-url-copy"
which globus-url-copy
echo "-> which srmcp"
which srmcp

pwd
echo "--> End of env testing"

# BATCH START

# ----------------------------------------------------------------------
# -- Setup CMSSW
# ----------------------------------------------------------------------
echo "--> Setup CMSSW"
pwd
date
eval `scramv1 project CMSSW CMSSW_10_6_8`
#cmsrel CMSSW_10_2_18
#
# cd CMSSW_10_6_8/
# tar -zxvf ../Bmm.tar.gz

# pwd

ls -ltr
#cd src/Bmm/
#eval `scramv1 runtime -sh`
# export BMMBASE=$(pwd)

# cd RootAnalysis/common/
# make -j8 all links

# cd ../rootio/
# make -j8 all links

# cd ../macros/

# make -j8

# cd ../../

# ls -ltr


# scramv1 b ProjectRename
# #scramv1 b clean
# #scramv1 b myanalyzer
# #cmsenv

# scramv1 b

# cd RootAnalysis/macros/
# #bin/runPlot -y 2018 -s 2018 -d 2018 -c baseCuts.2018.cuts -f plotResults.2018.files -p results -m bdtopt

# ls *>file.log

# for lf in file.log;
# do eoscp $lf /eos/uscms/store/user/ckar/2018/$lf;
# done

# BATCH END

echo "run: This is the end, my friend"
cd ${_CONDOR_SCRATCH_DIR}
rm -rf CMSSW_10_6_8
