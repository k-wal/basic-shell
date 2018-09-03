#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include <errno.h>

//function to change directory (path checks are done too)
void change_dir(char input[],char cwd[],char home[])
{
	size_t allocSize = sizeof(char) * 1024;
	char* saveptr;
	char* token = strtok_r(input," ",&saveptr);
	token = strtok_r(NULL," ",&saveptr);
	char* f_cwd = (char*)malloc(allocSize);

	//if no arguments are given or if ~ is the argument, go to home directory
	if(token==NULL || strcmp(token,"~")==0)
	{
		chdir(home);
		getcwd(f_cwd,allocSize);
		strcpy(cwd,f_cwd);
		return;
	}


	//if error in changing directories
	if(chdir(token)!=0)
	{
		perror("Error");
	}

	//change current working directory to the argument
	else
	{
		getcwd(f_cwd,allocSize); 
		strcpy(cwd,f_cwd);

	}
	return;
}
	
//function to echo
void echo_command(char input[])
{
	char* saveptr;
	char copy_input[1000];
	strcpy(copy_input,input);
	char* token = strtok_r(copy_input," ",&saveptr);
	token = strtok_r(NULL," ",&saveptr);
	while(token!=NULL)
	{
		printf("%s ",token);
		token = strtok_r(NULL," ",&saveptr);
	}
	printf("\n");
	return;
}

