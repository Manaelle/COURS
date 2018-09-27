//////////////////////////////////////////////////////////////////////////////
// cr�ation et affichage d'une image en mode 256 niveaux de gris
// 
// N. SZAFRAN - UFR-IM2AG - 2015
//////////////////////////////////////////////////////////////////////////////

#include "lib_image.hpp"
#include <cstdio>

int main()
{
	// le tableau de pixels
	UINT8 p[] = 
	{   0, 128, 255 ,
	   64, 128, 192};
	
	//-------------------------------------------
	// cr�ation de l'image I1 de dimensions 3 x 2 
	// � partir du tableau de pixels p
	ImageGris I1(3,2,p);
	
	// affichage de l'image I1 avec un facteur d'�chelle 100
	UINT num_I1 = I1.afficher(100);
	changer_titre_fenetre_graphique(num_I1, "Image I1");


	//--------------------------------------------
	// cr�ation de l'image I2 de dimensions 16 x 8
	ImageGris I2(16,8);
	
	// d�finir les valeurs pixels
	for (UINT x=0; x<=15; x++)
	for (UINT y=0; y<=7; y++)
		I2.pixel(x,y) = x*17;
	
	// affichage de l'image I2 avec un facteur d'�chelle 20
	UINT num_I2 = I2.afficher(20);
	changer_titre_fenetre_graphique(num_I2, "Image I2");
	
	// sauvegarde de l'image I2 au format PNG
	I2.sauvegarder("degrade16.png");
	
	//----------------------------------------------------------------------
	// cr�ation et affichage d'une image avec fond et contenant trois carr�s
	// avec diff�rentes nuances de gris
	
	UINT8 t[] = 
	{  0, 0, 0 , 0  , 0, 0, 
	   0, 0, 0 , 242, 0, 0,
	   0, 0, 42, 142, 0, 0, 
	   0, 0, 0 , 0  , 0, 0,
	   };
	
	ImageGris I3(6,4,t);
	UINT num_I3 = I3.afficher(20);
	I3.sauvegarder_jpeg("image3 exo1.jpg",100);
	
	//----------------------------------------------------------
	// cr�ation et affichage d'une image de dimensions 256 x 100
	// avec un d�grad� des 256 nuances de gris
	
	ImageGris I4(256,100);
	
	for (UINT x=0; x<256; x++)
	for (UINT y=0; y<100; y++)
		I4.pixel(x,y) = x;
	
	UINT num_I4 = I4.afficher(10);
	I4.sauvegarder_jpeg("image4 exo1.jpg",50);
	

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
