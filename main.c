#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include "ls.c"
#include "system_commands.c"
#include "remindme.c"
#include "pinfo.c"
#include "clock.c"
#include "cd.c"
#include "setenv.c"
#include "jobs.c"

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



//function to check for background functions and call inbuilt functions
//infile and outfile are fpr redirection
void call_inbuilt_functions(char* token, char* saveptr, char infile[], char outfile[],int if_append)
{
	char *arg[100];
	int i=0;
	int redirect_flag=0;

	//arg[] is the array of arguments taken by exec function 
	while(token!=NULL && redirect_flag==0)
	{
		arg[i]=token;
		token = strtok_r(NULL," ",&saveptr);
		i++;
		if(!token)
		{
			continue;
		}
		if(token[0]=='<' || token[0]=='>')
		{
			redirect_flag=1;
		}
		else if(token[strlen(token)-1]=='<' || token[strlen(token)-1]=='>')
		{
			redirect_flag=-1;
		}
		
	}

	//if redirect sign is in the beginning
	//do nothing

	//if redirect sign is in the end
	if(redirect_flag==-1)
	{

		if(token[0]=='>' && if_append)
		{
			int i;
			char c_token[100];
			strcpy(c_token,token);
			for(i=1; i<strlen(c_token); i++)
			{
				token[i-1]=c_token[i];
			}
		}
		arg[i]=token;
		arg[i][strlen(token)-1]='\0';
		i++;
	}

	arg[i]=NULL;
	int n=strlen(arg[i-1]);
	if(arg[i-1][n-1]=='&')
	{
		//printf("background\n");
		if(n==1)
		{
			arg[i-1]=NULL;
			call_inbuilt_background(arg,infile,outfile,if_append);
		}
		else
		{
			arg[i-1][n-1]='\0';
			call_inbuilt_background(arg,infile,outfile,if_append);	
		}
		
	}
	else
	{
		call_inbuilt(arg,infile,outfile,if_append);	
	}
		
}

int find_redirection_files(char infile[], char outfile[], char input[])
{
	int in_index=0,out_index=0,out_flag=0,in_flag=0,if_append=0;
	char cur_input[1000];
	strcpy(cur_input,input);
	int size=strlen(cur_input);
	int i;
	for(i=0; i<size; i++)
	{
		if(cur_input[i]=='<')
		{
			in_flag=1;
		}
		if(cur_input[i]=='>')
		{
			in_flag=0;
			out_flag=1;
			if(cur_input[i+1]=='>')
			{
				if_append=1;
			}
		}
		if(in_flag==1 && cur_input[i]!='<')
		{
			infile[in_index]=cur_input[i];
			in_index++;
		}
		if(out_flag==1 && cur_input[i]!='>')
		{
			outfile[out_index]=cur_input[i];
			out_index++;
		}
	}
	outfile[out_index]='\0';
	infile[in_index]='\0';
	return if_append;
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
	char clock_string[]="clock";
	char setenv_string[]="setenv";
	char unsetenv_string[]="unsetenv";
	char jobs_string[]="jobs";

	//if first word is cd
	if(strcmp(cd,token)==0)
	{
		//token=strtok(NULL," ")
		change_dir(input,cwd,home);
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
			char infile[100];
			char outfile[100];
			//if_append = 1 if there's appending
			int if_append=find_redirection_files(infile,outfile,input);
			call_inbuilt_functions(token,saveptr,infile,outfile,if_append);		
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

	//if the first word is clock
	else if(strcmp(clock_string,token)==0)
	{
		clock_main(input);
	}

	//if the first word is setenv
	else if(strcmp(setenv_string,token)==0)
	{
		setenv_main(input);
	}

	//if the first word is unsetenv
	else if(strcmp(unsetenv_string,token)==0)
	{
		unsetenv_main(input);
	}
	
	//if the first word is jobs
	else if(strcmp(jobs_string,token)==0)
	{
		jobs_main(backid,is_running);
	}


	//to call inbuilt functions
	else
	{
		char infile[100];
		char outfile[100];
		int if_append = find_redirection_files(infile,outfile,input);
		call_inbuilt_functions(token,saveptr,infile,outfile,if_append);	
		
	}
	return;
}


//to execute single command(one part of the | separated commands)
//returns 1 if exit is found
int single_command(char input[])
{
	char copy_input[1000];
	char exit_string[]="exit";
	char* saveptr;
	strcpy(copy_input,input);
	if(strlen(copy_input))
	{
		//printf("%s\n",input);
		char* token = strtok_r(copy_input," ",&saveptr);
		
		//return 1 if command is exit
		if(strcmp(exit_string,token)==0)
		{
			is_exit_clock=1;
			exit(0);
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

//function that takes input between ; and passes to single_command function after input and output stuff
//returns 1 if exiting
int pipe_main(char input[])
{
	char copy_input[1000];
	strcpy(copy_input,input);
	char* saveptr;
	char* token = strtok_r(copy_input,"|",&saveptr);
	int in_desc,out_desc;
	//saving standard input
	int	save_stdin = dup(STDIN_FILENO);
	
	//saving standard output
	int save_stdout = dup(STDOUT_FILENO);
	int desc[2];

	//desc[1] = dup(STDOUT_FILENO);
	desc[0] = dup(STDIN_FILENO);

	//what you write in desc[1] can be read in desc[0]
	//pipe(desc);
	
	//in_desc = dup(STDIN_FILENO);
	
	dup2(desc[1],STDOUT_FILENO);
	dup2(desc[0],STDIN_FILENO);
		

	while(token)
	{
		if(single_command(token))
		{
			return 1;
		}
		else
		{
			token = strtok_r(NULL, "|", &saveptr);
		}
	}
	dup2(save_stdin,STDIN_FILENO);
	dup2(save_stdout,STDIN_FILENO);
	
	return 0;
}


int main()
{

	//initializing backid[] to -1
	int i;
	for(i=0; i<100; i++)
	{
		is_running[i]=-1;
		backid[i]=-1;
	}

	getcwd(home,sizeof(home));
	getcwd(rwd,sizeof(rwd));
	getcwd(cwd,sizeof(cwd));
	char input[1000];
	
	//copy_input so that input is preserved 
	char copy_input[1000];
	char copy_token[1000];
	int program_on = 1;
	
	//keep taking input till exit comes
	while(program_on)
	{
		check_background_over();
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
				//if(pipe_main(token))
				{
					is_exit_clock=1;
					program_on=0;
				}
				else
				{
					token = strtok_r(NULL,";",&saveptr);
				}
			}
		}
		if(!program_on)
		{
			is_exit_clock=1;
			exit(0);
		}
	}
	return 0;
}


