
import os
from os import system, environ
#dasgoclient -query 'file dataset=/BsToJpsiPhi_BMuonFilter_SoftQCDnonD_TuneCUEP8M1_13TeV-pythia8-evtgen/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2/MINIAODSIM'
#output                = logs/job_$(Cluster)_$(Process).stdout#

#Requirements = OpSys == "LINUX"&& (Arch != "DUMMY" )

submitFileTT="""
universe              = vanilla
Executable            = prodDigiReco_2017.sh

Should_Transfer_Files = YES
WhenToTransferOutput = ON_EXIT
Transfer_Input_Files  = Bmm2_trig.tar.gz


x509userproxy         = x509up_u56005

+maxWallTime          = 2880
RequestMemory         = 8000
RequestDisk           = 15000

"""
fileParts = [submitFileTT]
count =1234
for ij in range(50):
    count=123*ij+12
    fileParts.append("error = logs/jobJpsi_{}_$(Cluster)_$(Process).stderr\n".format(ij))
    fileParts.append("Log = logs/joJpsib_{}_$(Cluster)_$(Process).log\n".format(ij))
    fileParts.append("output = logs/joJpsib_{}_$(Cluster)_$(Process).stdout\n".format(ij))

    fileParts.append("Arguments = 2017 {} 20\n".format(count))
    fileParts.append("Queue\n\n")

    fout = open("condor_sub{}.txt".format(ij),"w")
    fout.write(''.join(fileParts))
    fout.close()
    fileParts.pop(-1)
    fileParts.pop(-1)
    fileParts.pop(-1)
    fileParts.pop(-1)
    fileParts.pop(-1)
    system('condor_submit condor_sub{}.txt'.format(ij))
