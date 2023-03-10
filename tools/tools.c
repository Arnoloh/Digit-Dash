#include "tools.h"

char *serialize_struct(void * buf,int id)
{
    char * serialize;
    switch (id)
    {
        case 1:
            serialize = malloc(sizeof(struct ID));
            memcpy(serialize,(struct ID*)buf,sizeof(struct ID));

    }
    return serialize;
}
