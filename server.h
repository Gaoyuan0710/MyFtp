/*
 * =====================================================================================
 *
 *       Filename:  client.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月05日 23时27分52秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  gaoyuan, 
 *        Company:  Class 1204 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef _MY_RECV_H
#define _MY_SECV_H
	#include <unistd.h>
 	#include <assert.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <dirent.h>
	#include <string.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <errno.h>
	#include <termios.h>
	#include <time.h>
	

	#define SERV_PORT 4507
	#define LISTENQ   12
	#define SIZE      1024
	#define SERV_ADDR "127.0.0.1"

	#define INVALID_USERINFO 'n'
	#define VALID_USERINFO   'y'
	#define REGISTER         'r'
	#define LAND             'l'
	#define USERNAME         'u'
	#define PASSWORD         'p'
	#define BEGIN            'b'
	#define ENTER            'e'
	#define SMALL            's'

	

	void my_error(const char * err_string, int line);
	int my_recv(int conn_fd, char * data_buf, int len);
	void send_data (int conn_fd, const char * string);
	//int return_size( char * pathname, char  *filename)
	int find_name(const char * name);
	int check_passwd(const char *name, const char *passwd);
	void deal(int conn_fd);
	void server_download(int conn_fd, const char * pathname, char * len);
	void server_browse(int conn_fd,  char * pathname);
	void server_resgiter(int conn_fd);
	void server_check(int conn_fd);
	char *ltoa(long value,char *string,int radix);
	void server_recv(int conn_fd, char * filename, char * len);
	void server_look(int conn_fd, char * local_path);
	int split_string(char *s, char filename[][SIZE]);
	void server_batch(int conn_fd);
	void sys_log(char * string);
	int lock_test(int fd, struct flock * lock);
	int lock_set(int fd, struct flock * lock);
	void err_log(char * string);

	char ip_name[SIZE];

#endif

