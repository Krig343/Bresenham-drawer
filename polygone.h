#ifndef __LISTE_H__
#define __LISTE_H__

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <math.h>

// Couple d'entier pour représenter un point
typedef struct
{
    int x, y;
} point;

// Liste doublement chainée comme structure du polygone
typedef struct strliste
{
    point sommet;
    struct strliste *suivant;
    struct strliste *precedent;
} Strliste, polygone;

polygone *P_nouveau(); // Créé un nouveau polygone vide

polygone *P_nouveauSommet(polygone *p, int x, int y); // Créé un point (x,y) et l'ajoute en tête de liste

int vide(polygone *p); // Vérifie si un polynome est vide

int longueur(polygone *p); // Calcule le nombre de point dans un polygone

int IsVertex(polygone *p, int x, int y); // Vérifie si un point appartient à un polynome

point tete(polygone *p); // Renvoie l'élément en tête de liste

point queue(polygone *p); // Renvoie l'élément en fin de liste

point ieme(polygone *p, int i); // Renvoie de ieme élément

polygone *suppi(polygone *p, int i); // Supprime le point en position i

polygone *moveVertex(polygone *p, int i, int dx, int dy); // Déplace un sommet

polygone *P_addi(polygone *p, int i, int x, int y); // Créé un point (x,y) et l'ajoute à la position i

int closestVertex(polygone *p, int x, int y); // Renvoie l'indice du sommet le plus proche de (x,y)

int closestEdge(polygone *p, int x, int y); // Renvoie l'indice de l'arrête la plus proche de (x,y)

#endif