#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * create_and_bind_socket - creates, binds, and listens on port 12345
 *
 * Return: the listening socket fd, or exits on failure
 */
int create_and_bind_socket(void)
{
	int server_fd;
	struct sockaddr_in server_addr;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(12345);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(server_fd, (struct sockaddr *)&server_addr,
		sizeof(server_addr)) < 0)
	{
		perror("bind");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 10) < 0)
	{
		perror("listen");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	return (server_fd);
}

/**
 * main - listens on port 12345, receives a message and prints it
 *
 * Return: 0 on success, EXIT_FAILURE on error
 */
int main(void)
{
	int server_fd, client_fd, bytes;
	socklen_t client_len;
	char buffer[1024];
	struct sockaddr_in client_addr;
	char client_ip[INET_ADDRSTRLEN];

	server_fd = create_and_bind_socket();

	printf("Server listening on port 12345\n");
	fflush(stdout);

	client_len = sizeof(client_addr);
	client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
		&client_len);
	if (client_fd < 0)
	{
		perror("accept");
		close(server_fd);
		return (EXIT_FAILURE);
	}

	inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
	printf("Client connected: %s\n", client_ip);

	memset(buffer, 0, sizeof(buffer));
	bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes < 0)
	{
		perror("recv");
		close(client_fd);
		close(server_fd);
		return (EXIT_FAILURE);
	}

	printf("Message received: \"%s\"\n", buffer);

	close(client_fd);
	close(server_fd);
	return (0);
}
