#ifndef GET_PAGE_H
#define GET_PAGE_H

void help();
void client();
void welcome();
void MainMenu();
void query(char message[]);
int __atoi(char message[]);
int detectRequest(char message[]);
void stockRequest(char message[], int req);
void rewrite(int fd, const void *buf, size_t count);

#endif
