#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<time.h>

//function to print ls -l and ls -la and ls -al results
//return 0 if successfull, 1 if not
int ls_with_l(int is_a,char path[])
{
	DIR *d;
	struct dirent *dir;
	
	if(path==NULL)
		d=opendir(".");
	else
	{
		d=opendir(path);
		if(d==NULL)
		{
			//perror("Error:");
			//return;
			return 1;
		}
	}
	if(d)
	{
		while((dir=readdir(d))!=NULL)
		{
			if((dir->d_name[0]=='.' && is_a==1)||(dir->d_name[0]!='.'))
			{
				struct stat f;
				stat(dir->d_name,&f);

				//to get the last modified time in the right format
				time_t t = f.st_mtime;
				struct tm lt;
				localtime_r(&t, &lt);
				char timbuf[80];
				strftime(timbuf, sizeof(timbuf), "%b %d  %H:%M", &lt);
				
				//printing file permissions
				printf( (S_ISDIR(f.st_mode)) ? "d" : "-");
    			printf( (f.st_mode & S_IRUSR) ? "r" : "-");
    			printf( (f.st_mode & S_IWUSR) ? "w" : "-");
    			printf( (f.st_mode & S_IXUSR) ? "x" : "-");
    			printf( (f.st_mode & S_IRGRP) ? "r" : "-");
    			printf( (f.st_mode & S_IWGRP) ? "w" : "-");
    			printf( (f.st_mode & S_IXGRP) ? "x" : "-");
	    		printf( (f.st_mode & S_IROTH) ? "r" : "-");
    			printf( (f.st_mode & S_IWOTH) ? "w" : "-");
    			printf( (f.st_mode & S_IXOTH) ? "x" : "-");
    			printf("\t");
    			
    			//printing file size
    			printf("%ld\t",f.st_size);
  				
    			//printing last modified time and date
  				printf("%s\t",timbuf);
				
  				//printing file/directory name
				printf("%s\n",dir->d_name);
			}
		}
		//printf("\n");
		closedir(d);

	}
	return 0;
}


//function to print ls -a and ls results
//returns 0 if successful, 1 if not
int ls_without_l(int is_a,char path[])
{
	DIR *d;
	struct dirent *dir;
	if(path==NULL)
		d=opendir(".");
	else
	{
		d=opendir(path);
		if(d==NULL)
		{
			return 1;
			//perror("Error:");
			//return;
		}
	}
	if(d)
	{
		while((dir=readdir(d))!=NULL)
		{
			if(dir->d_name[0]=='.' && is_a==1)
				printf("%s\n",dir->d_name);
			else if(dir->d_name[0]!='.')
				printf("%s\n",dir->d_name);
		}
		//printf("\n");
		closedir(d);

	}
	return 0;
}


//function that decides which ls function to call and finds errors(if any)
//returns 0 if successful, 1 if not
int ls_main(char input[])
{
	char* saveptr;
	char* token = strtok_r(input," ",&saveptr);
	token = strtok_r(NULL," ",&saveptr);
	
	//if no arguments/flags are given
	if(token==NULL)
	{
		return ls_without_l(0,NULL);
	}
	
	//if flag is a
	if(strcmp(token,"-a")==0)
	{
		token = strtok_r(NULL," ",&saveptr);
		return ls_without_l(1,token);
	}

	//if flag is l
	else if(strcmp(token,"-l")==0)
	{
		token = strtok_r(NULL," ",&saveptr);
		return ls_with_l(0,token);
	}

	//if flags are al
	else if(strcmp(token,"-al")==0)
	{
		token = strtok_r(NULL," ",&saveptr);
		return ls_with_l(1,token);
	}

	//if flags are la
	else if(strcmp(token,"-la")==0)
	{
		token = strtok_r(NULL," ",&saveptr);
		return ls_with_l(1,token);
	}

	else
	{
		if(token[0]=='-')
		{
			return 1;
		}
		else
		{
			return ls_without_l(0,token);
		}
	}

}