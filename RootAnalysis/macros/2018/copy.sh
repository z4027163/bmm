

for file in $(cat cp.log);
do xrdcp root://cmseos.fnal.gov//eos/uscms/store/user/ckar/2018/$file $file;
    echo -e "priniting file" $file
done
