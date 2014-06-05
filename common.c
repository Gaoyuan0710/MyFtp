/*
 * =====================================================================================
 *
 *       Filename:  common.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月04日 20时34分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  gaoyuan, 
 *        Company:  Class 1204 of Computer Science and Technology
 *
 * =====================================================================================
 */
#include "client.h"
#include "server.h"

void my_err(const char * err_string, int line)
{
	fprintf(stderr, "line: %d ", line);
	perror(err_string);
	exit (1);
}
void send_data (int conn_fd, const char * string)
{
	if (send(conn_fd, string, strlen(string), 0) < 0)
	{
		my_err("send", __LINE__);
	}
}

int my_recv(int conn_fd, char * data_buf, int len)
{
	static char recv_buf[SIZE];
	static char * pread;
	static int len_remain = 0;
	int i;

	if (len_remain <= 0)
	{
		if ((len_remain = recv(conn_fd, recv_buf, sizeof (recv_buf), 0)) < 0)
		{
			my_err("recv", __LINE__);
		}
		else if (len_remain == 0)
		{
			return 0;
		}
	pread = recv_buf;
	}
	for (i = 0; *pread != '\n'; i++)
	{
		if (i > len)
		{
			return -1;
		}
		data_buf[i] = *pread++;
		len_remain--;
	}

	len_remain--;
	pread++;

	return i;
}
int split_string(char *s, char filename[][SIZE])
{
	char *p = s;
	int i = 0;
	int j = 0;
	while(*p != '\0')
	{
		if(*p == ' ')
		{
			filename[i][j] ='\0';
			i++;
			j = 0;
			p++; /*处理多个空格的情况*/
			while(*p == ' ')
			{
				p++;
			}
		}
		else
		{
			filename[i][j] = *p;
			p++;
			j++;
		}
	}
	return i+1;
}
//int return_size( char * pathname, char  *filename)
//{
//	
//	int len;
//	float size;
//
//	char tmp[1024];
  //      struct stat buf;
 //       
//        chdir(pathname);
//	if (stat(filename, &buf) == -1)
//{
//    my_err("stat", __LINE__);
//}
//	len = buf.st_size;
//	return (len);
//
//}

