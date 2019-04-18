/*
 MIT License
 
 Copyright (c) 2019 Fredrik Blank
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*/

#define __USE_MINGW_ANSI_STDIO 1

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

#ifdef WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#else
	
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#endif

#define VER "0.1"
#define PORT 56167
#define FILEBUFSIZE 32768
#define RESPONSE_OK "OK"
#define NEWLINE "\r\n"

struct headers *parse_headers(char *);
char *get_filename(const char *path);
char *make_headers(size_t, const char *);
int get_socket();
int get_client(const char *);
int get_server();
void close_socket(int sockfd);
void print_progress(size_t, size_t);
void get_file(int, char **);
void send_file(int, char **);
void die(const char *);
void *xmalloc(size_t);
size_t get_filesize(const char *);
void show_info();
void show_help();

struct headers 
{
	size_t size;
	char *filename;
};
