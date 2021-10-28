#include "lsh.h"

/*
   @brief Builtin command: mv.
   @param args 
   @return Always returns 0, to terminate execution.
 */


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
				//printf("[TEST] 1 DIR\n");
			}
		}
		else{
			printf("%s: no such file or directory!\n", args[1]);
		}
		if(!stat(args[2], &stnew)){
			if(S_ISDIR(stnew.st_mode)){
				args2_dir = 1;
				//printf("[TEST] 2 DIR\n");
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
	long long blocks = 0;
	char *t;
	char path[80];
	char tmp[80];
	struct tm* timeinfo;
	int totalsize = 0;
	struct stat sb;
	DIR *dp = NULL;
	struct dirent *entry = NULL;
	int option_a = 0;
	int option_al = 0;
	int check_ls = 0;
	int argc_ls;
	int i = 1;
	int noargs_flag = 0;
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
					if(argc_ls > 2){
						printf("%s:\n", args[i]);
					}
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
					if(argc_ls > 3){
						printf("%s:\n", args[i]);
					}
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
					
					if ((dp = opendir(args[i])) == NULL)
					{
						printf("File or directory not found\n");
						return 1;
					}
					
					if(argc_ls > 3){
						printf("%s:\n", args[i]);
					}
					
					while ((entry = readdir(dp)) != NULL)
					{
						strcpy(path, args[i]);
						strcat(path, "/");
						strcat(path, entry->d_name);
						stat(path, &sb);
						blocks += sb.st_blocks / 2;
						
					}
					printf("total %lld\n", blocks );
					blocks = 0;
					closedir(dp);
					
					
					if ((dp = opendir(args[i])) == NULL)
					{
						printf("File or directory not found\n");
						return 1;
					}

					while ((entry = readdir(dp)) != NULL)
					{
						if (entry->d_name[0] != '.')
						{
							strcpy(path, args[i]);
							strcat(path, "/");
							strcat(path, entry->d_name);
							stat(path, &sb);
							
							if(S_ISREG(sb.st_mode))
							{
								printf("-");
							}
							else if(S_ISDIR(sb.st_mode))
							{
								printf("d");
							}
							else if(S_ISCHR(sb.st_mode))
							{
								printf("c");
							}
							else if(S_ISBLK(sb.st_mode))
							{
								printf("b");
							}
							else if(S_ISFIFO(sb.st_mode))
							{
								printf("p");
							}
							else if(S_ISLNK(sb.st_mode))
							{
								printf("l");
							}
							else if(S_ISSOCK(sb.st_mode)){
								printf("s");
							} // check file types

							printf( (sb.st_mode & S_IRUSR) ? "r" : "-");
							printf( (sb.st_mode & S_IWUSR) ? "w" : "-");
							printf( (sb.st_mode & S_IXUSR) ? "x" : "-");
							printf( (sb.st_mode & S_IRGRP) ? "r" : "-");
							printf( (sb.st_mode & S_IWGRP) ? "w" : "-");
							printf( (sb.st_mode & S_IXGRP) ? "x" : "-");
							printf( (sb.st_mode & S_IROTH) ? "r" : "-");
							printf( (sb.st_mode & S_IWOTH) ? "w" : "-");
							printf( (sb.st_mode & S_IXOTH) ? "x " : "- ");

							
							printf("%3ld ", sb.st_nlink);
							printf("%s ", getpwuid(sb.st_uid)->pw_name);
							printf("%s ", getgrgid(sb.st_gid)->gr_name);
							printf("%7ld ", sb.st_size);
							timeinfo = localtime(&sb.st_mtime);
							strftime(tmp, sizeof(tmp),"%a %d %H:%M", timeinfo);
							t = &tmp;
							if (t[strlen(t)-1] == '\n') t[strlen(t)-1] = '\0'; //remove new line
							
							printf("%s %s\n", t, entry->d_name);
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
			printf(".:\n");
			
					
			while ((entry = readdir(dp)) != NULL)
			{
				
				strcpy(path, "./");
				strcat(path, entry->d_name);
				stat(path, &sb);
				blocks += sb.st_blocks / 2;
				
			}
			printf("total %lld\n", blocks );
			blocks = 0;
			closedir(dp);
			
			
			if ((dp = opendir(".")) == NULL)
			{
				printf("File or directory not found\n");
				return 1;
			}

			while ((entry = readdir(dp)) != NULL)
			{
				if (entry->d_name[0] != '.')
				{
					strcpy(path, "./");
					strcat(path, entry->d_name);
					stat(path, &sb);
					
					if(S_ISREG(sb.st_mode))
					{
						printf("-");
					}
					else if(S_ISDIR(sb.st_mode))
					{
						printf("d");
					}
					else if(S_ISCHR(sb.st_mode))
					{
						printf("c");
					}
					else if(S_ISBLK(sb.st_mode))
					{
						printf("b");
					}
					else if(S_ISFIFO(sb.st_mode))
					{
						printf("p");
					}
					else if(S_ISLNK(sb.st_mode))
					{
						printf("l");
					}
					else if(S_ISSOCK(sb.st_mode)){
						printf("s");
					} // check file types

					printf( (sb.st_mode & S_IRUSR) ? "r" : "-");
					printf( (sb.st_mode & S_IWUSR) ? "w" : "-");
					printf( (sb.st_mode & S_IXUSR) ? "x" : "-");
					printf( (sb.st_mode & S_IRGRP) ? "r" : "-");
					printf( (sb.st_mode & S_IWGRP) ? "w" : "-");
					printf( (sb.st_mode & S_IXGRP) ? "x" : "-");
					printf( (sb.st_mode & S_IROTH) ? "r" : "-");
					printf( (sb.st_mode & S_IWOTH) ? "w" : "-");
					printf( (sb.st_mode & S_IXOTH) ? "x " : "- ");

					
					printf("%3ld ", sb.st_nlink);
					printf("%s ", getpwuid(sb.st_uid)->pw_name);
					printf("%s ", getgrgid(sb.st_gid)->gr_name);
					printf("%7ld ", sb.st_size);
					timeinfo = localtime(&sb.st_mtime);
					strftime(tmp, sizeof(tmp),"%a %d %H:%M", timeinfo);
					t = &tmp;
					if (t[strlen(t)-1] == '\n') t[strlen(t)-1] = '\0'; //remove new line
					
					printf("%s %s\n", t, entry->d_name);
				}
			}
			closedir(dp);
		}
		
	}
	else if (!strcmp(args[check_ls], "--help"))
    {
	printf("TIP : ls [PATH] [OPTIONS]\n");
	printf("Funny Options >\n-a : shows every entries.\n-l : shows the list in a long list format.\n--help : this command!\n");
	
    }
}

//to implement every functions of mv command, we need to use stat and rename. and also to check if the file is a directory or not. if it is a directory, we need to use opendir and readdir. if it is a file, we need to use open and read. to move the directory, we need to use opendir and readdir and rename. to move the file, we need to use open and read and rename.