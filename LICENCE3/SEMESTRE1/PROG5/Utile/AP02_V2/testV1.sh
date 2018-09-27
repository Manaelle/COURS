#!/bin/bash

#ATTENTION : C'est bin/BASH et non pas bin/sh ! (c'est ce qui a causé TOUS tes bugs *rit*

#Je te conseille de faire un chmod +x sur tous tes exécutables + ton .sh, avant de commencer ;)

#------------------------------------------
#Un conseil : fais TOUJOURS passer tes noms de fichiers/dossiers 
#externes en variable, comme ceci (ca évite d'avoir à les changer 
#un peu partout dans ton .sh, si un jour tu dois changer leur nom !):

dossier_test="tests"
dossier_prog="programs"
fichier_mauvais_parenthesage="resultat_mau.txt"

cpt_bon=0

#------------------------------------------
#Dans cette partie on a plusieurs problèmes :
#En gros, tu vas passer un test pour chaque programme, et 
#le compter comme "bon" si il renvoie "Mauvais parenthesage".
#Une fois tous les programmes testés, tu reset cpt_bon et tu 
#passes au test suivant. Mais que se passe-il si au test suivant
#se sont exactement les MEMES programmes qui bug ? Tu ne distingues 
#pas les programmes, tu dis juste "pour CE test, voilà combien ont buggé."

#Ajouté à ça, tu ne comptes QUE ceux qui trouvent un mauvais parenthesage
# dans un mauvais test (Ils sont à moitié juste.) Or ces même programmes 
#peuvent trouver un mauvais parenthesage AUSSI dans des bons tests ! Il est 
#donc indispensable de leur faire tester les deux cas.


for fichier_test in $(ls $dossier_test)
do 
	#après un echo, toujours des guillemets, ça évite certaines erreurs. Si tu veux que te variables soient prises en compte, ajoute l'option -e
	echo -e "$fichier_test"
   	cpt_bon=0
   	for programme_test in $(ls $dossier_prog)
   	do 
		./$dossier_prog/$programme_test < $dossier_test/$fichier_test > resultat.txt #si tu peux, termine les fichiers crées par des extensions (ici .txt)
      	DIFF=$(diff resultat.txt $fichier_mauvais_parenthesage)
		#Tu vas rire, mais si dans resultat_mau.txt y a pas un saut de ligne après "Mauvais parenthesage", ça marche pas !
		#C'est l'inconvénient de cette méthode de test : ton résultat attendu doit être EXACTEMENT comme la sortie des programmes...
      	if [ "$DIFF" == "" ]
      	then
	  		cpt_bon=$(( $cpt_bon + 1 ))
      	fi
   	done
   	echo -e "Nombre de mauvais programmes : $cpt_bon"
done

#------------------------------------------
for fichier_test in $(ls $dossier_test)
do 

   	for programme_test in $(ls $dossier_prog)
   	do 
		./$dossier_prog/$programme_test < $dossier_test/$fichier_test > resultat.txt
	done

done
#------------------------------------------
#Bon beh du coup ce qui te reste à faire est de tester tes programmes 
#avec des bons tests et des mauvais test !
#Tu trouve un nombre faible de mauvais programmes pour cette raison je pense !
#Le plus simple reste quand même de compter les programmes faux, 
#puisque UN ET UN SEUL test suffit à démontrer qu'il est faux.
#Or... pour montrer qu'un programme est juste, tu dois lui faire passer TOUS les tests...

#Voilà ! Que la force soit avec toi ! :D