#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXQUE 5
#define MSGBUFSIZE 80

void exit_error(char *error_message);

int main(int argc, char *argv[]){
	int serv_sock;
	int clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	unsigned short serv_port;
	unsigned int caddr_len;

	char msg_buf[MSGBUFSIZE];
	int msg_size;

	if(argc != 2){
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	serv_port = atoi(argv[1]);

	if((serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))<0)
		exit_error("socket() failed");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(serv_port);

	if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
		exit_error("bind() failed");

	if(listen(serv_sock, MAXQUE)<0)
		exit_error("listen() failed");

	while(1){
		caddr_len = sizeof(clnt_addr);
		if((clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &caddr_len))<0)
			exit_error("accept() failed");
		printf("Echo request from %s\n", inet_ntoa(clnt_addr.sin_addr),clnt_addr.sin_port);
		if((msg_size = recv(clnt_sock, msg_buf, MSGBUFSIZE, 0))<0)
			exit_error("recv() failed");
		if(send(clnt_sock, msg_buf, msg_size, 0) != msg_size)
			exit_error("send() failed");
		close(clnt_sock);
	}
}

void exit_error(char *error_message){
	perror(error_message);
	exit(1);
}
