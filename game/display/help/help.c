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
    WINDOW *leftbox = subwin(mainwin, height - 40, width / 2 - 2, 9, 1);
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
    mvwprintw(statsbox, 2, 2, "Here are the Digit Chat rules:");
    mvwprintw(statsbox, 4, 2, "1) Speed: The key to victory is speed.");
    mvwprintw(statsbox, 5, 4, "You will have to type the lines of code");
    mvwprintw(statsbox, 6, 4, "as quickly as possible to accumulate points");
    mvwprintw(statsbox, 7, 4, "and gain the advantage over your opponent.");
    mvwprintw(statsbox, 9, 2, "2) Accuracy: Accuracy is also crucial.");
    mvwprintw(statsbox, 10, 4, "Typing errors will penalize you by reducing your score.");
    mvwprintw(statsbox, 11, 4, "Be careful and make sure you type each character correctly.");
    mvwprintw(statsbox, 13, 2, "3) Advance without return: Once you have crossed a line of code,");
    mvwprintw(statsbox, 14, 4, "there is no going back. Be sure to read and understand each line");
    mvwprintw(statsbox, 15, 4, "before committing, as any mistakes will be final.");


    // Petite box avec barre de progression
    WINDOW *escape = subwin(mainwin, 3, width / 3, 1, 1);
    //wbkgd(progressbar, COLOR_PAIR(20));
    box(escape, 0, 0);
    mvwprintw(escape, 1, 1, " Press escape to return to the menu");

    // Petite box avec barre de progression
    WINDOW *help = subwin(mainwin, 10, width / 2 - 2, height - 31, 1);
    //wbkgd(progressbar, COLOR_PAIR(20));
    box(help, 0, 0);
    mvwprintw(help, 0, 2, "[ How to play multiplayer ]");
    mvwprintw(help, 3, 2, "Select multiplayer in menu.");
    mvwprintw(help, 4, 2, "A chat box will appear, wait for another player.");
    mvwprintw(help, 5, 2, "When two players are present, type \"ready\" in the chat.");
    mvwprintw(help, 6, 2, "When both players are ready the game starts automatically.");


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