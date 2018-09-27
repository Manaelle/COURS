//////////////////////////////////////////////////////////////////////////////
// création et affichage d'une image en mode RGBF réel
// 
// N. SZAFRAN - UFR-IM2AG - 2015
//////////////////////////////////////////////////////////////////////////////

#include "lib_image.hpp"
#include <cstdio>

// les couleurs de base
RGBF Blanc  (255,255,255);
RGBF Noir   (  0,  0,  0);
RGBF Rouge  (255,  0,  0);
RGBF Vert   (  0,255,  0);
RGBF Bleu   (  0,  0,255);
RGBF Cyan   (  0,255,255);
RGBF Magenta(255,  0,255);
RGBF Jaune  (255,255,  0);

int main()
{
	// le tableau de pixels
	RGBF p[] = 
	{  Noir , Bleu,    Vert,  Cyan,
	   Rouge, Magenta, Jaune, Blanc};
	
	// création de l'image I1 de dimensions 4 x 2 
	// à partir du tableau de pixels p
	ImageCouleurF I1(4,2,p);
	
	// affichage de l'image I1 avec un facteur d'échelle 100
	UINT num_I1 = I1.afficher(100);
	changer_titre_fenetre_graphique(num_I1, "Image I1");
	
	// création de l'image I2 de dimensions 256 x 256
	ImageCouleurF I2(256,256);
	
	// définir les valeurs pixels
	for (int x=0; x<=255; x++)
	for (int y=0; y<=255; y++)
		I2.pixel(x,y) = RGBF((float)x/255.0,(float)y/255.0,0.0);
	
	// affichage de l'image I2 
	UINT num_I2 = I2.afficher();
	changer_titre_fenetre_graphique(num_I2, "Image I2");
	
	
	
	ImageGrisF I3R(401,401);
	
	// définir les valeurs pixels
	for (int x=0; x<401; x++)
	for (int y=0; y<401; y++)
		I3R.pixel(x,y) = (200.0+(float)x-(float)y) / 400.0;
	
	// affichage de l'image I2 avec un facteur d'échelle 20
	UINT num_I3R = I3R.afficher(1);
	
	changer_titre_fenetre_graphique(num_I3R, "Image I3R");
	
	ImageGrisF I3G(401,401);
	
	// définir les valeurs pixels
	for (int x=0; x<401; x++)
	for (int y=0; y<401; y++)
		I3G.pixel(x,y) = ((float)x +(float)y -200.0) / 400.0;
	
	// affichage de l'image I2 avec un facteur d'échelle 20
	UINT num_I3G = I3G.afficher(1);
	
	changer_titre_fenetre_graphique(num_I3G, "Image I3G");
	
	ImageGrisF I3B(401,401);
	
	// définir les valeurs pixels
	for (int x=0; x<401; x++)
	for (int y=0; y<401; y++)
		I3B.pixel(x,y) = (((float)x-200.0)*((float)x-200.0)+((float)y -200.0)*((float)y-200.0)) / 40000.0;
	
	// affichage de l'image I2 avec un facteur d'échelle 20
	UINT num_I3B = I3B.afficher(1);
	
	changer_titre_fenetre_graphique(num_I3B, "Image I3B");
	
	ImageCouleurF I3(401,401);
	
	for (int x=0; x<401; x++)
	for (int y=0; y<401; y++)
		I3.pixel(x,y)=(I3R(x,y),I3G(x,y),I3B(x,y));
		
	UINT num_I3 = I3.afficher(1);
	
	changer_titre_fenetre_graphique(num_I3, "Image I3");
	
	ImageGrisF I4(401,401);
	
	for (int x=0; x<401; x++)
	for (int y=0; y<401; y++)
		I4.pixel(x,y)=(I3R(x,y)+I3G(x,y)+I3B(x,y))/3;
		
	UINT num_I4 = I4.afficher(1);
	
	changer_titre_fenetre_graphique(num_I4, "Image I4");
	
	I3R.sauvegarder("image3R exo7.png");
	I3B.sauvegarder("image3B exo7.png");
	I3G.sauvegarder("image3G exo7.png");
	I3.sauvegarder("image3 exo7.png");
	I4.sauvegarder("image4 exo7.png");
	
	// attente de taper une touche
	printf("Taper un caractere au clavier pour continuer");
	getchar();
	
	// suppression des fenetres graphiques	
	supprimer_fenetre_graphique(num_I1);
	supprimer_fenetre_graphique(num_I2);

	return 0;
}
