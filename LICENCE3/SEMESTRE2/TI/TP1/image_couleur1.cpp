//////////////////////////////////////////////////////////////////////////////
// création et affichage d'une image en mode RGB 24 bits
// 
// N. SZAFRAN - UFR-IM2AG - 2015
//////////////////////////////////////////////////////////////////////////////

#include "lib_image.hpp"
#include <cstdio>

// les couleurs de base
RGB Blanc  (255,255,255);
RGB Noir   (  0,  0,  0);
RGB Rouge  (255,  0,  0);
RGB Vert   (  0,255,  0);
RGB Bleu   (  0,  0,255);
RGB Cyan   (  0,255,255);
RGB Magenta(255,  0,255);
RGB Jaune  (255,255,  0);

int main()
{
	// le tableau de pixels
	RGB p[] = 
	{  Noir , Bleu,    Vert,  Cyan,
	   Rouge, Magenta, Jaune, Blanc};
	
	// création de l'image I1 de dimensions 4 x 2 
	// à partir du tableau de pixels p
	ImageCouleur I1(4,2,p);
	
	// affichage de l'image I1 avec un facteur d'échelle 100
	UINT num_I1 = I1.afficher(100);
	changer_titre_fenetre_graphique(num_I1, "Image I1");
	
	// création de l'image I2 de dimensions 256 x 256
	ImageCouleur I2(256,256);
	
	// définir les valeurs pixels
	for (int x=0; x<=255; x++)
	for (int y=0; y<=255; y++)
		I2.pixel(x,y) = RGB(x,y,0);
	
	// affichage de l'image I2 
	UINT num_I2 = I2.afficher();
	changer_titre_fenetre_graphique(num_I2, "Image I2");
	
	//-------------------------------------------------------------------
	// création et affichage d'une image représentant le drapeau français
	
	RGB t[] = 
	{ Bleu , Blanc, Rouge,
	  Bleu , Blanc, Rouge};
	
	ImageCouleur I3(3,2,t);
	
	UINT num_I3 = I3.afficher(100);
	
	
	//----------------------------------------------------------
	// création et affichage d'image représentant l'ensemble des couleurs RGB
	// en fixant une des 3 composantes :
	// - image avec la composante B égale à 255
	// - image avec la composante G égale à 0
	// - image avec la composante G égale à 255
	// - image avec la composante R égale à 0
	// - image avec la composante R égale à 255
	
	
	ImageCouleur I4(256,256);
	
	for (int x=0; x<=255; x++)
	for (int y=0; y<=255; y++)
		I4.pixel(x,y) = RGB(x,y,255);
	
	UINT num_I4 = I4.afficher();
	
	ImageCouleur I6(256,256);
	
	for (int x=0; x<=255; x++)
	for (int y=0; y<=255; y++)
		I6.pixel(x,y) = RGB(x,255,y);
	
	UINT num_I6 = I6.afficher();
	
	ImageCouleur I5(256,256);
	
	for (int x=0; x<=255; x++)
	for (int y=0; y<=255; y++)
		I5.pixel(x,y) = RGB(x,0,y);
	
	UINT num_I5 = I5.afficher();
	
	ImageCouleur I7(256,256);
	
	for (int x=0; x<=255; x++)
	for (int y=0; y<=255; y++)
		I7.pixel(x,y) = RGB(0,y,x);
	
	UINT num_I7 = I7.afficher();
	
	ImageCouleur I8(256,256);
	
	for (int x=0; x<=255; x++)
	for (int y=0; y<=255; y++)
		I8.pixel(x,y) = RGB(255,y,x);
	
	UINT num_I8 = I8.afficher();
	I3.sauvegarder("image3 exo2.png");
	I4.sauvegarder("image4 exo2.png");
	I5.sauvegarder("image5 exo2.png");
	I6.sauvegarder("image6 exo2.png");
	I7.sauvegarder("image7 exo2.png");
	I8.sauvegarder("image8 exo2.png");
	
	
	
	
	//-------------------------------
	// en attente de taper une touche
	printf("Taper un caractere au clavier pour continuer");
	getchar();
	
	
	//------------------------------------
	// suppression des fenetres graphiques	
	supprimer_fenetre_graphique(num_I1);
	supprimer_fenetre_graphique(num_I2);

	return 0;
}
