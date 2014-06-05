/*
 * =====================================================================================
 *
 *       Filename:  server.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月04日 19时42分54秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  gaoyuan, 
 *        Company:  Class 1204 of Computer Science and Technology
 *
 * =====================================================================================
 */
#include "server.h"

int find_name(const char * name)
{
	char pathname[32] = {"/home/qiong/userinfo/"};
	int fd;

	if (name == NULL)
	{
		sys_log("姓名为空");
		return -2;
	}

	strcat(pathname, name);

	if ((fd = open(pathname, O_RDONLY)) == -1)
	{
		return 1;                  //代表无该用户文件
	}

	close(fd);
	return 0;                      //代表有该用户
}

int check_passwd(const char *name, const char *passwd)
{
	char pathname[32] = {"/home/qiong/userinfo/"};
	char tmp[40];
	int len;
	int fd;
	int ret;

	if (name == NULL || passwd == NULL)
	{
		sys_log("用户名或密码为空");
		return -2;
	}

	strcat(pathname, name);

	if ((fd = open(pathname, O_RDONLY)) == -1)
	{
		return -1;
	}


	if (lseek(fd, 0, SEEK_END) == -1)
	{
		err_log("lseek 错误");
		my_err("lseek", __LINE__);
	}

	if ((len = lseek(fd, 0, SEEK_END))  == -1)
	{
		err_log("lseek 错误");
		my_err("lseek", __LINE__);
	}

	if (lseek(fd, 0, SEEK_SET) == -1)
	{
		err_log("lseek 错误");
		my_err("lseek", __LINE__);
	}

	if ((ret = read(fd, tmp, len)) < 0)
	{
		err_log("read 错误");
		my_err("read", __LINE__);
	}

	close(fd);

	return (strncmp(tmp, passwd, strlen(passwd)));               //若返回0,则表示用户名和密码都正确
}

void server_check(int conn_fd)
{
	char buf[32];
	char name[32];
	char passwd[32];
	char pathname[SIZE] = {"/home/qiong/userinfo/"};
	int  ret;
	int  flag;
	int  fd;
	char log[SIZE];
	int i;

	send_data(conn_fd, "b\n");
	sleep(1);
	while(1)
	{
		my_recv(conn_fd, buf, sizeof(buf));
		if (buf[0] == 'u')
		{
			send_data(conn_fd, "b\n");
			sleep(1);
			ret = recv(conn_fd, buf, sizeof(buf), 0);
			buf[ret-1] = '\0';
			strcpy(name, buf);
			flag = find_name(buf);
			if (flag == 0)
			{
				send_data(conn_fd, "y\n");
				sleep(1);
			}
			else
			{
				send_data(conn_fd, "n\n");
				sleep(1);
				continue;
			}
			my_recv(conn_fd, buf, sizeof(buf));
			send_data(conn_fd, "b\n");
			sleep(1);
			ret = recv(conn_fd, buf, sizeof(buf), 0);
			buf[ret-1] = '\0';
			strcpy(passwd, buf);
			memset(passwd, '\0', sizeof(passwd));
			flag = check_passwd(name, passwd);
			if (flag == 0)
			{
				send_data(conn_fd, "y\n");
				sleep(1);
				strcat(ip_name, "  用户名:");
				strcat(ip_name, name);
				strcpy(name, "验证成功");
				sys_log(name);
				deal(conn_fd);
			}
			else
			{
				send_data(conn_fd, "n\n");
				sleep(1);
				continue;
			}
		}
	}
}

void server_register(int conn_fd)
{
	char buf[32];
	char name[32];
	char passwd[32];
	char pathname[SIZE] = {"/home/qiong/userinfo/"};
	int  ret;
	int  flag;
	int  fd;
	char log[SIZE];

	send_data(conn_fd, "b\n");
	sleep(1);	
	while(1)
	{
		my_recv(conn_fd, buf, sizeof(buf));
		if (buf[0] == 'u')
		{
			strcpy(log, "请求注册用户名：");
			send_data(conn_fd, "b\n");
			sleep(1);
			ret = recv(conn_fd, buf, sizeof(buf), 0);
			buf[ret-1] = '\0';
			strcpy(name, buf);
			strcat(log, buf);
			flag = find_name(buf);
			if (flag == 0)
			{
				send_data(conn_fd, "n\n");
				sleep(1);
				strcat(log, "注册失败");
				sys_log(log);
				continue;
			}
			else
			{
				send_data(conn_fd, "y\n");
				sleep(1);
				strcat(log, "注册成功");
				strcat(ip_name, "用户名:");
				strcat(ip_name, buf);
				sys_log(log);
				continue ;
			}
		}	
		if (buf[0] == 'p')
		{
			send_data(conn_fd, "b\n");
			sleep(1);
			ret = recv(conn_fd, buf, sizeof(buf), 0);
			buf[ret-1] = '\0';
			strcpy(passwd, buf);
			send_data(conn_fd, "y\n");
			sleep(1);
			strcat(pathname, name);
			fd = open(pathname, O_WRONLY|O_CREAT|O_TRUNC, 0777);
			write(fd, passwd, sizeof(passwd));
			close(fd);
			sys_log("注册用户跳转至登陆界面");
			deal(conn_fd);
		}
	}
}

void deal(int conn_fd)
{
	char buf[32];
	char name[32];
	char passwd[32];
	char pathname[SIZE] = {"/home/qiong/userinfo/"};
	int  ret;
	int  flag;
	int  fd;
	int zan;


	my_recv(conn_fd, buf, sizeof(buf));
	printf("%c\n", buf[0]);
	if (buf[0] == 'l')
	{
		sys_log("进入用户登陆界面");
		server_check(conn_fd);
	}
	else if (buf[0] == 'r')
	{
		sys_log("进入用户注册界面");
		server_register(conn_fd);	
	}	
	else if (buf[0] == 'x')
	{
		chdir("/home/qiong/shujia/source/");
		server_browse(conn_fd, "../source/");
	}
	else if (buf[0] == 'q')
	{
		sys_log("浏览本地上传目录");
		chdir("/home/qiong/shujia/source/");
		server_look(conn_fd, "../source/");
	}
	else if (buf[0] == 'd')
	{
		sys_log("查找资源");
		sys_find(conn_fd);
		deal(conn_fd);
	}
}

void server_download(int conn_fd, const char * pathname, char * len)
{
	int fd;
	char buf[SIZE];
	char choose[SIZE];
	int ret;
	int lenght = 0;
	int sum = 0;
	int flag;
	int i = 0;
	char tmp[SIZE];
	long r_len;
	char log[SIZE];

	if ((fd = open(pathname, O_RDONLY)) == -1)
	{
		strcpy(log, pathname);
		strcat(log, " 下载失败");
		sys_log(log);
		return ;
	}
	strcpy(log, " 文件名:");
	strcat(log, pathname);
	strcat(log, " 下载文件大小:");
	strcat(log, len);
	sys_log(log);
	
	send_data(conn_fd, len);
	recv(conn_fd, tmp, sizeof(tmp), 0);
	r_len = atol(tmp);
	lseek(fd, r_len, SEEK_SET);

	while (1)
	{
		flag = read(fd, buf, SIZE);
		sum += flag;
		ret = send(conn_fd, buf, flag, 0);
		if (flag !=  SIZE)
		{
			break;
		}
	}
	close (fd);

	memset(log, '\0', sizeof(log));
	strcpy(log, " 下载成功,");
	my_recv(conn_fd, buf, sizeof(buf));

	if (buf[0] == 'y')
	{
		sys_log(" 用户返回选择界面 ");
		deal(conn_fd);
	}
	else
	{
		return ;
	}
}

void server_browse(int conn_fd,  char * pathname)
{
	int i = 0;
	int j = 0;
	char tmp[SIZE];
	char string[SIZE];
	char file_name[SIZE] = {"\n\n\t\t.\n\n\t\t..\n\n\t\t"};
	char info[SIZE];
	struct dirent * ptr;
	struct stat buf;
	DIR  * dir;
	char  lenght[SIZE];
	int len;
	char log[SIZE];

	stat(pathname, &buf);
	if (S_ISREG(buf.st_mode))
	{
		send_data(conn_fd, "b\n");
		my_recv(conn_fd, info, sizeof(info));
		if (info[0] == 'y')
		{
			sys_log("下载单个文件");
			sprintf(lenght, "%d", (int)buf.st_size);
			printf("%s\n", lenght);
			server_download(conn_fd, pathname, lenght);
		}
	}
	chdir(pathname);
	memset(string, '\0', sizeof(string));
	getcwd(string, sizeof(string));
	string[strlen(string)] = '/';
	string[strlen(string)+1] = '\0';

	strcpy(log, "浏览服务器目录:");
	strcat(log, string);
	sys_log(log);
	if (strcmp(string, "/home/qiong/") == 0)
	{
		send_data(conn_fd, "n\n");
		deal(conn_fd);
	}

	while (1)
	{
		if ((dir = opendir (string)) == NULL)
		{
			sys_log("打开目录失败");
			printf("%s\n", string);
			return ;
		}
		while ((ptr = readdir(dir)) != NULL)
		{
			if (strcmp(ptr->d_name, ".") && strcmp(ptr->d_name, ".."))
			{
				strcat(file_name, ptr->d_name);
				strcat(file_name, "\n\n\t\t");
			}
		}
		file_name[strlen(file_name)] = '\0';
		len = send(conn_fd, file_name, strlen(file_name), 0);
		memset(info, 0, sizeof(info));
		len = recv(conn_fd, info, sizeof(info), 0);
		info[strlen(info)] = '\0';
		j = 0;
		for (i =0; i < len; i++)
		{
			if (info[i] == ' '|| info[i] == ',')
			{
				j = 1;
				break;
			}
		}
		if (j == 1)
		{
			sys_log("进行批量下载操作");
			server_batch(conn_fd);            
			deal(conn_fd);
		}
		
		
		if (strncmp(info, "..", 2) == 0)
		{
			sys_log("返回上层目录");
			server_browse(conn_fd, "..");
		}
		else if (info[0] == '.')
		{
			;
		}
		else 
		{
			getcwd(string, sizeof(string));
			string[strlen(string)] = '/';
			string[strlen(string)+1] = '\0';
			strcat(string, info);
			string[strlen(string)] = '\0';
			server_browse(conn_fd, string);
		}
		
	}
}

void server_look(int conn_fd, char * local_path)
{

	char server_path[SIZE];
	char  filename[SIZE];
	char file_path[SIZE];
	char len[SIZE];
	char tmp[SIZE] = {"\n\n\t\t.\n\n\t\t.."};
	struct dirent * ptr;
	struct stat buf;
	DIR  * dir;
	int fd;
	char name[SIZE][SIZE];
	int i = 0;
	int num;
	char log[SIZE];
	char log_path[SIZE];

	strcpy(log_path, local_path);
	memset(log, '\0', sizeof(log));
	strcpy(log, "浏览服务器目录:");
	strcat(log, log_path);
	sys_log(log);
	stat (local_path, &buf);
	if (S_ISREG(buf.st_mode))
	{
		;
	}
	chdir(local_path);
	getcwd(file_path, sizeof(file_path));
	file_path[strlen(file_path)] = '/';
	file_path[strlen(file_path) + 1] = '\0';
	dir = opendir(file_path);
	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") && strcmp(ptr->d_name, ".."))
		{
			strcat(tmp, "\n\n\t\t");
			strcat(tmp, ptr->d_name);
			strcpy(name[i++], ptr->d_name);
		}
	}
	strcat(tmp, "\n请选择要保存的位置\n..返回上层目录\n");
	send(conn_fd, tmp, SIZE, 0);
	recv(conn_fd, filename, SIZE, 0);
	if (strcmp(filename, "1") == 0)
	{
		recv(conn_fd, filename, SIZE, 0);
		mkdir(filename, 0777);
		chdir(filename);
		sys_log("进入新建文件夹目录");
		server_look(conn_fd, filename);
	}
	else if (strcmp (filename, "2") == 0)
	{
loop:
		num = i - 1;
		recv(conn_fd, filename, SIZE, 0);
		for (; num >= 0; num--)
		{
			if (strcmp(filename, name[num]) == 0)
			{
				send_data(conn_fd, "n\n");
				goto loop;
			}
		}
		send_data(conn_fd, "y\n");
		recv(conn_fd, len, SIZE, 0);
		server_recv(conn_fd, filename, len);

	}	
	else if (strncmp(filename, "..", 2) == 0)
	{
		server_look(conn_fd, "..");
	}
	else 
	{
		server_look(conn_fd, filename);
	}
}

void server_recv(int conn_fd, char * filename, char * len)
{
	int fd;
	long size;
	long lenght;
	struct stat buf;
	char tmp[SIZE];
	int flag = 0;
	int ret = 0;
	char log[SIZE];

	fd = open(filename, O_WRONLY | O_CREAT | O_EXCL | O_APPEND, 0600);
	size = atol(len);
	while ((size - flag) > SIZE)
	{
		memset(tmp, '\0', sizeof(tmp));
		ret = recv(conn_fd, tmp, SIZE, MSG_WAITALL);
		flag += write(fd, tmp, SIZE); 
	}

	ret = recv(conn_fd, tmp, SIZE, 0);
	flag += write(fd, tmp, SIZE); 
	strcpy(log, "用户上传成功, 文件名：");
	strcat(log, filename);
	sys_log(log);
}

void server_batch(int conn_fd)
{
	char name[SIZE];
	char filename[SIZE][SIZE];
	int ret;
	int flag1 = 0;
	int flag2 = 0;
	int i;
	int k = 0;
	int num = 0;
	int fd;
	char c;
	struct stat buf;
	char lenght[SIZE];
	char number[SIZE];
	char tmp[SIZE];
	char log[SIZE];

	ret = recv(conn_fd, name, sizeof(name), 0);

	num = split_string(name, filename);
	sprintf(number, "%d", num);
	strcpy(log, "用户批量下载文件数量");
	strcat(log, number);
	sys_log(log);
	i = num - 1;
	for (; i >= 0; i--)
	{
		memset(tmp, '\0', sizeof(tmp));
		memset(log, '\0', sizeof(log));
		strcpy(log, "下载文件 ");
		strcat(log, filename[i]);
		sys_log(log);
		fd = open(filename[i], O_RDONLY);
		fstat(fd, &buf);
		sprintf(lenght, "%d", (int)buf.st_size);
		close(fd);
		server_download(conn_fd, filename[i], lenght);
		close(fd);
	}
}
void sys_find(int conn_fd)
{
	char important[SIZE];
	char filename[SIZE];
	int flag = 0;

	memset(filename, '\0', sizeof(filename));
	send(conn_fd, "y\n", strlen("y\n"), 0);
	recv(conn_fd, filename, sizeof(filename), 0);
	filename[strlen(filename)] = '\0';
	dir_recur(conn_fd, "/home/qiong/shujia/source/", filename, &flag);

	printf("flag = %d", flag);
	if (flag == 0)
	{
		send_data(conn_fd, "服务端无该资源\n");
	}
	send_data(conn_fd, "_ok");
}
void dir_recur(int conn_fd, char* dir_path, char *filename, int * flag)
{
	
	int i;
	int j;
	char tmp1[1];
    struct dirent * ptr;
    struct stat buf;
    char dir_list[SIZE][SIZE];
	char pathname[SIZE];
	char string[SIZE];
	char important[SIZE];
    DIR * dir = NULL;

    chdir(dir_path);      
	memset(pathname, '\0',sizeof(pathname));
    getcwd((pathname), sizeof(pathname));
	pathname[strlen(pathname)]='/';
	pathname[strlen(pathname)+1]='\0';       
    memset(string, '\0', sizeof(string));
	memset(important, '\0', sizeof(important));
    if ((dir = opendir ("./")) == NULL)   
    {
        printf("null");
        return ;
    }

    while ((ptr = readdir(dir)) != NULL)
    {
        if (stat(ptr->d_name, &buf) == -1)
        {
            my_err("stat", __LINE__);
        }
       
        if (strcmp(ptr->d_name, ".") && strcmp(ptr->d_name, ".."))
        {
			if (strcmp(ptr->d_name, filename) == 0)
			{
				(*flag)++;
				strcpy(important, pathname);
				strcat(important, ptr->d_name);
				strcat(important, "\n");
				send(conn_fd, important, strlen(important), 0);

			}
            if (S_ISDIR(buf.st_mode))
            {
                strcpy(dir_list[i++], ptr->d_name);
            }
        }
    }


    for (j = 0; j < i; j++)
    {
        dir_recur(conn_fd, dir_list[j], filename, flag);
    }
    //回到上层目录，将其设置为工作目录
    chdir("..");
	closedir(dir);
}

void sys_log(char * string)
{
	int fd;
	char str[SIZE];
	time_t timep;
	struct tm *timenow;
	char log[SIZE];
	struct flock lock;

	memset(&lock, 0, sizeof(struct flock));
	lock.l_start = SEEK_SET;
	lock.l_whence = 0;
	lock.l_len = 0;

	lock.l_type = F_WRLCK;
	if (lock_test(fd, &lock) == 0)
	{
		lock.l_type = F_WRLCK;
		lock_set(fd, &lock);
	}
	strcpy(log, string);

	time (&timep);
	timenow = localtime(&timep);
	strcpy(str, asctime(timenow));
	fd = open("/home/qiong/shujia/Net/sys_log.txt", O_WRONLY | O_CREAT | O_APPEND, 0600);
	write(fd, "\n\n时间:", strlen("\n\n时间:"));
	write(fd, str, strlen(str)-1);
	write(fd, "\nip:", strlen("\nip:"));
	write(fd, ip_name, strlen(ip_name));
	write(fd, "\n操作:", strlen("\n操作:"));
	write(fd, log, strlen(log));

	lock.l_type = F_UNLCK;
	lock_set(fd, &lock);

	close(fd);
}
int lock_set(int fd, struct flock * lock)
{
	int flag;

	flag = fcntl(fd, F_SETLK, lock);
	return flag;
}

int lock_test(int fd, struct flock * lock)
{
	int flag;

	flag = fcntl(fd, F_GETLK, lock);

	return flag;
}
void err_log(char * string)
{
	int fd;
	time_t timep;
	struct tm *timenow;
	char log[SIZE];
	char str[SIZE];

	fd = open("/home/qiong/shujia/Net/err_log.txt", O_WRONLY | O_CREAT | O_APPEND, 0600);
	strcpy(log, string);
	time (&timep);
	timenow = localtime(&timep);
	strcpy(str, asctime(timenow));
	write(fd, "\n\n时间:", strlen("\n\n时间:"));
	write(fd, str, strlen(str)-1);
	write(fd, "\n\n错误事件:", strlen("\n\n错误事件"));
	write(fd, log, strlen(log));

	close(fd);

}
