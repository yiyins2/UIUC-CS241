/**
 * Lab: Utilities Unleashed
 * CS 241 - Fall 2017
 */
#include"format.h"
#include<unistd.h>
#include<time.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<stdlib.h>


int main(int argc, char *argv[]) { 
	if(argc<2){
		print_time_usage();
	}

	struct timespec tp_start = {0, 0};
	struct timespec tp_finish = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &tp_start);		// 程序开始执行的时间

	// 构建execvp的参数数组
	char **argv_temp = (char **)malloc(argc*sizeof(char **));
	for(int i=1;i<argc;i++){
		argv_temp[i-1] = malloc(strlen(argv[i])+1);
		strcpy(argv_temp[i-1],argv[i]);
	}
	free(argv_temp[argc-1]);
	argv_temp[argc-1] = NULL;
	
	pid_t child = fork();
	if(child == -1){
		print_fork_failed();
	}
	else if(child == 0){

		if(execvp(argv[1],argv_temp)<0){
			print_exec_failed();
		}

	}else{
		int status;
	    waitpid(child , &status ,0);
		// 释放资源
	    for(int i=0;i<argc;i++){
			if(argv_temp[i]!=NULL)
				free(argv_temp[i]);
		}
		free(argv_temp);
		if(status != 0){
			return 1;
		}
		clock_gettime(CLOCK_MONOTONIC, &tp_finish);		// 程序结束的时间
		display_results(argv, (double)(tp_finish.tv_sec - tp_start.tv_sec)+(tp_finish.tv_nsec - tp_start.tv_nsec)/(double)1000000000);
		
	}
	return 0;
}
