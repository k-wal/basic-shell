#include<stdio.h>
#include<string.h>
#include<unistd.h>	
#include<fcntl.h>


//function to calculate Relative path
void find_relative_path(char cwd[],char home[],char rwd[])
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


void pinfo_main(char input[],char home[])
{
	char input_id[100];
	char proc[100];
	proc[0]='\0';
	strcpy(proc,"/proc/");
	char* saveptr;
	char* token = strtok_r(input," ",&saveptr);
	token = strtok_r(NULL," ",&saveptr);
	if(token!=NULL)
	{
		//strcpy(input_id,token);
		strcat(proc,token);
	}
	else
	{		
		//strcpy(input_id,str(getpid()));
		strcat(proc,"self");
	}
	char stat[100];
	
	//setting stat = /proc/id/stat
	strcpy(stat,proc);
	strcat(stat,"/stat");
	FILE* statfile;
	if(!(statfile = fopen(stat,"r")))
	{
		printf("Error finding info on process\n.");
		return;
	}
	long long int pid;
	char status;
	char name[20];
	
	//scanning pid, name and status
	fscanf(statfile,"%lld %s %c",&pid,name,&status);
	fclose(statfile);

	printf("pid -- %lld\n",pid);
	printf("Process Status -- %c\n",status);

	//setting stat = /proc/id/statm
	strcpy(stat,proc);
	strcat(stat,"/statm");
	if(!(statfile = fopen(stat,"r")))
	{
		printf("Error finding info on process : %d\n",getpid());
	}

	long long int size;

	//scanning size in virtual memory
	fscanf(statfile,"%lld",&size);
	fclose(statfile);

	printf("%lld {Virtual Memory}\n",size);

	//setting stat = /proc/id/exe
	strcpy(stat,proc);
	strcat(stat,"/exe");
	char path[100];
	ssize_t path_len = readlink(stat, path, sizeof(path));
    if(path_len < 0) 
    {
        printf("Error reading symbolic link %s.\n", stat);
        return;
    }

    char rwd[100];
    find_relative_path(path,home,rwd);
    printf("%s\n",rwd);
	

}


