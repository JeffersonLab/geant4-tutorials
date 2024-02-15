#!/bin/sh


# Copy the files to the release directory
for hdir in `ls -d HandsOn*/`
do
	echo "Copying $hdir to release directory"
	cp -r $hdir/index.html ../geant4-tutorials/$hdir
    cp -r $hdir/*.png      ../geant4-tutorials/$hdir
done