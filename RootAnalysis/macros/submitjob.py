
import os
from os import system, environ
#dasgoclient -query 'file dataset=/BsToJpsiPhi_BMuonFilter_SoftQCDnonD_TuneCUEP8M1_13TeV-pythia8-evtgen/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2/MINIAODSIM'
#output                = logs/job_$(Cluster)_$(Process).stdout#

#Requirements = OpSys == "LINUX"&& (Arch != "DUMMY" )

submitFileTT="""
universe              = vanilla
Executable            = prodDigiReco.sh

Should_Transfer_Files = YES

WhenToTransferOutput = ON_EXIT
Transfer_Input_Files  = ../../../Bmm.tar.gz

error                 = logs/job_2018_$(Cluster)_$(Process).stderr
Output                = logs/job_2018_$(Cluster)_$(Process).stdout
Log                   = logs/job_2018_$(Cluster)_$(Process).log
x509userproxy         = x509up_u56005

+maxWallTime          = 2880
RequestMemory         = 8000
RequestDisk           = 15000

"""
fileParts = [submitFileTT]
fileParts.append("Queue\n\n")

fout = open("condor_sub.txt","w")
fout.write(''.join(fileParts))
fout.close()
system('condor_submit condor_sub.txt')

