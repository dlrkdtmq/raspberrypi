#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <wait.h>

#include <sys/file.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXQUE 5
#define MSGBUFSIZE 80

void exit_error(char *error_message);
void do_echo(int csock);
void zombie_collector(int sig_num);

int main(int argc, char *argv[]){
	int serv_sock;
	int clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	unsigned short serv_port;
	unsigned int caddr_len;
	int cpid;
	struct sigaction sig_handler;

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

	sig_handler.sa_handler = zombie_collector;
	sig_handler.sa_flags = SA_RESTART;
	
	if(sigfillset(&sig_handler.sa_mask)<0)
		exit_error("sigfillset() failed");

	if(sigaction(SIGCHLD, &sig_handler, 0)<0)
		exit_error("sigaction() failed");


	while(1){
		caddr_len = sizeof(clnt_addr);
		if((clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &caddr_len))<0)
			exit_error("accept() failed");
		printf("Echo request from %s\n", inet_ntoa(clnt_addr.sin_addr));
		if((cpid = fork())<0)
			exit_error("fork() failed");
		if(cpid == 0){
			close(serv_sock);
			do_echo(clnt_sock);
			exit(0);
		}
		close(clnt_sock);
	}
}

void zombie_collector(int sig_num){
	int zid;
	
	while((zid = waitpid((pid_t)-1, NULL, WNOHANG))>0);
	if(zid == -1 && errno != ECHILD)
		exit_error("waitpid() failed");
	else
		printf(“zombie process removed");
}

void do_echo(int csock){
	char msg_buf[MSGBUFSIZE];
	int msg_size;

	if((msg_size = recv(csock, msg_buf, MSGBUFSIZE, 0))<0)
		exit_error("recv() failed");
	if(send(csock, msg_buf, msg_size, 0) != msg_size)
		exit_error("send() failed");
	close(csock);
}


void exit_error(char *error_message){
	perror(error_message);
	exit(1);
}