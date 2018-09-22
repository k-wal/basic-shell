#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<fcntl.h> 

void jobs_main(long long int backid[],long long int is_running[])
{
	int i;
	for(i=0; i<100; i++)
	{
		if(is_running[i]==-1)
		{
			break;
		}

		//printing the number of the process
		printf("[%d] ",i+1);
		
		//printing the running status 
		if(is_running[i]==0)
		{
			printf("Stopped ");
		}
		else
		{
			printf("Running ");
		}

		//getting the name of process from proc file
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

		//printng the name and pid of the process
		printf("%s[%lld]\n",name,backid[i]);

	}
}