if [ $1 = "fbock" ]; then
    LFHCALTBDIR=/home/fbock/EIC/Software/epic-LFHCalTB
else 
    echo "Please add your user name to the script and define the location of the TBAnalysisSoftware"
    exit
fi

echo $LFHCALTBDIR

mkdir -p configs

ln -sf $LFHCALTBDIR/configs/*.txt configs/
ln -sf $LFHCALTBDIR/configs/*.csv configs/

if [ $2 = "old" ]; then
    mkdir -p OldStructure
    ln -sf $LFHCALTBDIR/OldStructure/*.C OldStructure/
    ln -sf $LFHCALTBDIR/OldStructure/*.h OldStructure/
    ln -sf $LFHCALTBDIR/OldStructure/*.sh OldStructure/
fi 

if [ $2 = "new" ]; then
    mkdir -p NewStructure
    ln -sf $LFHCALTBDIR/NewStructure/*.h NewStructure/
    ln -sf $LFHCALTBDIR/NewStructure/*.cc NewStructure/
    ln -sf $LFHCALTBDIR/NewStructure/*.sh NewStructure/
    ln -sf $LFHCALTBDIR/NewStructure/Makefile NewStructure/
fi
