#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include "ls.c"
#include<stdlib.h>
#include "system_commands.c"
#include "remindme.c"
#include "pinfo.c"



#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"



//home : Home Directory
char home[1000];

//cwd : Current Working Directory
char cwd[1000];

//rwd : Relative Working Directory
char rwd[1000];


//function to calculate Relative Working Directory
void find_rwd(void)
{
	long long i,n=strlen(cwd),h=strlen(home);
	
	//current working directory is definitely not inside the home directory
	if(n<h)
	{
		if(n==1 && cwd[0]=='/')
		{
			strcpy(rwd,"/");
			return;
		}

		char copy_cwd[1000];
		strcpy(copy_cwd,cwd);
		char cur_dir[1000];
		char* token=strtok(copy_cwd,"/");
		while(token!=NULL)
		{
			strcpy(cur_dir,token);
			token=strtok(NULL,"/");
		}
		rwd[0]='/';
		strcpy(&rwd[1],cur_dir);

	}
	else
	{
		int flag=0;

		//to check if current working directory isn't inside home
		for(i=0; i<h; i++)
		{
			if(home[i]!=cwd[i])
			{
				flag=1;
				break;
			}			
		}

		//if current working directory isn't inside home
		if(flag==1)
		{
			char copy_cwd[1000];
			strcpy(copy_cwd,cwd);
			char cur_dir[1000];
			char* token=strtok(copy_cwd,"/");
			while(token!=NULL)
			{
				strcpy(cur_dir,token);
				token=strtok(NULL,"/");
			}
			rwd[0]='/';
			strcpy(&rwd[1],cur_dir);
		}
		else
		{
			strcpy(rwd,"~");
			strcpy(&rwd[1],&cwd[i]);
		}
	}
	
}


//function to print prompt, take input and return a string
void take_input(char input[])
{
	find_rwd();
	char user[100];
	char host[100];
	
	//getting current user's name
	getlogin_r(user,100);
	
	//getting the host name
	gethostname(host,100);
	printf(GREEN "%s@%s:"RESET BLUE "%s " RESET ,user,host,rwd);
	fgets (input, 100, stdin);
	int len = strlen(input);
	if(len)
	{	
		input[len-1]='\0';
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
	printf("%s\n",token);
	return;
}


//function to change directory (path checks are done too)
void change_dir(char input[])
{
	char* saveptr;
	char* token = strtok_r(input," ",&saveptr);
	token = strtok_r(NULL," ",&saveptr);
	
	//if no arguments are given, go to home directory
	if(token==NULL)
	{
		chdir(home);
		getcwd(cwd,sizeof(cwd));
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
		getcwd(cwd,sizeof(cwd));
		printf("directory is now: %s\n",cwd);
	}
	return;
}
	
void call_inbuilt_functions(char* token, char* saveptr)
{
	char *arg[100];
	int i=0;
	while(token!=NULL)
	{
		arg[i]=token;
		token = strtok_r(NULL," ",&saveptr);
		i++;
		
	}
	arg[i]=NULL;
	int n=strlen(arg[i-1]);
	if(arg[i-1][n-1]=='&')
	{
		printf("background\n");
		if(n==1)
		{
			arg[i-1]=NULL;
			call_inbuilt_background(arg);
		}
		else
		{
			arg[i-1][n-1]='\0';
			call_inbuilt_background(arg);	
		}
		
	}
	else
	{
		call_inbuilt(arg);	
	}
		
}
//function to call another file for a command
void call_for_command(char command[],char input[])
{
	char copy_input[1000];
	strcpy(copy_input,input);
	char* saveptr;
	char* token = strtok_r(copy_input," ",&saveptr);
	char cd[]="cd";
	char pwd[]="pwd";
	char ls[]="ls";
	char echo[]="echo";
	char clear[]="clear";
	char pinfo[]="pinfo";
	char remindme_string[]="remindme";

	//if first word is cd
	if(strcmp(cd,token)==0)
	{
		//token=strtok(NULL," ")
		change_dir(input);
	}

	//if first word is pwd
	else if(strcmp(pwd,token)==0)
	{
		printf("%s\n",cwd);
	}

	//if first word is ls
	else if(strcmp(ls,token)==0)
	{
		if(ls_main(input)!=0)
		{
			call_inbuilt_functions(token,saveptr);		
		}		
	}

	//if the first word is echo
	else if(strcmp(echo,token)==0)
	{
		echo_command(input);
	}

	//if the first word is clear
	else if(strcmp(clear,token)==0)
	{
		system("clear");
	}

	//if the first word is pinfo
	else if(strcmp(pinfo,token)==0)
	{
		pinfo_main(input,home);
		
	}

	//if the first word is remindme
	else if(strcmp(remindme_string,token)==0)
	{
		remindme_main(input);
		
	}

	//to call inbuilt functions
	else
	{
		call_inbuilt_functions(token,saveptr);	
	}
	return;
}


//to execute single command(one part of the ; separated commands)
//returns 1 if exit is found
int single_command(char input[])
{
	char copy_input[1000];
	char exit[]="exit";
	char copy_input2[1000];
	char* saveptr;
	strcpy(copy_input,input);
	strcpy(copy_input2,input);
	if(strlen(copy_input))
	{
		//printf("%s\n",input);
		char* token = strtok_r(copy_input," ",&saveptr);
		
		//return 1 if command is exit
		if(strcmp(exit,token)==0)
		{
			return 1;
		}
		else
		{
			call_for_command(token,input);
			return 0;
		}
	}
	
	return 0;	
}


int main()
{
	getcwd(home,sizeof(home));
	getcwd(rwd,sizeof(rwd));
	getcwd(cwd,sizeof(cwd));
	char input[1000];
	
	//copy_input so that input is preserved 
	char copy_input[1000];
	char copy_token[1000];
	char exit[]="exit";
	int program_on = 1;
	
	//keep taking input till exit comes
	while(program_on)
	{
		take_input(input);
		strcpy(copy_input,input);
		char* saveptr;
		
		if(strlen(copy_input))
		{
			char* token = strtok_r(copy_input,";",&saveptr);
			
			//separating commands by ';' and calling single_command function on each of them
			while(token!=NULL && program_on)
			{
				if(single_command(token))
				{
					program_on=0;
				}
				else
				{
					token = strtok_r(NULL,";",&saveptr);
				}
			}
		}
		
			
	}
	return 0;
}


