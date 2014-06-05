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

#ifndef _CLIENT_H
#define _CLIENT_H
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
	void use_register(int conn_fd);
	int getch();
	void get_passwd( char *string);
	int get_userinfo(char *buf, int len);
	void input_userinfo(int conn_fd, const char *string);
	int user_homepage();
	void use_download(int conn_fd);
	void user_browse(int conn_fd);
	void choose(int conn_fd);
	void user_land(int conn_fd);
	void send_data (int conn_fd, const char * string);
	//int return_size( char * pathname, char  *filename)
	char *ltoa(long value,char *string,int radix);
	void user_choose(int conn_fd);
	void user_batch(int conn_fd, char * s);
	int split_string(char *s, char filename[][SIZE]);
	void dir_recur(int conn_fd,char *filename);

#endif

