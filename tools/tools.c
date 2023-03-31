#include "tools.h"
enum TYPE{
    Game,
    Chat,
};
typedef struct{
    enum TYPE type;
    int id;
} Generic;
typedef struct{
    Generic info;
    char * Message;
} Chat_info;
typedef struct{
    Generic info;
    char test;
    int in_info;
} info_game;
#define CAST_GENERIC_PTR(generic_ptr) \
    ((generic_ptr)->type == Game ? (info_game *)(generic_ptr) : \
     (generic_ptr)->type == Chat ? (Chat_info *)(generic_ptr) : NULL)
char *serialize(Generic *buf)
{
    switch (buf->type){
        case Game:
            {
                info_game *game = (info_game *) buf;
                char* str = malloc(sizeof(char) * (snprintf(NULL, 0, "%i,%c,%i", game->info.id, game->test, game->in_info) + 1));
                sprintf(str, "%i,%c,%i", game->info.id, game->test, game->in_info); 
                return str;
            }
        case Chat:
            {
                Chat_info *game = (Chat_info *) buf;
                char* str = malloc(sizeof(char) * (snprintf(NULL, 0, "%i,\"%s\"", game->info.id, game->Message) + 1));
                sprintf(str, "%i,\"%s\"", game->info.id, game->Message); 
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

    if (type_id == 0) // info_game
    {
        info_game *game = malloc(sizeof(info_game));
        sscanf(str, "%d,%c,%i", &game->info.id, &game->test, &game->in_info);

        game->info.type = Game;

        return (Generic *)game;
    }
    else if (type_id == 1) // Chat_info
    {
        Chat_info *chat = malloc(sizeof(Chat_info));

        int message_length;
        sscanf(str, "%*d,%*d,%n", &message_length);

        char *tempMessage = malloc(message_length + 1);

        sscanf(str, "%i,%[^,]", &chat->info.id, tempMessage);

        chat->Message = tempMessage;

        chat->info.type = Chat;

        return (Generic *)chat;
    }
    else
    {
        return NULL;
    }
}

int main()
{
    Generic gen = {Chat,Chat};
    Chat_info game = {gen,"Hello World !"};
    char *str = serialize ((Generic *) &game);
    printf("%s\n",str);
    Generic *deserialized = deserialize(str);
    if (deserialized->type == Game)
    {
        info_game *game = (info_game *)deserialized;
        printf("Game structure:\n");
        printf("ID: %d\n", game->info.id);
        printf("Test: %c\n", game->test);
        printf("In_info: %d\n", game->in_info);
    }
    else if (deserialized->type == Chat)
    {
        Chat_info *chat = (Chat_info *)deserialized;
        printf("\nChat structure:\n");
        printf("ID: %d\n", chat->info.id);
        printf("Message: %s\n", chat->Message);
    }

    free(((Chat_info *)deserialized)->Message);
    free(deserialized);
    free(str);

    return 0;
}
