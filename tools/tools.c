#include "tools.h"
char *serialize(void *buf, size_t size)
{
    char *result = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
    switch (size)
    {
    case sizeof(struct info_game):
    {
        info_game *buffer = (info_game *)buf;
        sprintf(result, "%d,%c,%c", buffer->id, buffer->info, buffer->Hello);
        break;
    }
    case sizeof(struct chat):
    {
        chat *buffer = (chat *)buf;
        sprintf(result, "%d,\"%s\"", buffer->id, buffer->message);
        break;
    }

    default:
        break;
    }

    return result;
}

// Fonction de désérialisation générique
void *deserialize(const char *str)
{
    void *ptr = NULL;

    // Récupération du code numérique en début de chaîne
    int code = str[0] - '0';

    // Utilisation de sscanf pour extraire les valeurs des champs de la structure correspondante
    switch (code)
    {
    case 0:
    {
        struct chat *c = (struct chat *)malloc(sizeof(struct chat));
        c->message = (char *)malloc(sizeof(char) * 100);

        sscanf(str, "%d,%[^,]", &c->id, c->message);

        ptr = (void *)c;
        break;
    }
    case 1:
    {
        struct info_game *ig = (struct info_game *)malloc(sizeof(struct info_game));

        sscanf(str, "%d,%c,%c", &ig->id, &ig->info, &ig->Hello);

        ptr = (void *)ig;
        break;
    }
    }
    return ptr;
}

int main()
{
    // Initialisation des structures
    struct chat c = {0, "Bonjour !"};

    // Sérialisation des structures
    char *str2 = serialize(&c, sizeof(chat));

    // Affichage des chaînes de caractères
    printf("Structure info_game sérialisée: %s\n", str2);

    // Désérialisation des chaînes de caractères
    void *ptr2 = deserialize(str2);

    // Conversion des pointeurs en pointeurs de structure
    struct chat *c2 = (struct chat *)ptr2;

    // Affichage des valeurs des champs de la structure désérialisée
    printf("id: %d, message: %s\n", c2->id, c2->message);
}
