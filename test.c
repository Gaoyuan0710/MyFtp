#include "client.h"

void my_err(const char * err_string, int line)
{
	fprintf(stderr, "line: %d ", line);
	perror(err_string);
	exit (1);
}
int return_size( char * pathname, char  *filename)
{
	
	int len;
	float size;

	char tmp[1024];
        struct stat  buf;
        
        chdir(pathname);
	if (stat(filename, &buf) == -1)
{
    my_err("stat", __LINE__);
}
	len = buf.st_size;
	return (len);
}
void user_find(int conn_fd, char * local_path)
{

	char server_path[SIZE];
	char  filename[SIZE];
	char file_path[SIZE];
	struct dirent * ptr;
	struct stat buf;
	DIR  * dir;
	int fd;

	stat (local_path, &buf);
	if (S_ISREG(buf.st_mode))
	{
		printf("进入上传页面\n");
		while (1)
		  ;
	}
	chdir(local_path);
	getcwd(file_path, sizeof(file_path));
	file_path[strlen(file_path)] = '/';
	file_path[strlen(file_path) + 1] = '\0';
	dir = opendir(file_path);
	printf("\n\n\t\t.\n\n\t\t.");
		while ((ptr = readdir(dir)) != NULL)
		{
			if (strcmp(ptr->d_name, ".") && strcmp(ptr->d_name, ".."))
			{
				printf("\n\n\t\t%s", ptr->d_name);
			}
		}
		printf("\n");
		printf("请选择要上传的文件\n..返回上层目录\n");
		scanf("%s", filename);
		if (strncmp(filename, "..", 2) == 0)
		{
			user_find(conn_fd, "..");
		}
		else
		{
			user_find(conn_fd, filename);
		}

}
int main ()
{
	char file_path[SIZE];

	chdir("/home/");
	getcwd(file_path, sizeof(file_path));

user_find(1, file_path);

}

