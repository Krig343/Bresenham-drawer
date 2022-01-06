#ifndef __LISTE_H__
#define __LISTE_H__

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

typedef struct
{
    int x, y;
} point;

typedef struct strliste
{
    point sommet;
    struct strliste *suivant;
    struct strliste *precedent;
} Strliste, polygone;

polygone *P_nouveau();

polygone *P_nouveauSommet(polygone *p, int x, int y);

int vide(polygone *p);

int longueur(polygone *p);

int IsVertex(polygone *p, int x, int y);

point tete(polygone *p);

point queue(polygone *p);

point ieme(polygone *p, int i);

polygone *suppi(polygone *p, int i);

polygone *moveVertex(polygone *p, int i, int dx, int dy);

polygone *P_addi(polygone *p, int i, int x, int y);

#endif