#include "lib_image.hpp"
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>

ImageCouleurF gris2rgb(ImageGrisF &I);
template <typename Image, typename Couleur>
void dessiner_pixel(Image &I, int x, int y, Couleur c, int e);
template <typename Image, typename Couleur>
void dessiner_segment(Image &I, int x1, int y1, int x2, int y2, Couleur c,int e);
void tracer_droite(ImageCouleurF &IC, float theta, float alpha, RGBF coul, int e=1);
void normaliser(ImageGrisF &I);
ImageGrisF transformee_hough(ImageGrisF &I, UINT M, UINT N);
// structure pour un pixel
struct pixel_valeur {
	UINT x,y; // coordonnées du pixel
	float v;  // valeur du pixel
};
bool comp_pixel_valeur(pixel_valeur p1, pixel_valeur p2);
//vector<pixel_valeur> sequence_maxima_locaux(ImageGrisF &TH, float s, int d=1);
//void print(vector<pixel_valeur> &V);
ImageGrisF image_test_1();
ImageGrisF image_test_2();
