///////////////////////////////////////////////////////////////////////////////
// cube 3D
//    la rotation du cube 3D se fait en utilisant les touches du clavier
//      touche x : rotation élémentaire dans le sens - autour de Ox  
//      touche X : rotation élémentaire dans le sens + autour de Ox  
//      touche y : rotation élémentaire dans le sens - autour de Oy  
//      touche Y : rotation élémentaire dans le sens + autour de Oy  
//      touche z : rotation élémentaire dans le sens - autour de Oz  
//      touche Z : rotation élémentaire dans le sens + autour de Oz
// 	   
//    le zoom se fait a l'aide des touches + (zoom avant) et - (zoom arriere)
//    
//    la touche q ou ESC met fin a l'execution du programme
//    	   
//    compilation
//      g++ cubes3d.cpp -lglut -lGLU -lGL -o cubes3d
// 	 
//    Nicolas SZAFRAN - 2011	 
//  
///////////////////////////////////////////////////////////////////////////////

#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

GLdouble delta_rot=2.0;  // modification elementaire d'angle 

///////////////////////////
// les ordres graphiques //
///////////////////////////

/* cylindre de base circulaire de rayon 1 et de hauteur 2 
   l'axe du cylindre entre les points (0,0,-1) et (0,0,1) */
#define NB_FACES_CYLINDRE 100
void cylindre()
{
	/* coordonnées du cercle unité discrétisé */
	GLdouble Cx[NB_FACES_CYLINDRE];
	GLdouble Cy[NB_FACES_CYLINDRE];
	
	int i;
	
	/* calcul du cercle unité */
	for (i=0; i<NB_FACES_CYLINDRE; i++)
	{
		GLdouble a;
		a = 2.0*i*M_PI/NB_FACES_CYLINDRE;
		Cx[i] = cos(a);
		Cy[i] = sin(a);
	}
	
	/* la base de centre (0,0,-1) */
	glColor3d(0.8,0.8,0.8);
	glBegin(GL_POLYGON);
		for (i=0; i<NB_FACES_CYLINDRE; i++)	
		{	
			glVertex3d(Cx[i], Cy[i], -1.0);
		}
	glEnd();
	/* la base de centre (0,0,1) */
	glColor3d(0.8,0.8,0.8);
	glBegin(GL_POLYGON);
		for (i=0; i<NB_FACES_CYLINDRE; i++)	
		{	
			glVertex3d(Cx[i], Cy[i],  1.0);
		}
	glEnd();
	/* le bord du cylindre */
	for (i=0; i<NB_FACES_CYLINDRE; i++)
	{
		GLdouble gris = 0.3*Cx[i]+0.7;
		int j = (i+1)%NB_FACES_CYLINDRE;
		glColor3d(gris, gris, gris);
		glBegin(GL_QUADS);
			glVertex3d(Cx[i], Cy[i], -1.0);
			glVertex3d(Cx[i], Cy[i],  1.0);
			glVertex3d(Cx[j], Cy[j],  1.0);
			glVertex3d(Cx[j], Cy[j], -1.0);
		glEnd();
	}	
	
}
   
// le cube3d unite 
#define NB_FACES_CUBE 6
void cube()
{
    // les sommets du cube 
	GLdouble sommets[][3] = {
	  {-1.0,-1.0,-1.0},
	  { 1.0,-1.0,-1.0},
	  {-1.0, 1.0,-1.0},
	  { 1.0, 1.0,-1.0},
	  {-1.0,-1.0, 1.0},
	  { 1.0,-1.0, 1.0},
	  {-1.0, 1.0, 1.0},
	  { 1.0, 1.0, 1.0},
	};
	
	// les faces du cube, chaque face décrite par ses quatre sommets 
	int faces[][4] = {
	  {0,1,3,2},
	  {5,4,6,7},
	  {0,4,5,1},
	  {6,2,3,7},
	  {0,2,6,4},
	  {3,1,5,7}
	};
	
	// les couleurs des faces 
	GLdouble couleurs[][3] = {
	  {1.0,0.5,0.5},
	  {0.0,0.5,0.5},
	  {0.5,1.0,0.5},
	  {0.5,0.0,0.5},
	  {0.5,0.5,1.0},
	  {0.6,0.3,0.1}
	};
	
	int i;
	
	// tracé des faces 
	for (i=0; i<NB_FACES_CUBE; i++)
	{
		int s0 = faces[i][0];
		int s1 = faces[i][1];
		int s2 = faces[i][2];
		int s3 = faces[i][3];
		glColor3dv(couleurs[i]);
		glBegin(GL_QUADS);
			glVertex3dv(sommets[s0]);
			glVertex3dv(sommets[s1]);
			glVertex3dv(sommets[s2]);
			glVertex3dv(sommets[s3]);
		glEnd();
	}
}
/**********************************************/
/**Exercice 4**/
/*************************/
void toit()
{

   	 // les sommets du cube 
	//x largeur
	//y hauteur
	//z profondeur
	GLdouble sommets[][3] = {
	  {-0.0,-0.0,-1.0},//Devant bas gauche
	  { 3.0,-0.0,-1.0},//Devant bas droite
	  {-0.0, 0.5,-1.0},//Devant haut gauche
	  { 3.0, 0.5,-1.0},//Devant haut droite
	  {-0.0,-0.0, 1.0},// bas gauche
	  { 3.0,-0.0, 1.0},// bas droite
	  {-0.0, 0.5, 1.0},// haut gauche
	  { 3.0, 0.5, 1.0},// haut droite
	};
	
	// les faces du cube, chaque face décrite par ses quatre sommets 
	int faces[][4] = {
	  {0,1,3,2},
	  {5,4,6,7},
	  {0,4,5,1},
	  {6,2,3,7},
	  {0,2,6,4},
	  {3,1,5,7}
	};
	
	// les couleurs des faces 
	GLdouble couleurs[][3] = {
	  {0.0,1.0,0.0},//Arriere
	  {0.0,1.0,0.0},//Avant
	  {0.18,0.31,0.31},//Dessous
	  {0.18,0.31,0.31},//Dessus
	  {0.0,0.0,0.8},//Gauche
	  {0.3,0.3,0.3}//Droite
	};
	
	int i;
	
	// tracé des faces 
	for (i=0; i<NB_FACES_CUBE; i++)
	{
		int s0 = faces[i][0];
		int s1 = faces[i][1];
		int s2 = faces[i][2];
		int s3 = faces[i][3];
		glColor3dv(couleurs[i]);
		glBegin(GL_QUADS);
			glVertex3dv(sommets[s0]);
			glVertex3dv(sommets[s1]);
			glVertex3dv(sommets[s2]);
			glVertex3dv(sommets[s3]);
		glEnd();
	}
}
/*************************/
void cheminee()
{
	/* coordonnées du cercle unité discrétisé */
	GLdouble Cx[NB_FACES_CYLINDRE];
	GLdouble Cy[NB_FACES_CYLINDRE];
	
	int i;
	
	/* calcul du cercle unité */
	for (i=0; i<NB_FACES_CYLINDRE; i++)
	{
		GLdouble a;
		a = 2.0*i*M_PI/NB_FACES_CYLINDRE;
		Cx[i] = 0.5*cos(a);
		Cy[i] = 0.5*sin(a);
	}
	
	/* la base de centre (0,0,-1) */
	glColor3d(0.4,0.4,0.4);
	glBegin(GL_POLYGON);
		for (i=0; i<NB_FACES_CYLINDRE; i++)	
		{	
			glVertex3d(Cx[i], -0.0, Cy[i]);
		}
	glEnd();
	/* la base de centre (0,0,1) */
	glColor3d(0.4,0.4,0.4);
	glBegin(GL_POLYGON);
		for (i=0; i<NB_FACES_CYLINDRE; i++)	
		{	
			glVertex3d(Cx[i], 1.5, Cy[i]);
		}
	glEnd();
	/* le bord du cylindre */
	for (i=0; i<NB_FACES_CYLINDRE; i++)
	{
		GLdouble gris = 0.3*Cx[i]+0.7;
		int j = (i+1)%NB_FACES_CYLINDRE;
		glColor3d(gris, gris, gris);
		glBegin(GL_QUADS);
			glVertex3d(Cx[i], -0.0, Cy[i]);
			glVertex3d(Cx[i], 1.5, Cy[i]);
			glVertex3d(Cx[j], 1.5, Cy[j]);
			glVertex3d(Cx[j], -0.0, Cy[j]);
		glEnd();
	}	
	
}
/***************************/
void corps_loco_cy()
{
	/* coordonnées du cercle unité discrétisé */
	GLdouble Cx[NB_FACES_CYLINDRE];
	GLdouble Cy[NB_FACES_CYLINDRE];
	
	int i;
	
	/* calcul du cercle unité */
	for (i=0; i<NB_FACES_CYLINDRE; i++)
	{
		GLdouble a;
		a = 2.0*i*M_PI/NB_FACES_CYLINDRE;
		Cx[i] = 1.75*cos(a);
		Cy[i] = 1.75*sin(a);
	}
	
	/* la base de centre (0,0,-1) */
	glColor3d(0.3,0.3,0.3);
	glBegin(GL_POLYGON);
		for (i=0; i<NB_FACES_CYLINDRE; i++)	
		{	
			glVertex3d(-0.0, Cy[i], Cx[i]);
		}
	glEnd();
	/* la base de centre (0,0,1) */
	glColor3d(0.3,0.3,0.3);
	glBegin(GL_POLYGON);
		for (i=0; i<NB_FACES_CYLINDRE; i++)	
		{	
			glVertex3d(9.0, Cy[i], Cx[i]);
		}
	glEnd();
	/* le bord du cylindre */
	for (i=0; i<NB_FACES_CYLINDRE; i++)
	{
		GLdouble gris = 0.3*Cx[i]+0.7;
		int j = (i+1)%NB_FACES_CYLINDRE;
		glColor3d(gris, gris, gris);
		glBegin(GL_QUADS);
			glVertex3d(-0.0, Cy[i], Cx[i]);
			glVertex3d(9.0, Cy[i], Cx[i]);
			glVertex3d(9.0, Cy[j], Cx[j]);
			glVertex3d(-0.0, Cy[j],Cx[j]);
		glEnd();
	}	
	
}
/***********************/
void corps_loco1()
{

   	 // les sommets du cube 
	//x largeur
	//y hauteur
	//z profondeur
	GLdouble sommets[][3] = {
	  {-2.0,-0.0,-2.0},//Devant bas gauche
	  { 7.0,-0.0,-2.0},//Devant bas droite
	  {-2.0, 2.0,-2.0},//Devant haut gauche
	  { 7.0, 2.0,-2.0},//Devant haut droite
	  {-2.0,-0.0, 2.0},// bas gauche
	  { 7.0,-0.0, 2.0},// bas droite
	  {-2.0, 2.0, 2.0},// haut gauche
	  { 7.0, 2.0, 2.0},// haut droite
	};
	
	// les faces du cube, chaque face décrite par ses quatre sommets 
	int faces[][4] = {
	  {0,1,3,2},
	  {5,4,6,7},
	  {0,4,5,1},
	  {6,2,3,7},
	  {0,2,6,4},
	  {3,1,5,7}
	};
	
	// les couleurs des faces 
	GLdouble couleurs[][3] = {
	  {0.0,1.0,0.0},//Arriere
	  {0.0,1.0,0.0},//Avant
	  {0.18,0.31,0.31},//Dessous
	  {0.18,0.31,0.31},//Dessus
	  {0.0,0.0,0.8},//Gauche
	  {0.3,0.3,0.3}//Droite
	};
	
	int i;
	
	// tracé des faces 
	for (i=0; i<NB_FACES_CUBE; i++)
	{
		int s0 = faces[i][0];
		int s1 = faces[i][1];
		int s2 = faces[i][2];
		int s3 = faces[i][3];
		glColor3dv(couleurs[i]);
		glBegin(GL_QUADS);
			glVertex3dv(sommets[s0]);
			glVertex3dv(sommets[s1]);
			glVertex3dv(sommets[s2]);
			glVertex3dv(sommets[s3]);
		glEnd();
	}
}
/************************/
void corps_loco2()
{

   	 // les sommets du cube 
	//x largeur
	//y hauteur
	//z profondeur
	GLdouble sommets[][3] = {
	  {-0.0,-0.0,-2.0},//Devant bas gauche
	  { 2.25,-0.0,-2.0},//Devant bas droite
	  {-0.0, 5.0,-2.0},//Devant haut gauche
	  { 2.25, 5.0,-2.0},//Devant haut droite
	  {-0.0,-0.0, 2.0},// bas gauche
	  { 2.25,-0.0, 2.0},// bas droite
	  {-0.0, 5.0, 2.0},// haut gauche
	  { 2.25, 5.0, 2.0},// haut droite
	};
	
	// les faces du cube, chaque face décrite par ses quatre sommets 
	int faces[][4] = {
	  {0,1,3,2},
	  {5,4,6,7},
	  {0,4,5,1},
	  {6,2,3,7},
	  {0,2,6,4},
	  {3,1,5,7}
	};
	
	// les couleurs des faces 
	GLdouble couleurs[][3] = {
	  {0.0,1.0,0.0},//Arriere
	  {0.0,1.0,0.0},//Avant
	  {0.18,0.31,0.31},//Dessous
	  {0.18,0.31,0.31},//Dessus
	  {0.0,0.0,0.8},//Gauche
	  {0.3,0.3,0.3}//Droite
	};
	
	int i;
	
	// tracé des faces 
	for (i=0; i<NB_FACES_CUBE; i++)
	{
		int s0 = faces[i][0];
		int s1 = faces[i][1];
		int s2 = faces[i][2];
		int s3 = faces[i][3];
		glColor3dv(couleurs[i]);
		glBegin(GL_QUADS);
			glVertex3dv(sommets[s0]);
			glVertex3dv(sommets[s1]);
			glVertex3dv(sommets[s2]);
			glVertex3dv(sommets[s3]);
		glEnd();
	}
}
/************************/
void corps_loco3()
{

   	 // les sommets du cube 
	//x largeur
	//y hauteur
	//z profondeur
	GLdouble sommets[][3] = {
	  {-0.0,-0.0,-2.0},//Devant bas gauche
	  { 2.25,-0.0,-2.0},//Devant bas droite
	  {-0.0, 2.0,-2.0},//Devant haut gauche
	  { 2.25, 2.0,-2.0},//Devant haut droite
	  {-0.0,-0.0, 2.0},// bas gauche
	  { 2.25,-0.0, 2.0},// bas droite
	  {-0.0, 2.0, 2.0},// haut gauche
	  { 2.25, 2.0, 2.0},// haut droite
	};
	
	// les faces du cube, chaque face décrite par ses quatre sommets 
	int faces[][4] = {
	  {0,1,3,2},
	  {5,4,6,7},
	  {0,4,5,1},
	  {6,2,3,7},
	  {0,2,6,4},
	  {3,1,5,7}
	};
	
	// les couleurs des faces 
	GLdouble couleurs[][3] = {
	  {0.0,1.0,0.0},//Arriere
	  {0.0,1.0,0.0},//Avant
	  {0.18,0.31,0.31},//Dessous
	  {0.18,0.31,0.31},//Dessus
	  {0.0,0.0,0.8},//Gauche
	  {0.3,0.3,0.3}//Droite
	};
	
	int i;
	
	// tracé des faces 
	for (i=0; i<NB_FACES_CUBE; i++)
	{
		int s0 = faces[i][0];
		int s1 = faces[i][1];
		int s2 = faces[i][2];
		int s3 = faces[i][3];
		glColor3dv(couleurs[i]);
		glBegin(GL_QUADS);
			glVertex3dv(sommets[s0]);
			glVertex3dv(sommets[s1]);
			glVertex3dv(sommets[s2]);
			glVertex3dv(sommets[s3]);
		glEnd();
	}
}
/************************/
void attache()
{
    // les sommets du cube 
	GLdouble sommets[][3] = {
	  {-0.0,-0.0,-0.0},
	  { 1.0,-0.0,-0.0},
	  {-0.0, 0.3,-0.0},
	  { 1.0, 0.3,-0.0},
	  {-0.0,-0.0, 0.3},
	  { 1.0,-0.0, 0.3},
	  {-0.0, 0.3, 0.3},
	  { 1.0, 0.3, 0.3},
	};
	
	// les faces du cube, chaque face décrite par ses quatre sommets 
	int faces[][4] = {
	  {0,1,3,2},
	  {5,4,6,7},
	  {0,4,5,1},
	  {6,2,3,7},
	  {0,2,6,4},
	  {3,1,5,7}
	};
	
	// les couleurs des faces 
	GLdouble couleurs[][3] = {
	  {0.0,1.0,0.0},//Arriere
	  {0.0,1.0,0.0},//Avant
	  {0.18,0.31,0.31},//Dessous
	  {0.18,0.31,0.31},//Dessus
	  {0.0,0.0,0.8},//Gauche
	  {0.46,0.53,0.6}//Droite
	};
	
	int i;
	
	// tracé des faces 
	for (i=0; i<NB_FACES_CUBE; i++)
	{
		int s0 = faces[i][0];
		int s1 = faces[i][1];
		int s2 = faces[i][2];
		int s3 = faces[i][3];
		glColor3dv(couleurs[i]);
		glBegin(GL_QUADS);
			glVertex3dv(sommets[s0]);
			glVertex3dv(sommets[s1]);
			glVertex3dv(sommets[s2]);
			glVertex3dv(sommets[s3]);
		glEnd();
	}
}
/***********************/
void corps_loco4()
{

   	 // les sommets du cube 
	//x largeur
	//y hauteur
	//z profondeur
	GLdouble sommets[][3] = {
	  {-0.0,-0.0,-2.0},//Devant bas gauche
	  { 4.5,-0.0,-2.0},//Devant bas droite
	  {-0.0, 0.5,-2.0},//Devant haut gauche
	  { 4.5, 0.5,-2.0},//Devant haut droite
	  {-0.0,-0.0, 2.0},// bas gauche
	  { 4.5,-0.0, 2.0},// bas droite
	  {-0.0, 0.5, 2.0},// haut gauche
	  { 4.5, 0.5, 2.0},// haut droite
	};
	
	// les faces du cube, chaque face décrite par ses quatre sommets 
	int faces[][4] = {
	  {0,1,3,2},
	  {5,4,6,7},
	  {0,4,5,1},
	  {6,2,3,7},
	  {0,2,6,4},
	  {3,1,5,7}
	};
	
	// les couleurs des faces 
	GLdouble couleurs[][3] = {
	  {0.0,1.0,0.0},//Arriere
	  {0.0,1.0,0.0},//Avant
	  {0.18,0.31,0.31},//Dessous
	  {0.18,0.31,0.31},//Dessus
	  {0.0,0.0,0.8},//Gauche
	  {0.3,0.3,0.3}//Droite
	};
	
	int i;
	
	// tracé des faces 
	for (i=0; i<NB_FACES_CUBE; i++)
	{
		int s0 = faces[i][0];
		int s1 = faces[i][1];
		int s2 = faces[i][2];
		int s3 = faces[i][3];
		glColor3dv(couleurs[i]);
		glBegin(GL_QUADS);
			glVertex3dv(sommets[s0]);
			glVertex3dv(sommets[s1]);
			glVertex3dv(sommets[s2]);
			glVertex3dv(sommets[s3]);
		glEnd();
	}
}
/************************/
void corps_wagon()
{
    // les sommets du cube 
	//x largeur
	//y hauteur
	//z profondeur
	GLdouble sommets[][3] = {
	  {-2.5,-0.0,-2.0},//Devant bas gauche
	  { 7.5,-0.0,-2.0},//Devant bas droite
	  {-2.5, 5.0,-2.0},//Devant haut gauche
	  { 7.5, 5.0,-2.0},//Devant haut droite
	  {-2.5,-0.0, 2.0},// bas gauche
	  { 7.5,-0.0, 2.0},// bas droite
	  {-2.5, 5.0, 2.0},// haut gauche
	  { 7.5, 5.0, 2.0},// haut droite
	};
	
	// les faces du cube, chaque face décrite par ses quatre sommets 
	int faces[][4] = {
	  {0,1,3,2},
	  {5,4,6,7},
	  {0,4,5,1},
	  {6,2,3,7},
	  {0,2,6,4},
	  {3,1,5,7}
	};
	
	// les couleurs des faces 
	GLdouble couleurs[][3] = {
	  {0.0,1.0,0.0},//Arriere
	  {0.0,1.0,0.0},//Avant
	  {0.18,0.31,0.31},//Dessous
	  {0.18,0.31,0.31},//Dessus
	  {0.0,0.0,0.8},//Gauche
	  {0.3,0.3,0.3}//Droite
	};
	
	int i;
	
	// tracé des faces 
	for (i=0; i<NB_FACES_CUBE; i++)
	{
		int s0 = faces[i][0];
		int s1 = faces[i][1];
		int s2 = faces[i][2];
		int s3 = faces[i][3];
		glColor3dv(couleurs[i]);
		glBegin(GL_QUADS);
			glVertex3dv(sommets[s0]);
			glVertex3dv(sommets[s1]);
			glVertex3dv(sommets[s2]);
			glVertex3dv(sommets[s3]);
		glEnd();
	}
}
/*******************/
void roue()
{
	/* coordonnées du cercle unité discrétisé */
	GLdouble Cx[NB_FACES_CYLINDRE];
	GLdouble Cy[NB_FACES_CYLINDRE];
	
	int i;
	
	/* calcul du cercle unité */
	for (i=0; i<NB_FACES_CYLINDRE; i++)
	{
		GLdouble a;
		a = 2.0*i*M_PI/NB_FACES_CYLINDRE;
		Cx[i] = cos(a);
		Cy[i] = sin(a);
	}
	
	/* la base de centre (0,0,-1) */
	glColor3d(0.3,0.3,0.3);
	glBegin(GL_POLYGON);
		for (i=0; i<NB_FACES_CYLINDRE; i++)	
		{	
			glVertex3d(Cx[i], Cy[i], -0.0);
		}
	glEnd();
	/* la base de centre (0,0,1) */
	glColor3d(0.3,0.3,0.3);
	glBegin(GL_POLYGON);
		for (i=0; i<NB_FACES_CYLINDRE; i++)	
		{	
			glVertex3d(Cx[i], Cy[i],  0.5);
		}
	glEnd();
	/* le bord du cylindre */
	for (i=0; i<NB_FACES_CYLINDRE; i++)
	{
		GLdouble gris = 0.3*Cx[i]+0.7;
		int j = (i+1)%NB_FACES_CYLINDRE;
		glColor3d(gris, gris, gris);
		glBegin(GL_QUADS);
			glVertex3d(Cx[i], Cy[i], -0.0);
			glVertex3d(Cx[i], Cy[i],  0.5);
			glVertex3d(Cx[j], Cy[j],  0.5);
			glVertex3d(Cx[j], Cy[j], -0.0);
		glEnd();
	}	
	
}
/*********************/

/**********************************************/
// afficher un cube unité en position (x,y,z)
void dessin_wagon(GLdouble x, GLdouble y, GLdouble z)
{
	glPushMatrix();
		glTranslated(x,y,z);
		corps_wagon();
		glTranslated(x+1.5,y+1.5,z-1);
		roue();
		glTranslated(x+6.5,y+1.5,z+1.5);
		roue();
		glTranslated(x+1.5,y+1.5,z+6);
		roue();
		glTranslated(x-3.5,y+1.5,z+1.5);
		roue();
		glTranslated(x-2.0,y+1.5,z-0.5);
		attache();

	glPopMatrix();
}
/*******/
void dessin_loco(GLdouble x, GLdouble y, GLdouble z)
{
	glPushMatrix();
		glTranslated(x,y,z);
		corps_loco1();
		glTranslated(x+8.5,y+1.5,z+1.5);
		corps_loco2();
		glTranslated(x+3.5,y+1.5,z+1.5);
		corps_loco3();
		glTranslated(x-0.5,y+6.5,z+1.5);
		corps_loco4();
		glTranslated(x-7.5,y+0.25,z+1.48);
		corps_loco_cy();
		glTranslated(x+6,y+3.25,z+1.5);
		cheminee();
		glTranslated(x,y+3,z+1.5);
		toit();
		glTranslated(x+0.5,y-5.5,z-1.0);
		roue();
		glTranslated(x+5.0,y+1.5,z+1.5);
		roue();
		glTranslated(x+6.5,y+1.5,z+1.5);
		roue();
		glTranslated(x+1.5,y+1.5,z+6.0);
		roue();
		glTranslated(x-3.5,y+1.5,z+1.5);
		roue();
		glTranslated(x-2.0,y+1.5,z+1.5);
		roue();
	glPopMatrix();
}
void dessin() 
{

	// effacer la fenetre avec la couleur blanche 
	glClearColor(1.0,1.0,1.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslated(0.0,0.0,0.0);
	dessin_loco(-1.5,-1.5,-1.5);        
	glPopMatrix();

	glPushMatrix();
	glTranslated(14.5,0.0,0.0);
	dessin_wagon(-1.5,-1.5,-1.5);       
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(25.5,0.0,0.0);
	dessin_wagon(-1.5,-1.5,-1.5);       
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(36.5,0.0,0.0);
	dessin_wagon(-1.5,-1.5,-1.5);       
	glPopMatrix();

	glPushMatrix();
	glTranslated(47.5,0.0,0.0);
	dessin_wagon(-1.5,-1.5,-1.5);       
	glPopMatrix();

	//glFlush();
	glutSwapBuffers();
}
/*void dessin() 
{

	// effacer la fenetre avec la couleur blanche 
	glClearColor(1.0,1.0,1.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT); 

	// dessiner les cubes
	dessin_cube(-1.5,-1.5,-1.5);
	dessin_cube(-1.5,-1.5, 1.5);
	dessin_cube(-1.5, 1.5,-1.5);
	dessin_cube(-1.5, 1.5, 1.5);
	dessin_cube( 1.5,-1.5,-1.5);
	dessin_cube( 1.5,-1.5, 1.5);
	dessin_cube( 1.5, 1.5,-1.5);
	dessin_cube( 1.5, 1.5, 1.5);
	 
	glFlush(); 
}*/

// prise en compte d'un redimensionnement de la fenetre 
void redimensionnement(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5.0, 5.0, -5.0, 5.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
}

// effectuer une rotation elementaire d'angle delta 
//   autour d'un des axes de l'ecran :
//   1 : axe 0x (horizontal)
//   2 : axe 0y (vertical)
//   3 : axe 0z (perpendiculaire)
void rotation(GLdouble delta, int axe)
{
	GLdouble M[16];
	
	// recuperer la matrice courante du modele -> M 
	glMatrixMode(GL_MODELVIEW);
	glGetDoublev(GL_MODELVIEW_MATRIX, M);
	
	// initialiser avec la matrice identite 
	glLoadIdentity();
	
	// effectuer la rotation 
	if (axe==1)
		glRotated(delta, 1.0, 0.0, 0.0);
	else if (axe==2)
		glRotated(delta, 0.0, 1.0, 0.0);
	else
		glRotated(delta, 0.0, 0.0, 1.0);
		
	// multiplier par la matrice M 	
	glMultMatrixd(M);
	
	// envoyer un evenement pour rafraichir l'affichage  
	glutPostRedisplay();
}

// gestion des touches clavier 
void clavier(unsigned char key, int x, int y) 
{ 
	switch(key) 
	{ 
	// fin du programme 
	case 27 :  
	case 'q' :  
	case 'Q' :  
		exit(0); 
		break; 
	 
	// rotations 
	case 'x':
		 rotation(-delta_rot, 1);
		 break; 
	case 'X':
		 rotation( delta_rot, 1);
		 break; 
	case 'y':
		 rotation(-delta_rot, 2);
		 break; 
	case 'Y':
		 rotation( delta_rot, 2);
		 break; 
	case 'z':
		 rotation(-delta_rot, 3);
		 break; 
	case 'Z':
		 rotation( delta_rot, 3);
		 break; 
	
	// zooms 	 
	case '-':
		glScaled(10.0/11.0,10.0/11.0,10.0/11.0);	 
		glutPostRedisplay();
		break; 
	
	case '+':
		glScaled(11.0/10.0,11.0/10.0,11.0/10.0);	 
		glutPostRedisplay();
		break; 
	
	}
}

// la fonction principale 
int main(int argc,char **argv) {
	// initialisation de la librairie GLUT
	//   doit etre appelee avant toute autre instruction 
	//   de la librairie GLUT 
	glutInit(&argc,argv);
	
	// creation d'une fenetre graphique 
	//   de dimension 500 x 500
	//   en simple buffer et mode RGB
	//   dont le nom est celui du programme 
	glutInitWindowSize(500,500);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	glEnable(GL_DEPTH_TEST);
	// definition de la routine de dessin 
	glutDisplayFunc(dessin);
	// definition de la routine de redimensionnement de la fenetre 
	glutReshapeFunc(redimensionnement);
	// definition de la routine de gestion des touches clavier
	//   (touches avec code ASCII correspondant) 
	glutKeyboardFunc(clavier);
	
	// initialisation de la matrice pour le modele 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// boucle principale 
	glutMainLoop();
	
	return(0);
}
