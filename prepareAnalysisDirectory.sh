if [ $1 = "fbock" ]; then
    LFHCALTBDIR=/home/fbock/EIC/Software/epic-LFHCalTB
else 
    echo "Please add your user name to the script and define the location of the TBAnalysisSoftware"
    exit
fi

echo $LFHCALTBDIR

mkdir -p configs
mkdir -p NewStructure

ln -sf $LFHCALTBDIR/configs/*.txt configs/

ln -sf $LFHCALTBDIR/*.C .
ln -sf $LFHCALTBDIR/*.h .
ln -sf $LFHCALTBDIR/*.sh .

# ln -sf $LFHCALTBDIR/NewStructure/*.C NewStructure/
ln -sf $LFHCALTBDIR/NewStructure/*.h NewStructure/
ln -sf $LFHCALTBDIR/NewStructure/*.cc NewStructure/
# ln -sf $LFHCALTBDIR/NewStructure/*.cxx NewStructure/
ln -sf $LFHCALTBDIR/NewStructure/*.csv NewStructure/
ln -sf $LFHCALTBDIR/NewStructure/README NewStructure/
ln -sf $LFHCALTBDIR/NewStructure/Makefile NewStructure/
