///////////////////////////////////////////////////////////////////////////////
// Filtres détecteurs de contour
// 
// N. SZAFRAN - UFR-IM2AG 
///////////////////////////////////////////////////////////////////////////////

#include "lib_image.hpp"
#include <cstdio>
#include <cstdlib>
#include <cmath>

//////////////////////////////////////////////////////////////////////////////
// operations sur les images ImageGrisF
//////////////////////////////////////////////////////////////////////////////

// fonction valeur absolue
ImageGrisF abs(ImageGrisF &I1)
{
	UINT L = I1.largeur();
	UINT H = I1.hauteur();
	ImageGrisF Ires(L,H);
	for (UINT i=0; i<L*H; i++)
		Ires(i) = fabsf(I1(i));
	return Ires;
}

// fonction puissance
ImageGrisF pow(ImageGrisF &I1, float p)
{
	UINT L = I1.largeur();
	UINT H = I1.hauteur();
	ImageGrisF Ires(L,H);
	for (UINT i=0; i<L*H; i++)
		Ires(i) = powf(I1(i), p);
	return Ires;
}

// fonction racine carree
ImageGrisF sqrt(ImageGrisF &I1)
{
	UINT L = I1.largeur();
	UINT H = I1.hauteur();
	ImageGrisF Ires(L,H);
	for (UINT i=0; i<L*H; i++)
		Ires(i) = sqrt(I1(i));
	return Ires;
}

// operateur +
ImageGrisF operator+(ImageGrisF &I1, ImageGrisF &I2)
{
	UINT L = I1.largeur();
	UINT H = I1.hauteur();
	ImageGrisF Ires(L,H);
	for (UINT i=0; i<L*H; i++)
		Ires(i) = I1(i)+I2(i);
	return Ires;
}

// fonction max
ImageGrisF max(ImageGrisF &I1, ImageGrisF &I2)
{
	UINT L = I1.largeur();
	UINT H = I1.hauteur();
	ImageGrisF Ires(L,H);
	for (UINT i=0; i<L*H; i++)
		Ires(i) = I1(i)<I2(i) ? I2(i) : I1(i);
	return Ires;
}

// operateur +=
ImageGrisF operator+=(ImageGrisF &I1, ImageGrisF &I2)
{
	UINT L = I1.largeur();
	UINT H = I1.hauteur();
	for (UINT i=0; i<L*H; i++)
		I1(i) += I2(i);
	return I1;
}

// operateur * par un réel
ImageGrisF operator*(ImageGrisF &I1, float a)
{
	UINT L = I1.largeur();
	UINT H = I1.hauteur();
	ImageGrisF Ires(L,H);
	for (UINT i=0; i<L*H; i++)
		Ires(i) = I1(i)*a;
	return Ires;
}

ImageGrisF operator*(float a, ImageGrisF &I1)
{
	UINT L = I1.largeur();
	UINT H = I1.hauteur();
	ImageGrisF Ires(L,H);
	for (UINT i=0; i<L*H; i++)
		Ires(i) = I1(i)*a;
	return Ires;
}

// operateur *= par un réel
ImageGrisF operator*=(ImageGrisF &I1, float a)
{
	UINT L = I1.largeur();
	UINT H = I1.hauteur();
	for (UINT i=0; i<L*H; i++)
		I1(i) *= a;
	return I1;
}

// seuillage automatique par la méthode de Otsu
// Entrée : I = image en échelle de gris
// La fonction renvoie l'image seuillée avec chaque pixel soit noir, soit blanc
ImageGrisF seuillage_automatique(ImageGrisF &I)
{
	// nb de classes pour l'histogramme
	UINT nc=256;
	
	// nombre de pixels de I
	UINT N = I.nb_pixels();
	
	// calculer l'histogramme de I avec nc classes
	Histogramme H(I,nc); 
	
	// calculer le seuil optimal
    float sum = 0.0;
    for (UINT i = 1; i < nc; i++)
        sum += i * H.classe(i);
    float sumB = 0.0;
    float wB = 0.0;
    float wF = 0.0;
    float mB;
    float mF;
    float max = 0.0;
    float between = 0.0;
    float threshold1 = 0.0;
    float threshold2 = 0.0;
    for (UINT i = 0; i < 256; i++) 
	{
        wB += H.classe(i);
        if (wB == 0.0)
            continue;
        wF = N - wB;
        if (wF == 0.0)
            break;
        sumB += i * H.classe(i);
        mB = sumB / wB;
        mF = (sum - sumB) / wF;
        between = wB * wF * (mB - mF)*(mB - mF);
        if ( between >= max ) 
		{
            threshold1 = i;
            if ( between > max ) 
			{
                threshold2 = i;
            }
            max = between;            
        }
    }
    float s = ( threshold1 + threshold2 ) / 2.0;

	// calcul de l'image seuillée
	float seuil = (float)s/(float)nc;
// printf("Seuillage methode de Otsu - seuil = %f\n", seuil); 
	ImageGrisF Is(I);
	for (UINT i=0; i<N; i++)
	{
		Is(i) = Is(i)<seuil ? 0.0 : 1.0;
	}
	
	return Is;
}

//////////////////////////////////////////////////////////////////////////////
// classe FiltreLineaire :
//   définition de filtres linéaires
//   trois types de filtres prédéfinis :
//     FiltreLineaire::Moyenne 
//     FiltreLineaire::Chapeau 
//     FiltreLineaire::Gaussien
//////////////////////////////////////////////////////////////////////////////
class FiltreLineaire
{
	int n; // taille du filtre --> dimension (2n+1) * (2n+1)
	float *T; // tableau contenant les valeurs du filtre
	
	// initialiser le tableau T avec les 9 valeurs a0*r,...,a8*r
	void init_tab9(float a0, float a1, float a2
	             , float a3, float a4, float a5
	             , float a6, float a7, float a8, float r = 1.0)
	{
		T[0] = a0*r; T[1] = a1*r; T[2] = a2*r; 
		T[3] = a3*r; T[4] = a4*r; T[5] = a5*r; 
		T[6] = a6*r; T[7] = a7*r; T[8] = a8*r; 
	}

	// creer un filtre moyenne de taille n
	void filtre_moyenne()
	{
		UINT p=0;
		float v = 1.0/(float)((2*n+1)*(2*n+1));
		for (int i=-n; i<=n; i++)
		for (int j=-n; j<=n; j++)
		{
			T[p++] = v;
		}
	}

	// creer un filtre chapeau de taille n
	void filtre_chapeau()
	{
		UINT p=0;
		float v = 1.0/powf((float)(n+1),4.0);
		for (int i=-n; i<=n; i++)
		for (int j=-n; j<=n; j++)
		{
			T[p++] = v*(n+1-fabsf(i))*(n+1-fabsf(j));
		}
	}

	// creer un filtre gaussien de taille n
	void filtre_gaussien()
	{
		UINT p=0;
		float s = 0.0;
		float a = (float)(n*n)/3.0;
		for (int i=-n; i<=n; i++)
		for (int j=-n; j<=n; j++)
		{
			s += T[p++] = exp((float)(-i*i-j*j)/a);
		}
		
		// normalisation
		for (p=0; p<(2*n+1)*(2*n+1); p++)
			T[p] /= s;
	}

public :
	static const int Moyenne  = 1;
	static const int Chapeau  = 2;
	static const int Gaussien = 3;
	static const int DeriveeN  = 10;
	static const int DeriveeS  = 11;
	static const int DeriveeE  = 12;
	static const int DeriveeW  = 13;
	static const int DeriveeNE = 14;
	static const int DeriveeSE = 15;
	static const int DeriveeNW = 16;
	static const int DeriveeSW = 17;
	static const int PrewittN_S   = 20;
	static const int PrewittE_W   = 21;
	static const int PrewittNE_SW = 22;
	static const int PrewittNW_SE = 23;
	static const int SobelN_S   = 30;
	static const int SobelE_W   = 31;
	static const int SobelNE_SW = 32;
	static const int SobelNW_SE = 33;
	
	// crée un filtre de taille 0
	FiltreLineaire()
	{
		n = 0;
		T = new float[1];
		T[0] = 1.0;
	}
	
	// crée un filtre de taille n0 suivant le type t
	FiltreLineaire(int n0, int type_filtre)
	{
		if (n0<0) n0=-n0;
		n = n0;
		UINT N = (2*n+1)*(2*n+1);
		T = new float[N];
		switch (type_filtre)
		{
			case Chapeau  : filtre_chapeau();  break;
			case Gaussien : filtre_gaussien(); break;
			default       : filtre_moyenne();  break;
		}
	}
	
	// crée un filtre de taille 1 (3x3) suivant le type t
	FiltreLineaire(int type_filtre)
	{
		n = 1;
		UINT N = 9;
		T = new float[N];
		float a=1;
		float b=2;

		switch (type_filtre)
		{
			case DeriveeN  :
				init_tab9( 0,-1, 0, 0, 1, 0, 0, 0, 0); break;
			case DeriveeS  :
				init_tab9( 0, 0, 0, 0, 1, 0, 0,-1, 0); break;
			case DeriveeE  :
				init_tab9( 0, 0, 0, 0, 1,-1, 0, 0, 0); break;
			case DeriveeW  :
				init_tab9( 0, 0, 0,-1, 1, 0, 0, 0, 0); break;
			case DeriveeNE :
				init_tab9( 0, 0, 0, 0, 1, 0,-1, 0, 0); break;
			case DeriveeSE :
				init_tab9( 0, 0, 0, 0, 1, 0, 0, 0,-1); break;
			case DeriveeNW :
				init_tab9(-1, 0, 0, 0, 1, 0, 0, 0, 0); break;
			case DeriveeSW :
				init_tab9( 0, 0,-1, 0, 1, 0, 0, 0, 0); break;

// A COMPLETER POUR IMPLEMENTER LES FILTRES DIRECTIONNELS
// DE SOBEL ET PREWITT	
			case SobelN_S :
				init_tab9(0, -a*(1.0/(a+2.0)), 0, 0, 0, 0, 0, a*(1.0/(a+2.0)), 0);break;
			case SobelNE_SW :
				init_tab9(-1.0*(1.0/(a+2.0)), 0, 0, 0, 0, 0, 0, 0, 1.0*(1.0/(a+2.0)));break;
			case SobelNW_SE :
				init_tab9(0, 0, -1.0*(1.0/(a+2.0)), 0, 0, 0, 1.0*(1.0/(a+2.0)), 0, 0);break;
			case PrewittE_W :
				init_tab9(0, 0, 0, -b*(1.0/(b+2.0)), 0, b*(1.0/(b+2.0)), 0, 0, 0);break;
			case PrewittNE_SW :
				init_tab9(-1.0*(1.0/(b+2.0)), 0, 0, 0, 0, 0, 0, 0, 1.0*(1.0/(b+2.0)));break;
			case PrewittNW_SE :
				init_tab9(0, 0, -1.0*(1.0/(b+2.0)), 0, 0, 0, 1.0*(1.0/(b+2.0)), 0, 0);break;
			case Chapeau  : filtre_chapeau();  break;
			case Gaussien : filtre_gaussien(); break;
			default       : filtre_moyenne();  break;
		}
	}
	
	// constructeur par copie
	FiltreLineaire(FiltreLineaire &K)
	{
		n = K.n;
		UINT N = (2*n+1)*(2*n+1);
		T = new float[N];
		for (UINT i=0; i<N; i++)
			T[i] = K.T[i];
	}
	
	// affectation
	FiltreLineaire& operator=(const FiltreLineaire &K)
	{
		if (this != &K)
		{
			delete[] T;
			n = K.n;
			UINT N = (2*n+1)*(2*n+1);
			T = new float[N];
			for (UINT i=0; i<N; i++)
				T[i] = K.T[i];
		}
		return *this;
	}
	
	// destructeur
	~FiltreLineaire()
	{
		delete[] T;
	}
	
	// acces à la valeur d'indice (i,j), -n <= i,j <= n
	float operator()(int i, int j)
	{
		if (-i>n || -j>n || i>n || j>n) return 0.0;
		return T[(i+n)+(2*n+1)*(j+n)];
	}
	
	// renvoie la taille du filtre
	UINT taille() { return n; }

	// renvoie la dimension du filtre
	UINT dimension() { return 2*n+1; }

}; // class FiltreLineaire


// calculer l'image résultat de la convolution de 
// l'image I par le filtre linéaire de noyau K
// Entrée : I = image de type ImageGrisF
//          K = filtre linéaire
// La fonction renvoie une image de meme dimension que I
ImageGrisF convolution(ImageGrisF& I, FiltreLineaire K) 
{
	UINT L = I.largeur();
	UINT H = I.hauteur();
	ImageGrisF R(L,H);
	int n = K.taille();
	
	for (int x=0; x<L; x++)
	for (int y=0; y<H; y++)
	{
		float s = 0.0;
		int k=0;
		for (int i=-n; i<=n; i++)
		for (int j=-n; j<=n; j++)
		{
			int xr = x+i;
			int yr = y+j;
			if (xr<0)  xr = 0;
			if (xr>=L) xr = L-1;
			if (yr<0)  yr = 0;
			if (yr>=H) yr = H-1;
			s = s+I(xr,yr)*K(i,j);
		}
		R(x,y) = s;
	}
	
	return R;
}

// appliquer à l'image I le filtre dérivée à l'image I
// méthode 1
ImageGrisF filtre_derivee_methode1(ImageGrisF &I)
{
	UINT L = I.largeur();
	UINT H = I.hauteur();
	
	ImageGrisF     I1, I2, I3, I4, I5, I6, I7, I8;
	FiltreLineaire K1, K2, K3, K4, K5, K6, K7, K8;
	
	// appliquer le dégradé N
	K1 = FiltreLineaire(FiltreLineaire::DeriveeN);
	I1 = convolution(I,K1);  
	
	// appliquer le dégradé S
	K2 = FiltreLineaire(FiltreLineaire::DeriveeS);
	I2 = convolution(I,K2);  
	
	// appliquer le dégradé E
	K3 = FiltreLineaire(FiltreLineaire::DeriveeE);
	I3 = convolution(I,K3);  
	
	// appliquer le dégradé W
	K4 = FiltreLineaire(FiltreLineaire::DeriveeW);
	I4 = convolution(I,K4);  
	
	// appliquer le dégradé NE
	K5 = FiltreLineaire(FiltreLineaire::DeriveeNE);
	I5 = convolution(I,K5);  
	
	// appliquer le dégradé SE
	K6 = FiltreLineaire(FiltreLineaire::DeriveeSE);
	I6 = convolution(I,K6);  
	
	// appliquer le dégradé NW
	K7 = FiltreLineaire(FiltreLineaire::DeriveeNW);
	I7 = convolution(I,K7);  
	
	// appliquer le dégradé SW
	K8 = FiltreLineaire(FiltreLineaire::DeriveeSW);
	I8 = convolution(I,K8);  
	
	// calcul de l'image finale 
	//                  1
	// methode 1 : IR = - somme(k=1..8, |Ik|) 
	//                  8
	
	// calcul de la somme
	ImageGrisF IR(L,H,0.0);
	I1 = abs(I1); IR += I1; 
	I2 = abs(I2); IR += I2; 
	I3 = abs(I3); IR += I3; 
	I4 = abs(I4); IR += I4; 
	I5 = abs(I5); IR += I5; 
	I6 = abs(I6); IR += I6; 
	I7 = abs(I7); IR += I7; 
	I8 = abs(I8); IR += I8; 
	
	// multiplier par 1/8
	IR *= 1.0/8.0;

	return IR;
}
//-------------------------------------------------------------
//AJOUTE : méthode 2
ImageGrisF filtre_derivee_methode2(ImageGrisF &I)
{
	UINT L = I.largeur();
	UINT H = I.hauteur();
	
	ImageGrisF     I1, I2, I3, I4, I5, I6, I7, I8;
	FiltreLineaire K1, K2, K3, K4, K5, K6, K7, K8;
	
	// appliquer le dégradé N
	K1 = FiltreLineaire(FiltreLineaire::DeriveeN);
	I1 = convolution(I,K1);  
	
	// appliquer le dégradé S
	K2 = FiltreLineaire(FiltreLineaire::DeriveeS);
	I2 = convolution(I,K2);  
	
	// appliquer le dégradé E
	K3 = FiltreLineaire(FiltreLineaire::DeriveeE);
	I3 = convolution(I,K3);  
	
	// appliquer le dégradé W
	K4 = FiltreLineaire(FiltreLineaire::DeriveeW);
	I4 = convolution(I,K4);  
	
	// appliquer le dégradé NE
	K5 = FiltreLineaire(FiltreLineaire::DeriveeNE);
	I5 = convolution(I,K5);  
	
	// appliquer le dégradé SE
	K6 = FiltreLineaire(FiltreLineaire::DeriveeSE);
	I6 = convolution(I,K6);  
	
	// appliquer le dégradé NW
	K7 = FiltreLineaire(FiltreLineaire::DeriveeNW);
	I7 = convolution(I,K7);  
	
	// appliquer le dégradé SW
	K8 = FiltreLineaire(FiltreLineaire::DeriveeSW);
	I8 = convolution(I,K8);  
	
	// calcul de l'image finale 
	//                  
	// methode 2 : IR = max(|I1|, |I2| ... |Ik|) 
	//                  
	
	// calcul de la somme
	ImageGrisF IR(L,H,0.0);
	I1 = abs(I1); IR += I1;
	I2 = abs(I2); IR += I2; 
	I3 = abs(I3); IR += I3; 
	I4 = abs(I4); IR += I4; 
	I5 = abs(I5); IR += I5; 
	I6 = abs(I6); IR += I6; 
	I7 = abs(I7); IR += I7; 
	I8 = abs(I8); IR += I8;
	IR = max(I1,I2);
	IR = max(IR,I3);
	IR = max(IR,I4);
	IR = max(IR,I5);
	IR = max(IR,I6);
	IR = max(IR,I7);
	IR = max(IR,I8);
	
	return IR;
}

//-------------------------------------------------------------
//AJOUTEE
ImageGrisF filtre_Sobel_Prewitt(ImageGrisF &I)
{
	UINT L = I.largeur();
	UINT H = I.hauteur();
	
	ImageGrisF     I1, I2, I3;
	FiltreLineaire K1, K2, K3;
	
	// appliquer le dégradé N
	K1 = FiltreLineaire(FiltreLineaire::SobelN_S);
	I1 = convolution(I,K1); 

	// appliquer le dégradé N
	K2 = FiltreLineaire(FiltreLineaire::SobelNE_SW);
	I2 = convolution(I,K2); 

	// appliquer le dégradé N
	K3 = FiltreLineaire(FiltreLineaire::SobelNW_SE);
	I3 = convolution(I,K3);


	// calcul de la somme
	ImageGrisF IR(L,H,0.0);
	ImageGrisF IS(L,H,0.0);
	I1 = abs(I1);
	I2 = abs(I2);
	I3 = abs(I3);
	IS= max(I1,I2);
	IS= max(IS,I3);

	// appliquer le dégradé N
	K1 = FiltreLineaire(FiltreLineaire::PrewittE_W);
	I1 = convolution(I,K1); 

	// appliquer le dégradé N
	K2 = FiltreLineaire(FiltreLineaire::PrewittNE_SW);
	I2 = convolution(I,K2); 

	// appliquer le dégradé N
	K3 = FiltreLineaire(FiltreLineaire::PrewittNW_SE);
	I3 = convolution(I,K3);
	
	ImageGrisF IP(L,H,0.0);
	I1 = abs(I1);
	I2 = abs(I2);
	I3 = abs(I3);
	IP= max(I1,I2);
	IP= max(IP,I3);
	
	IR = max(IS,IP);
	
	return IR;
}
//-------------------------------------------------------------

int main(int argc, char *argv[])
{
	int num_f;
	ImageGrisF I;
	ImageGrisF I_F;
	ImageGrisF I_S;
	ImageGrisF I_F3;
	ImageGrisF I_S3;
	ImageGrisF I_F2;
	ImageGrisF I_S2;
	
	// charger une image en niveaux de gris
	if (argc<2)
		I = ImageGrisF("maison.png");
	else
		I = ImageGrisF(argv[1]);
	num_f = I.afficher();
	changer_titre_fenetre_graphique(num_f, "Image I");		

	// appliquer le filtre derivee - méthode 1
	I_F = filtre_derivee_methode1(I);
	num_f = I_F.afficher();
	changer_titre_fenetre_graphique(num_f, "Image I_F");

	// appliquer le filtre derivee - méthode 2
	I_F2 = filtre_derivee_methode2(I);
	num_f = I_F2.afficher();
	changer_titre_fenetre_graphique(num_f, "Image I_F2");			
	
	// effectuer un seuillage automatique sur l'image filtree I_F
	I_S = seuillage_automatique(I_F);
	num_f = I_S.afficher();
	changer_titre_fenetre_graphique(num_f, "Image I_S");			
	
	// effectuer un seuillage automatique sur l'image filtree I_F2
	I_S2 = seuillage_automatique(I_F2);
	num_f = I_S2.afficher();
	changer_titre_fenetre_graphique(num_f, "Image I_S2");			
	
	// appliquer le filtre sobel prewitt
	I_F3 = filtre_Sobel_Prewitt(I);
	num_f = I_F3.afficher();
	changer_titre_fenetre_graphique(num_f, "Image I_F3");			
	
	// effectuer un seuillage automatique sur l'image filtree I_F3
	I_S3 = seuillage_automatique(I_F3);
	num_f = I_S3.afficher();
	changer_titre_fenetre_graphique(num_f, "Image I_S3");	 
	 
	// en attente de taper une touche
	printf("Taper un caractere au clavier pour continuer");
	getchar();

	return 0;
}
