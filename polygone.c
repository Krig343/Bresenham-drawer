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

float distance(int xA, int yA, int xB, int yB)
{
    return sqrt(pow(xB - xA, 2) + pow(yB - yA, 2));
}

int closestVertex(polygone *p, int x, int y)
{
    int i = 0;
    int compteur;
    float d1 = distance(x, y, p->sommet.x, p->sommet.y);
    float d2;
    int l = longueur(p);
    for (compteur = 1; compteur < l; compteur++)
    {
        p = p->suivant;
        d2 = distance(x, y, p->sommet.x, p->sommet.y);
        if (d2 < d1)
        {
            i = compteur;
            d1 = d2;
        }
    }
    return i;
}

int closestEdge(polygone *p, int x, int y)
{
    int iVertex = closestVertex(p, x, y);
    if (iVertex == 0)
        return iVertex;
    if (iVertex == longueur(p) - 1)
        return iVertex - 1;
    point pt = ieme(p, iVertex);
    point apres = ieme(p, iVertex + 1);
    float a = distance(x, y, apres.x, apres.y);
    float b = distance(x, y, pt.x, pt.y);
    float c = distance(pt.x, pt.y, apres.x, apres.y);
    float cosa = (pow(b, 2) + pow(c, 2) - pow(a, 2)) / (2 * b * c);
    float angle1 = acos(cosa);

    point avant = ieme(p, iVertex - 1);
    a = distance(x, y, avant.x, avant.y);
    c = distance(pt.x, pt.y, avant.x, avant.y);
    cosa = (pow(b, 2) + pow(c, 2) - pow(a, 2)) / (2 * b * c);
    float angle2 = acos(cosa);

    if (angle1 < angle2)
        return iVertex;
    return iVertex - 1;
}