#!/bin/bash

emplacement=$PWD

#Creer CORBEILLE si il n'existe pas
[ -d $emplacement/.corbeille ] || mkdir $emplacement/.corbeille
#Databin.txt contient les info relatives aux fichiers/dossiers contenus dans la corbeille
[ -e $emplacement/.corbeille/.databin.txt ] || touch $emplacement/.corbeille/.databin.txt


#possibilité de choisir entre "efface", "restaure", "info" et "vide"
case $1 in
	"efface" )
	  #Déplacer les fichiers dans Corbeille : le point permet de masquer un dossier
		for element in ${*:2}
		do
			position=$( find $emplacement -type f -name "$element" )
			nbpos=$( find $emplacement -type f -name "$element" | wc -l )
			
			if [ $nbpos -ne 1 ]
			then
				echo "Plusieurs fichiers $element ont été trouvé. Pour annuler, taper c. Pour tout supprimer, taper a. Pour selectionner, tapez s."
				read selection
				
				case $selection in
					"a")
							for i in $( seq 1 $nbpos )
							do
									#position2 est le chemin absolue du fichier qu'on traite dans ce for
									position2=$( echo "$position" | sed -n "$i p" )
									date=$( stat -c "%z" $position2  | cut -d' ' -f1-2 )
									access=$( readlink -f $( dirname $position2) )
									#Il faut avant tout renommer le fichier pour le distinguer de ses semblables. Ici on lui ajoutera seulement un numéro : test.txt devient (1)test.txt
									renommage=$( echo "$position2" | sed "s/$element/($i)$element/" )
									mv $position2 $renommage
									
									#on effectue le déplacement en corbeille + modification de databin.txt
									if [ -e $renommage ]
									then
										mv $renommage $emplacement/.corbeille
										echo -e "$element $date $access" >> $emplacement/.corbeille/.databin.txt
									else
										echo -e "Le fichier $element n'existe pas."
									fi
							done
					;;
					"c")
							echo "Opération annulée"
					;;
					"s")
							echo "Entrez le chemin relatif du fichier à déplacer."
							read dossier
							if [ -e $dossier ]
							then
								position2=$( readlink -f $dossier )
								if [ -e $position2 ]
								then
									date=$( stat -c "%z" "$position2" | cut -d' ' -f1-2 )
									access=$( readlink -f $( dirname $position2) )
									mv $position2 $emplacement/.corbeille
									echo -e "$element $date $access" >> $emplacement/.corbeille/.databin.txt
								else
									echo -e "Le fichier $element n'existe pas."
								fi
							else
								echo "Le dossier $dossier n'existe pas."
							fi
					;;
				esac
			else
			
					if [ -e $position ]
					then
						date=$( stat -c "%z" $position  | cut -d' ' -f1-2 )
						access=$( readlink -f $( dirname $position) )
						mv $position $emplacement/.corbeille
						echo -e "$element $date $access" >> $emplacement/.corbeille/.databin.txt
					else
						echo -e "Le fichier $element n'existe pas."
					fi
			fi
		done
  	;;

	"restaure" )
	  #Restaurer les fichiers dans le répertoire courant.
		cd $emplacement/.corbeille
		for element in ${*:2}
		do
			if [ -e $element ]
			then
		   		#lire databin
				access=$( cat .databin.txt | sed -n "/$element/p" | cut -d' ' -f4 )
				mv $element $access/
				#Actualiser databin.txt
				sed "/$element/d" .databin.txt > .toto.txt
				mv .toto.txt .databin.txt

			else
				echo -e "Le fichier $element n'existe pas."
			fi
		done
		cd $emplacement
  	;;

	"info" )
	  #CHercher les infos des fichiers en paramètre, ou tout si aucun argument est donné
		if [ $# -eq 1 ]
		then
			cat $emplacement/.corbeille/.databin.txt
		else
			for element in ${*:2}
			do
				cat $emplacement/.corbeille/.databin.txt | sed -n "/$element/p" 
			done
		fi
  	;;

	"vide" )
	  #Suppression des fichiers en corbeille
		if [ $# -eq 1 ]
		then
			rm $emplacement/.corbeille/*
			touch .databin.txt
		else
			for element in ${*:2}
			do
				rm -r $emplacement/.corbeille/$element 
				sed "/$element/d" .databin.txt > .toto.txt
				mv .toto.txt .databin.txt
			done
		fi
  	;;
esac

