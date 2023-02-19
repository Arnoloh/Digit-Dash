#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define BUFFER_SIZE 32
#define END_OF_FILE 0

enum colors {
    GREY,
    GREEN,
    RED
};
void progress_bar(size_t size, size_t w)
{
    size_t purcentage = (w * 100) / size;
    size_t to_write = purcentage / 5 ;
    size_t space = 20 - to_write;

    printf("[");

    while (to_write > 0)
    {
        printf("=");
        to_write--;
    }

    while (space > 0)
    {
        printf(" ");
        space--;
    }

    printf("] %lu%%\n\n", purcentage);

}
/**
  \brief Get char one by one in the standart input.
  */
int getch(void) 
{
    int ch;
    struct termios oldt;
    struct termios newt;

    tcgetattr(STDIN_FILENO, &oldt); /*store old settings */
    newt = oldt; /* copy old settings to new settings */
    newt.c_lflag &= ~(ICANON | ECHO); /* make one change to old settings in new settings */

    tcsetattr(STDIN_FILENO, TCSANOW, &newt); /*apply the new settings immediatly */

    ch = getchar(); /* standard getchar call */

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); /*reapply the old settings */

    return ch; /*return received char */
}

/**
  \brief Set the writing color.
  */
void set_color(int color)
{
    switch (color)
    {
        case GREEN:
            write(STDOUT_FILENO, "\033[1;32m", 8);
            break;
        case RED:
            write(STDOUT_FILENO, "\033[1;31m", 8);
            break;

        default:
            write(STDOUT_FILENO, "\033[0;30m", 8);
            break;
    }   
}

/**
  \brief Write the contents of the buffer in the file passed as parameter.
  */
void rewrite(int fd, char *buf, size_t count)
{
    size_t written = 0;

    do {
        buf += written;  
        written += write(fd, buf, count - written);

        if (written == -1)
            err(1, "error with rewrite");

    } while (count != written);
}

/**
  \brief Print the level on terminal.
  */
size_t print_level(int level, int curr)
{
    int rollback = 0;
    size_t len = 0;
    char char_curr;
    char char_level;

    char buffer[BUFFER_SIZE];

    lseek(level, 0, SEEK_SET);
    lseek(curr, 0, SEEK_SET);

    size_t readen = read(curr, &char_curr, 1);

    if (readen == (ssize_t)-1)
        errx(EXIT_FAILURE, "print_level: error with read (first)");

    while (readen != END_OF_FILE)
    {
        read(level, &char_level, 1);

        if (char_level == '\n')
        {
            rewrite(STDOUT_FILENO, &char_level, 1);
            lseek(curr, 1, SEEK_CUR);
            read(level, &char_level, 1);
        }

        if (char_curr == 0)
        {
            set_color(GREY);
            rewrite(STDOUT_FILENO, &char_level, 1);
            rollback -= 1;
            len++;
        }
        else
        {
            if (char_curr == char_level)
                set_color(GREEN);
            else
            {
                if (char_curr == ' ')
                    char_curr = char_level;

                set_color(RED);

            }
            len++;
            rewrite(STDOUT_FILENO, &char_curr, readen);
        }

        readen = read(curr, &char_curr, 1);

        if (readen == (ssize_t)-1)
            errx(EXIT_FAILURE, "print_level: error with read (first)");
    }

    set_color(GREY);
    readen = read(level, buffer, BUFFER_SIZE);

    while (readen != END_OF_FILE)
    {
        rewrite(STDOUT_FILENO, buffer, readen);  

        readen = read(level, buffer, BUFFER_SIZE);

        if (readen == (ssize_t)-1)
            errx(EXIT_FAILURE, "print_level: error with read (second)");
    }

    lseek(curr, rollback, SEEK_END);
    return len;
}

int main(int argc, char* argv[])
{
    char input;
    ssize_t readen;

    int level = open(argv[1], O_RDONLY);
    int curr  = open("curr_input.txt", O_RDWR | O_CREAT, 0666);
    size_t len = 0;
    size_t size = lseek(level,0,SEEK_END) - 1; 
    lseek(level,0,SEEK_SET);

    while(lseek(level, 0, SEEK_CUR) != lseek(curr, 0, curr))
    {
        system("clear");
        progress_bar(size,len);
        len = print_level(level, curr);


        input = getch();

        if (input == 127)
        {
            lseek(curr, -1, SEEK_CUR);
            input = 0;
        }

        rewrite(curr, &input, 1);
    }

    close(level);
    close(curr);

    printf("\n GREAT JOB !\n");

    if (remove("curr_input.txt") != 0)
        errx(EXIT_FAILURE, "main: error while remove file");

    return EXIT_SUCCESS;
}
