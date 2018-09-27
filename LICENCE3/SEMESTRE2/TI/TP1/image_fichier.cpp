//////////////////////////////////////////////////////////////////////////////
// chargement / sauvegarde sur fichier
// 
// N. SZAFRAN - UFR-IM2AG - 2015
//////////////////////////////////////////////////////////////////////////////

#include "lib_image.hpp"
#include <cstdio>
#include <cmath>

// récupère la taille en octets du fichier nommé nom_f
unsigned long taille_fichier(const char *nom_f)
{
	FILE *f;
	unsigned long res;
	
	f = fopen(nom_f, "r");
	fseek(f, 0L, SEEK_END);
	res = ftell(f);
	fclose(f);
	return res;
}

// calculer et afficher les écarts moyen et max entre I1 et I2
void infos_ecart(ImageCouleurF &I1, ImageCouleurF &I2)
{
	UINT N = I1.nb_pixels();
	float e_moy, e_max;
	
	e_moy = e_max = 0.0;
	for (UINT i=0; i<N; i++)
	{
		RGBF v1 = I1(i); 
		RGBF v2 = I2(i); 
		float e;
		
		// composante R
		e = v1.r<v2.r ? v2.r-v1.r : v1.r-v2.r;
		e_moy += e;
		if (e_max<e) e_max=e;
		
		// composante G
		e = v1.g<v2.g ? v2.g-v1.g : v1.g-v2.g;
		e_moy += e;
		if (e_max<e) e_max=e;
		
		// composante B
		e = v1.b<v2.b ? v2.b-v1.b : v1.b-v2.b;
		e_moy += e;
		if (e_max<e) e_max=e;
	}	
	e_moy /= 3.0*(float)N;
	
	printf("Ecart moyen : %10.7f\n", e_moy);
	printf("Ecart maxi. : %10.7f\n", e_max);
}

int main()
{
	char nom1[] = "bastille.ppm";
	char nom2[] = "bastille.png";
	
	// créer l'image I1 à partir du fichier nom1
	ImageCouleurF I1(nom1);
	ImageCouleurF I9("chat.ppm");
	ImageCouleurF I10("uga.ppm");
	ImageCouleurF I11("alea_couleur.ppm");
	
	UINT8 q;
	// sauvegarder dans un autre format
	I1.sauvegarder(nom2);
	
	q=100;
	I1.sauvegarder_jpeg("bastille100.jpg",q);
	I9.sauvegarder_jpeg("chat100.jpg",q);
	I10.sauvegarder_jpeg("uga100.jpg",q);
	I11.sauvegarder_jpeg("alea_couleur100.jpg",q);
	q=90;
	I1.sauvegarder_jpeg("bastille90.png",q);
	I9.sauvegarder_jpeg("chat90.jpg",q);
	I10.sauvegarder_jpeg("uga90.jpg",q);
	I11.sauvegarder_jpeg("alea_couleur90.jpg",q);
	q=75;
	I1.sauvegarder_jpeg("bastille75.jpg",q);
	I9.sauvegarder_jpeg("chat75.jpg",q);
	I10.sauvegarder_jpeg("uga75.jpg",q);
	I11.sauvegarder_jpeg("alea_couleur75.jpg",q);
	q=50;
	I1.sauvegarder_jpeg("bastille50.jpg",q);
	I9.sauvegarder_jpeg("chat50.jpg",q);
	I10.sauvegarder_jpeg("uga50.jpg",q);
	I11.sauvegarder_jpeg("alea_couleur50.jpg",q);
	q=10;
	I1.sauvegarder_jpeg("bastille10.png",q);
	I9.sauvegarder_jpeg("chat10.jpg",q);
	I10.sauvegarder_jpeg("uga10.jpg",q);
	I11.sauvegarder_jpeg("alea_couleur10.jpg",q);
	q=1;
	I1.sauvegarder_jpeg("bastille1.jpg",q);
	I9.sauvegarder_jpeg("chat1.jpg",q);
	I10.sauvegarder_jpeg("uga1.jpg",q);
	I11.sauvegarder_jpeg("alea_couleur1.jpg",q);
	
	// afficher la taille des deux fichiers images
	printf("Taille du fichier %s : %ld\n", nom1, taille_fichier(nom1));
	printf("Taille du fichier %s : %ld\n", nom2, taille_fichier(nom2));
	printf(" taux de compression pour le fichier %s : %f\n", 
	  nom2, 3.0*I1.nb_pixels()/taille_fichier(nom2));
	
	// créer l'image I2 à partir du fichier nom2
	ImageCouleurF I2(nom2);
	
	
	// calculer et afficher les écarts moyen et max entre I1 et I2
	infos_ecart(I1, I2);
	
	// afficher les deux images
	I1.afficher();

	// attente de taper une touche
	printf("Taper un caractere au clavier pour continuer");
	getchar();

	return 0;
}
