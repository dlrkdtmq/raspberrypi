#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MSGBUFSIZE 80

void exit_error(char *error_message);

int main(int argc, char *argv[]){
	int sock;
	struct sockaddr_in serv_addr;
	unsigned short serv_port;
	char *serv_IP;
	char msg_buf[MSGBUFSIZE];
	unsigned int msg_size;

	if(argc != 3){
		fprintf(stderr, "Usage: %s <server IP> <echo port>\n", argv[0]);
		exit(1);
	}
	
	serv_IP = argv[1];
	serv_port = atoi(argv[2]);

	if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))<0)
		exit_error("socket() failed");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_IP);
	serv_addr.sin_port = htons(serv_port);

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
		exit_error("connect() failed");

	fputs("input message :", stdout);
	fgets(msg_buf, sizeof(msg_buf), stdin);
	
	if(send(sock, msg_buf, sizeof(msg_buf), 0) != sizeof(msg_buf))
		exit_error("send() failed");
	if((msg_size = recv(sock, msg_buf, MSGBUFSIZE-1, 0))<0)
		exit_error("recv() failed");
		
	msg_buf[msg_size] = '\0';
	printf("Echo from the server: %s", msg_buf); 

	close(sock);
	exit(0);
}

void exit_error(char *error_message){
	perror(error_message);
	exit(1);
}
