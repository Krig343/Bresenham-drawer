#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include "Image.h"

Image *img;
polygone *p;
int closed;			 // Toggle pour le passage de ligne brisée à polynome
int full;			 // Toggle pour le remplissage
int mode;			 // Switch pour le mode append, vertex et edge
int selected_vertex; // Sommet actuellement sélectionné
int selected_edge;	 // Arrête actuellement sélectionnée

//------------------------------------------------------------------
//	C'est le display callback. A chaque fois qu'il faut
//	redessiner l'image, c'est cette fonction qui est
//	appelee. Tous les dessins doivent etre faits a partir
//	de cette fonction.
//------------------------------------------------------------------

void display_CB()
{
	I_reset(img, p); // Draw le polygone de base sur fond noir
	if (closed)		 // Ajoute ou non la droite entre le premier et le dernier sommet
	{
		point depart = tete(p);
		point fin = queue(p);
		Color white = C_new(255, 255, 255);
		I_changeColor(img, white);
		Close(img, p, depart, fin);
	}
	if (full) // Ajoute ou non le remplissage
	{
		point fin = queue(p);
		point depart = tete(p);
		Color white = C_new(255, 255, 255);
		I_changeColor(img, white);
		Close(img, p, depart, fin); // La droie brisée est fermé avant le remplissage
		P_fill(img, p);
	}
	switch (mode)
	{
	case 2: // Mode vertex
	{
		Color red = C_new(255, 0, 0);
		I_changeColor(img, red);
		I_drawCarre(img, p, selected_vertex);
		break;
	}
	case 3: // Mode edge
	{
		point fin = ieme(p, selected_edge);
		point depart = ieme(p, selected_edge + 1);
		Color blue = C_new(0, 0, 255);
		I_changeColor(img, blue);
		Close(img, p, depart, fin); // Permet de redessiner une arrête précise d'une autre couleur
		break;
	}
	}
	I_draw(img); // Dessine l'image dans la fenêtre

	glutSwapBuffers();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir en fonction de la position de
// la souris (x,y), en fonction du bouton de la souris qui a été
// pressé ou relaché.
//------------------------------------------------------------------

void mouse_CB(int button, int state, int x, int y)
{
	switch (mode)
	{
	case 1: // Mode append
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
		{
			I_focusPoint(img, x, img->_height - y);
			if (vide(p))
				selected_vertex = 0; // Passe le sélecteur au premier sommet ajouté
			else
				selected_vertex++; // Empêche le décalage du carré lors de l'ajout
			if (longueur(p) == 1)
				selected_edge = 0; // Passe le sélecteur à la première arrête ajoutée
			else
				selected_edge++; // Empêche le décalage de la ligne lors de l'ajout
			p = P_nouveauSommet(p, x, y);
		}
		break;
	case 2: // Mode vertex
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
		{
			I_focusPoint(img, x, img->_height - y);
			selected_vertex = closestVertex(p, x, y);
		}
		break;
	case 3: // Mode edge
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
		{
			I_focusPoint(img, x, img->_height - y);
			selected_edge = closestEdge(p, x, y);
		}
		if ((button == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN))
		{
			I_focusPoint(img, x, img->_height - y);
			if (selected_edge < selected_vertex)
				selected_vertex++; // Empêche le décalage du carré lors de l'ajout
			p = P_addi(p, selected_edge + 1, x, y);
		}
		break;
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche (non-spéciale) du clavier.
//------------------------------------------------------------------

void keyboard_CB(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // échap
		exit(1);
		break;
	case 'z': // Zoom +
		I_zoom(img, 2.0);
		break;
	case 's': // Zoom -
		I_zoom(img, 0.5);
		break;
	case 'r': // Réinitialise le zoom
		I_zoomInit(img);
		break;
	case 'c': // Modifie le toogle du mode close
		if (closed)
			closed = 0;
		else
			closed = 1;
		break;
	case 'f': // Modifie le toggle du mode full
		if (full)
			full = 0;
		else
			full = 1;
		break;
	case 'i': // Passe en mode append
		if (mode == 1)
		{
			mode = 0;
			printf("--------------------\n");
			printf("Mode append désactivé\n");
			printf("Attention ! Aucun mode sélectionné\n");
		}
		else
		{
			mode = 1;
			printf("--------------------\n");
			printf("Mode append activé\n");
			printf("Mode vertex désactivé\n");
			printf("Mode edge désactivé\n");
		}
		break;
	case 'v': // Passe en mode verex
		if (mode == 2)
		{
			mode = 0;
			printf("--------------------\n");
			printf("Mode vertex désactivé\n");
			printf("Attention ! Aucun mode sélectionné\n");
		}
		else
		{
			if (selected_vertex != -1)
			{
				mode = 2;
				printf("--------------------\n");
				printf("Mode append désactivé\n");
				printf("Mode vertex activé\n");
				printf("Mode edge désactivé\n");
			}
			else
				printf("Aucun point dans le polynome. Mode vertex indisponible\n");
		}
		break;
	case 'e': // Passe en mode edge
		if (mode == 3)
		{
			mode = 0;
			printf("--------------------\n");
			printf("Mode edge désactivé\n");
			printf("Attention ! Aucun mode sélectionné\n");
		}
		else
		{
			if (selected_edge != -1)
			{
				mode = 3;
				printf("--------------------\n");
				printf("Mode append désactivé\n");
				printf("Mode vertex désactivé\n");
				printf("Mode edge activé\n");
			}
			else
				printf("Aucune arrête dans le polynome. Mode edge indisponible\n");
		}
		break;
	case 127: // suppr
		if (mode == 2)
		{
			if (p->suivant != NULL)
			{
				p = suppi(p, selected_vertex);
				selected_vertex--;
				selected_edge--;
			}
			else
				printf("Attention ! Le dernier point ne peut pas être supprimé\n");
		}
		break;
	default:
		fprintf(stderr, "keyboard_CB : %d : unknown key.\n", key);
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche spéciale (F1, F2 ... F12, home, end, insert,
// haut, bas, droite, gauche etc).
//------------------------------------------------------------------

void special_CB(int key, int x, int y)
{

	int d = 10;

	switch (mode)
	{
	case 2: // Mode vertex
		switch (key)
		{
		case GLUT_KEY_UP:
			p = moveVertex(p, selected_vertex, 0, -d);
			break;
		case GLUT_KEY_DOWN:
			p = moveVertex(p, selected_vertex, 0, d);
			break;
		case GLUT_KEY_LEFT:
			p = moveVertex(p, selected_vertex, -d, 0);
			break;
		case GLUT_KEY_RIGHT:
			p = moveVertex(p, selected_vertex, d, 0);
			break;
		case 104:
			selected_vertex = (selected_vertex + 1) % longueur(p);
			break;
		case 105:
			selected_vertex = (selected_vertex - 1 + longueur(p)) % longueur(p);
			break;
		default:
			fprintf(stderr, "special_CB : %d : unknown key.\n", key);
		}
		break;
	case 3: // Mode edge
		switch (key)
		{
		case 104:
			selected_edge = (selected_edge + 1) % (longueur(p) - 1);
			break;
		case 105:
			selected_edge = (selected_edge - 2 + longueur(p)) % (longueur(p) - 1);
			break;
		default:
			fprintf(stderr, "special_CB : %d : unknown key.\n", key);
		}
		break;
	default: // Mode append ou aucun mode
		switch (key)
		{
		case GLUT_KEY_UP:
			I_move(img, 0, d);
			break;
		case GLUT_KEY_DOWN:
			I_move(img, 0, -d);
			break;
		case GLUT_KEY_LEFT:
			I_move(img, d, 0);
			break;
		case GLUT_KEY_RIGHT:
			I_move(img, -d, 0);
			break;
		default:
			fprintf(stderr, "special_CB : %d : unknown key.\n", key);
		}
		break;
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage : %s <width> <height>\n", argv[0]);
		exit(1);
	}
	else
	{
		int largeur, hauteur;

		largeur = atoi(argv[1]);
		hauteur = atoi(argv[2]);
		img = I_new(largeur, hauteur);
		Color noir = C_new(0, 0, 0);
		I_fill(img, noir);

		int windowPosX = 100, windowPosY = 100;
		p = P_nouveau();
		closed = 0;
		full = 0;
		mode = 1;
		selected_vertex = -1;
		selected_edge = -1;

		glutInitWindowSize(largeur, hauteur);
		glutInitWindowPosition(windowPosX, windowPosY);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
		glutInit(&argc, argv);
		glutCreateWindow(argv[0]);

		glViewport(0, 0, largeur, hauteur);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glOrtho(0, largeur, hauteur, 0, -1, 1);

		glutDisplayFunc(display_CB);
		glutKeyboardFunc(keyboard_CB);
		glutSpecialFunc(special_CB);
		glutMouseFunc(mouse_CB);

		glutMainLoop();

		return 0;
	}
}
