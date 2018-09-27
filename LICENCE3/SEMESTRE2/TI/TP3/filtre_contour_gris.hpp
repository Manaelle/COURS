#include "lib_image.hpp"
#include <cstdio>
#include <cstdlib>
#include <cmath>


ImageGrisF abs(ImageGrisF &I1);
ImageGrisF pow(ImageGrisF &I1, float p);
ImageGrisF sqrt(ImageGrisF &I1);
ImageGrisF operator+(ImageGrisF &I1, ImageGrisF &I2);
ImageGrisF max(ImageGrisF &I1, ImageGrisF &I2);
ImageGrisF operator+=(ImageGrisF &I1, ImageGrisF &I2);
ImageGrisF operator*(ImageGrisF &I1, float a);
ImageGrisF operator*(float a, ImageGrisF &I1);
ImageGrisF operator*=(ImageGrisF &I1, float a);
ImageGrisF seuillage_automatique(ImageGrisF &I);
class FiltreLineaire;
ImageGrisF convolution(ImageGrisF& I, FiltreLineaire K);
ImageGrisF filtre_derivee_methode1(ImageGrisF &I);
ImageGrisF filtre_derivee_methode2(ImageGrisF &I);
ImageGrisF filtre_Sobel_Prewitt(ImageGrisF &I);
