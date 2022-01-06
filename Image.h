#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

#include "Ppm.h"
#include "polygone.h"

typedef struct
{
	float _red, _green, _blue;
} Color;

Color C_new(float red, float green, float blue);
void C_check(Color c, char *message);

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
Image *I_read(char *ppmfilename);

void I_fill(Image *img, Color c);
void I_checker(Image *img, Color c1, Color c2, int step);

void I_changeColor(Image *img, Color c);
void I_plot(Image *img, int x, int y);
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

void Close(Image *img, polygone *p);

#endif
