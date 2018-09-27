#!/bin/bash

for fichier in $(ls $1)
do
	echo $fichier
	if [ -f $1/$fichier ] && [ $(echo $fichier | cut -c1-3 ) = "DSC" ]
	then	
		mv $1/$fichier $2
	fi
done

