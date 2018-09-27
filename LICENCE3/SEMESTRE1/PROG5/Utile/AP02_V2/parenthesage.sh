#!/bin/bash

echo "DEMARRAGE DE LA PHASE DE TEST" 
echo "----------------------------------------" 
# liste des programmes ayant échoué à un test + les tests échoués
emplacement=$PWD
touch $emplacement/Liste_Erreurs_Programmes.txt

#préparation des fichiers
if [ $# != 0 ]
	then
		dossier_prog="$1"
		[ -d $emplacement/$dossier_prog/PROG_OK ] || mkdir $emplacement/$dossier_prog/PROG_OK
		mv $emplacement/$dossier_prog/* $emplacement/$dossier_prog/PROG_OK
	else
		dossier_prog="PROG_TEST"
fi

dossier_test_OK="TESTS/TEST_OK"
dossier_test_NOTOK="TESTS/TEST_NOTOK"


#création du fichier .txt de comparaison :
echo "Bon parenthesage" >> $emplacement/bon_parenthesage.txt
#les programmes posant problème sont placés dans PROG_ERROR, et seront analysés ensuite
[ -d $emplacement/$dossier_prog/PROG_ERROR ] || mkdir $emplacement/$dossier_prog/PROG_ERROR

#----------------------------------------------------------------
#TRI : Les programmes erronés sont placés dans PROG_ERROR, le reste est dans PROG_OK
echo "ANALYSE DES PROGRAMMES... Veuillez patienter."
for test in $(ls $dossier_test_OK)
	do	
		for element in $(ls $dossier_prog/PROG_OK)
			do
				./$dossier_prog/PROG_OK/$element < $dossier_test_OK/$test > resultat.txt
				DIFF=$(diff resultat.txt bon_parenthesage.txt)
				if [ "$DIFF" != "" ]
				then
					#echo -e "	$element : NOT OK"
					#si le programme n'existe pas (a déjà été déplacé), rien faire, sinon effectuer le "move"
					mv $dossier_prog/PROG_OK/$element $dossier_prog/PROG_ERROR
				#else
					#echo -e "	$element : OK"
				fi	
			done
	done
	
#--------------------
for test in $(ls $dossier_test_NOTOK)
	do	
		for element in $(ls $dossier_prog/PROG_OK)
			do
				./$dossier_prog/PROG_OK/$element < $dossier_test_NOTOK/$test > resultat.txt
				DIFF=$(diff resultat.txt bon_parenthesage.txt)
				if [ "$DIFF" == "" ]
				then
					#echo -e "	$element : NOT OK"
					mv $dossier_prog/PROG_OK/$element $dossier_prog/PROG_ERROR
				#else
					#echo -e "	$element : OK"
				fi	
			done
	done
echo "Resultat : $(ls $dossier_prog/PROG_ERROR | wc -l) programmes errones."
echo "ANALYSE TERMINEE. Nous vous préparons un rapport d'erreur, veuillez patienter."
echo "----------------------------------------" 

#--------------------

#ANALYSE : On regarde pourquoi les programmes de PROG_ERROR sont faux. La synthèse se trouve dans le .txt
echo "PREPARATION DU RAPPORT D'ERREUR EN COURS."
for element in $(ls $dossier_prog/PROG_ERROR)
	do
		echo -e "$element : " >> $emplacement/Liste_Erreurs_Programmes.txt
		
		#chaque programme subit un lot de tests bons (contenus dans TESTOK) : si le test échoue, le programme est déplacé dans un dossier PROG_ERROR
		for test in $(ls $dossier_test_OK)
			do	
				./$dossier_prog/PROG_ERROR/$element < $dossier_test_OK/$test > resultat.txt
				DIFF=$(diff resultat.txt bon_parenthesage.txt)
				if [ "$DIFF" != "" ]
				then
					echo -e "	$test : NOT OK" >> $emplacement/Liste_Erreurs_Programmes.txt
					
				else
					echo -e "	$test : OK" >> $emplacement/Liste_Erreurs_Programmes.txt
				fi	
			done
			
		#chaque programme subit un lot de tests mauvais (contenus dans TESTNOTOK)
		for test in $(ls $dossier_test_NOTOK)
			do	
				./$dossier_prog/PROG_ERROR/$element < $dossier_test_NOTOK/$test > resultat.txt
				DIFF=$(diff resultat.txt bon_parenthesage.txt)
				if [ "$DIFF" != "" ]
				then
					echo -e "	$test : OK" >> $emplacement/Liste_Erreurs_Programmes.txt
				else
					echo -e "	$test : NOT OK" >> $emplacement/Liste_Erreurs_Programmes.txt
				fi	
			done
		echo "--------------------------------------------------" >> $emplacement/Liste_Erreurs_Programmes.txt
	done
	echo "RAPPORT TERMINE : Liste_Erreurs_Programmes.txt"
	echo "----------------------------------------" 

#----------------------------------------------------------------
#Nettoyage :

rm bon_parenthesage.txt
rm resultat.txt
