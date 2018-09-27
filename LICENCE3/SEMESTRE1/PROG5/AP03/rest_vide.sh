#!/bin/bash

case $2 in							#case pour savoir si l'interval et en jours j, heure h, minute min et seconde s.
	j)
		inter=$(($3*24*3600))	#chaque valeur et transformer en seconde
	;;
	h)
		inter=$(($3*3600))
	;;
	min)
		inter=$(($3*60)) 
	;;
	s)
		inter=$3
	;;
	*)
		echo ce n\'est pas une option autoriser
	;;
esac

date=$( date -u +%s )			#calcule de la date en seconde 

date_limite=$(($date-$inter))	#date minumun qui reste dans l'interval 

x=$(($( wc -l .corbeille/.databin.txt | cut -d " " -f 1 )+1)) #nombre de ligne dans databin qui a tout les informations de chaque fichier

for i in $(seq 1 $x)
do
	date_fichier=$(sed -n "$i p" .corbeille/.databin.txt|cut -d " " -f 2-3)		#date de mise a la poubelle du fichier
	date_fichier=$(date -d "$date_fichier" +%s)											#transformation en seconde

	fichier=$(sed -n "$i p" .corbeille/.databin.txt|cut -d " " -f -1)				#recuperation nom fichier
	if [ "$date_limite" -ge "$date_fichier" ]												#vérification que la date de mise a la corbeille n'est pas l'interval autoriser et la supression
	then
		cd .corbeille																				
		rm $fichier
	fi
	
done
