/*
 * =====================================================================================
 *
 *       Filename:  my_client.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月02日 09时45分26秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  gaoyuan, 
 *        Company:  Class 1204 of Computer Science and Technology
 *
 * =====================================================================================
 */
#include "client.h"

int main(int argc, char *argv[])
{
	int i;
	int ret;
	int conn_fd;
	int serv_port;
	struct sockaddr_in serv_addr;
	char recv_buf[SIZE];
	
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_port = SERV_PORT;
	
	if (serv_port < 0 || serv_port > 65535)
	{
		printf("不可取的端口\n");
		exit (1);
	}
	else
	{
		serv_addr.sin_port = htons(serv_port);
	}
	
	if (inet_aton(SERV_ADDR, &serv_addr.sin_addr) == 0)
	{
		printf("不可取的服务器ip地址\n");
		exit (1);
	}

	if (serv_addr.sin_port == 0 || serv_addr.sin_addr.s_addr == 0)
	{
		printf("端口或ip不可为0\n");
		exit (1);
	}

	conn_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (conn_fd < 0)
	{
		my_err("socket", __LINE__);
	}

	if (connect(conn_fd, (struct sockaddr *)&serv_addr, sizeof (struct sockaddr)) < 0)
	{
		my_err ("connect", __LINE__);
	}

	switch(user_homepage())
	{
		case 1:
			system("clear");
			user_land(conn_fd);
			break;
		case 2:
			system("clear");
			use_register(conn_fd);
			user_land(conn_fd);
            break;
	}

	if ((ret = my_recv(conn_fd, recv_buf, sizeof(recv_buf))) < 0)
	{
		printf("信息太长\n");
		exit (1);
	}

	for (i = 0; i < ret; i++)
	{
		printf("%c", recv_buf[i]);
	}
	printf("\n");

	close(conn_fd);

	return EXIT_SUCCESS;
}

