#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* portul folosit */
#define PORT 2024

int main(int argc, char **argv)
{
	struct sockaddr_in server; // structura folosita de server
	struct sockaddr_in from;
	int sd;					 //descriptorul de socket

	/* crearea unui socket */
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("%s on line %d\n", strerror(errno), __LINE__);
		exit(EXIT_FAILURE);
	}

	//reusing the address and port
	int32_t on = 1;
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	/* pregatirea structurilor de date */
	bzero(&server, sizeof(server));
	bzero(&from, sizeof(from));

	/* umplem structura folosita de server */
	/* stabilirea familiei de socket-uri */
	server.sin_family = AF_INET;
	/* acceptam orice adresa */
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	/* utilizam un port utilizator */
	server.sin_port = htons(PORT);

	/* atasam socketul */
	if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
	{
		printf("%s on line %d\n", strerror(errno), __LINE__);
		exit(EXIT_FAILURE);
	}

	/* punem serverul sa asculte daca vin clienti sa se conecteze */
	if (listen(sd, 1) == -1)
	{
		printf("%s on line %d\n", strerror(errno), __LINE__);
		exit(EXIT_FAILURE);
	}

	/* servim in mod concurent clientii... */
	while (1)
	{
		int client;
		int length = sizeof(from);

		printf("[server]Asteptam la portul %d...\n", PORT);
		fflush(stdout);

		/* acceptam un client (stare blocanta pina la realizarea conexiunii) */
		client = accept(sd, (struct sockaddr *)&from, &length);

		/* eroare la acceptarea conexiunii de la un client */
		if (client == -1)
		{
			printf("%s on line %d\n", strerror(errno), __LINE__);
			continue;
		}
		else
		{
			int pid;
			if ((pid = fork()) == -1)
			{
				printf("%s on line %d\n", strerror(errno), __LINE__);
				close(client);
				continue;
			}
			else 
			if (pid > 0)
			{
				// parinte
				close(client);
				while (waitpid(-1, NULL, WNOHANG));
			}
			else 
			if (pid == 0)
			{
				// copil
				close(sd);

				/* s-a realizat conexiunea, se astepta mesajul */
				printf("[server]Asteptam mesajul...\n");
				fflush(stdout);

				/* citirea mesajului */
				while (1)
				{
					if (read(client, msg, 100) <= 0)
					{
						perror("[server]Eroare la read() de la client.\n");
						close(client); /* inchidem conexiunea cu clientul */
						exit(EXIT_SUCCESS);
					}
					else
					{
						printf("[server]Mesajul a fost receptionat...%s\n", msg);

						/*pregatim mesajul de raspuns */
						bzero(msgrasp, 100);
						strcat(msgrasp, "Hello ");
						strcat(msgrasp, msg);

						printf("[server]Trimitem mesajul inapoi...%s\n", msgrasp);

						/* returnam mesajul clientului */
						if (write(client, msgrasp, 100) <= 0)
						{
							perror("[server]Eroare la write() catre client.\n");
							continue; /* continuam sa ascultam */
						}
					}
				}
				/* am terminat cu acest client, inchidem conexiunea */
				close(client);
				exit(EXIT_SUCCESS);
			}
		}

	} /* while */
}