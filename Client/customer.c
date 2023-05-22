#include "customer.h"

void rewrite(int fd, const void *buf, size_t count)
{
	ssize_t bytes_written = 0;

	while ((size_t)bytes_written < count)
	{

		ssize_t current_write = write(fd, buf + bytes_written, count - bytes_written);

		if (current_write < 0)
		{
			errx(3, "Error writing to file");
		}

		bytes_written += current_write;
	}

	return;
}

void query(char message[])
{
	scanf("%s", message);
}

void welcome()
{
	char msg[] = "Bienvenue sur DigitDash.\n\n";
	rewrite(STDOUT_FILENO, msg, strlen(msg));
}

void help()
{
	rewrite(STDOUT_FILENO, "Menu Principal\n", strlen("Menu Principal\n"));
	rewrite(STDOUT_FILENO, "[entrainement] --- Affûte ton clavier avant d'abattre l'adversaire\n", strlen("[entrainement] --- Affûte ton clavier avant d'abattre l'adversaire\n"));
	rewrite(STDOUT_FILENO, "[multijoueur] --- Affronte des joueurs en ligne\n", strlen("[multijoueur] --- Affronte des joueurs en ligne\n"));
	rewrite(STDOUT_FILENO, "[quitter]   --- Quitter DigitDash\n", strlen("[quitter]   --- Quitter DigitDash\n"));
	rewrite(STDOUT_FILENO, "[aide]   --- Permet de connaître les options disponibles\n\n", strlen("[aide]   --- Permet de connaître les options disponibles\n\n"));
}

int detectServer(char message[])
{
	char tmp[BUFFER_SIZE] = {0};
	size_t i = 0;
	for (i = 0; message[i] != ' '; i++)
	{
		tmp[i] = message[i];
	}

	tmp[i] = '\0';

	if (strcmp(tmp, "Server:") == 0 || strcmp(tmp, "Server") == 0)
		return 1;

	return 0;
}

int detectRequest(char message[])
{
	if (message != NULL)
	{
		if (message[6] == ':')
			return 1;
		else if (message[6] == ' ')
			return 0;
		else
			return 2;
	}

	return -1;
}

void stockRequest(char message[], int req)
{
	int requestIsPresent = detectServer(message);

	if (req != -1 && req != 2 && requestIsPresent)
	{
		int start = 0;

		if (req == 1)
			start = 8;
		else if (req == 0)
			start = 13;

		int i = 0;

		for (i = 0; message[i + start] != '\0'; i++)
		{
			message[i] = message[i + start];
		}

		message[i] = '\0';
	}
}

long __atoi(char message[])
{
	long number = 0;

	for (size_t i = 0; message[i] != '\0'; i++)
	{
		if (message[i] < 48 || message[i] > 57)
			return -1;

		number = number * 10 + message[i] - '0';
	}

	return number;
}

void MainMenu()
{
	char message[BUFFER_SIZE] = {0};
	welcome();
	help();

	while (1)
	{
		char *welcome = "Choisissez une option: ";
		write(0, welcome, strlen(welcome));
		query(message);

		if (strcmp(message, "quitter") == 0)
			break;

		else if (strcmp(message, "entrainement") == 0)
			system("../training/display/test");
		else if (strcmp(message, "multijoueur") == 0)
		{
			printf("En attente de connexion ...\n");
			sleep(1);
			system("clear");
			// u2u();
		}
		else
		{
			rewrite(STDOUT_FILENO, "Option invalide. Entrez [aide] pour connaître les options disponibles.\n", strlen("Option invalide. Entrez [aide] pour connaître les options disponibles.\n"));
		}

		if (!strcmp(message, "aide"))
		{
			help();
		}
	}

	rewrite(STDOUT_FILENO, "\nDigitDash fermé.\n", strlen("\nDigitDash fermé.\n"));
}

void client()
{
	char message[BUFFER_SIZE] = {0};

	if (strcmp(message, "quitter"))
	{
		// Première connexion au serveur
		int sfd = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in addrClient;
		addrClient.sin_addr.s_addr = inet_addr("82.65.173.135");
		addrClient.sin_family = AF_INET;
		addrClient.sin_port = htons(13080);
		connect(sfd, (const struct sockaddr *)&addrClient, sizeof(addrClient));

		// Lecture de la première requête serveur
		char buffer[BUFFER_SIZE] = {0};
		recv(sfd, &buffer, BUFFER_SIZE, 0);

		// Détection du type de requête
		int req = detectRequest(buffer);
		stockRequest(buffer, req);

		// Affichage de la première requête client
		char message_tmp[BUFFER_SIZE] = {0};
		rewrite(1, buffer, strlen(buffer));
		ssize_t r = read(1, message_tmp, BUFFER_SIZE);
		if (r == -1)
			errx(3, "Error reading option");

		rewrite(sfd, &message_tmp, strlen(message_tmp));

		char *waiting = "En attente de joueurs...\n\n";
		rewrite(1, waiting, strlen(waiting));
		char *welcome = "Envoyez une requête: ";

		// Boucle pour les différents envois de requêtes
		while (1)
		{
			// Lecture de la requête envoyée au serveur
			char message2[BUFFER_SIZE] = {0};

			rewrite(1, welcome, strlen(welcome));

			ssize_t r = read(1, message2, BUFFER_SIZE);
			if (r == -1)
				errx(3, "Error reading option");

			if (strcmp(message2, "quitter\n") == 0)
				break;

			// Réception de la réponse
			rewrite(sfd, message2, r);

			// Réception de la réponse
			char answer[BUFFER_SIZE] = {0};
			recv(sfd, &answer, BUFFER_SIZE, 0);

			// Détection du type de requête
			int req = detectRequest(answer);
			stockRequest(answer, req);

			// Affichage de la réponse
			printf("%s\n", answer);
		}

		// Déconnexion du joueur
		close(sfd);
		rewrite(STDOUT_FILENO, "\nDigitDash fermé.\n", strlen("\nDigitDash fermé.\n"));
		printf("Joueur déconnecté : %d\n", sfd);
	}
}
