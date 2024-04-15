#include <ncurses.h>

void createEmptyWindow() {
		// Création de la fenêtre
		WINDOW *window = newwin(10, 30, 5, 5);
		box(window, 0, 0);
		wrefresh(window);

		// Boucle infinie pour maintenir la fenêtre ouverte
		int ch;
		while ((ch = getch()) != KEY_F(1)) {
				// Affichage d'un message pour chaque touche pressée
				mvprintw(0, 0, "Touche pressée : %d", ch);
				refresh();
		}

		// Nettoyage et fermeture de ncurses
		delwin(window);
		endwin();
}

