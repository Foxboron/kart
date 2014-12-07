#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

static void die(const char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

int start_server(int port)
{
	int sockfd, err;
	struct sockaddr_in serv_addr;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		die("Failed to create socket");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);
	if ((err = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
		die("Failed to bind to port");
	if ((err = listen(sockfd, 10)) < 0)
		die("Failed to call listen on socket");
	printf("Started server on port %d\n", port);
	return sockfd;
}

int start_client(const char *addr, int port)
{
	int sockfd, err;
	struct sockaddr_in serv_addr;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		die("Failed to create socket");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	if ((err = inet_pton(AF_INET, addr, &serv_addr.sin_addr)) < 1)
		die("inet_ptons failed, inet address probably not valid");
	if ((err = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
		die("Failed to connect to server");
	printf("Connected to server on %s:%d\n", addr, port);
	return sockfd;
}