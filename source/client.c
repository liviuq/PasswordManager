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
#define FAIL_IF(EXP) ({ if((EXP) == -1) { printf("%s on line %d\n", strerror(errno), __LINE__); exit(EXIT_FAILURE); }})

int main(int argc, char *argv[])
{
	int32_t bytes_read, bytes_written;
	(void)bytes_written;
	int32_t exit_client = 0;
	int32_t port;
	int32_t sd;				   // socket descriptor used to talk to the server
	struct sockaddr_in server; // struct for server ip, port, tcp option

	if (argc != 3)
	{
		printf("[CLIENT]: %s ip_address port\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// setting the port
	port = atoi(argv[2]);

	// creating the socket
	FAIL_IF(sd = socket(AF_INET, SOCK_STREAM, 0));

	// filling struct sockaddr with info
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_port = htons(port);

	// connecting to the server
	FAIL_IF(connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)));
		
	while (!exit_client)
	{
		// reading the number of bytes from the server
		int length_in = 0;
		FAIL_IF(bytes_read = read(sd, &length_in, sizeof(length_in)));
		if (bytes_read == 0)
		{
			printf("Goodbye server..\n");
			return EXIT_SUCCESS;
		}

		// reading the whole message
		char *incoming_message = malloc(sizeof(char) * length_in);
		memset(incoming_message, 0, length_in);

		FAIL_IF(read(sd, incoming_message, length_in));

		// displaying the message
		printf("%s\n", incoming_message);
		fflush(stdout);

		// reading the input from the user
		char *message = malloc(sizeof(char) * LOCAL_MESSAGE_LEN);
		memset(message, 0, LOCAL_MESSAGE_LEN);
		FAIL_IF(read(STDIN_FILENO, message, LOCAL_MESSAGE_LEN));
		// message[strlen(message)] = '\0';

		// writing to the server the response's length
		int length_out = strlen(message);
		FAIL_IF(write(sd, &length_out, sizeof(length_out)));

		// writing to the server the whole response
		FAIL_IF(write(sd, message, length_out));

		// freeing the memory
		free(incoming_message);
		free(message);
	}

	close(sd);
	return 0;
}