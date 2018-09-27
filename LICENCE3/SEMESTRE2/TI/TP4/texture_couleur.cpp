/* 
	Utilisation de texture + couleur 

    touches x,y,z,X,Y,Z : rotation de la scene autour des axes x,y,z

	touches +/- : zoom + et -
	
	compilation :
	
	  gcc texture_couleur.c -lglut -lGLU -lGL -lm -o texture_couleur
 */

#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

GLdouble delta_rot=2.0;  /* modification d'angle */

/* position de la camera */
GLdouble cameraX=-5.0, cameraY=0.0, cameraZ=0.0;

GLdouble ratio_fenetre; /* rapport largeur sur hauteur */

////////////////////////////////////////////////////////////////////////////
// gestion des textures 
////////////////////////////////////////////////////////////////////////////

/////////////////
// classe Texture 
class Texture
{
	// dimension de l'image 
	GLint L,H;	
	
	// nb de bytes du tableau image
	GLint N;
	
	// image de la texture 
	// doit etre un tableau de dimension Lligne * h
	// avec Lligne : plus petit entier multiple de 4 supérieur ou égal à 3*L
	GLubyte *image;
	
	// numéro identificateur de la texture 
	GLuint id;

	// creation de la texture correspondant à l'image PPM contenu
	//   dans le tableau t_image_ppm
	//   Entrée : t_image_ppm = tableau de 3*L*H GLubyte (unsigned char)
	//            le pixel en ligne i et colonne j est donné par :
	//			  t_image_ppm[3*(L*j+i)  ] (composante R entre 0 et 255)
	//			  t_image_ppm[3*(L*j+i)+1] (composante G entre 0 et 255)
	//			  t_image_ppm[3*(L*j+i)+2] (composante B entre 0 et 255)
	//			  avec 0<=i<H et 0<=j<L
	//			L,H : dimensions de l'image
	//   Sortie : structure correspondante
	void init_texture()
	{ 	
		
		// creation de la texture 
		// genere un numero de texture 
		glGenTextures(1,&id); 	
		// selectionne ce numero 
		glBindTexture(GL_TEXTURE_2D,id); 	
		
		glTexImage2D (
			GL_TEXTURE_2D, 	// Type : texture 2D 
			0,              // Mipmap : aucun 
			3, 				// ModeRGB (3 composantes) 
			L, 	 			// Largeur 
			H, 				// Hauteur 
			0, 				// Largeur du bord : 0 
			GL_RGB,  		// Format : RGBA 
			GL_UNSIGNED_BYTE, 	// composante de type UNSIGNED_BYTE 
			image   		// buffer de l'image de texture 
		); 	
		
		// definit le type de texture (2D) et comment elle est appliquee 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	} 

	// lecture de l'image PPM au format brut RGB - unsigned char 
	//       (valeurs entre 0 et 255)
	//  la fonction renvoie 1 si l'image a pu etre lue et 
	//  les dimensions sont correctes, 0 sinon
	int ppm2rgb(const char *nom_f)
	{
		GLint nb_bytes_ligne, i,j,k;
		char *ligne;
		size_t nligne;
		FILE *f;
		
		// ouverture du fichier 
		f = fopen(nom_f, "r");
		if (f == (FILE*)NULL)
		{
			fprintf(stderr, "ppm2rgb\n");
			fprintf(stderr, " le fichier %s ne peut etre ouvert\n", nom_f);
			return 0;	
		}
		
		// lecture ligne 1 et test 
		ligne=NULL;
		getline(&ligne, &nligne, f);
		if (ligne[0]!='P' || ligne[1]!='6')
		{
			fprintf(stderr, "ppm2rgb\n");
			fprintf(stderr, " format fichier incorrect\n");
			fclose(f);
			free(ligne);
			return 0;	
		}
		free(ligne);
		
		// lecture ligne 2 
		ligne=NULL;
		getline(&ligne, &nligne, f);
		if (ligne[0]=='#')
		{
			// ligne de commentaire -> lire la ligne suivante 
			free(ligne);
			ligne=NULL;
			getline(&ligne, &nligne, f);
		}
		sscanf(ligne,"%d %d", &L, &H);
		if (L<1 || H<1)
		{
			fprintf(stderr, "ppm2rgb\n");
			fprintf(stderr, " dimensions incorrectes (%d,%d)\n", L, H);
			fclose(f);
			free(ligne);
			return 0;	
		}
		free(ligne);
		
		// lecture ligne 3 - cas PGM ou PPM 
		ligne=NULL;
		getline(&ligne, &nligne, f);
		sscanf(ligne, "%d", &i);
		if (i!=255)
		{
			fprintf(stderr, "ppm2rgb\n");
			fprintf(stderr, " N (%d) incorrect, 255 attendu\n",i);
			fclose(f);
			free(ligne);
			return 0;	
		}
		free(ligne);
	
		// allocation de la mémoire nécessaire 
		nb_bytes_ligne = 4*((3*L+3)/4);
		N = nb_bytes_ligne*H;
		image = new GLubyte[N];
		if (!image)
		{
			fprintf(stderr, "ppm2rgb\n");
			fprintf(stderr, " allocation memoire impossible\n");
			fclose(f);
			return 0;
		}
		
		for (j=0; j<H; j++)
		{
			k=j*nb_bytes_ligne;
			// lire les L pixels de la ligne 
			for (i=0; i<L; i++)
			{
				unsigned char cRGB[3];
				fread(cRGB, 3, 1, f);
				image[k++] = cRGB[0]; 
				image[k++] = cRGB[1]; 
				image[k++] = cRGB[2]; 
			}
		}
		
		fclose(f);
		return 1;
	} 

	// créer une texture à partir d'une image de dimensions
	// L0 x H0 contenue dans le tableau Timage
	void creer_texture(GLint L0, GLint H0, GLubyte *Timage)
	{
		L = L0;
		H = H0;
		N = 4*((3*L+3)/4)*H;
		image = new GLubyte[N];
		for (int i=0; i<N; i++)
			image[i] = Timage[i];
	}

	// créer une texture basique (noir-blanc/blac-noir)
	void creer_texture_basique()
	{
		GLubyte image_basique[] = {
		    0,  0,  0   , 255,255,255 , 0,0 , // ligne 1 
		    255,255,255 ,   0,  0,  0 , 0,0}; // ligne 2
		L = 2;
		H = 2;
		N = 4*((3*L+3)/4)*H;
		image = new GLubyte[N];
		for (int i=0; i<N; i++)
			image[i] = image_basique[i];
	}

public:
	////////////////
	// constructeurs
	
	// creer une texture basique
	Texture()
	{
		creer_texture_basique();
		init_texture();
	}
	
	// créer une texture OpenGL à partir d'un fichier image au format PPM brut
	Texture(const char* nom_f)
	{
		image = NULL;
		if (ppm2rgb(nom_f)==0)
		{
			if (image) delete[] image; // supprimer le tableau image 
			
			// créer une texture basique
			creer_texture_basique();
		}
		init_texture();
	}
	
	// créer une texture OpenGL à partir d'une image de dimensions
	// L0 x H0 contenue dans le tableau Timage
	Texture(GLint L0, GLint H0, GLubyte *Timage)
	{
		creer_texture(L0, H0, Timage);
		init_texture();
	}
	
	// création par copie
	Texture(const Texture &t)
	{
		creer_texture(t.L, t.H, t.image);
		init_texture();
	}
	
	// surcharge affectation
	Texture &operator=(const Texture &t)
	{
		if (this != &t)
		{
			if (image) delete[] image; // supprimer le tableau image 
			
			creer_texture(t.L, t.H, t.image);
			init_texture();
		}
		return *this;
	}
	
	// renvoie le numéro de la texture
	int numero()
	{
		return id;
	}
	
	// destructeur
	~Texture()
	{
		if (image)
		delete[] image;
	}
};	

// la texture de la sphere
Texture	texture_sphere;

///////////////////////////////////////////////////////////////////////////////
// classe Point en dimension 3
class Point {
public :
	GLdouble c[3]; // les coordonnées
	
	////////////////
	// constructeurs
	
	// créer un point avec les coordonnées (x,y,z)
	Point(GLdouble x=0.0, GLdouble y=0.0, GLdouble z=0.0)
	{
		c[0] = x; c[1] = y; c[2] = z; 
	}
	
	// affectation
	Point& operator=(const Point& p)
	{
		c[0] = p.c[0];
		c[1] = p.c[1];
		c[2] = p.c[2];
		return *this; 
	}
	
	// opérations sur les points
	GLdouble norme()
	{
		return sqrt(c[0]*c[0]+c[1]*c[1]+c[2]*c[2]);
	} 
	
	Point& operator+=(const Point &p)
	{
		c[0] += p.c[0];
		c[1] += p.c[1];
		c[2] += p.c[2];
		return *this;
	}
	
	Point& operator-=(const Point &p)
	{
		c[0] -= p.c[0];
		c[1] -= p.c[1];
		c[2] -= p.c[2];
		return *this;
	}
	
	Point& operator*=(GLdouble r)
	{
		c[0] *= r;
		c[1] *= r;
		c[2] *= r;
		return *this;
	}
	
	Point& operator/=(GLdouble r)
	{
		c[0] /= r;
		c[1] /= r;
		c[2] /= r;
		return *this;
	}
	
	void normalise()
	{
		GLdouble np = norme();
		if (np>0.0)
		{
			c[0] /= np;
			c[1] /= np;
			c[2] /= np;
		}	
	}
	
	friend Point operator+(Point p1, const Point& p2)
	{
		return p1 += p2;
	}
	
	friend Point operator-(Point p1, const Point& p2)
	{
		return p1 -= p2;
	}
	
	friend Point operator*(GLdouble r, Point p)
	{
		return Point(r*p.c[0],r*p.c[1],r*p.c[2]);
	}
	
	friend Point operator*(Point p, GLdouble r)
	{
		return Point(r*p.c[0],r*p.c[1],r*p.c[2]);
	}
	
	friend GLdouble produit_scalaire(Point p1, Point p2)
	{
		return p1.c[0]*p2.c[0]+p1.c[1]*p2.c[1]+p1.c[2]*p2.c[2];
	}
	
	friend Point produit_vectoriel(Point p1, Point p2)
	{
		return Point(
		 p1.c[1]*p2.c[2]-p1.c[2]*p2.c[1],
		 p1.c[2]*p2.c[0]-p1.c[0]*p2.c[2],
		 p1.c[0]*p2.c[1]-p1.c[1]*p2.c[0]);
	}
	
};

#define Vecteur Point
#define CouleurRGB Point
#define GL_COULEUR_RGB(_c) glColor3dv(_c.c)
#define GL_VERTEX(_v)      glVertex3dv(_v.c)
#define GL_TRANSLATE(_v)   glTranslated(_v.c[0],_v.c[1],_v.c[2])

// ratio radian/degre 
GLdouble r_radian_degre = M_PI/180.0;

// la caméra  
// angle d'ouverture de la camera 
GLdouble angle_camera = 45.0;

// position angulaire de l'axe de visee de la camera 
//   par rapport a l'axe Ox 
GLdouble azimut = -45.0;       // azimut
GLdouble delta_angle = 0.5;
GLdouble hCamera = 0.0;    // hauteur 
GLdouble RCamera = 50.0;    // rayon de cylindre
GLdouble delta_h = 0.5;   

// angle de rotation de la scene / axe 0z
GLdouble angle_scene = 0.0;

// couleur de fond 
CouleurRGB couleur_fond(0.0,0.0,0.0);

// direction de lumiere 
GLdouble La=45.0; // azimut de la direction de la lumière 
GLdouble Le=30.0; // elevation de la direction de la lumière 
// direction de la lumière L = ( cos(La)*cos(Le) , sin(La)*cos(Le) , sin(Le) )
Vecteur  Lumiere; 

// boolean pour l'affichage des axes du modelview
bool ind_repere_modelview = true;

// boolean pour l'affichage de la direction de lumiere
bool ind_direction_lumiere = true;

///////////////////////////////////////////////////////////////////////////////
// partie pour la prise en compte du modèle d'illumination de Lambert
///////////////////////////////////////////////////////////////////////////////

// modifie le vecteur normal unitaire N suivant la matrice de modelview
void modif_normale_suivant_matrice_modelview(Vecteur &N)
{
	// la matrice de modelview
	GLdouble M[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, &M[0]); 

	Vecteur NM; // le vecteur modifié
	for (int i=0; i<3; i++)
	{
		NM.c[i] = M[i]*N.c[0]+M[i+4]*N.c[1]+M[i+8]*N.c[2];
	}
	
	// modifier N;
	N = NM;
}

// calcul de la couleur résultante en utilisant le modèle de Lambert 
// en fonction de la normale N, la couleur de l'objet c           
// ainsi que la couleur de fond couleur_fond                               
// et la direction de lumière Lumiere                                  
// Entrée : N = normale orientée    
//          couleur_fond = couleur de fond   
//          c = couleur de l'objet   
CouleurRGB calcul_couleur_modele_lambert
 (Vecteur N, CouleurRGB couleur_fond, CouleurRGB c)
{
	modif_normale_suivant_matrice_modelview(N);
	GLdouble norme_N = N.norme();
	GLdouble l = produit_scalaire(N,Lumiere)/norme_N;
	if (l<0.0) l=0.0;
// 	if (l>1.0) 
// 	{
// 		printf("erreur : l=%f\n", l);
// 		l=1.0;
// 	}
	return (1-l)*couleur_fond + l*c;
}

///////////////////////////////////////////////////////////////////////////////
// les ordres graphiques 
///////////////////////////////////////////////////////////////////////////////

// dessine le repere MODELVIEW (Oxyz)
void dessin_repere_modelview()
{
	// longueur des axes
	GLdouble lAxe = 100.0;
	
	// axe Ox en rouge
	glColor3d(1.0,0.0,0.0);
	glBegin(GL_LINES);
		glVertex3d( 0.0,0.0,0.0);
		glVertex3d(lAxe,0.0,0.0);
	glEnd();
		
	// axe Oy en vert
	glColor3d(0.0,1.0,0.0);
	glBegin(GL_LINES);
		glVertex3d(0.0, 0.0,0.0);
		glVertex3d(0.0,lAxe,0.0);
	glEnd();
		
	// axe Oz en bleu
	glColor3d(0.0,0.0,1.0);
	glBegin(GL_LINES);
		glVertex3d(0.0,0.0, 0.0);
		glVertex3d(0.0,0.0,lAxe);
	glEnd();	
}

// dessine la direction de la lumiere
void dessin_direction_lumiere()
{
	// longueur du vecteur
	GLdouble lV = 100.0;
	#define GL_VERTEX(_v)      glVertex3dv(_v.c)
	glColor3d(0.5,0.5,0.5);
	glBegin(GL_LINES);
		glVertex3d( 0.0,0.0,0.0);
		glVertex3d(lV*Lumiere.c[0],lV*Lumiere.c[1],lV*Lumiere.c[2]);
	glEnd();
}
/* la sphere unite */
#define DIM1 80
#define DIM2 40
void sphere_texturee()
{
	int i,j;
	
	/* active le texturing */
	glEnable(GL_TEXTURE_2D); 	
	
	/* selectionne la texture sphere */
	glBindTexture(GL_TEXTURE_2D,texture_sphere.numero()); 
		
	/* sphere de couleur blanche */
	CouleurRGB couleur_sphere(1.0,1.0,1.0);
	
	
	for (i=0; i<DIM1; i++)
	for (j=0; j<DIM2; j++)
	{
		GLdouble x,y,z;
		GLdouble Nx,Ny,Nz;
		
		GLdouble x1texture = (GLdouble)(i)  /(GLdouble)DIM1;
		GLdouble x2texture = (GLdouble)(i+1)/(GLdouble)DIM1;
		GLdouble y1texture = (GLdouble)(j)  /(GLdouble)DIM2;
		GLdouble y2texture = (GLdouble)(j+1)/(GLdouble)DIM2;
		GLdouble u1 = 2.0*M_PI*x1texture; 
		GLdouble u2 = 2.0*M_PI*x2texture; 
		GLdouble v1 = (y1texture-0.5)*M_PI; 
		GLdouble v2 = (y2texture-0.5)*M_PI; 
		
		GLdouble u = 0.5*(u1+u2);
		GLdouble v = 0.5*(v1+v2);
		Vecteur N = Point(cos(u)*cos(v), cos(u)*sin(v), sin(u) );
		CouleurRGB c = calcul_couleur_modele_lambert(N, couleur_fond, couleur_sphere);
		
		glBegin(GL_QUADS);
		/* sommet 1 */
		Nx = cos(u1)*cos(v1);
		Ny = sin(u1)*cos(v1);
		Nz = sin(v1); 
		x = Nx;
		y = Ny;
		z = Nz; 
		//EXO9
		Point P = Point(x,y,z); 
		N = P;
		c = calcul_couleur_modele_lambert(N, couleur_fond, couleur_sphere);
		GL_COULEUR_RGB(c);
		
		glTexCoord2d(x1texture, 1.0-y1texture);
		GL_VERTEX(P);

		/* sommet 2 */
		Nx = cos(u1)*cos(v2);
		Ny = sin(u1)*cos(v2);
		Nz = sin(v2);
		x = Nx;
		y = Ny;
		z = Nz; 
		//EXO9
		P = Point(x,y,z); 
		N = P;
		c = calcul_couleur_modele_lambert(N, couleur_fond, couleur_sphere);
		GL_COULEUR_RGB(c);
		
		glTexCoord2d(x1texture, 1.0-y2texture);
		GL_VERTEX(P);

		/* sommet 3 */
		Nx = cos(u2)*cos(v2);
		Ny = sin(u2)*cos(v2);
		Nz = sin(v2);
		x = Nx;
		y = Ny;
		z = Nz; 
		//EXO9
		P = Point(x,y,z); 
		N = P;
		c = calcul_couleur_modele_lambert(N, couleur_fond, couleur_sphere);
		GL_COULEUR_RGB(c);
		
		glTexCoord2d(x2texture, 1.0-y2texture);
		GL_VERTEX(P);

		/* sommet 4 */
		Nx = cos(u2)*cos(v1);
		Ny = sin(u2)*cos(v1);
		Nz = sin(v1);
		x = Nx;
		y = Ny;
		z = Nz; 
		//EXO9
		P = Point(x,y,z); 
		N = P;
		c = calcul_couleur_modele_lambert(N, couleur_fond, couleur_sphere);
		GL_COULEUR_RGB(c);
		
		glTexCoord2d(x2texture, 1.0-y1texture);
		GL_VERTEX(P);
		
		glEnd();
	}

	/* désactive le texturing */
	glDisable(GL_TEXTURE_2D); 	
	
}
// dessin d'une sphere de centre C, de rayon r et de couleur cS 
void sphere(Point C, GLdouble r)
{
	// utiliser la sphère unité 
	glPushMatrix();
	GL_TRANSLATE(C);
	glScaled(r,r,r);
	sphere_texturee();
	glPopMatrix();
}

void dessin() {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* effacer la fenetre avec la couleur noire */
	glClearColor(0.0,0.0,0.0,1.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// dessin des axes du modelview
	if (ind_repere_modelview)
	{
		dessin_repere_modelview();
	}
	
	// calcul de la direction de lumiere 
	GLdouble La_radian = La*r_radian_degre;
	GLdouble Le_radian = Le*r_radian_degre;
	Lumiere = Point(cos(La_radian)*cos(Le_radian),sin(La_radian)*cos(Le_radian),sin(Le_radian));

	// dessin de la direction de lumière
	if (ind_direction_lumiere)
	{
		dessin_direction_lumiere();
	}

	/* la sphere */
    	sphere(Point( 0.0, 0.0, 0.0),  10.0);
	
	glutSwapBuffers();
}

void positionnement_camera()
{
	GLdouble Cx,Cy,Cz; // position de la camera 
	GLdouble nx,ny,nz; // direction de visee 
	GLdouble vx,vy,vz; // vecteur de vue vertical 
	GLdouble azimut_radian    = azimut*r_radian_degre;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle_camera, ratio_fenetre, 0.1, 10000.0); 
    
	// calcul de la position de la camera 
	Cx = RCamera*cos(azimut_radian);
	Cy = RCamera*sin(azimut_radian);
	Cz = hCamera;
	
	// calcul de la direction de visee 
	nx = -Cx;
	ny = -Cy;
	nz = -Cz;
	
	// calcul du vecteur vertical de vue 
	vx = -Cx*Cz;
	vy = -Cy*Cz;
	vz =  RCamera*RCamera;
	
	gluLookAt(Cx, Cy, Cz,
	          nx, ny, nz,
		  vx, vy, vz);
}

/* prise en compte d'un redimensionnement de la fenetre */
void redimensionnement(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	ratio_fenetre = (double)(w)/(double)(h);
	positionnement_camera();
}

// gestion des touches clavier 
void clavier(unsigned char key, int x, int y) 
{ 
	switch(key) 
	{ 
	
	// modification de la direction de la lumiere 
	case 'g' :  
	case 'G' :
		La -= delta_angle;
		glutPostRedisplay();
		break; 
	
	case 'h' :  
	case 'H' :
		La += delta_angle;
		glutPostRedisplay();
		break; 
	
	case 'b' :  
	case 'B' :
		Le -= delta_angle;
		glutPostRedisplay();
		break; 
	
	case 'y' :  
	case 'Y' :
		Le += delta_angle;
		glutPostRedisplay();
		break; 
	
	
	// modification de l'azimut 
	case 'j' :  
	case 'J' :
		azimut -= delta_angle;
		positionnement_camera();
		glutPostRedisplay();
		break; 
	
	case 'l' :  
	case 'L' :
		azimut += delta_angle;
		positionnement_camera();
		glutPostRedisplay();
		break; 
	
	// modification de la hauteur de la camera 
	case 'i' :  
	case 'I' :
		hCamera += delta_h;
		positionnement_camera();
		glutPostRedisplay();
		break; 
	
	case 'k' :  
	case 'K' :
		hCamera -= delta_h;
		positionnement_camera();
		glutPostRedisplay();
		break; 
	
	// modification de l'angle d'ouverture de la camera 
	case '-' :
		if (angle_camera<90.0)
			angle_camera /= 1.01;
		else
			angle_camera = 180.0-(180.0-angle_camera)*1.01;
		positionnement_camera();
		glutPostRedisplay();	
		break; 
	case '+' :
		if (angle_camera<90.0)
			angle_camera *= 1.01;
		else
			angle_camera = 180.0-(180.0-angle_camera)/1.01;
		positionnement_camera();
		glutPostRedisplay();	
		break; 
	
	// modification de l'angle de rotation de la scène 
	case 'r' :
		angle_scene -= 1.0;  
		glutPostRedisplay();
		break; 
	case 'R' :
		angle_scene += 1.0;  
		glutPostRedisplay();
		break; 
	
	// affichage ou non des axes du modelview
	case 'a' :
	case 'A' :
		ind_repere_modelview = !ind_repere_modelview;
		glutPostRedisplay();
		break; 
		
	
	// affichage ou non de la direction de lumiere
	case 'z' :
	case 'Z' :
		ind_direction_lumiere = !ind_direction_lumiere;
		glutPostRedisplay();
		break;
		
	// fin du programme 
	case 27 :  
	case 'q' :  
	case 'Q' :  
		exit(0); 
		break; 
	 
	}
}

/* la routine principale */
int main(int argc,char **argv) 
{
	/* initialisation de la librairie GLUT
	   doit etre appelee avant toute autre instruction 
	   de la librairie GLUT */
	glutInit(&argc,argv);
	
	/* creation d'une fenetre graphique 
	   de dimension 500 x 500
	   en simple buffer et mode RGB
	   dont le nom est celui du programme */
	glutInitWindowSize(500,500);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	
	/* activer le z-buffer */
    	glEnable(GL_DEPTH_TEST); 
	
	/* definition de la routine de dessin */
	glutDisplayFunc(dessin);
	/* definition de la routine de redimensionnement de la fenetre */
	glutReshapeFunc(redimensionnement);
	/* definition de la routine de gestion des touches clavier
	   (touches avec code ASCII correspondant) */
	glutKeyboardFunc(clavier);
	
	/* initialisation de la matrice pour le modele */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	/* création de la texture */
	texture_sphere = Texture("texture_mappemonde.ppm");
	
	/* boucle principale */
	glutMainLoop();
	
	return(0);
}
