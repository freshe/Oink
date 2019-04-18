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

void send_file(int argc, char *argv[])
{
	FILE *fp;
	size_t len = 0;
	size_t filesize = 0;
	size_t sent = 0;
	char buf[1024];
	char filebuf[FILEBUFSIZE];
	char *headers;
	char *filepath;
	char *filename;
	char *host;
	int sockfd, n, i = 0;
	
	if (argc < 4)
	{
		die("Missing arguments. Bye");
	}
		
	fp = fopen(argv[2], "rb");

	if (fp == NULL)
	{
		die("File not found");
	}
	
	filepath = argv[2];
	host = argv[3];

	sockfd = get_client(host);

	if (sockfd < 0)
	{
		fclose(fp);
		die("Error getting client");
	}

	filesize = get_filesize(filepath);
	
	if (filesize <= 0)
	{
		fclose(fp);
		close_socket(sockfd);
		die("Error getting file size");
	}

	filename = get_filename(filepath);
	headers = make_headers(filesize, filename);
	
	//Write headers
	n = send(sockfd, headers, strlen(headers), 0);
	
	if (n < 0)
	{
		free(headers);
		fclose(fp);
		close_socket(sockfd);
		die("Error sending headers");
	}
		
	//Get response
	n = recv(sockfd, buf, sizeof(buf), 0);
	
	if (n < 0)
	{
		free(headers);
		fclose(fp);
		close_socket(sockfd);
		die("Error receiving response");
	}
		
	buf[n] = '\0';
	
	if (strcmp(buf, RESPONSE_OK) == 0)
	{
		printf("Sending file %s %zu bytes", filename, filesize);
		printf(NEWLINE);
		printf(NEWLINE);
		fflush(stdout);

		while ( (len = fread(filebuf, 1, sizeof(filebuf), fp)) > 0)
		{
			n = send(sockfd, filebuf, len, 0);
		
			if (n < 0)
			{
				printf("Error sending file");
				break;
			}

			sent += n;

			if ((i % 100) == 0 || sent >= filesize)
				print_progress(filesize, sent);
			
			i++;
		}
	}
	else 
	{
		printf("Error getting response");
	}

	free(headers);
	close_socket(sockfd);
	fclose(fp);
}
