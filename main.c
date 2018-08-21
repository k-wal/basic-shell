#include<stdio.h>
#include<string.h>
#include<unistd.h>

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
		for(i=0; i<h; i++)
		{
			if(home[i]!=cwd[i])
			{
				flag=1;
				break;
			}			
		}
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
			strcpy(rwd,"~/");
			strcpy(&rwd[2],&cwd[i]);
		}
	}
	
}


//function to print prompt, take input and return a string
void take_input(char input[])
{
	find_rwd();
	printf("<Olivia@UBUNTU: %s> ",rwd);
	fgets (input, 100, stdin);
	int len = strlen(input);
	if(len)
	{	
		input[len-1]='\0';
	}

	return;
}

//function to change directory
void change_dir(char input[])
{
	char* token = strtok(input," ");
	token = strtok(NULL," ");
	
	if(token==NULL)
	{
		printf("Expected path not found. Its my shell, fuck off.\n");
		return;
	}
	if(chdir(token)!=0)
	{
		perror("Error");
	}
	else
	{
		getcwd(cwd,sizeof(cwd));
		printf("directory is now: %s\n",cwd);
	}
	return;
}


//function to call another file for a command
void call_for_command(char command[],char input[])
{
	char copy_input[1000];
	strcpy(copy_input,input);
	char* token = strtok(copy_input," ");
	char cd[]="cd";
	char pwd[]="pwd";
	char ls[]="ls";

	//if first word is cd
	if(strcmp(cd,token)==0)
	{
		//token=strtok(NULL," ")
		change_dir(input);
	}

	//if first word is pwd
	if(strcmp(pwd,token)==0)
	{
		printf("%s\n",cwd);
	}

	//if first word is ls
	if(strcmp(ls,token)==0)
	{
		execlp("./ls","./ls",input,cwd,(char*)NULL);		
	}
	return;
}


int main()
{
	getcwd(home,sizeof(home));
	getcwd(rwd,sizeof(rwd));
	getcwd(cwd,sizeof(cwd));
	char input[1000];
	char copy_input[1000];
	char exit[]="exit";
	int program_on = 1;
	while(program_on)
	{
		take_input(input);
		strcpy(copy_input,input);
		if(strlen(copy_input))
		{

			//printf("%s\n",input);
			char* token = strtok(copy_input," ");
			if(strcmp(exit,token)==0)
			{
				program_on=0;
			}
			else
			{
				call_for_command(token,input);
				/*
				while(token!=NULL)
				{
					printf("%s\n",token);
					token=strtok(NULL," ");
				}*/
			}
		}
	}
	return 0;
}


