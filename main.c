
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

//------------------------------------------------------------------
//	C'est le display callback. A chaque fois qu'il faut
//	redessiner l'image, c'est cette fonction qui est
//	appelee. Tous les dessins doivent etre faits a partir
//	de cette fonction.
//------------------------------------------------------------------

void display_CB()
{
	Color white = C_new(255, 255, 255);
	I_changeColor(img, white);
	P_draw(img, p);
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
		P_nouveauSommet(p, x, y);
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
	case 'i':
		I_zoomInit(img);
		break;
	case 'c':
		if (closed)
		{
			closed = 0;
			Color black = C_new(0, 0, 0);
			I_changeColor(img, black);
			Close(img, p);
			Color white = C_new(255, 255, 255);
			I_changeColor(img, white);
			p->drawn_sommets = 0;
			P_draw(img, p);
		}
		else
		{
			closed = 1;
			Color white = C_new(255, 255, 255);
			I_changeColor(img, white);
			Close(img, p);
		}
		break;
	case 'f':
		if (full)
		{
			full = 0;
			closed = 0;
			Color black = C_new(0, 0, 0);
			I_fill(img, black);
			Color white = C_new(255, 255, 255);
			I_changeColor(img, white);
			p->drawn_sommets = 0;
			// P_draw(img, p);
		}
		else
		{
			full = 1;
			Color white = C_new(255, 255, 255);
			I_changeColor(img, white);
			Close(img, p);
			P_fill(img, p);
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
	// int mod = glutGetModifiers();

	int d = 10;

	switch (key)
	{
	case GLUT_KEY_UP:
		I_move(img, 0, -d);
		break;
	case GLUT_KEY_DOWN:
		I_move(img, 0, d);
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
