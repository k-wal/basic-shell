#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<dirent.h>

int main(int argc, char *argv[])
{
	//printf("THIS IS THE LS FILE\n");	

	if(argc!=2)
	{
		printf("Error: not enough arguments for ls\n");
	}
	DIR *d;
	struct dirent *dir;
	d=opendir(argv[1]);
	if(d)
	{
		while((dir=readdir(d))!=NULL)
		{
			printf("%s\n",dir->d_name);
		}
		closedir(d);
	}
	return 0;
}