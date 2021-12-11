#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "xmlfunctions.h"

#define FAIL_IF(EXP) ({ if((EXP) == -1) { printf("%s on line %d\n", strerror(errno), __LINE__); exit(EXIT_FAILURE); }})

int main(int argc, char **argv)
{
	int32_t port = 60123;
	int32_t exit_server = 0;   // condition for server shutdown
	struct sockaddr_in server; // struct used by the server
	struct sockaddr_in from;   // struct containing ip and port of client
	int32_t sd;				   // socket descriptor which accepts connections

	// socket creation
	FAIL_IF((sd = socket(AF_INET, SOCK_STREAM, 0)));

	// reusing the address and port
	int32_t on = 1;
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	// setting the structs to 0
	memset(&server, 0, sizeof(server));
	memset(&from, 0, sizeof(from));

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);

	// binding the socket to the ip provided
	FAIL_IF(bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)));

	// listening to max 3 connections in queue
	FAIL_IF(listen(sd, 3));

	// the main loop
	while (!exit_server)
	{
		int client;
		socklen_t length = sizeof(from);

		printf("[SERVER] Listening on port %d\n", port);
		fflush(stdout);

		// waiting in here untill a client comes
		client = accept(sd, (struct sockaddr *)&from, &length);

		if (client == -1)
		{
			printf("%s on line %d\n", strerror(errno), __LINE__);
			continue;
		}
		else // forking the execution, a child for every connection
		{
			int pid;
			if ((pid = fork()) == -1)
			{
				printf("%s on line %d\n", strerror(errno), __LINE__);
				close(client);
				continue;
			}
			else
			{
				if (pid != 0) // parent
				{
					close(client);
					while (waitpid(-1, NULL, WNOHANG))
						;
				}
				else // child
				{
					// closing the socket where we accepted the connection
					// because we no longer need it
					close(sd);

					printf("[SERVER] Connection established.\n");
					fflush(stdout);

					int32_t login = 0; // login = 0 -> client not logged in
					// login = 1 -> client logged in
					int32_t exit_client = 0; // if exit_client == 1, close server
					int32_t bytes_read, bytes_written;
					(void)bytes_written; // not used for now

					char *username = NULL; // holds the username
					char *password = NULL; // holds the password

					while (!exit_client)
					{
						// enter a loop while the user logins
						while (!login)
						{
							char *input_username = "Input a username: ";
							int input_username_length = strlen(input_username);

							// writing length to client
							FAIL_IF(write(client, &input_username_length, sizeof(input_username_length)));

							// write message
							FAIL_IF(write(client, input_username, input_username_length));

							// reading client's response
							int username_length = 0;
							FAIL_IF((bytes_read = read(client, &username_length, sizeof(username_length))));
							if (bytes_read == 0)
							{
								printf("Goodbye client..\n");
								return EXIT_SUCCESS;
							}

							// malloc the username
							username = malloc(sizeof(char) * username_length);
							memset(username, 0, username_length);

							// reading the username
							FAIL_IF(read(client, username, username_length));
							username[username_length - 1] = '\0';

							fflush(stdout);
							printf("[SERVER] Username is %s", username);
							fflush(stdout);

							// the same procedure with the password
							char *input_password = "Input a password: ";
							int input_password_length = strlen(input_password);

							// writing length to client
							FAIL_IF(write(client, &input_password_length, sizeof(input_password_length)));

							// write message
							FAIL_IF(write(client, input_password, input_password_length));

							// reading client's response
							int password_length = 0;
							FAIL_IF((bytes_read = read(client, &password_length, sizeof(password_length))));
							if (bytes_read == 0)
							{
								printf("Goodbye client..\n");
								return EXIT_SUCCESS;
							}

							// malloc the password
							password = malloc(sizeof(char) * password_length);
							memset(password, 0, password_length);

							// reading the password
							int bytes_read = 0;
							FAIL_IF((bytes_read = read(client, password, password_length)));
							password[password_length - 1] = 0;

							fflush(stdout);
							printf("[SERVER] Password is %s, bytes in: %d\n", password, bytes_read);
							fflush(stdout);

							// generate the name of the file to be opened
							char userxml[256];
							memset(userxml, 0, 255);
							strcat(userxml, username);
							strcat(userxml, ".xml");

							int register_bit = 0;
							// check if it exists
							if (access(userxml, F_OK) != 0) // file does not exist
							{
								// ask user if he wants to register
								char *register_question = "Do you want to register? Enter 1 for yes, 0 for no.\n";
								int register_question_length = strlen(register_question);

								FAIL_IF(write(client, &register_question_length, sizeof(register_question_length)));
								FAIL_IF(write(client, register_question, register_question_length));

								int reply_length;
								char reply;
								FAIL_IF(read(client, &reply_length, sizeof(reply_length)));
								FAIL_IF(read(client, &reply, reply_length));

								if(reply == 49)
									register_bit = 1;

								if (register_bit == 1)
								{
									//set login bit
									login = 1;

									// create user.xml
									xmlCreateUser(userxml, password);	
								}
							}
							else // file exists => user exists
							{
								fflush(stdout);
								printf("[SERVER] User exists:%s:%s.Check password..\n", username, password);
								fflush(stdout);

								if (xmlCheckPassword(userxml, password))
								{
									//set logi n bit
									login = 1;
									
									//update login field
									xmlReplaceLoginField(userxml, login);
								}
							}
						} //exit login loop

						// Logged in as user:pass
						fflush(stdout);
						printf("Logged in as %s:%s\n", username, password);
						fflush(stdout);
					} //client loop

					close(client);
					exit(EXIT_SUCCESS);
				} //child
			} //after fork returns
		} //after accept returns
	} //while(!exit_client)

	return EXIT_SUCCESS;
}