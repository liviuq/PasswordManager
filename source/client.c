#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOCAL_MESSAGE_LEN 256

int main(int argc, char *argv[])
{
	int32_t bytes_read, bytes_written;
	(void)bytes_written;
	int32_t exit_client = 0;
	int32_t port;
	int32_t sd;				  //socket descriptor used to talk to the server 
	struct sockaddr_in server; //struct for server ip, port, tcp option 

	if (argc != 3)
	{
		printf("[CLIENT]: %s ip_address port\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	//setting the port
	port = atoi(argv[2]);

	//creating the socket
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("%s on line %d\n", strerror(errno), __LINE__);
		exit(EXIT_FAILURE);
	}

	//filling struct sockaddr with info
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_port = htons(port);

	//connecting to the server
	if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
	{
		printf("%s on line %d\n", strerror(errno), __LINE__);
		exit(EXIT_FAILURE);
	}

	while (!exit_client)
	{
		// reading the number of bytes from the server
		int length_in = 0;
		if ((bytes_read = read(sd, &length_in, sizeof(length_in))) == -1)
		{
			printf("%s on line %d\n", strerror(errno), __LINE__);
			exit(EXIT_FAILURE);
		}
		else if (bytes_read == 0)
		{
			printf("Goodbye server..\n");
			return EXIT_SUCCESS;
		}

		// reading the whole message
		char *incoming_message = malloc(sizeof(char) * length_in);
		memset(incoming_message, 0, length_in);

		if (read(sd, incoming_message, length_in) == -1)
		{
			printf("%s on line %d\n", strerror(errno), __LINE__);
			exit(EXIT_FAILURE);
		}

		// displaying the message
		printf("%s\n", incoming_message);
		fflush(stdout);

		// reading the input from the user
		char *message = malloc(sizeof(char) * LOCAL_MESSAGE_LEN);
		memset(message, 0, LOCAL_MESSAGE_LEN);
		if (read(STDIN_FILENO, message, LOCAL_MESSAGE_LEN) == -1)
		{
			printf("%s on line %d\n", strerror(errno), __LINE__);
			exit(EXIT_FAILURE);
		}
		// message[strlen(message)] = '\0';

		// writing to the server the response's length
		int length_out = strlen(message);
		if (write(sd, &length_out, sizeof(length_out)) == -1)
		{
			printf("%s on line %d\n", strerror(errno), __LINE__);
			exit(EXIT_FAILURE);
		}

		// writing to the server the whole response
		if (write(sd, message, length_out) == -1)
		{
			printf("%s on line %d\n", strerror(errno), __LINE__);
			exit(EXIT_FAILURE);
		}

		// freeing the memory
		free(incoming_message);
		free(message);
	}

	close(sd);
	return 0;
}