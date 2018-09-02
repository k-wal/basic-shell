#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

//function to call inbuilt functions foreground
void call_inbuilt(char* arg[])
{	
	pid_t child_id;
	child_id = fork();
	if(child_id==0)
	{
		execvp(arg[0],arg);
		perror("Error:");

	}
	else
	{
		wait(NULL);
	}
	/*fork();
	if()
			perror("Error:");*/
	return;
}

//function to call functions background
void call_inbuilt_background(char* arg[])
{
	pid_t child_id;
	child_id = fork();
	if(child_id==0)
	{
		execvp(arg[0],arg);
		perror("Error:");

	}
	return;
}