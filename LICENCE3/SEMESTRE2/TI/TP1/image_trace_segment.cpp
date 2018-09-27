//////////////////////////////////////////////////////////////////////////////
// tracé de segment pixel par pixel - Algorithme de Bresenham
// 
// N. SZAFRAN - UFR-IM2AG - 2015
//////////////////////////////////////////////////////////////////////////////

#include "lib_image.hpp"
#include <cstdio>
#include <cmath>

// les couleurs de base
RGBF Blanc  (255,255,255);
RGBF Noir   (  0,  0,  0);
RGBF Rouge  (255,  0,  0);
RGBF Vert   (  0,255,  0);
RGBF Bleu   (  0,  0,255);
RGBF Cyan   (  0,255,255);
RGBF Magenta(255,  0,255);
RGBF Jaune  (255,255,  0);

// tracer le segment du pixel (x1,y1) au pixel (x2,y2)
// dans l'image I avec la couleur c
/*
void dessiner_segment(ImageCouleur &I, int x1, int y1, int x2, int y2, RGB c)
{
	if (x2>=x1 && y2>=y1 && x2-x1>=y2-y1)
	{	// le cas vu en cours 
	//rouge
		// initialisation
		int dx = x2-x1;
		int dy = y2-y1;
		int x = x1;
		int y = y1;
		int fP = 0;
		int dfM  =   dx-2*dy;
		int dfPcas1 = 2*dx-2*dy;
		int dfPcas2 =     -2*dy;
		
		while (x != x2)
		{
			I.pixel(x,y,c); // dessiner le pixel (x,y) avec la couleur c
			
			// passer au pixel suivant
			if (fP+dfM < 0)
			{
				y += 1;
				fP += dfPcas1;
			}
			else
			{
				fP += dfPcas2;
			}
			x += 1;
		}
		I.pixel(x,y,c); // dessiner le pixel (x,y) avec la couleur c
	}else if (x2<=x1 && y2>=y1 && x2-x1>=y2-y1)
	{
		// initialisation
		int dx = x2-x1;
		int dy = y1-y2;
		int x = x1;
		int y = y1;
		int fP = 0;
		int dfM  =   dx-2*dy;
		int dfPcas1 = 2*dx-2*dy;
		int dfPcas2 =     -2*dy;
		
		while (x != x2)
		{
			I.pixel(x,y,c); // dessiner le pixel (x,y) avec la couleur c
			
			// passer au pixel suivant
			if (fP+dfM < 0)
			{
				y += 2;
				fP += dfPcas1;
			}
			else
			{
				fP += dfPcas2;
			}
			x -= 1;
		}
		I.pixel(x,y,c); // dessiner le pixel (x,y) avec la couleur c
	}else if (x2>=x1 && y2>=y1 && x2-x1<=y2-y1)
	{	
	//Rouge
		// initialisation
		int dx = x2-x1;
		int dy = y2-y1;
		int x = x1;
		int y = y1;
		int fP = 0;
		int dfM  =   dy-2*dx;
		int dfPcas1 = 2*dy-2*dx;
		int dfPcas2 =     -2*dx;
		
		while (y != y2)
		{
			I.pixel(x,y,c); // dessiner le pixel (x,y) avec la couleur c
			
			// passer au pixel suivant
			if (fP+dfM < 0)
			{
				x += 1;
				fP += dfPcas1;
			}
			else
			{
				fP += dfPcas2;
			}
			y += 1;
		}
		I.pixel(x,y,c); // dessiner le pixel (x,y) avec la couleur c
	}else if (x2<=x1 && y2>=y1 && x2-x1<=y2-y1)
	{	
	//vert
		// initialisation
		int dx = x1-x2;
		int dy = y2-y1;
		int x = x1;
		int y = y1;
		int fP = 0;
		int dfM  =   dy-2*dx;
		int dfPcas1 = 2*dy-2*dx;
		int dfPcas2 =     -2*dx;
		
		while (y != y2)
		{
			I.pixel(x,y,c); // dessiner le pixel (x,y) avec la couleur c
			
			// passer au pixel suivant
			if (fP+dfM < 0)
			{
				x -= 1;
				fP += dfPcas1;
			}
			else
			{
				fP += dfPcas2;
			}
			y += 1;
		}
		I.pixel(x,y,c); // dessiner le pixel (x,y) avec la couleur c
	}
}*/

void dessiner_segment(ImageCouleur &I, int x1, int y1, int x2, int y2, RGB c)
{ int dx = x2-x1;
  int dy = y2-y1;
  int x = x1;
	int y = y1;
	if (dx>0 && dy>0 && dx>=dy)	{	
		// le cas vu en cours 
	  //rouge
		// initialisation
		int e = dx;
		dx *= 2;
		dy *= 2;
	
    while (x != x2){
			I.pixel(x,y,c);
			e -= dy;			
			if (e<0) {
				y += 1;
				e += dx;
			}
			x += 1;
		}
	}else if (dx>0 && dy>0 && dx<dy)	{	
		int e = dy;
		dy *= 2;
		dx *= 2;
		
		while (y != y2){
			I.pixel(x,y,c);
			e -= dx;			
			if (e<0) {
				x += 1;
				e += dy;
			}
			y += 1;
		}
	}else if (dx>0 && dy<0 && dx >= -dy){
		int e = dx;
		dx *= 2;
		dy *= 2;
		
		while (x !=x2){
			I.pixel(x,y,c);
			e += dy;			
			if (e<0) {
				y -= 1;
				e += dx;
			}
			x += 1;
		}
	}else if (dx>0 && dy<0 && dx < -dy){
		int e = dy;
		dx *= 2;
		dy *= 2;	
		
		while (y !=y2){
			I.pixel(x,y,c);
			e += dx;			
			if (e>0) {
				x += 1;
				e += dy;
			}
			y -= 1;
		}
	}else if (dx>0 && dy==0){
		int x = x1;
		while (x!=x2){
			I.pixel(x,y,c);
			x +=1;
		}
	}else if (dx<0 && dy>0 && -dx>=dy){
		int e = dx;
		dx *= 2;
		dy *= 2;
		
		while (x!=x2){
			I.pixel(x,y,c);
			e +=dy;
			if (e>=0) {
				y+=1;
				e+=dx;
			}
			x-=1;
		}
	}else if (dx<0 && dy>0 && -dx<dy){
		int e = dy;
		dx *= 2;
		dy *= 2;
		
		while (y!=y2){
			I.pixel(x,y,c);
			e +=dx;
			if (e<=0) {
				x-=1;
				e+=dy;
			}
			y+=1;
		}
	}else if (dx<0 && dy<0 && dx<=dy){
		int e = dx;
		dx *= 2;
		dy *= 2;
		
		while (x!=x2){
			I.pixel(x,y,c);
			e -=dy;
			if (e>=0) {
				y-=1;
				e+=dx;
			}
			x-=1;
		}
	}else if (dx<0 && dy<0 && dx>dy){
		int e = dy;
		dx *= 2;
		dy *= 2;
		
		while (y!=y2){
			I.pixel(x,y,c);
			e -=dx;
			if (e>=0) {
				x-=1;
				e+=dy;
			}
			y-=1;
		}
	}else if (dx<0 && dy==0){
		while (x!=x2){
			I.pixel(x,y,c);
			x-=1;
		}
	}else if (dx==0 && dy>0){
		while (y!=y2){
			I.pixel(x,y,c);
			y+=1;
		}
	}else if (dx==0 && dy<0){
		while (y!=y2){
			I.pixel(x,y,c);
			y-=1;
		}
	}
	I.pixel(x,y,c);
}

int main(int argc, char **argv)
{
	// créer une image de dimension 100 x 100 avec un fond blanc
	ImageCouleur I(100,100,Blanc);

	//tracer un segment rouge de (50,10) à (80,20)
	dessiner_segment(I , 15,25 , 50,50 , Rouge);
	//dessiner_segment(I , 25,25 , 50,50 , Rouge);
	dessiner_segment(I , 35,25 , 50,50 , Rouge);
	dessiner_segment(I , 85,75 , 50,50 , Bleu);
	//dessiner_segment(I , 75,75 , 50,50 , Bleu);
	dessiner_segment(I , 65,75 , 50,50 , Bleu);
	dessiner_segment(I , 85,25 , 50,50 , Vert);
	//dessiner_segment(I , 75,25 , 50,50 , Vert);
	dessiner_segment(I , 65,25 , 50,50 , Vert);	
	dessiner_segment(I , 15,75 , 50,50 , Noir);
	//dessiner_segment(I , 25,75 , 50,50 , Noir);
	dessiner_segment(I , 35,75 , 50,50 , Noir);
	
	UINT num_I = I.afficher(5);
	changer_titre_fenetre_graphique(num_I, "Image I");

	// attente de taper une touche
	printf("Taper un caractere au clavier pour continuer");
	getchar();

	return 0;
}
