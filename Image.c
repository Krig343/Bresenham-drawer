#include "Image.h"

Color C_new(float red, float green, float blue)
{
	Color c;
	c._red = red;
	c._green = green;
	c._blue = blue;
	return c;
}

Image *I_new(int width, int height)
{
	Image *img_new = (Image *)malloc(sizeof(Image));
	img_new->_width = width;
	img_new->_height = height;
	img_new->_xzoom = 0;
	img_new->_yzoom = 0;
	img_new->_zoom = 1.0;

	img_new->_xoffset = 0;
	img_new->_yoffset = 0;

	img_new->_current_color = C_new(255, 255, 255);

	img_new->_buffer = (Color **)calloc(width, sizeof(Color *));

	int x;
	for (x = 0; x < width; x++)
		img_new->_buffer[x] = (Color *)calloc(height, sizeof(Color));

	return img_new;
}

void I_fill(Image *img, Color c)
{
	int x, y;
	for (x = 0; x < img->_width; x++)
		for (y = 0; y < img->_height; y++)
			img->_buffer[x][y] = c;
}

void I_changeColor(Image *img, Color c)
{
	img->_current_color = c;
}

void I_plotColor(Image *img, int x, int y, Color c)
{
	if ((x >= 0) && (x < img->_width) &&
		(y >= 0) && (y < img->_height))
		img->_buffer[x][y] = c;
	else
	{
		fprintf(stderr, "I_plotColor : ERROR !!!\n");
		fprintf(stderr, "x (=%d) must be in the [%d,%d] range and\n", x, 0, img->_width);
		fprintf(stderr, "y (=%d) must be in the [%d,%d] range\n", y, 0, img->_height);
	}
}

static void _windowToImage(Image *img, int xwin, int ywin, int *ximg, int *yimg)
{

	*ximg = img->_xoffset + img->_xzoom + (xwin - img->_xzoom) / img->_zoom;
	*yimg = img->_yoffset + img->_yzoom + (ywin - img->_yzoom) / img->_zoom;
}

void I_focusPoint(Image *img, int xwin, int ywin)
{
	int dx = xwin - img->_xzoom;
	int dy = ywin - img->_yzoom;
	img->_xoffset -= dx * (1 - 1.0 / img->_zoom);
	img->_yoffset -= dy * (1 - 1.0 / img->_zoom);
	img->_xzoom = xwin;
	img->_yzoom = ywin;
}

void I_zoomInit(Image *img)
{
	img->_xoffset = 0;
	img->_yoffset = 0;
	img->_zoom = 1.0;
}

void I_zoom(Image *img, double zoom_coef)
{
	img->_zoom = img->_zoom * zoom_coef;
}

void I_move(Image *img, int x, int y)
{
	img->_xoffset += x;
	img->_yoffset += y;
}

void I_draw(Image *img)
{
	glBegin(GL_POINTS);
	int xwin, ywin, ximg, yimg;
	for (xwin = 0; xwin < img->_width; xwin++)
		for (ywin = 0; ywin < img->_height; ywin++)
		{
			_windowToImage(img, xwin, ywin, &ximg, &yimg);
			Color c;
			if ((ximg >= 0) && (ximg < img->_width) &&
				(yimg >= 0) && (yimg < img->_height))
				c = img->_buffer[ximg][yimg];
			else
				c = C_new(0, 0, 0);

			glColor3f(c._red, c._green, c._blue);
			glVertex2i(xwin, ywin);
		}
	glEnd();
}

void P_draw(Image *img, polygone *p)
{
	if (longueur(p) > 1) // On ne draw pas s'il n'y a qu'un seul point
	{
		while (p->suivant != NULL) // Draw des sommet 2 par 2
		{
			int xA = p->sommet.x;
			int yA = p->sommet.y;
			p = p->suivant;
			int xB = p->sommet.x;
			int yB = p->sommet.y;
			I_bresenham(img, xA, yA, xB, yB);
		}
	}
}

// Test si deux couleurs sont identiques
int TestColor(Color c1, Color c2)
{
	if ((c1._red == c2._red) && (c1._green == c2._green) && (c1._blue == c2._blue))
		return 1;
	return 0;
}

void FindYMaxima(Image *img, int *ymin, int *ymax)
{
	Color white = C_new(255, 255, 255);
	// initialisation de tampon
	int max = 0;
	int min = img->_height;

	int i;
	int j = 0;
	/* 	Test de couleur ligne après ligne jusqu'à ce que le min change.
	*	C'est à dire jusqu'à ce qu'un pixel blanc soit rencontré.
	*	En partant du bas, le premier pixel blanc rencontré est forcément
	*	le minimum.
	*/
	while (min == img->_height && j != img->_height)
	{
		i = 0;
		while (min == img->_height && i < img->_width)
		{
			Color cur_pixel = img->_buffer[i][j];
			if (TestColor(cur_pixel, white))
				min = j;
			else
				i++;
		}
		j++;
	}
	j = img->_height;
	/* 	Test de couleur ligne après ligne jusqu'à ce que le max change.
	*	C'est à dire jusqu'à ce qu'un pixel blanc soit rencontré.
	*	En partant du haut, le premier pixel blanc rencontré est forcément
	*	le maximum.
	*/
	while (max == 0 && j != 0)
	{
		i = 0;
		while (max == 0 && i < img->_width)
		{
			Color cur_pixel = img->_buffer[i][j];
			if (TestColor(cur_pixel, white))
				max = j;
			else
				i++;
		}
		j--;
	}
	*ymin = min;
	*ymax = max;
}

void P_fill(Image *img, polygone *p)
{
	int ymin, ymax, i, j, k;
	Color white = C_new(255, 255, 255);
	Color black = C_new(0, 0, 0);
	Color latest_color = black;		// Définie la couleur de base comme étant celle du fond
	FindYMaxima(img, &ymin, &ymax); // Cherche la hauteur max et min du polygone
	for (j = ymin; j < ymax; j++)
	{
		point intersection[img->_width]; // Créé un tableau de point de la taille de la largeur de l'image
		int compteur = 0;				 // Initialise le compteur de points retenus à 0
		int first = 1;					 // Initialise le toggle du premier élément à 0
		for (i = 0; i < img->_width; i++)
		{
			Color cur_pixel = img->_buffer[i][j];
			/* Si le pixel courant n'a pas la même couleur que la couleur de base et qu'il est blanc 
			 * alors, s'il n'est pas un sommet, on l'ajoute et on définie la couleur de base comme
			 * étant celle du pixel.
			 */
			if (!TestColor(cur_pixel, latest_color) && TestColor(cur_pixel, white))
			{
				if (first) // Permet d'ajouter les sommets qui ne sont pas des maxima locaux
				{
					first = 0;
					intersection[compteur].x = i;
					compteur++;
					latest_color = cur_pixel;
				}
				else if (!IsVertex(p, i, j))
				{
					intersection[compteur].x = i;
					compteur++;
					latest_color = cur_pixel;
				}
			}
			else
			{
				/* Si le pixel courant n'a pas la même couleur que la couleur de base et qu'il est noir
				 * alors la couleur de base devient le noir. Cela signifie que l'on a quitté le point
				 * our l'arrête du polygone pour repasser sur le fond.
				 */
				if (!TestColor(cur_pixel, latest_color) && TestColor(cur_pixel, black))
					latest_color = cur_pixel;
			}
		}
		// Pour tous les points entre deux coordonnées succesives du tableau, on ajoute les points à dessiner
		for (i = 0; i < compteur - 1; i += 2)
		{
			for (k = intersection[i].x; k < intersection[i + 1].x; k++)
				I_plotColor(img, k, j, white);
		}
	}
}

// Algorithme vu en TD
void ToFirstOctan(int xA, int yA, int xB, int yB, int *xA_1o, int *yA_1o, int *xB_1o, int *yB_1o)
{
	int xA_1q, yA_1q, xB_1q, yB_1q;
	if (xB > xA)
	{
		xA_1q = xA;
		xB_1q = xB;
	}
	else
	{
		xA_1q = -xA;
		xB_1q = -xB;
	}
	if (yB > yA)
	{
		yA_1q = yA;
		yB_1q = yB;
	}
	else
	{
		yA_1q = -yA;
		yB_1q = -yB;
	}
	if (fabs(xB - xA) > fabs(yB - yA))
	{
		*xA_1o = xA_1q;
		*xB_1o = xB_1q;
		*yA_1o = yA_1q;
		*yB_1o = yB_1q;
	}
	else
	{
		*xA_1o = yA_1q;
		*yA_1o = xA_1q;
		*xB_1o = yB_1q;
		*yB_1o = xB_1q;
	}
}

// Algorithme vu en TD
void FromFirstOctan(int xA, int yA, int xB, int yB, int x_1o, int y_1o, int *x, int *y)
{
	int x_1q, y_1q;
	if (fabs(xB - xA) > fabs(yB - yA))
	{
		x_1q = x_1o;
		y_1q = y_1o;
	}
	else
	{
		x_1q = y_1o;
		y_1q = x_1o;
	}
	if (xB > xA)
		*x = x_1q;
	else
		*x = -x_1q;
	if (yB > yA)
		*y = y_1q;
	else
		*y = -y_1q;
}

// Algorithme vu en TD
void I_bresenham(Image *img, int xA, int yA, int xB, int yB)
{
	Color c = img->_current_color;
	int xAo, yAo, xBo, yBo, x, y;
	ToFirstOctan(xA, yA, xB, yB, &xAo, &yAo, &xBo, &yBo);
	int i;
	int j = yAo;
	int deltaX = xBo - xAo;
	int deltaY = yBo - yAo;
	int incrd1 = 2 * deltaY;
	int incrd2 = 2 * (deltaY - deltaX);
	int critere = 2 * deltaY - deltaX;
	for (i = xAo; i < xBo; i++)
	{
		FromFirstOctan(xA, yA, xB, yB, i, j, &x, &y);
		I_plotColor(img, x, y, c);
		if (critere < 0)
			critere += incrd1;
		else
		{
			critere += incrd2;
			j++;
		}
	}
}

// Il s'agit d'un P_draw particulier qui ne dessine qu'un seul trait allant de fin à depart
void Close(Image *img, polygone *p, point depart, point fin)
{
	int xA = fin.x;
	int yA = fin.y;
	int xB = depart.x;
	int yB = depart.y;
	I_bresenham(img, xA, yA, xB, yB);
}

void I_drawCarre(Image *img, polygone *p, int selected)
{
	point pt = ieme(p, selected); // Coordonnées du point sélectionné
	Color c = img->_current_color;
	int i, j;
	// Si les contours du carrés sont dans l'image, dessine un carré de 81x81 px autour du point
	for (i = pt.x - 4; i < pt.x + 4; i++)
	{
		for (j = pt.y - 4; j < pt.y + 4; j++)
		{
			if ((i >= 0) && (i < img->_width) &&
				(j >= 0) && (j < img->_height))
				I_plotColor(img, i, j, c);
		}
	}
}

void I_reset(Image *img, polygone *p)
{
	Color black = C_new(0, 0, 0);
	I_changeColor(img, black);
	I_fill(img, black); // Rempli l'image de noir
	Color white = C_new(255, 255, 255);
	I_changeColor(img, white);
	P_draw(img, p); // Dessine le polygone
}