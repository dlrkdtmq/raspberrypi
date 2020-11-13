#include <stdio.h>
int main(){
	int cpid;
	int zpid;
	if((cpid=fork())==-1){
		fprintf(stderr, "can't fork");
		exit(0);
	}
	else if(cpid!=0){ 
		printf("\n parent process: PID=%d PPID=%d\n", getpid(), getppid());
		sleep(20);
		zpid=waitpid(-1, NULL, NULL); 자식 프로세스에 대해서 완전 무장 해제
		printf("zombie process %d removed\n", zpid);
		sleep(10);
		exit(0);
	}
	else{
		printf("\n child process: PID=%d PPID=%d\n", getpid(), getppid());
		sleep(10);
		printf("child process %d terminated\n", getpid());
		exit(0); //자식 프로세스가 종료되었지만 아직 부모 프로세스에 의해 해제된 것은 아닌 좀비 프로세스
	}
}//zombie1.c
