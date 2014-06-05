/*
 * =====================================================================================
 *
 *       Filename:  client.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月04日 19时44分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  gaoyuan, 
 *        Company:  Class 1204 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include "client.h"

/*
 * 用户信息注册，conn_ed---从该连接套接字上接收信息
 *
 */
void use_register(int conn_fd)
{
    char buf[32];
	char name[32];                                  //用户名
	char passwd1[32];                               //分别保存两次输入密码
	char passwd2[32];
	char string[40];                                //缓存区
	int flag = 1;                                   //循环标志，1时执行，0退出
	memset(string, 0, sizeof(string));
	send_data(conn_fd, "r\n");
	sleep(1);
    my_recv(conn_fd, buf, sizeof(buf));
	if (buf[0] == 'b')
	{
		do
		{
			send_data(conn_fd, "u\n");
			sleep(1);
			my_recv(conn_fd, buf, sizeof(buf));
			input_userinfo(conn_fd, "\n\n\t\t用户名：");
			sleep(1);
			my_recv(conn_fd, buf, sizeof(buf));
			if (buf[0] == 'n')
			{
				printf("该用户已存在\n");
				continue;
			}
			printf("恭喜您，该用户名可以使用，请继续操作\n");
			printf("请输入密码：");
			get_passwd(passwd1);
			printf("\n请再次输入密码：");
			get_passwd(passwd2);
			if (strcmp(passwd1, passwd2) != 0)
			{
				printf("两次输入不一致，请重新输入\n");
				continue;
			}
			else
			{
				flag = 0;
			}
			passwd1[strlen(passwd1)] = '\n';
			passwd1[strlen(passwd1) + 1] = '\0';
			send_data(conn_fd, "p\n");
			sleep(1);
			my_recv(conn_fd, buf, sizeof(buf));
			send_data(conn_fd, passwd1);
			sleep(1);
			my_recv(conn_fd, buf, sizeof(buf));
			if (buf[0] == 'y')
			{
				printf("注册成功\n");
				return;
			}
		}while (flag);
	}

}

/*
 *  * 输入时不回显
 *   */
int getch()
{
	int c = 0;
	struct termios org_opts, new_opts;
	int res = 0;
	res = tcgetattr(STDIN_FILENO, &org_opts);
	assert (res == 0);
	memcpy (&new_opts, &org_opts, sizeof(new_opts));
	new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
	tcsetattr (STDIN_FILENO, TCSANOW, &new_opts);
	c = getchar();
	res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
	assert (res == 0);
	return c;
}

void get_passwd( char *string)
{
	char *p;

	p = string;
	while ((*p = getch()) != '\n')
	{
		if (*p == 127)
		{
			printf("\b \b");    //实现密码的回删
			p-=1;
		}
		else
		{
			putchar('*');
			p++;
		}
	}
	*p = '\n';
    *++p = '\0';
}

int get_userinfo(char *buf, int len)
{
	int i;
	int c;

	if (buf == NULL)
	{
		return -1;
	}

	i = 0;
    c = getchar();
	if (c != 10)
	{
		buf[i++] = c;
	}
	while ( ((c = getchar()) != '\n') && (c != EOF) && (i < len - 2))
	{
		buf[i++] = c;
	}
	buf[i++] = '\n';
	buf[i++] = '\0';
	return 0;
}

void input_userinfo(int conn_fd, const char *string)
{
	char input_buf[32];
	char recv_buf[SIZE];
	int flag_userinfo;

	printf("%s", string);
	if (get_userinfo(input_buf, 32) < 0)
	{
		printf("error return from get_userinfo\n");
		exit (1);
	}
	if (send(conn_fd, input_buf, strlen(input_buf), 0) < 0)
	{
		my_err("send", __LINE__);
	}
}
int user_homepage()
{
	int flag;

	printf("\n\n\t\tFTP服务器客户端\n");
	printf("\n\t\t1.用户登陆\n");
	printf("\n\t\t2.用户注册\n");	
	printf("\n\t\t0.退出\n");

	scanf("%d", &flag);

	return flag;
}

void use_download(int conn_fd)
{
	char pathname[SIZE];
	char name[SIZE];
	char tmp[SIZE];
	char choose[SIZE];
	char lenght[SIZE];
	char u_lenght[SIZE];
	long size;
	long d_size;
	double percent;
	struct stat file;
	int fd;
	int dir;
	int ret;
	long len = 0;
	int flag;
	int i = 0;


	printf("\n\n\t请输入要保存的本地路径:");

	scanf("%s", pathname);

	dir = chdir(pathname);

	printf("\t\t请输入保存名称:");
	scanf("%s", name);

	flag = if_read(pathname, name);
	send_data(conn_fd, "y\n");
	i = recv(conn_fd, lenght, sizeof(lenght), 0);
	size = atol(lenght);

	if (flag == 1)
	{
		fd = open(name, O_WRONLY | O_APPEND );
	}
	else
	{
		fd = open(name, O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0600);
//需要修改
	}
	fstat(fd, &file);
	d_size = (long)file.st_size;
	getchar();
	getch();
	sprintf(u_lenght, "%ld", d_size);
	send(conn_fd, u_lenght, sizeof(lenght), 0);
	
		while((size - len) > SIZE)
	{
		stat(name, &file);
		d_size = (long)file.st_size;
		percent = (double)d_size*100.0/(double)size;
		printf("\n\n\t已下载%.1lf%%\n", percent);
		memset(tmp, '\0', sizeof(tmp));
		flag = recv(conn_fd, tmp, SIZE, MSG_WAITALL);
		len += write(fd, tmp, SIZE);
		system("clear");
	}
	memset(tmp, '\0', sizeof(tmp));
	flag = recv(conn_fd, tmp, SIZE, 0);
	len += write(fd, tmp, flag);
	d_size = (long)file.st_size;
	percent = (double)d_size*100.0/(double)size;
	printf("\n\n\t已下载%.1lf%%\n", percent);
	sleep(1);
	system("clear");
	printf("\n\n\t已下载100%%,接收完毕\n");
	ftruncate(fd, size);
	close(fd);
    printf("\n\n\t请按任意健返回\n");
	getch();
	send_data(conn_fd, "y\n");
	user_choose(conn_fd);
}
int if_read(char *pathname, char * name)
{
	DIR * dir;
	struct dirent * ptr;
	int flag;
	char choose;

	if ((dir = opendir(pathname)) == NULL)
	{
		my_err("opendir", __LINE__);
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(name, ptr->d_name) == 0)
		{
			printf("该文件已存在，是否继续下载未完成的部分,否则将覆盖原来的内容?y/n");
			getchar();
			scanf("%c", &choose);
			if (choose == 'y'|| choose == 'Y')
			{
				return 1;
			}
		}
	}

	return 0;
}
void user_upload(int conn_fd, int fd, char * len)
{
	char tmp[SIZE];
	char buf[SIZE];
	char file[SIZE];
	int flag;
	int sum = 0;
	int ret;
	int lenght;
	int i;

	printf("进入上传页面了\n");
	while (1)
	{
		lenght = recv(conn_fd, buf, SIZE, 0);
		for (i = 0; i < lenght; i++)
		{
			printf("%c", buf[i]);
		}

		printf("1.在此新建文件夹\n");
		printf("2.保存在当前位置\n");
		scanf("%s", buf);	
		send(conn_fd, buf, SIZE, 0);
		if (strcmp(buf, "1") == 0)
		{
			printf("请输入新建文件夹名称：");
			scanf("%s", file);
			send(conn_fd, file, SIZE, 0);
			continue;
		}
		else if (strcmp(buf, "2") == 0)
		{
loop:
			printf("请重名名上传文件\n");
			scanf("%s", file);
			send(conn_fd, file, SIZE, 0);
			my_recv(conn_fd, buf, sizeof(buf));
			if (buf[0] == 'n')
			{
				printf("本目录中已存在%s，请重新命名\n", file);
				goto loop;
			}
			break;
		}
	
	}
	send_data(conn_fd, len);

	while (1)
	{
		flag = read(fd, tmp, SIZE);
		sum += flag;
		ret = send(conn_fd, tmp, SIZE, 0);
		ret += lenght;
		if (flag != SIZE)
		{
			break;
		}
	}
	user_choose(conn_fd);
}
void user_find(int conn_fd, char * local_path)
{

	char server_path[SIZE];
	char  filename[SIZE];
	char file_path[SIZE];
	char len[SIZE];
	struct dirent * ptr;
	struct stat buf;
	DIR  * dir;
	int fd;
	int zan;

	system("clear");
	stat (local_path, &buf);
	if (S_ISREG(buf.st_mode))
	{
		if ((fd = open(local_path, O_RDONLY)) == -1)
		{
			my_err("open", __LINE__);
		}
		send_data(conn_fd, "q\n");
		sprintf(len, "%ld", (long)buf.st_size);
		user_upload(conn_fd, fd, len);
		close(fd);

	}
	chdir(local_path);
	getcwd(file_path, sizeof(file_path));
	file_path[strlen(file_path)] = '/';
	file_path[strlen(file_path) + 1] = '\0';
	dir = opendir(file_path);
	printf("\n\n\t\t.\n\n\t\t..");
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

void user_browse(int conn_fd)
{
	char buf[SIZE];
	int i;
	int len;
	char order[SIZE];
	int flag = 0;
	char c;
	
	send_data(conn_fd, "x\n");

	while (1)
	{			system("clear");
		system("clear");
		memset(order, 0, sizeof(order));
		memset(buf, '\0', sizeof(buf));

		len = recv(conn_fd, buf, sizeof(buf), 0);
		buf[strlen(buf)] = '\0';

		for (i = 0; i < len; i++)
		{
			printf("%c", buf[i]);
		}
		if (buf[0] == 'b')
		{
			system("clear");
			use_download(conn_fd);
		}
		if (buf[0] == 'n')
		{
			user_choose(conn_fd);
		}
		printf("\n\n\t请输入要下载的文件或打开的目录，多个文件名请用空格或，隔开\n");
		printf("\n\t..返回上层\n");
		c = getchar();
		i = 0;
		if (c != 10)
		{
			order[i++] = c;
		}
		while ( ((c = getchar()) != '\n') && (c != EOF) && (i < len - 2))
		{
			order[i++] = c;
		}
		order[i++] = '\0';
		for (i = 0; i < strlen(order); i++)
		{
			if (order[i] == ' ' || order[i] == ',')
			{
				flag = 1;
				break;
			}
		}

		if (flag == 0)
		{
			send_data(conn_fd, order);
			sleep(1);
		}
		else 
		{
			send(conn_fd, order, sizeof(order), 0);
			send(conn_fd, order, sizeof(order), 0);		
			user_batch(conn_fd, order);
			user_choose(conn_fd);
		}
	}
}
void user_choose(int conn_fd)
{
	int flag;
	char path[SIZE];
	char filename[SIZE];
	char tmp[SIZE];

	sleep(1);
	system("clear");

	printf("\n\n\t\t1.浏览资源\n");
	printf("\t\t2.上传资源\n");
	printf("\t\t3.查找资源\n");

	scanf("%d", &flag);

	switch(flag)
	{
		case 1:
			user_browse(conn_fd);
			break;
		case 2:
			chdir("/home/");
			getcwd(path, sizeof(path));
			user_find(conn_fd, path);
			break;
		case 3:
			send_data(conn_fd, "d\n");
			printf("请输入文件名称：");
			memset(filename, '\0', sizeof(filename));
			scanf("%s", filename);
			dir_recur(conn_fd, filename);
			getchar();
			getch();
			user_choose(conn_fd);

	}
}

void user_land(int conn_fd)
{

	char name[32];
	char passwd[32];
	char buf[SIZE];
	int i;

	send_data(conn_fd, "l\n");
	sleep(1);
	my_recv(conn_fd, buf, sizeof(buf));
	if (buf[0] == BEGIN)
	{
		while(1)
		{

			send_data(conn_fd, "u\n");
			sleep(1);
			system("clear");
			my_recv(conn_fd, buf, sizeof(buf));
			input_userinfo(conn_fd, "\n\n\t\t用户名：");
			sleep(1);
			my_recv(conn_fd, buf, sizeof(buf));
			if (buf[0] == 'n')
			{
				printf("该用户不存在,请重新输入\n");
				continue;
			}
			printf("\t\t密码:");
			get_passwd(passwd);
			send_data(conn_fd, "p\n");
			sleep(1);
			my_recv(conn_fd, buf, sizeof(buf));
			send_data(conn_fd, passwd);
			sleep(1);
			my_recv(conn_fd, buf, sizeof(buf));
			if (buf[0] == INVALID_USERINFO)
			{
				printf("密码错误，请重新输入\n");
				continue;
			}
			else
			{
				break;
			}
		}
		system("clear");
		printf("\n\n\tftp服务器欢迎您，请选择操作\n");
		
		sleep(1);
		user_choose(conn_fd);
	}
}
void user_batch(int conn_fd, char * s)
{
	char pathname[SIZE];
	char name[SIZE];
	char tmp[SIZE];
	char choose[SIZE];
	char lenght[SIZE];
	char u_lenght[SIZE];
	char filename[SIZE][SIZE];
	long size;
	long d_size;
	double percent;
	struct stat file;
	int fd;
	int dir;
	int ret;
	long len = 0;
	int flag;
	int i = 0;
	int number;


	printf("\n\n\t请输入要保存的本地路径:");

	scanf("%s", pathname);

	dir = chdir(pathname);

	printf("\t\t请输入新建文件夹名称:");
	scanf("%s", name);
	mkdir (name, 0777);
	chdir (name);
	number = split_string(s, filename);
	while(number != 0)
	{
		len = 0;
		memset(name, '\0', sizeof(name));
		memset(lenght, '\0', sizeof(lenght));
		recv(conn_fd, lenght, sizeof(lenght), 0);
		size = atol(lenght);
		fd = open(filename[number-1], O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0600);
		fstat(fd, &file);
		d_size = (long)file.st_size;
		sprintf(u_lenght, "%ld", d_size);
		send(conn_fd, u_lenght, sizeof(lenght), 0);
		while((size - d_size) > SIZE)
		{
			memset(tmp, '\0', sizeof(tmp));
			d_size += recv(conn_fd, tmp, SIZE, MSG_WAITALL);
			write(fd, tmp, SIZE);
		}
		memset(tmp, '\0', sizeof(tmp));
		flag = recv(conn_fd, tmp, SIZE, 0);
		write(fd, tmp, flag);
		number--;
		if (number != 0)
		{
			send_data(conn_fd, "n\n");
		}
		else
		{
			send_data(conn_fd, "y\n");
		}
		printf("\n\n\t已下载100%%,接收完毕\n");
		close(fd);
	}
}


void dir_recur(int conn_fd, char *filename)
{
	char tmp[SIZE];
	char buf[SIZE];

	strcpy(buf, filename);
	recv(conn_fd, tmp, sizeof(tmp), 0);
	send(conn_fd, filename, strlen(filename), 0);
	while(1)
	{
		memset(tmp, '\0', sizeof(tmp));
		recv(conn_fd, tmp, sizeof(tmp), 0);
		tmp[strlen(tmp)] = '\0';
		if(strcmp(tmp, "_ok") == 0)
		{
			break;
		}
		printf("%s", tmp);
	}
}


