#include "ui.h"

unsigned int level_seed;

const char *boutons[NB_BOUTONS] = {
    "Entraînement",
    "Multijoueur",
    "Chat",
    "Aide",
    "Quitter",
};

void display_name(int largeur, int hauteur)
{

    int nomJeuX = (largeur / 2) - 25; // Calcul de la position X pour centrer le texte "Digit Dash"
    int nomJeuY = (hauteur - NB_BOUTONS) / 2 - 10;

    mvprintw(nomJeuY, nomJeuX, "  _____  _       _ _     _____            _        ");
    mvprintw(nomJeuY + 1, nomJeuX, " |  __ \\(_)     (_) |   |  __ \\          | |     ");
    mvprintw(nomJeuY + 2, nomJeuX, " | |  | |_  __ _ _| |_  | |  | | __ _ ___| |__     ");
    mvprintw(nomJeuY + 3, nomJeuX, " | |  | | |/ _` | | __| | |  | |/ _` / __| '_ \\   ");
    mvprintw(nomJeuY + 4, nomJeuX, " | |__| | | (_| | | |_  | |__| | (_| \\__ \\ | | | ");
    mvprintw(nomJeuY + 5, nomJeuX, " |_____/|_|\\__, |_|\\__| |_____/ \\__,_|___/_| |_|");
    mvprintw(nomJeuY + 6, nomJeuX, "            __/ |                                  ");
    mvprintw(nomJeuY + 7, nomJeuX, "           |___/                                   ");
}

void display_menu(int largeur, int hauteur)
{
    // Initialisation des couleurs
    init_pair(1, COLOR_WHITE, COLOR_MAGENTA); // Couleur du cadre
    init_pair(2, COLOR_WHITE, COLOR_BLACK);   // Couleur du texte
    init_pair(3, COLOR_BLACK, COLOR_WHITE);   // Couleur du bouton sélectionné

    // Efface l'écran et met à jour l'affichage
    clear();
    refresh();

    // Affichage du cadre
    box(stdscr, 0, 0);

    // Affichage du nom du jeu en haut au centre
    display_name(largeur, hauteur);

    // Affichage des boutons
    int x = (largeur - 12) / 2;         // Position X des boutons pour les centrer
    int y = (hauteur - NB_BOUTONS) / 2; // Position Y des boutons pour les centrer

    int choix = 0;
    int boutonSelectionne = 0;

    while (1)
    {
        // Affichage des boutons
        for (int i = 0; i < NB_BOUTONS; i++)
        {
            if (i == boutonSelectionne)
                attron(COLOR_PAIR(3));

            mvprintw(y + i, x, "            "); // Efface le bouton précédent
            mvprintw(y + i, x, " *%s ", boutons[i]);

            if (i == boutonSelectionne)
                attroff(COLOR_PAIR(3));
        }

        refresh();

        choix = getch();

        switch (choix)
        {
        case KEY_UP:
            // Déplacement vers le bouton précédent
            boutonSelectionne = (boutonSelectionne + NB_BOUTONS - 1) % NB_BOUTONS;
            break;
        case KEY_DOWN:
            // Déplacement vers le bouton suivant
            boutonSelectionne = (boutonSelectionne + 1) % NB_BOUTONS;
            break;
        case '\n':
            // Action du bouton sélectionné
            attron(COLOR_PAIR(2));
            mvprintw(y + NB_BOUTONS + 2, x - 6, "Vous avez choisi %s", boutons[boutonSelectionne]);
            attroff(COLOR_PAIR(2));
            break;
        }

        refresh();

        // Sortie de la boucle lorsque le bouton Quitter est sélectionné
        if (choix == '\n')
            break;
    }

    clear();
    refresh();

    if (boutonSelectionne == 0)
    {
        start();
        display_menu(largeur, hauteur);
    }
    else if (boutonSelectionne == 1)
    {
        char *name = NULL;
        int cfd = connect_to_server();
    end_game:
        name = u2u(cfd, name);
        Player *player = new_player(name);
        int dict_size = 0;
        DictEntry *dict = generate_dict("game/find_word/database/c.txt", &dict_size);
        if (!dict)
        {
            return; // Failed to generate the dictionary
        }
        srand(level_seed);
        char **lines = generate_lines(dict, dict_size, 5);

        run(player, lines, 5);
        req = 1;
        goto end_game;
    }
}

void menu()
{
    // Initialisation de la librairie ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();

    // Calcul des dimensions de la fenêtre
    int hauteur, largeur;
    getmaxyx(stdscr, hauteur, largeur);

    // Affichage du menu du jeu
    display_menu(largeur, hauteur);

    // Nettoyage et fermeture de la librairie ncurses
    endwin();
}