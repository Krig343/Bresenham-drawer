#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

#include "polygone.h"

typedef struct
{
	float _red, _green, _blue;
} Color;

Color C_new(float red, float green, float blue);

typedef struct
{
	int _width, _height;
	int _xzoom, _yzoom;
	int _xoffset, _yoffset;
	double _zoom;
	Color _current_color;
	Color **_buffer;
} Image;

Image *I_new(int _width, int _height);

void I_fill(Image *img, Color c);

void I_changeColor(Image *img, Color c);
void I_plotColor(Image *img, int x, int y, Color c);

void I_focusPoint(Image *img, int x, int y);
void I_zoomInit(Image *img);
void I_zoom(Image *img, double zoom_coef);
void I_move(Image *img, int x, int y);

void I_draw(Image *img);

void P_draw(Image *img, polygone *p);
void P_fill(Image *img, polygone *p);

void FindYMaxima(Image *img, int *ymin, int *ymax);

void ToFirstOctan(int xA, int yA, int xB, int yB, int *xA_1o, int *yA_1o, int *xB_1o, int *yB_1o);
void FromFirstOctan(int xA, int yA, int xB, int yB, int x_1o, int y_1o, int *x, int *y);
void I_bresenham(Image *img, int xA, int yA, int xB, int yB);

void Close(Image *img, polygone *p, point depart, point fin);

void I_drawCarre(Image *img, polygone *p, int selected);

void I_reset(Image *img, polygone *p);

#endif
