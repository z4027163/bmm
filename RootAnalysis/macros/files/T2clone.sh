
#root://cmsxrootd.fnal.gov//store/user/ckar/Bmm5/
for file in $(cat t2cp.log);
  do xrdcp root://cmsxrootd.fnal.gov//store/user/ckar/Bmm5/$file $file;
 echo -e "priniting file" $file
 done
