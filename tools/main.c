#include "tools.h"



int main(void) {
    struct ID hello = {1,"My name is Arnaud"};
    char *str = serialize_struct ((void *)&hello,hello.id);
    printf("result: %s\n",str);
    free(str);
    return 0;

}
