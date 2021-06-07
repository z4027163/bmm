#/eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/
#/eos/uscms/store/user/ckar/Bmm5/NanoAOD/Merged/

# xrdfs root://cmseos.fnal.gov ls /store/user/ckar/Charmonium/
# tmp
# root://cmseos.fnal.gov//eos/uscms/store/user/ckar/Bmm5_508/
# basic_string=root://cmseos.fnal.gov//eos/uscms/store/user/ckar/

# my_copyfunc(){
#     my_file="$1"
#     dir_check="$2"
#     xrd_dir="$3"
#     echo -e "easily copy file" $my_file
#     echo -e "Directory file" $dir_check
#     for folder in $(cat $my_file);   
#     do 
# 	eosls "${xrd_dir}${folder}/*">delte.log
# 	for file in $(cat delte.log);
# 	do
# 	xrdcp "${xrd_dir}"$file "${dir_check}"$file;
#         file_check="${dir_check}"$file
#         echo -e "file file" $file_check
#         if [ -f "$file_check" ]
#         then
#         else
#             echo "$file not found."
#             xrdcp "${xrd_dir}"/$file "${dir_check}"$file;
#             file_check1="${dir_check}"$file
#             if [ -f "$file_check1" ]
#             then
#             else
#                 echo "$file not found second check."
#             fi
#         fi

#         echo -e "priniting file" $file
#     done
# }

# basic_str='root://cmseos.fnal.gov/'
# folder_init='/store/user/ckar/Bmm5/NanoAOD/508/'
# for_copy='tmp/'
# for_eosls='/store/user/ckar/Bmm5/NanoAOD/'
# /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/

# for_xrd="${for_copy}${data_output}"
# xrdfs root://cmseos.fnal.gov ls "${folder_init}" >Char_00.log
# my_copyfunc Char_00.log tmp/ $for_xrd
# echo -e "0000 done"

#For BuToJpsiKp
hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BuToJpsiK_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BuToJpsiK_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BuToJpsiK_BMuonFilter_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BuToJpsiK_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root


hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BuToJpsiK_BMuonFilter_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BuToJpsiK_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3+MINIAODSIM/*/*.root

hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BuToJpsiK_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BuToJpsiK_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

#For BsToJpsiPhi

hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToJpsiPhi_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToJpsiPhi_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToJpsiPhi_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToJpsiPhi_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToJpsiPhi_BMuonFilter_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToJpsiPhi_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-N1_102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToJpsiPhi_BMuonFilter_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToJpsiPhi_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_N1_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

# #For BsToMuMu

hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToMuMu_BMuonFilter_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToMuMu_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1+MINIAODSIM/*/*.root

hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToMuMu_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToMuMu_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToMuMu_BMuonFilter_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToMuMu_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToMuMu_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToMuMu_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

#ForBdToMuMu

hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BdToMuMu_BMuonFilter_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BdToMuMu_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BdToMuMu_BMuonFilter_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BdToMuMu_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BdToMuMu_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BdToMuMu_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BdToMuMu_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BdToMuMu_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

##For Charmonium2018


# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/Merged/bmm-reduced_BuToJpsiK_Data2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2017B-31Mar2018-v1+MINIAOD/Bukmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2017C-31Mar2018-v1+MINIAOD/Bukmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2017D-31Mar2018-v1+MINIAOD/Bukmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2017E-31Mar2018-v1+MINIAOD/Bukmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2017F-31Mar2018-v1+MINIAOD/Bukmm_flat/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/Merged/bmm-reduced_BsToMuMu_Data2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2017B-31Mar2018-v1+MINIAOD/Bsdmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2017C-31Mar2018-v1+MINIAOD/Bsdmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2017D-31Mar2018-v1+MINIAOD/Bsdmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2017E-31Mar2018-v1+MINIAOD/Bsdmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2017F-31Mar2018-v1+MINIAOD/Bsdmm_flat/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/Merged/bmm-reduced_BsToJpsiPhi_Data2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2017B-31Mar2018-v1+MINIAOD/Bskkmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2017C-31Mar2018-v1+MINIAOD/Bskkmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2017D-31Mar2018-v1+MINIAOD/Bskkmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2017E-31Mar2018-v1+MINIAOD/Bskkmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2017F-31Mar2018-v1+MINIAOD/Bskkmm_flat/*.root
# #ForCharmonium2018

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/Merged/bmm-reduced_BuToJpsiK_Data2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2018A-17Sep2018-v1+MINIAOD/Bukmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2018B-17Sep2018-v1+MINIAOD/Bukmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2018C-17Sep2018-v1+MINIAOD/Bukmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2018D-PromptReco-v2+MINIAOD/Bukmm_flat/*.root


# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/Merged/bmm-reduced_BsToJpsiPhi_Data2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2018A-17Sep2018-v1+MINIAOD/Bskkmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2018B-17Sep2018-v1+MINIAOD/Bskkmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2018C-17Sep2018-v1+MINIAOD/Bskkmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2018D-PromptReco-v2+MINIAOD/Bskkmm_flat/*.root



# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/Merged/bmm-reduced_BsToMuMu_Data2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2018A-17Sep2018-v1+MINIAOD/Bsdmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2018B-17Sep2018-v1+MINIAOD/Bsdmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2018C-17Sep2018-v1+MINIAOD/Bsdmm_flat/*.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/Charmonium+Run2018D-PromptReco-v2+MINIAOD/Bsdmm_flat/*.root



# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BdToMuMuPi_BMuonFilter_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BdToMuMuPi_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BdToMuMuPi_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BdToMuMuPi_SoftQCDnonD_TuneCUEP8M1_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BdToMuMuPi_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BdToMuMuPi_SoftQCDnonD_TuneCUEP8M1_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root


# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BdToPiMuNu_BMuonFilter_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BdToPiMuNu_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BdToPiMuNu_BMuonFilter_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BdToPiMuNu_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BdToPiMuNu_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BdToPiMuNu_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BdToPiMuNu_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BdToPiMuNu_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BdToPiPi_BMuonFilter_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BdToPiPi_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BdToPiPi_BMuonFilter_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BdToPiPi_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToKMuNu_BMuonFilter_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToKMuNu_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToKMuNu_BMuonFilter_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToKMuNu_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v4+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToKMuNu_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToKMuNu_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToKMuNu_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToKMuNu_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BuToMuMuPi_BMuonFilter_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BuToMuMuPi_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BuToMuMuPi_BMuonFilter_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BuToMuMuPi_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BuToMuMuPi_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BuToMuMuPi_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BuToMuMuPi_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BuToMuMuPi_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_LambdaBToPK_BMuonFilter_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/LambdaBToPK_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-N1_102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_LambdaBToPK_BMuonFilter_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/LambdaBToPK_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_N1_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_LambdaBToPK_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/LambdaBToPK_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_LambdaBToPK_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/LambdaBToPK_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_LambdaBToPMuNu_BMuonFilter_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/LambdaBToPMuNu_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_LambdaBToPMuNu_BMuonFilter_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/LambdaBToPMuNu_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_LambdaBToPPi_BMuonFilter_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/LambdaBToPPi_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_LambdaBToPPi_BMuonFilter_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/LambdaBToPPi_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_LambdaBToPMuNu_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/LambdaBToPMuNu_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_LambdaBToPMuNu_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/LambdaBToPMuNu_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root




# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BdToKPi_BMuonFilter_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BdToKPi_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BdToKPi_BMuonFilter_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BdToKPi_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BdToKPi_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BdToKPi_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BdToKPi_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BdToKPi_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToKK_BMuonFilter_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToKK_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToKK_BMuonFilter_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToKK_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToKK_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToKK_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToKK_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToKK_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToKPi_BMuonFilter_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToKPi_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-N1_102X_upgrade2018_realistic_v15-v2+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToKPi_BMuonFilter_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToKPi_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_N1_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToPiPi_BMuonFilter_MC2018_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToPiPi_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1+MINIAODSIM/*/*.root

# hadd /eos/uscms/store/user/ckar/Bmm5/NanoAOD/bmm-reduced_BsToPiPi_BMuonFilter_MC2017_nano_v3.root /eos/uscms/store/user/ckar/Bmm5/NanoAOD/508/test/BsToPiPi_BMuonFilter_SoftQCDnonD_TuneCP5_13TeV-pythia8-evtgen+RunIIFall17MiniAODv2-PU2017_12Apr2018_N1_94X_mc2017_realistic_v14-v1+MINIAODSIM/*/*.root




# # MCmatch_flat_Bukmm
# # MCmatch_flat_Bskkmm
# # MCmatch_flat_Bsmm
# # MCmatch_flat_Bdmm
# #MCmatch_flat_BsKPi
# # Bsdmm_flat  Bskkmm_flat  Bukmm_flat
