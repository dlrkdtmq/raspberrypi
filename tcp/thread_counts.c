/* thread_counts.c
This program shows how to generate multi threads,
which executes even number and odd number counts respectively.*/
# include <pthread.h>
# include <stdio.h>

void *thread_func(void *arg);

main(){
	pthread_t tid1, tid2;
	int arg1, arg2;

	arg1=1;
	if(pthread_create(&tid1, NULL, thread_func, (void*)&arg1)!=0)
		exit(0);
	arg2=2;
	if(pthread_create(&tid2, NULL, thread_func, (void*)&arg2)!=0)
		exit(0);
	sleep(20);
}

void *thread_func(void *args){
	int i;
	int number;

	pthread_detach(pthread_self()); //해당 쓰레드의 실행이 끝나면 쓰레드 자원을 해제.
	number=*(int *)args;
	if(number==1){
		for(i=0; i<10; i=i+2)
			printf(“first thread: %d\n”, i);
		sleep(10);
		printf(“first thread returns\n”);
	}
	else{
		for(i=1; i<10; i=i+2)
			printf(“second thread: %d\n”, i);
		sleep(10);
		printf(“second thread returns\n”);
	}
	return(NULL);
}
