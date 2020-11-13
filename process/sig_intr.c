#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sig_handler(int sig);

int main(){
	struct sigaction act; //signal action 구조체
	act.sa_handler=sig_handler; //시스널 받을 때 액션을 할 함수 포인터
	sigfillset(&act.sa_mask); //다른 시그널 마스크
	act.sa_flags=SA_RESETHAND; //핸들러 실행후 기본값으로 리셋
	(void) sigaction(SIGINT, &act, NULL); //Ctrl + C 인터럽트에 의한 시그널 받았을 때 앞서 정의된 action으로 처리
	while(1){
		sleep(3);
		printf("waiting for SIGINT...\n");
	}
}

void sig_handler(int sig){ //Ctrl + C 인터럽트에 의한 시그널 받았을 때 동작
	printf("I catched a signal (number= %d)\n", sig);
} // signal_intr.c
