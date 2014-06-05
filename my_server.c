/*
 * =====================================================================================
 *
 *       Filename:  my_server.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月01日 18时29分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  gaoyuan, 
 *        Company:  Class 1204 of Computer Science and Technology
 *
 * =====================================================================================
 */
#include "server.h"


int main(int argc, char *argv[])
{
	int sock_fd, conn_fd;
	int optval;
	int flag_recv = USERNAME;
	int ret;
	int name_num;
	pid_t pid;
	socklen_t cli_len;
	struct sockaddr_in cli_addr, serv_addr;
	char recv_buf[128];
	char log[SIZE];

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0)
	{
		my_err("socket", __LINE__);
	}

	optval = 1;
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int)) < 0)
	{
		my_err("sersockopt", __LINE__);
	}

	memset (&serv_addr, 0, sizeof (struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof (struct sockaddr_in)) < 0)
	{
		my_err("blind", __LINE__);
	}

	if (listen (sock_fd, LISTENQ) < 0)
	{
		my_err("listen", __LINE__);
	}

	cli_len = sizeof(struct sockaddr_in);
	while (1)
	{
		conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);
		if (conn_fd < 0)
		{
			my_err("accept", __LINE__);
		}
		strcpy(log, "接到一个新的客户端， ip是");
		strcpy(ip_name, inet_ntoa(cli_addr.sin_addr));
		strcat(log, inet_ntoa(cli_addr.sin_addr));
		sys_log(log);

		if ( (pid = fork()) == 0)
		{
			while (1)
			{
				deal(conn_fd);
				
			}
			close(sock_fd);
			close(conn_fd);
			exit (0);
		}
		else
		{
			close(conn_fd);
		}
	}
	return EXIT_SUCCESS;
}

