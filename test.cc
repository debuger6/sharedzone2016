#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>


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

