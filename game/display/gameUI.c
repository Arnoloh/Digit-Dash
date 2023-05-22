#include "gameUI.h"

void display_little_name(WINDOW* frame, int largeur, int hauteur)
{

    int nomJeuX = largeur / 2 + 5;
    int nomJeuY = 2;

    wattron(frame, A_BOLD);
    mvwprintw(frame, nomJeuY, nomJeuX,      " ____   _       _  _      ____             _ ");
    mvwprintw(frame, nomJeuY + 1 , nomJeuX, "|    \\ |_| ___ |_|| |_   |    \\  ___  ___ | |_ ");
    mvwprintw(frame, nomJeuY + 2 , nomJeuX, "|  |  || || . || ||  _|  |  |  || .'||_ -||   |");
    mvwprintw(frame, nomJeuY + 3 , nomJeuX, "|____/ |_||_  ||_||_|    |____/ |__,||___||_|_|");
    mvwprintw(frame, nomJeuY + 4 , nomJeuX, "          |___|                                ");
    wattroff(frame, A_BOLD);
}


void display_win(WINDOW** all_win) 
{
    // Couleurs
    start_color();
    init_pair(10, COLOR_WHITE, COLOR_BLACK);
    init_pair(20, COLOR_MAGENTA, COLOR_BLACK);

    // Calcul des dimensions
    int height, width;
    getmaxyx(stdscr, height, width);

    // Fenêtre principale
    WINDOW *mainwin = newwin(height, width, 0, 0);
    //wbkgd(mainwin, COLOR_PAIR(10));
    box(mainwin, 0, 0);
    

    // Grande box à gauche
    WINDOW *leftbox = subwin(mainwin, height - 6, width / 2 - 2, 5, 1);
    //wbkgd(leftbox, COLOR_PAIR(20));
    box(leftbox, 0, 0);
    mvwprintw(leftbox, 0, 2, "[ Page de code | 1/10 ]");

    // Petite box avec barre de progression
    WINDOW *progressbar = subwin(mainwin, 4, width / 2 - 2, 1, 1);
    //wbkgd(progressbar, COLOR_PAIR(20));
    box(progressbar, 0, 0);

    // Dernière box avec les statistiques
    WINDOW *statsbox = subwin(mainwin, height - 10, width / 2, 9, width / 2 - 1);
    //wbkgd(statsbox, COLOR_PAIR(20));
    box(statsbox, 0, 0);

    // Affiche le nom du jeu
    display_little_name(mainwin, width, height);

    // Rafraîchir toutes les fenêtres
    refresh();
    wrefresh(mainwin);
    wrefresh(leftbox);
    wrefresh(progressbar);
    wrefresh(statsbox);

    all_win[0] = mainwin;
    all_win[1] = leftbox;
    all_win[2] = progressbar;
    all_win[3] = statsbox;

    // Nettoyage
    /*delwin(statsbox);
    delwin(progressbar);
    delwin(leftbox);
    delwin(mainwin);
    endwin();*/
}