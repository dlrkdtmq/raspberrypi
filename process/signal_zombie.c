#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void zombie_collector(int sig);

int main(){
	pid_t cpid;
	struct sigaction act;

	act.sa_handler = zombie_collector;
	sigfillset(&act.sa_mask);
	(void) sigaction(SIGCHLD, &act, NULL);	

	if((cpid=fork())==-1){
		fprintf(stderr, "can't fork");
		exit(0);
	}
	else if(cpid!=0){
		printf("\n parent process: PID=%d PPID=%d\n", getpid(), getppid());
		sleep(30);
		exit(0);
	}
	else{
		printf("\n child process: PID=%d PPID=%d\n", getpid(), getppid());
		sleep(10);
		printf("child process %d terminated\n", getpid());
		exit(0);
	}
}

void zombie_collector(int sig){
	pid_t zpid;

	printf("parent caught a signal number= %d\n", sig);
	sleep(10);	
	zpid=waitpid((pid_t)-1, NULL, NULL);
	printf("zombie process %d removed\n", zpid);
}

