// l'exemple du cours

// utilisation de la librairie GLUT
#include <GL/glut.h>

// les ordres graphiques 
// --> dessin de la sc�ne 3D
void dessin() 
{
  glMatrixMode(GL_MODELVIEW); // rep�re de la sc�ne
  glLoadIdentity(); 
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 1.0, 0.0);
  glRectf(-25.0, -25.0, 25.0, 25.0);
  glFlush();
}

// prise en compte d'un redimensionnement de la fenetre 
// --> d�finition de la cam�ra
void redimensionnement(int w, int h)
{
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION); // rep�re de la cam�ra
  glLoadIdentity();
  glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
}

// la fonction principale 
int main(int argc,char **argv) 
{
  glutInit(&argc,argv);
  glutInitWindowSize(500,500);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutCreateWindow("simple rectangle");
  glutDisplayFunc(dessin);
  glutReshapeFunc(redimensionnement);
  glutMainLoop();
  return(0);
}
