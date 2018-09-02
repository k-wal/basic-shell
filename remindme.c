#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

/*
void remindme_print(long long int time, char message[])
{
	
	pid_t child_id;
	child_id = fork();
	if(child_id==0)
	{
		sleep(time);
		printf("Reminder : %s\n",message);

	}*/
	



//main function for remindme command
void remindme_main(char input[])
{
	long long int time=0;
	char* saveptr;
	char* arg[100];
	char message[100];
	message[0]='\0';
	int i=0,flag=0;
	char* token = strtok_r(input," ",&saveptr);
	token = strtok_r(NULL," ",&saveptr);
	
	//if no arguments are present
	if(token==NULL)
	{
		printf("Error : Too less arguments for remindme.\n");
		return;
	}
	
	//checking if the second argument has only digits
	for(i=0; i<strlen(token); i++)
	{
		if(token[i]<'0' || token[i]>'9')
		{
			flag=1;
			break;
		}
		else
		{
			time = time*10+(token[i]-'0');
		}
	}

	//printing error as first argument isn't all digits
	if(flag==1)
	{
		printf("Please enter an integer value as time\n");
		return;
	}

	//creating message
	token=strtok_r(NULL," ",&saveptr);
	while(token!=NULL)
	{
		strcat(message,token);
		strcat(message," ");
		token=strtok_r(NULL," ",&saveptr);
	}

	//printing message
	//remindme_print(time,message);
	
	int child_id=fork();
	if(child_id==0)
	{
		sleep(time);
		printf("Reminder : %s\n",message);
		_exit(0);

	}
	return;

}