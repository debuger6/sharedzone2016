#ifndef _CREATE_DIR_OR_FILE_H_
#define _CREATE_DIR_OR_FILE_H_


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

int create_multi_dir(const char *path)
{
	int i, len;
 	len = strlen(path);
	char dir_path[len+1];
	dir_path[len] = '\0';

	strncpy(dir_path, path, len);

	for (i=0; i<len; i++)
	{
		if (dir_path[i] == '/' && i > 0)
		{		dir_path[i]='\0';
																																	  if (access(dir_path, F_OK) < 0)
																															   		  {
				if (mkdir(dir_path, 0755) < 0)
																																		  {
					 printf("mkdir=%s:msg=%s\n", dir_path, strerror(errno));
					 return -1;
																																																									                                }																																																							                     }
																																							                        dir_path[i]='/';
																																													                }
																					        }

													        return 0;
}


//判断文件是否存在
int is_file_exist(const char *file_path)
{
	if (NULL == file_path)
	{
		return -1;
	}

	if (access(file_path, F_OK) == 0)
	{
		return 0;
	}
	return -1;
}

//判断目录是否存在
int is_dir_exist(const char *dir_path)
{
	if (NULL == dir_path)
		return -1;
	if (opendir(dir_path) == NULL)
		return -1;
	return 0;
}
#endif
