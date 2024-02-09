if [ $1 = "fbock" ]; then
    LFHCALTBDIR=/home/fbock/EIC/Software/epic-LFHCalTB
else 
    echo "Please add your user name to the script and define the location of the TBAnalysisSoftware"
    exit
fi

echo $LFHCALTBDIR

mkdir -p configs

ln -sf $LFHCALTBDIR/configs/*.txt configs/

ln -sf $LFHCALTBDIR/*.C .
ln -sf $LFHCALTBDIR/*.h .
ln -sf $LFHCALTBDIR/*.sh .

