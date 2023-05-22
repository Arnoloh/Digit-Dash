#include "tools.h"

typedef struct
{
    Generic info;
    char test;
    int in_info;
} info_game;

char *serialize(Generic *buf)
{
    switch (buf->type)
    {
    case Player_Info:
    {
        info_game *game = (info_game *)buf;
        char *str = malloc(sizeof(char) * (snprintf(NULL, 0, "%i,%c,%i", game->info.id, game->test, game->in_info) + 1));
        snprintf(str, sizeof(char) * (snprintf(NULL, 0, "%i,%c,%i", game->info.id, game->test, game->in_info) + 1), "%i,%c,%i", game->info.id, game->test, game->in_info);
        return str;
    }
    case Chat:
    {
        Chat_info *game = (Chat_info *)buf;
        char *str = malloc(sizeof(char) * (snprintf(NULL, 0, "%i,\"%s\",\"%s\"", game->info.id, game->Message, game->name) + 1));
        snprintf(str, sizeof(char) * (snprintf(NULL, 0, "%i,\"%s\",\"%s\"", game->info.id, game->Message, game->name) + 1), "%i,\"%s\",\"%s\"", game->info.id, game->Message, game->name);
        return str;
    }
    default:
        return NULL;
    }
}

Generic *deserialize(const char *str)
{
    int type_id;

    type_id = str[0] - '0';

    if (type_id == 1) // Chat_info
    {
        Chat_info *chat = malloc(sizeof(Chat_info));

        chat->Message = malloc(100 * sizeof(char));
        chat->name = malloc(20 * sizeof(char));

        sscanf(str, "%i,\"%99[^\"]\",\"%19[^\"]\"", &chat->info.id, chat->Message, chat->name);

        chat->info.type = Chat;

        return (Generic *)chat;
    }
    else
    {
        return NULL;
    }
}

// int main()
// {
//     Generic gen = {Chat,Chat};
//     Chat_info game = {gen,"Hello World !"};
//     char *str = serialize ((Generic *) &game);
//     printf("%s\n",str);
//     Generic *deserialized = deserialize(str);
//     if (deserialized->type == Game_info)
//     {
//         info_game *game = (info_game *)deserialized;
//         printf("Game structure:\n");
//         printf("ID: %d\n", game->info.id);
//         printf("Test: %c\n", game->test);
//         printf("In_info: %d\n", game->in_info);
//     }
//     else if (deserialized->type == Chat)
//     {
//         Chat_info *chat = (Chat_info *)deserialized;
//         printf("\nChat structure:\n");
//         printf("ID: %d\n", chat->info.id);
//         printf("Message: %s\n", chat->Message);
//     }

//     free(((Chat_info *)deserialized)->Message);
//     free(deserialized);
//     free(str);

//     return 0;
// }
