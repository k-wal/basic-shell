#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<fcntl.h> 

long long int backid[100];


//sets input descriptor to infile[] and returns infile_desc
int set_input_desc(char infile[], int infile_desc, int save_stdin)
{
	//to remove leading whitespaces
	char in2[100];
	int i=0;
	while(infile[i]==' ')
	{
		i++;
	}

	int j;
	for( j=i;j<strlen(infile);j++)
	{
		in2[j-i] = infile[j];
	}

	in2[j-i]='\0';
	infile_desc = open(in2,O_RDONLY);

	if(infile_desc<0)
	{	
		printf("Error opening the file.\n");
		_exit(0);
	}
	//replacing stdin with given file
	dup2(infile_desc,STDIN_FILENO);
	return infile_desc;		
}


int set_output_desc(char outfile[], int outfile_desc, int save_stdout, int if_append)
{
	//to remove leading whitespaces
	char out2[100];
	int i=0;
	while(outfile[i]==' ')
	{
		i++;
	}
	int j;
	for( j=i;j<strlen(outfile);j++)
	{
		out2[j-i] = outfile[j];
	}
	out2[j-i]='\0';

	//to check if appended or not
	if(if_append)
	{
		outfile_desc = open(out2,O_WRONLY|O_APPEND|O_CREAT, 0777);
	}
	else
	{
		outfile_desc = open(out2,O_WRONLY|O_CREAT, 0777);
	}

	if(outfile_desc<0)
	{	
		printf("Error opening the file.\n");
		_exit(0);
	}
			
	//replacing stdin with given file
	dup2(outfile_desc,STDOUT_FILENO);
	
}

//to set input mode back to standard input
void set_stdin_back(int infile_desc, int save_stdin)
{
	dup2(save_stdin,STDIN_FILENO);
	close(infile_desc);
	return;
}

void set_stdout_back(int outfile_desc, int save_stdout)
{
	dup2(save_stdout,STDOUT_FILENO);
	close(outfile_desc);
	return;
}

//function to call inbuilt functions foreground
//infile and outfile to implement redirection
//function is of the form arg < infile > outfile
void call_inbuilt(char* arg[], char infile[], char outfile[],int if_append)
{
	pid_t child_id;
	child_id = fork();
	int if_infile=0,if_outfile=0;
	int save_stdin,save_stdout,infile_desc,outfile_desc;
	
	//saving standard input
	save_stdin = dup(STDIN_FILENO);
	
	//saving standard output
	save_stdout = dup(STDOUT_FILENO);
	
	if(child_id==0)
	{
		//To compute infile and change input file from stdin to infile
		if(infile)
		{
			//to make sure infile isn't just whitespaces
			char* saveptr;
			char* token = strtok_r(infile," ",&saveptr);
			if(token)
			{
				if_infile=1;
			}
		}

		//if there's an infile/ input redirection
		if(if_infile)
		{
			infile_desc=set_input_desc(infile,infile_desc,save_stdin);
		}	

		//To compute outfile and change output file from stdout to outfile
		if(outfile)
		{
			//to make sure outfile isn't just whitespaces
			char* saveptr;
			char* token = strtok_r(outfile," ",&saveptr);
			if(token)
			{
				if_outfile=1;
			}
		}

		//if there's an outfile/ output redirection
		if(if_outfile)
		{
			outfile_desc=set_output_desc(outfile,outfile_desc,save_stdout,if_append);
		}	

		//executing the command
		execvp(arg[0],arg);
			
		//getting stdin back
		if(if_infile)
		{
			set_stdin_back(infile_desc,save_stdin);
		}

		//getting stdout back
		if(if_outfile)
		{
			set_stdout_back(outfile_desc,save_stdout);	
		}
		perror("Error:");
		_exit(0);

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
//infile and outfile to implement redirection
//function is of the form arg < infile > outfile
void call_inbuilt_background(char* arg[], char infile[], char outfile[],int if_append)
{
	pid_t child_id;
	child_id = fork();
	int if_infile=0,if_outfile=0;
	int save_stdin,save_stdout,infile_desc,outfile_desc;
	
	//saving standard input
	save_stdin = dup(STDIN_FILENO);
	
	//saving standard output
	save_stdout = dup(STDOUT_FILENO);
	
	if(child_id==0)
	{
		/*
		execvp(arg[0],arg);
		perror("Error:");
		_exit(0);
		*/
		//To compute infile and change input file from stdin to infile
		if(infile)
		{
			//to make sure infile isn't just whitespaces
			char* saveptr;
			char* token = strtok_r(infile," ",&saveptr);
			if(token)
			{
				if_infile=1;
			}
		}

		//if there's an infile/ input redirection
		if(if_infile)
		{
			infile_desc=set_input_desc(infile,infile_desc,save_stdin);
		}	

		//To compute outfile and change output file from stdout to outfile
		if(outfile)
		{
			//to make sure outfile isn't just whitespaces
			char* saveptr;
			char* token = strtok_r(outfile," ",&saveptr);
			if(token)
			{
				if_outfile=1;
			}
		}

		//if there's an outfile/ output redirection
		if(if_outfile)
		{
			outfile_desc=set_output_desc(outfile,outfile_desc,save_stdout,if_append);
		}	

		//executing the command
		execvp(arg[0],arg);
			
		//getting stdin back
		if(if_infile)
		{
			set_stdin_back(infile_desc,save_stdin);
		}

		//getting stdout back
		if(if_outfile)
		{
			set_stdout_back(outfile_desc,save_stdout);	
		}
		perror("Error:");
		_exit(0);

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