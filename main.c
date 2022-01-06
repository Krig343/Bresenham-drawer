
/*===============================================================*\

	Arash Habibi

	main.c

	Un programme equivalent à 02_glut.c et qui ne prend en compte
	que trois événements pour quitter le programme.

\*===============================================================*/

#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include "Image.h"

Image *img;
polygone *p;
int closed;
int full;
int mode;
int selected_vertex;

//------------------------------------------------------------------
//	C'est le display callback. A chaque fois qu'il faut
//	redessiner l'image, c'est cette fonction qui est
//	appelee. Tous les dessins doivent etre faits a partir
//	de cette fonction.
//------------------------------------------------------------------

void display_CB()
{
	I_reset(img, p);
	if (closed)
	{
		point fin = queue(p);
		point depart = tete(p);
		Color white = C_new(255, 255, 255);
		I_changeColor(img, white);
		Close(img, p, depart, fin);
	}
	if (full)
	{
		point fin = queue(p);
		point depart = tete(p);
		Color white = C_new(255, 255, 255);
		I_changeColor(img, white);
		Close(img, p, depart, fin);
		P_fill(img, p);
	}
	switch (mode)
	{
	case 2:
	{
		Color red = C_new(255, 0, 0);
		I_changeColor(img, red);
		I_drawCarre(img, p, selected_vertex);
		break;
	}
	case 3:
		/* code */
		break;
	default:
		break;
	}
	I_draw(img);

	glutSwapBuffers();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir en fonction de la position de
// la souris (x,y), en fonction du bouton de la souris qui a été
// pressé ou relaché.
//------------------------------------------------------------------

void mouse_CB(int button, int state, int x, int y)
{
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		I_focusPoint(img, x, img->_height - y);
		if (mode == 1)
		{
			if (vide(p))
				selected_vertex = 0;
			else
				selected_vertex++;
			p = P_nouveauSommet(p, x, y);
		}
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche (non-spéciale) du clavier.
//------------------------------------------------------------------

void keyboard_CB(unsigned char key, int x, int y)
{
	// fprintf(stderr,"key=%d\n",key);
	switch (key)
	{
	case 27:
		exit(1);
		break;
	case 'z':
		I_zoom(img, 2.0);
		break;
	case 's':
		I_zoom(img, 0.5);
		break;
	case 'r':
		I_zoomInit(img);
		break;
	case 'c':
		if (closed)
			closed = 0;
		else
			closed = 1;
		break;
	case 'f':
		if (full)
			full = 0;
		else
			full = 1;
		break;
	case 'i':
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
	case 'v':
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
	case 'e':
		if (mode == 3)
		{
			mode = 0;
			printf("--------------------\n");
			printf("Mode edge désactivé\n");
			printf("Attention ! Aucun mode sélectionné\n");
		}
		else
		{
			mode = 3;
			printf("--------------------\n");
			printf("Mode append désactivé\n");
			printf("Mode vertex désactivé\n");
			printf("Mode edge activé\n");
		}
		break;
	case 127:
		if (mode == 2)
		{
			if (p->suivant != NULL)
			{
				p = suppi(p, selected_vertex);
				selected_vertex--;
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

	if (mode == 2)
	{
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
	}
	else
	{
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
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if ((argc != 3) && (argc != 2))
	{
		fprintf(stderr, "\n\nUsage \t: %s <width> <height>\nou", argv[0]);
		fprintf(stderr, "\t: %s <ppmfilename> \n\n", argv[0]);
		exit(1);
	}
	else
	{
		int largeur, hauteur;
		if (argc == 2)
		{
			img = I_read(argv[1]);
			largeur = img->_width;
			hauteur = img->_height;
		}
		else
		{
			largeur = atoi(argv[1]);
			hauteur = atoi(argv[2]);
			img = I_new(largeur, hauteur);
			Color noir = C_new(0, 0, 0);
			I_fill(img, noir);
		}
		int windowPosX = 100, windowPosY = 100;
		p = P_nouveau();
		closed = 0;
		full = 0;
		mode = 1;
		selected_vertex = -1;

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
		// glutMotionFunc(mouse_move_CB);
		// glutPassiveMotionFunc(passive_mouse_move_CB);

		glutMainLoop();

		return 0;
	}
}
