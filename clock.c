#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>


int is_exit_clock=0;

void clock_main(char input[])
{
	char* saveptr;
	char* token = strtok_r(input," ",&saveptr);
	token = strtok_r(NULL," ",&saveptr);
	

	//if -t is not present
	if(token==NULL || strcmp(token,"-t"))
	{
		printf("Error : flag '-t' not found\n");
		return;
	}
	token = strtok_r(NULL," ",&saveptr);
	if(token==NULL)
	{
		printf("Error : time argument missing\n");
		return;
	}

	long long int time=0,i;
	long long int n=strlen(token);
	for(i=0; i<n; i++)
	{
		if(token[i]>='0' && token[i]<='9')
		{
			token[i]=token[i]-'0';
			time=time*10 + token[i];
		}
		else
		{
			printf("Please enter an integer for time\n");
			return;
		}	
	}
	while(!is_exit_clock)
	{
		sleep(time);
		FILE* statfile;
		if(!(statfile = fopen("/proc/driver/rtc","r")))
		{
			printf("Error finding time\n");
			return;
		}
		char blah[10];
		char blah2[10];
		char date_string[100];
		char time_string[100];

		//scanning date and time
		fscanf(statfile,"%s %s %s",blah,blah2,time_string);
		fscanf(statfile,"%s %s %s",blah,blah2,date_string);
			
		fclose(statfile);
		printf("%s , %s\n",date_string,time_string);
	}
}


