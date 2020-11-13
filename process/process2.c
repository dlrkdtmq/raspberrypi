#include <stdio.h>
int main(){
	int cpid;
	if((cpid=fork())==-1){
		fprintf(stderr, "can't fork");
		exit(0);
	}
	else if(cpid!=0){ //fork( )가 0이 아닌 자신의 PID를 반환하면 부모 프로세스
		printf("\n parent process: PID=%d PPID=%d\n", getpid(), getppid());
		sleep(20);
	}
	else{ //fork( )가 0을 반환하면 자식 프로세스
		printf("\n child process: PID=%d PPID=%d\n", getpid(), getppid());
		sleep(20);
	}
	exit(0);
}
