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

#include "oink.h"

int get_socket()
{
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	return sockfd;
}

int get_server()
{
	int sockfd, newsockfd;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	
	sockfd = get_socket();

	if (sockfd < 0)
	{
		return -1;
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT);
 
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		close_socket(sockfd);
		return -100;
	}
		
	listen(sockfd, 8);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
	
	if (newsockfd < 0)
	{
		close_socket(sockfd);
		return -200;
	}

	close_socket(sockfd);

	return newsockfd;
}

int get_client(const char *host) 
{
	int sockfd;
	struct hostent *server;
	struct sockaddr_in server_addr;
	
	sockfd = get_socket();

	if (sockfd < 0)
	{
		return -1;
	}
	
	server = gethostbyname(host);
	
	if (server == NULL)
	{
		return -100;
	}
		
	server_addr.sin_addr.s_addr = inet_addr(host);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);

	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		close_socket(sockfd);
		return -200;
	}
	
	return sockfd;
}

void close_socket(int sockfd)
{
#ifdef WINDOWS
	closesocket(sockfd);
#else
	close(sockfd);
#endif
}
