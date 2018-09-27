///////////////////////////////////////////////////////////////////////////////
// ombrages de face
//    affichage de 2 spheres facettisées s'intersectant
//    
//    les touches 1,2,3,4 permettent de choisir un niveau de facettisation
//      1 : grossier
//      2 : moyen
//      3 : fin
//      4 : tres fin
//    
//    La caméra se déplace sur un cylindre centré à l'origine, d'axe vertical, et
//    de rayon RCamera, et pointe en direction de l'origine.
//    Le déplacement de la caméra est fait en modifiant 
//    soit sa hauteur hCamera, soit sa position angulaire azimut :
//    	   
//    - le changement de la valeur de l'azimut de la camera se fait
//      à l'aide des touches j et l
//    	  
//    - le changement de la valeur de la hauteur de la camera se fait 
//      à l'aide des touches i et k
//    	  
//    - le changement de l'angle d'ouverture de la camera se fait
//      à l'aide des touches - et +
//    
//    - le changement de l'angle de rotation de la scène autour de l'axe 0z
//      se fait à l'aide des touches r et R
//    
//    - l'affichage ou non des axes du modelview
//      se fait à l'aide de la touche a
//    
//    - l'affichage ou non dela direction de lumiere
//      se fait à l'aide de la touche z
//    
//    Une direction d'illumination est définie en coordonnées sphériques 
//    (deux angles, La - azimu - et Le elevation) :  
//    - le changement de la direction de la lumiere se fait
//      à l'aide des touches g,h (azimut) et b,y (elevation)
//       	  
//    La touche q ou ESC met fin a l'execution du programme
// 
//    compilation
//      g++ ombrage_face.cpp -lglut -lGLU -lGL -o ombrage_face_v2
// 	 
//    Nicolas SZAFRAN - UFRIM2AG - 2016
//  
///////////////////////////////////////////////////////////////////////////////

#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>


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

GLdouble ratio_fenetre; // rapport largeur sur hauteur 

// angle de rotation de la scene / axe 0z
GLdouble angle_scene = 0.0;

// couleur de fond 
CouleurRGB couleur_fond(0.0,0.0,0.1);  

// les différentes facettisations possibles  
int dim_facettisation[] = {10,20,40,80};
int facettisation=0;

// direction de lumiere 
GLdouble La=45.0; // azimut de la direction de la lumière 
GLdouble Le=30.0; // elevation de la direction de la lumière 
// direction de la lumière L = ( cos(La)*cos(Le) , sin(La)*cos(Le) , sin(Le) )
Vecteur  Lumiere; 

// discretisation de la surface suivant le parametre u 
int DIM1;
// discretisation de la surface suivant le parametre v 
int DIM2;

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
	
	glColor3d(0.5,0.5,0.5);
	glBegin(GL_LINES);
		glVertex3d( 0.0,0.0,0.0);
		glVertex3d(lV*Lumiere.c[0],lV*Lumiere.c[1],lV*Lumiere.c[2]);
	glEnd();
}

// fonctions définissant la courbe génératrice de la sphère
//               ( r_sphere(u) )
// courbe G(u) = (             )
//               ( z_sphere(u) )
GLdouble r_sphere(GLdouble u)
{
	return cos(u);
}

GLdouble z_sphere(GLdouble u)
{
	return sin(u);
}

// dessin d'une sphere unité (centre (0,0,0) et rayon 1) et de couleur couleur_sphere 
// remarque : pour la sphere unité, la normale extérieure unitaire N
// à un point P de la sphère unité est égale à P
void sphere_unite(CouleurRGB couleur_sphere)
{
	int i,j;
	GLdouble umin = -M_PI/2.0;
	GLdouble umax = +M_PI/2.0;
	GLdouble vmin = 0;
	GLdouble vmax = 2.0*M_PI;
	
	// les faces 
	for (i=0; i<DIM1; i++)
	for (j=0; j<DIM2; j++)
	{
		Point   P11, P12, P21, P22; // les quatre sommets de la face
		Vecteur N11, N12, N21, N22; // les normales correspondantes aux sommets
		Vecteur NF;                 // la normale à la face
		CouleurRGB c;
		
		// face = quadrilatere correspondant au parametrage (u,v) 
		// avec u1 <= u <= u2 et v1 <= v <= v2                    
		GLdouble u1  = umin + (i+0.0)*(umax-umin)/(double)DIM1; 
		GLdouble u2  = umin + (i+1.0)*(umax-umin)/(double)DIM1; 
		GLdouble v1  = vmin + (j+0.0)*(vmax-vmin)/(double)DIM2; 
		GLdouble v2  = vmin + (j+1.0)*(vmax-vmin)/(double)DIM2; 
		
		// les quatre sommets du quadrangle et normales unitaires associées
		// sommet P11 correspondant aux parametres (u1,v1) 
		P11 = Point( r_sphere(u1)*cos(v1) , r_sphere(u1)*sin(v1) , z_sphere(u1) );
		N11 = P11; // normale extérieure correspondante
		
		// sommet P12 correspondant aux parametres (u1,v2) 
		P12 = Point( r_sphere(u1)*cos(v2) , r_sphere(u1)*sin(v2) , z_sphere(u1) );
		N12 = P12; // normale extérieure correspondante
		
		// sommet P21 correspondant aux parametres (u2,v1) 
		P21 = Point( r_sphere(u2)*cos(v1) , r_sphere(u2)*sin(v1) , z_sphere(u2) );
		N21 = P21; // normale extérieure correspondante
		
		// sommet P22 correspondant aux parametres (u2,v2) 
		P22 = Point( r_sphere(u2)*cos(v2) , r_sphere(u2)*sin(v2) , z_sphere(u2) );
		N22 = P22; // normale extérieure correspondante
		
		// calcul de la normale à la face : normale au point de la face
		// correspondant à u=(u1+u2)/2 et v=(v1+v2)/2
		GLdouble u  = 0.5*(u1+u2); 
		GLdouble v  = 0.5*(v1+v2); 
		NF = Point( r_sphere(u)*cos(v) , r_sphere(u)*sin(v) , z_sphere(u) );
		
		// calcul de la couleur de la face
		c = calcul_couleur_modele_lambert(NF, couleur_fond, couleur_sphere);
		GL_COULEUR_RGB(c);
				
		// dessin de la face 
		//EXO4
		glBegin(GL_QUADS);
			c = calcul_couleur_modele_lambert(N11, couleur_fond, couleur_sphere);
			GL_COULEUR_RGB(c);
			GL_VERTEX(P11);
			
			c = calcul_couleur_modele_lambert(N12, couleur_fond, couleur_sphere);
			GL_COULEUR_RGB(c);
			GL_VERTEX(P12);
			
			c = calcul_couleur_modele_lambert(N22, couleur_fond, couleur_sphere);
			GL_COULEUR_RGB(c);
			GL_VERTEX(P22);
			
			c = calcul_couleur_modele_lambert(N21, couleur_fond, couleur_sphere);
			GL_COULEUR_RGB(c);
			GL_VERTEX(P21);
		glEnd();
	}
}

// dessin d'une sphere de centre C, de rayon r et de couleur cS 
void sphere(Point C, GLdouble r, CouleurRGB cS)
{
	// utiliser la sphère unité 
	glPushMatrix();
	GL_TRANSLATE(C);
	glScaled(r,r,r);
	sphere_unite(cS);
	glPopMatrix();
}

void dessin() {
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// effacer la fenetre avec la couleur blanche 
	glClearColor(1.0,1.0,1.0,0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	// dessin des axes du modelview
	if (ind_repere_modelview)
	{
		dessin_repere_modelview();
	}
	
	// calcul de la direction de lumiere 
	GLdouble La_radian = La*r_radian_degre;
	GLdouble Le_radian = Le*r_radian_degre;
	Lumiere = Point(cos(La_radian)*cos(Le_radian),
	                sin(La_radian)*cos(Le_radian),
					sin(Le_radian));

	// dessin de la direction de lumière
	if (ind_direction_lumiere)
	{
		dessin_direction_lumiere();
	}
	
	// la scene 
	DIM2 = 2*dim_facettisation[facettisation];
	DIM1 = dim_facettisation[facettisation];
	glPushMatrix();
	
	// rotation globale de la scène 
	glRotated(angle_scene, 0.0, 0.0, 1.0); 
	{
		CouleurRGB couleur_1(1.0,0.4,0.0); // couleur de la sphere 1
		CouleurRGB couleur_2(0.4,1.0,0.0); // couleur de la sphere 2
		
		// sphere de centre (-3,-3,-3) et rayon 10 
		sphere(Point(-3.0,-3.0,-3.0), 10.0, couleur_1);			
		// sphere de centre (4,4,1) et rayon 9 
		sphere(Point( 4.0, 4.0, 1.0),  9.0, couleur_2);			
	}
	glPopMatrix();
	
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
	          0.0,0.0,0.0,
			  vx, vy, vz);
}

// prise en compte d'un redimensionnement de la fenetre 
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
		
	
	// choix de la facettisation  
	case '1' :
	case '2' :
	case '3' :
	case '4' :
	{
		int cas = key-49;
		if (cas!=facettisation)
		{				
			facettisation = cas;	
			glutPostRedisplay();
		}
		break;
	}
		
	// fin du programme 
	case 27 :  
	case 'q' :  
	case 'Q' :  
		exit(0); 
		break; 
	 
	}
}

// la fonction principale 
int main(int argc, char **argv) {
	// initialisation de la librairie GLUT
	//   doit etre appelee avant toute autre instruction 
	//   de la librairie GLUT 
	glutInit(&argc,argv);
	
	// creation d'une fenetre graphique 
	//   de dimension 500 x 500
	//   en double buffer graphique, mode RGB et z-buffer 
	//   dont le nom est celui du programme 
	glutInitWindowSize(500,500);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutCreateWindow(argv[0]);
	glEnable(GL_DEPTH_TEST);
	
	// definition de la fonction de dessin 
	glutDisplayFunc(dessin);
	// definition de la fonction de redimensionnement de la fenetre 
	glutReshapeFunc(redimensionnement);
	// definition de la fonction de gestion des touches clavier
	//   (touches avec code ASCII correspondant) 
	glutKeyboardFunc(clavier);
	
	// initialisation de la matrice pour le modele 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		
	// boucle principale 
	glutMainLoop();
	
	return(0);
}
