COMPTE RENDU :

- huff_encode.c est terminé : Les tests fonctionnent (on en a ajouté)
	On a eu pas mal de segmentation fault : 
		- Pour créer l'encodage.
		- Au moment du parcours, on a regardé ArbreVide(T) alors qu'il fallait regarder si les deux fils de T étaient vides.
		- Etrangement, notre printf bugait, il fallait ajouter \n... 
	
- huff_decode.c n'a pas été touché hélas...


 En ce qui concerne les files à priorités : 
 
 	- L'initialisation de Huffman
 	- Dans la création de l'arbre de Huffman : On crée les feuilles ( Chaque feuille est ajouté dans la file avec comme priorité l'occurence des caractères ). On extrait les éléments de la file pour créer les noeuds constituant les sous-arbres. Puis on insère ces sous-arbres.
 
 Pour l'utilisation de arbrebin.h :
 	- A la création de l'arbre, pour créer des nouveaux noeuds.
 	- Au moment du parcours de l'arbre permettant de créer le tableau de code (codeHuffman)
