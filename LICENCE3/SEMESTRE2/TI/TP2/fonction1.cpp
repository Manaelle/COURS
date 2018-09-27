///////////////////////////////////////////////////////////////////////////////
// modification globale d'une image par une fonction
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

///////////////////////////////////////////////////////////////////////////////
// calcul et affichage de la moyenne m et de l'écart-type sigma
// pour l'image en niveaux de gris I
void statistiques(ImageGrisF &I)
{
	UINT N = I.nb_pixels();
	float s1, s2;
	float moyenne, sigma;
	
	s1 = s2 = 0.0;
	for (UINT i=0; i<N; i++)
	{
		float v = I(i);
		s1 += v; s2 += v*v;
	}
	moyenne = s1 / (float)N;
	sigma   = s2 / (float)N - moyenne*moyenne;
	printf("  moyenne    = %8.5f \n", moyenne);
	printf("  ecart-type = %8.5f \n", sigma);
}

///////////////////////////////////////////////////////////////////////////////
// calcul et affichage de la moyenne m et de l'écart-type sigma
// pour l'image couleur I
void statistiques(ImageCouleurF &I)
{
	UINT N = I.nb_pixels();
	float s1, s2;
	float moyenne, sigma;
	
	s1 = s2 = 0.0;
	for (UINT i=0; i<N; i++)
	{
		RGBF v = I(i);
		s1 += v.r; s2 += v.r*v.r; // composante R
		s1 += v.g; s2 += v.g*v.g; // composante G
		s1 += v.b; s2 += v.b*v.b; // composante B
	}
	moyenne = s1 / (float)(3*N);
	sigma   = s2 / (float)(3*N) - moyenne*moyenne;
	printf("  moyenne    = %8.5f \n", moyenne);
	printf("  ecart-type = %8.5f \n", sigma);
}

template <typename Image>
void traitement_image_gris(Image &I, Fonction F)
{	
	int num_f;
	
	// chargement de l'image
	num_f = I.afficher();
	changer_titre_fenetre_graphique(num_f, "Image I");
	
	// afficher et calculer l'histogramme sur 32 classes
	Histogramme H(I,32);
	num_f = H.afficher_0_1();
	changer_titre_fenetre_graphique(num_f, "Histo. de I");
	
	// afficher et calculer la moyenne et l'écart-type
	printf("\nStatistiques de l'image I :\n");
	statistiques(I);
	
	// tracer la fonction F
	num_f = F.afficher();
	changer_titre_fenetre_graphique(num_f, "Fonction F");
	
	// modifier l'image par la fonction F
	Image I_F;
	I_F = F(I);
	num_f = I_F.afficher();
	changer_titre_fenetre_graphique(num_f, "Image F(I)");
	
	// afficher et calculer l'histogramme sur 32 classes
	Histogramme H_F(I_F,32);
	num_f = H_F.afficher_0_1();
	changer_titre_fenetre_graphique(num_f, "Histo. de F(I)");
	
	// afficher et calculer la moyenne et l'écart-type
	printf("\nStatistiques de l'image F(I) :\n");
	statistiques(I_F);	
	I_F.sauvegarder("exo2-1.png");
}		

///////////////////////////////////////////////////////////////////////////////
// EXO 2 ET 3 : exemple de traitement d'une image
float f1(float x)
{
	return pow(x,0.5); // correction gamma avec alpha=0.5
}

void exemple1()
{
	ImageCouleurF I("oiseau.png");
	traitement_image_gris(I, Fonction(f1));
}

float f4(float x)
{
	return pow(x,1.6); // correction gamma avec alpha=1.6
}

void exemple4()
{
	ImageGrisF I("arbre-brume.png");
	traitement_image_gris(I, Fonction(f4));
}


float f2(float x)
{
	return 1.5*x; // éclaircissement x 1.5
}

float f3(float x)
{
	return 0.9*x; // assombrissement x0.9
}

float f5(float x)
{
	if (x <=0.15) return 0.0;
	else if (x <= 0.3) return 0.15;
	return x; 
}

void exemple2()
{
	ImageCouleurF I("plante.png");
	traitement_image_gris(I, Fonction(f2));
}

void exemple5()
{
	ImageCouleurF I("belledonne.pgm");
	traitement_image_gris(I, Fonction(f5));
}

void exemple3()
{
	ImageCouleurF I("amphi-weil.png");
	traitement_image_gris(I, Fonction(f3));
}

///////////////////////////////////////////////////////////////////////////////

int main()
{
	//exemple1();//oiseau
	//exemple2();//plante
	//exemple3();//amphi
	//exemple4();//brume
	exemple5();
	
	// en attente de taper une touche
	printf("Taper un caractere au clavier pour continuer");
	getchar();

	return 0;
}
