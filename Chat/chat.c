#include "chat.h"

char buffer_seed[BUFFER_SIZE];
int req = 10;
char name[20];
pthread_mutex_t lock;
WINDOW *message_win, *input_win;
typedef struct thread_info
{
    int sockfd;
    int level_seed;
} thread_info;
void *read_from_server(void *arg)
{
    thread_info *info = arg;
    int sockfd = info->sockfd;
    char buffer[BUFFER_SIZE];

    while (1)
    {
        while (req != 0)
        {
            ssize_t received = recv(sockfd, buffer, BUFFER_SIZE, 0);
            if (received <= 0)
            {
                perror("Error reading from server");
                break;
            }
            buffer[received] = '\0';
            req = detectRequest(buffer);
            stockRequest(buffer, req);
            int i = 0;
            while (i < 20 && buffer[i] != '\0')
            {
                if (buffer[i] == ':')
                {
                    i = 14;
                    wattron(message_win, COLOR_PAIR(2));
                    break;
                }

                ++i;
            }
            pthread_mutex_lock(&lock);
            if (i != 14)
            {
                wattron(message_win, COLOR_PAIR(4));
            }
            wprintw(message_win, "%s\n", buffer);
            wattroff(message_win, COLOR_PAIR(2));
            wattroff(message_win, COLOR_PAIR(4)); // Remove color
            wrefresh(message_win);
            pthread_mutex_unlock(&lock);
        }

        if (req == 0)
        {
            strcpy(buffer_seed, buffer);

            info->level_seed = atoi(buffer_seed);
            break;
        }
    }

    pthread_exit(buffer_seed);
    return NULL;
}

void *write_to_server(void *arg)
{
    int sockfd = *(int *)arg;
    char buffer[BUFFER_SIZE] = {0};
    char formatted_buffer[BUFFER_SIZE] = {0};

    while (1)
    {
        mvwprintw(input_win, 0, 0, "%s: ", name);
        wrefresh(input_win);
        echo();
        wgetstr(input_win, buffer);
        werase(input_win);
        noecho();
        snprintf(formatted_buffer, sizeof(formatted_buffer), "%s", buffer);

        ssize_t sent = send(sockfd, formatted_buffer, strlen(formatted_buffer), 0);
        if (sent < 0)
        {
            perror("Error writing to server");
            break;
        }
        pthread_mutex_lock(&lock);
        wattron(message_win, COLOR_PAIR(1));
        wprintw(message_win, "%s: %s\n", name, buffer);
        wattroff(message_win, COLOR_PAIR(1));
        wrefresh(message_win);
        pthread_mutex_unlock(&lock);

        if (req == 0)
        {

            pthread_exit(NULL);
            return NULL;
        }
    }
    pthread_exit(NULL);
    return NULL;
}
int connect_to_server()
{
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error opening socket");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IP);
    server_addr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error connecting to server");
        return 1;
    }

    return sockfd;
}

char *u2u(int sockfd, bool name_already, unsigned int *level_seed)
{
    // Initialization of ncurses
    initscr();
    noecho();
    cbreak();

    // Get max window size
    int max_y = 0, max_x = 0;
    getmaxyx(stdscr, max_y, max_x);

    // Creation of the windows with borders
    WINDOW *message_win_border = newwin(max_y - 3, max_x, 0, 0);
    WINDOW *input_win_border = newwin(3, max_x, max_y - 3, 0);

    // Draw borders with color
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK); // New color pair for cyan
    wattron(message_win_border, COLOR_PAIR(3));
    wattron(input_win_border, COLOR_PAIR(3));
    box(message_win_border, 0, 0);
    box(input_win_border, 0, 0);
    wattron(message_win_border, COLOR_PAIR(4));
    wattron(input_win_border, COLOR_PAIR(1));
    wrefresh(message_win_border);
    wrefresh(input_win_border);

    // Creation of the windows for text
    message_win = derwin(message_win_border, max_y - 5, max_x - 2, 1, 1);
    scrollok(message_win, TRUE);
    input_win = derwin(input_win_border, 1, max_x - 2, 1, 1);

    // Socket setup and connection
    char buffer[BUFFER_SIZE];
    if (name_already == false)
    {
        ssize_t received = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (received > 0)
        {
            buffer[received] = '\0';
            wprintw(message_win, "%s\n", buffer);
            wrefresh(message_win);

            echo();
            wgetstr(input_win, name);
            noecho();

            ssize_t sent = send(sockfd, name, strlen(name), 0);
            if (sent < 0)
            {
                perror("Error writing to server");
            }
        }
    }
    pthread_t read_thread, write_thread;
    thread_info info = {sockfd, 0};
    if (pthread_create(&read_thread, NULL, read_from_server, (void *)&info) != 0)
    {
        perror("Error creating read thread");
        return 1;
    }

    if (pthread_create(&write_thread, NULL, write_to_server, (void *)&sockfd) != 0)
    {
        perror("Error creating write thread");
        return 1;
    }

    pthread_join(read_thread, &buffer_seed);
    pthread_cancel(write_thread);

    // lancement
    *level_seed = info.level_seed;

    // End of ncurses session
    delwin(message_win);
    delwin(message_win_border);
    delwin(input_win);
    delwin(input_win_border);
    endwin();
    endwin();
    return name;
}