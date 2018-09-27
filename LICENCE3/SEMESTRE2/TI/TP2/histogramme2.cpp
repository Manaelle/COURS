///////////////////////////////////////////////////////////////////////////////
// affichage d'histogrammes
///////////////////////////////////////////////////////////////////////////////

#include "lib_image.hpp"
#include <cstdio>
#include <cstdlib>
#include <cmath>

// définition de quelques couleurs
RGB noir(0,0,0);
RGB blanc(255,255,255);
RGB rouge(255,0,0);
RGB vert(0,255,0);
RGB bleu(0,0,255);
RGB cyan(255,255,255);
RGB magenta(255,0,255);
RGB jaune(255,255,0);

int main(int argc, char *argv[])
{
	ImageCouleurF I;
	
	// chargement de l'image
	if (argc<2)
		I = ImageCouleurF("bastille.png");
	else
		I = ImageCouleurF(argv[1]);
	I.afficher();
	
	// nombre de classes de l'histogramme
	UINT nb_classes;
	if (argc<3)
		nb_classes = 256;
	else
		nb_classes = atoi(argv[2]);
		
	// ramener le nombre de classes entre 2 et 256
	if (nb_classes<2  ) nb_classes = 2  ;
	if (nb_classes>256) nb_classes = 256;
		
	// calculer l'histogramme de l'image I avec nb_classes classes
	Histogramme HR(I,nb_classes,true,false,false);
	Histogramme HG(I,nb_classes,false,true,false);
	Histogramme HB(I,nb_classes,false,false,true);
	Histogramme H(I,nb_classes,false,false,false);
	


	HR.afficher(noir, rouge, 400, 200);
	HG.afficher(noir, vert, 400, 200);
	HB.afficher(noir, bleu, 400, 200);
	H.afficher(blanc, noir, 400, 200);
	
	
	// affichage avec axe des x entre 0 et 1
	// avec la couleur noire pour le fond et 
	// la couleur blanc pour l'histogramme
	//H.afficher_0_1(noir,blanc); 
	
	// en attente de taper une touche
	printf("Taper un caractere au clavier pour continuer");
	getchar();
}
