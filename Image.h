#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

#include "polygone.h"

// Triplet de flottant représentant une couleur
typedef struct
{
	float _red, _green, _blue;
} Color;

Color C_new(float red, float green, float blue); // Créé une nouvelle couleur

// Structure de l'image
typedef struct
{
	int _width, _height;
	int _xzoom, _yzoom;
	int _xoffset, _yoffset;
	double _zoom;
	Color _current_color;
	Color **_buffer;
} Image;

Image *I_new(int _width, int _height); // Créé une nouvelle image

void I_fill(Image *img, Color c); // Rempli entièrement une image de la couleur c

void I_changeColor(Image *img, Color c);			 // Change la couleur dans la structure de l'image
void I_plotColor(Image *img, int x, int y, Color c); // Place la couleur c dans le buffer de l'image aux coordonnées (x,y)

void I_focusPoint(Image *img, int x, int y);
void I_zoomInit(Image *img);			   // Réinitialise le zoom de l'image
void I_zoom(Image *img, double zoom_coef); // Change le zoom de l'image
void I_move(Image *img, int x, int y);	   // Déplace l'image

void I_draw(Image *img); // Déssine l'image dans la fenêtre

void P_draw(Image *img, polygone *p); // Déssine le polynome dans l'image
void P_fill(Image *img, polygone *p); // Rempli le polynome avec la méthode scan-line

void FindYMaxima(Image *img, int *ymin, int *ymax); // Trouve le ymin et ymax du polynome

// Calcul les coordonnées de A et B dans le premier octant
void ToFirstOctan(int xA, int yA, int xB, int yB, int *xA_1o, int *yA_1o, int *xB_1o, int *yB_1o);
// Calcul les coordonnées d'un point dans son octant d'origine
void FromFirstOctan(int xA, int yA, int xB, int yB, int x_1o, int y_1o, int *x, int *y);
// Dessine la droite de bresenham dans l'image
void I_bresenham(Image *img, int xA, int yA, int xB, int yB);

// Trace la droite de bresenham entre le dernier et le premier sommet
void Close(Image *img, polygone *p, point depart, point fin);

// Dessine un carré autour du point d'indice selected
void I_drawCarre(Image *img, polygone *p, int selected);

// Rempli l'image de noir et redessine le polygone
void I_reset(Image *img, polygone *p);

#endif
