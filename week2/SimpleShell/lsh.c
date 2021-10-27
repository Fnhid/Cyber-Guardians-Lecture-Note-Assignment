#include "lsh.h"

/*
   @brief Builtin command: mv.
   @param args 
   @return Always returns 0, to terminate execution.
 */


typedef struct fileinfo{
	char perm[11];
	int linkcount;
	char uid[10];
	char gid[10];
	int size;
	int date[2];
	int time[2];
	char filename[NAME_MAX];
	char link[255];
}fi;

int lsh_mv(char **args){
	int args1_dir = 0;
	int args2_dir = 0;
	int args2_exist = 0;
	int argc_mv = 1;
	while(args[argc_mv] != NULL){
		argc_mv++;
	}
	if(argc_mv < 3){
		printf("failed cuz of no operand\n");
		return 0;
	}
	else if(argc_mv == 3){
		
		struct stat stold;
		struct stat stnew;
		if(!stat(args[1], &stold)){
			if(S_ISDIR(stold.st_mode)){
				args1_dir = 1;
				printf("[TEST] 1 DIR\n");
			}
		}
		else{
			printf("%s: no such file or directory!\n", args[1]);
		}
		if(!stat(args[2], &stnew)){
			if(S_ISDIR(stnew.st_mode)){
				args2_dir = 1;
				printf("[TEST] 2 DIR\n");
			}
		} 
		else{
			args2_exist = 1;
		}
		if(!args1_dir && !args2_dir){ // file to file (change file name)
			if(rename(args[1], args[2])){
				printf("failed while renaming the file\n");
			}
		}
		else if(args1_dir && args2_dir || !args1_dir && args2_dir) // dir or file to dir (move path)
		{
				char *path = malloc(sizeof(char)*(strlen(args[1])+strlen(args[2])+1));
				memset(path, 0, sizeof(char) * (strlen(args[1])+strlen(args[2])+1));
				strcpy(path, args[2]);
				strcat(path, "/");
				strcat(path, args[1]);
				if(rename(args[1], path)){
					printf("failed while renaming the files\n");
				}
				free(path);
		}
		else if(args1_dir && !args2_dir)
		{
			if(!args2_exist){
				printf("cannot overwrite non-directory '%s' with directory '%s'!\n", args[2], args[1]);
			} 
			else{ // dir to dir (change name)
				if(rename(args[1], args[2])){
					printf("failed while renaming the dir\n");
				}
			}
		}
	}
	else if(argc_mv > 3){
		struct stat stlast;
		if(!stat(args[argc_mv - 1], &stlast)){
			if(S_ISDIR(stlast.st_mode)){ //files or directories to diectory
				char *path;
				char *path_old = path; 
				for(int i = 0 ; i < argc_mv ; i++){
					path = malloc(sizeof(char) * (strlen(args[i]) + strlen(args[argc_mv-1])+1));
					memset(path, 0, sizeof(char) * (strlen(args[i]) + strlen(args[argc_mv-1])+1));
					strcpy(path, args[argc_mv-1]);
					strcat(path, "/");
					strcat(path, args[i]);
					rename(args[i], path);
					
					free(path);
				}
			}
			else{
				for(int i = 0 ; i < argc_mv ; i++){
					if(!args[i]){
						printf("%s: no such file or directory!\n", args[i]);
					}
				}
				printf("%s is not a directory!\n", args[argc_mv - 1]);
			}
		}
		
		
	}
	
}

int lsh_ls(char **args)
{
	int totalsize = 0;
	struct stat sb;
	DIR *dp = NULL;
	struct dirent *entry = NULL;
	int option_a = 0;
	int option_al = 0;
	int check_ls, argc_ls;
	int i = 1;
	int noargs_flag = 0;
	check_ls = NULL;
	while (args[i] != NULL)
	{
		if (args[i][0] == '-')
		{
			check_ls = i;
		}

		i++;
	}

	argc_ls = i;

	//printf("%d ", check_ls);
	if (argc_ls - check_ls == 1)
	{
		noargs_flag = 1;
	}

	//printf("%d ", argc_ls);
	//printf("%d", noargs_flag);

	if (!check_ls)
	{
		if (!noargs_flag)
		{
			for (i = 1; i < argc_ls; i++)
			{
				if (i != check_ls)
				{
					if ((dp = opendir(args[i])) == NULL)
					{
						printf("File or directory not found\n");
						return 1;
					}

					printf("%s:\n", args[i]);
					while ((entry = readdir(dp)) != NULL)
					{
						if (entry->d_name[0] != '.')
						{
							printf("%s ", entry->d_name);
						}
					}

					printf("\n");
					closedir(dp);
				}
			}
		}
		else
		{
			dp = opendir(".");
			while ((entry = readdir(dp)) != NULL)
			{
				if (entry->d_name[0] != '.')
				{
					printf("%s ", entry->d_name);
				}
			}

			printf("\n");
			closedir(dp);
		}
	}
	else if (!strcmp(args[check_ls], "-a"))
	{
		if (!noargs_flag)
		{
			for (i = 1; i < argc_ls; i++)
			{
				if (i != check_ls)
				{
					if ((dp = opendir(args[i])) == NULL)
					{
						printf("File or directory not found\n");
						return 1;
					}

					printf("%s:\n", args[i]);
					while ((entry = readdir(dp)) != NULL)
					{
						printf("%s ", entry->d_name);
					}

					printf("\n");
					closedir(dp);
				}
			}
		}
		else
		{
			dp = opendir(".");
			while ((entry = readdir(dp)) != NULL)
			{
				printf("%s ", entry->d_name);
			}

			printf("\n");
			closedir(dp);
		}
	}
	else if (!strcmp(args[check_ls], "-al") || !strcmp(args[check_ls], "-la"))
	{
		if (!noargs_flag)
		{
			for (i = 1; i < argc_ls; i++)
			{
				if (i != check_ls)
				{
					char temp[80];
					char *str[argc_ls];
					if ((dp = opendir(args[i])) == NULL)
					{
						printf("File or directory not found\n");
						return 1;
					}

					printf("%s:\n", args[i]);
					while ((entry = readdir(dp)) != NULL)
					{
						
					}

					printf("\n");
					closedir(dp);
				}
			}
		}
		else
		{
			

			dp = opendir(".");
			while ((entry = readdir(dp)) != NULL)
			{
				printf("%s\n", entry->d_name);
			}

			printf("\n");
			closedir(dp);
		}
	}
	else if (!strcmp(args[check_ls], "--help"))
    {
	printf("TIP : ls[PATH][OPTIONS]\n");
	printf("Funny Options >\n-a : shows every entries.\n-l : shows the list in a long list format.\n--help : this command!");
	printf("made by Fnhid.\n");
    }
}

//to implement every functions of mv command, we need to use stat and rename. and also to check if the file is a directory or not. if it is a directory, we need to use opendir and readdir. if it is a file, we need to use open and read. to move the directory, we need to use opendir and readdir and rename. to move the file, we need to use open and read and rename.