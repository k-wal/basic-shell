#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<fcntl.h> 

//function to set environment variable 'var' to value 'val'
void setenv_main(char input[])
{
	char *saveptr;
	char* token = strtok_r(input," ",&saveptr);
	token = strtok_r(NULL," ",&saveptr);
	

	//if no arguments are present
	if(token==NULL)
	{
		printf("Error : Too less arguments for setenv\n");
		return;
	}
	char var[100];
	strcpy(var,token);
	char val[100];

	token = strtok_r(NULL," ",&saveptr);
	//if value is specified as an argument
	if(token!=NULL)
	{
		strcpy(val,token);
		token = strtok_r(NULL," ",&saveptr);
		//if there are more than 2 arguments
		if(token)
		{
			printf("Error : Too many argumentds for setenv\n");
			return;
		}	
	}
	//if value is not specified as an argument
	else
	{
		val[0]=' ';
		val[1]='\0';
	}

	//calling setenv(inbuilt) with overwrite=1
	if(setenv(var,val,1)!=0)
	{
		perror("Error:");
	}
	else
	{
		printf("Environment variable %s set to value %s successfully\n",var,val);
	}
}

void unsetenv_main(char input[])
{
	char* saveptr;
	char* token = strtok_r(input," ",&saveptr);
	token = strtok_r(NULL," ",&saveptr);
	if(token==NULL)
	{
		printf("Error : Too less arguments for unsetenv\n");
		return;
	}
	char var[100];
	strcpy(var,token);
	token = strtok_r(NULL," ",&saveptr);
	if(token)
	{
		printf("Error : Too many arguments for unsetenv\n");
		return;
	}
	//calling unsetenv(inbuilt)
	if(unsetenv(var)!=0)
	{
		perror("Error:");
	}
	else
	{
		printf("Environment variable %s deleted successfully\n",var);
	}

}