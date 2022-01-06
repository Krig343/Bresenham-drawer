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
    {
        p->precedent = p1;
        p1->suivant = p;
    }
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

point ieme(polygone *p, int i)
{
    while (i > 0)
    {
        p = p->suivant;
        i--;
    }
    return p->sommet;
}

polygone *suppi(polygone *p, int i)
{
    polygone *p1 = p;
    while (i > 0)
    {
        p1 = p1->suivant;
        i--;
    }
    if (p1->precedent == NULL)
    {
        p1->suivant->precedent = NULL;
        p = p1->suivant;
    }
    else
    {
        if (p1->suivant == NULL)
            p1->precedent->suivant = NULL;
        else
        {
            p1->precedent->suivant = p1->suivant;
            p1->suivant->precedent = p1->precedent;
        }
    }
    free(p1);
    return p;
}

polygone *moveVertex(polygone *p, int i, int dx, int dy)
{
    polygone *p1 = p;
    while (i > 0)
    {
        p1 = p1->suivant;
        i--;
    }
    p1->sommet.x += dx;
    p1->sommet.y += dy;
    return p;
}

polygone *P_addi(polygone *p, int i, int x, int y)
{
    if (i == 0)
        P_nouveauSommet(p, x, y);
    polygone *p1 = p;
    polygone *p2;
    while (i > 0)
    {
        p1 = p1->suivant;
        i--;
    }
    p2 = p1->precedent;
    p1 = P_nouveauSommet(p1, x, y);
    p1->precedent = p2;
    p2->suivant = p1;
    return p;
}