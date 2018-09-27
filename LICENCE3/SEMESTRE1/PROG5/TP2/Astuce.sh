#!/bin/bash

#tableau : attention, sans espace !
tableau=('valeur0' 'valeur1' 'valeur2')
echo ${tableau[*]}

#chaine
chaine="bonjour"

# -p met une phrase d'intro "entrez vot login", -n pour ne pas dépasser 5 caractères dans 'nom', -e pour les sauts de lignes (permet \n)
read -p 'Entrez votre login (5 caractères max) : ' -n 5 nom
echo -e "\nBonjour $nom !"

# -t, compte à rebours avant que le shell s'arrete
read -p 'Entrez le code de désamorçage de la bombe (vous avez 5 secondes) : '  -t 5 code
echo -e "\nBoum !"

# -s cache la saisie
read -p 'Entrez votre mot de passe : ' -s pass
echo -e "\nMerci ! Je vais dire à tout le monde que votre mot de passe est $pass ! :)"

# manip mathématique
let "a = 5"
let "b = 2"
let "c = a + b"
echo $c

#if : attention, il FAUT des espaces dans la condition
if [ $1 = "Bruno" ]
then
        echo "Salut Bruno !"
elif [ $1 = "Michel" ]
then
        echo "Bien le bonjour Michel"
elif [ $1 = "Jean" ]
then
        echo "Hé Jean, ça va ?"
else
        echo "J'te connais pas, ouste !"
fi
