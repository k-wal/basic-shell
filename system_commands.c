#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

long long int backid[100];

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
	int i;
	for(i=0; i<100; i++)
	{
		//printf("in for loop\n");
		if(backid[i]==-1)
		{
			backid[i]=child_id;
			//printf("i : %d\n",i);
			break;
		}
	}
	return;
}


//to check if a background function is over
void check_background_over(void)
{
	int i;
	for(i=0; i<100; i++)
	{
		if(backid[i]==-1)
			continue;

		char proc[100];
		proc[0]='\0';
		strcpy(proc,"/proc/");
		char string_id[10];
		sprintf(string_id,"%lld",backid[i]);
		strcat(proc,string_id);
		strcat(proc,"/stat");

		FILE* statfile;
		if(!(statfile = fopen(proc,"r")))
		{
			return;
		}
		long long int pid;
		char status;
		char name[20];
	
		//scanning pid, name and status
		fscanf(statfile,"%lld %s %c",&pid,name,&status);
		fclose(statfile);

		if(status!='R')
		{
			backid[i]=-1;
			printf("%s with pid %lld exited normally\n",name,pid);
		}

	}


}