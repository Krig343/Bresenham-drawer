#include "polygone.h"

polygone *P_nouveau()
{
    polygone *p = NULL;
    return p;
}

polygone *P_nouveauSommet(polygone *p, int x, int y)
{
    polygone *p1 = (polygone *)malloc(sizeof(polygone));
    point pt;
    pt.x = x;
    pt.y = y;
    p1->sommet = pt;
    p1->precedent = NULL;
    if (vide(p))
        p1->suivant = NULL;
    else
        p1->suivant = p;
    p = p1;
    return p;
}

int vide(polygone *p)
{
    return (p == NULL);
}

int longueur(polygone *p)
{
    return (vide(p) ? 0 : 1 + longueur(p->suivant));
}

int IsVertex(polygone *p, int x, int y)
{
    if (vide(p))
        return 0;
    else
    {
        if (p->sommet.x == x && p->sommet.y == y)
            return 1;
        else
            IsVertex(p->suivant, x, y);
    }
}

point tete(polygone *p)
{
    return p->sommet;
}

point queue(polygone *p)
{
    polygone *p1 = p;
    while (p1->suivant != NULL)
        p1 = p1->suivant;
    return p1->sommet;
}