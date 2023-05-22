#include "help.h"

void display_little_name_help(WINDOW* frame, int largeur, int hauteur)
{

    int nomJeuX = largeur / 2 + 4;
    int nomJeuY = 2;

    wattron(frame, A_BOLD);
    mvwprintw(frame, nomJeuY, nomJeuX,      " ____   _       _  _      ____             _ ");
    mvwprintw(frame, nomJeuY + 1 , nomJeuX, "|    \\ |_| ___ |_|| |_   |    \\  ___  ___ | |_ ");
    mvwprintw(frame, nomJeuY + 2 , nomJeuX, "|  |  || || . || ||  _|  |  |  || .'||_ -||   |");
    mvwprintw(frame, nomJeuY + 3 , nomJeuX, "|____/ |_||_  ||_||_|    |____/ |__,||___||_|_|");
    mvwprintw(frame, nomJeuY + 4 , nomJeuX, "          |___|                                ");
    wattroff(frame, A_BOLD);
}

void display_help() 
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
    WINDOW *leftbox = subwin(mainwin, height - 10, width / 2 - 2, 9, 1);
    //wbkgd(leftbox, COLOR_PAIR(20));
    box(leftbox, 0, 0);
    mvwprintw(leftbox, 0, 2, "[ The game ]");
    mvwprintw(leftbox, 2, 2, "Welcome to Digit Chat, the ultimate typing game for hardcore coders!");
    mvwprintw(leftbox, 3, 2, "In this thrilling game, your objective is to type lines of code");
    mvwprintw(leftbox, 4, 2, "faster than your opponent to achieve victory. The brief description");

    // Dernière box avec les statistiques
    WINDOW *statsbox = subwin(mainwin, height - 10, width / 2, 9, width / 2 - 1);
    //wbkgd(statsbox, COLOR_PAIR(20));
    box(statsbox, 0, 0);
    mvwprintw(statsbox, 0, 2, "[ Rules ]");

    // Petite box avec barre de progression
    WINDOW *escape = subwin(mainwin, 3, width / 3, 1, 1);
    //wbkgd(progressbar, COLOR_PAIR(20));
    box(escape, 0, 0);
    mvwprintw(escape, 1, 1, " Press escape to return to the menu");

    // Affiche le nom du jeu
    display_little_name_help(mainwin, width, height);

    // Rafraîchir toutes les fenêtres
    refresh();
    wrefresh(mainwin);
    wrefresh(leftbox);
    wrefresh(statsbox);

    int exit = getch();

    while (exit != 27)
    {
        exit = getch();
    }

    // Nettoyage
    delwin(statsbox);
    delwin(escape);
    delwin(leftbox);
    delwin(mainwin);
}