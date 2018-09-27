#!/bin/bash

for argument in $*
do

#------------------------------------
#cas des dossiers
	if [ -d $argument ]
	then
		echo "DOSSIER : $argument"
		stat $argument | sed -n '4p' | cut -d':' -f2 | cut -c8-17
#------------------------------------
#cas des fichiers
	elif [ -f $argument ]
	then
		echo "FICHIER : $argument"
		ls -l $argument | cut -d' ' -f1 #on sait que les droits sont affichés dans le 1er champs de ls-l
	fi

done
