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

void get_file(int argc, char *argv[]) 
{
	char buf[1024];
	char filebuf[FILEBUFSIZE];
	int n, sockfd, i = 0;
	struct headers *headers;
	size_t fetched = 0;
	FILE *fp;
	
	printf("Waiting for file on port %d", PORT);
	printf(NEWLINE);
	fflush(stdout);

	sockfd = get_server();

	if (sockfd < 0)
	{
		die("Error creating socket");
	}
	
	n = recv(sockfd, buf, sizeof(buf), 0);
	
	if (n < 0)
	{
		close_socket(sockfd);
		die("Error recv");
	}

	//Parse headers
	buf[n] = '\0';
	headers = parse_headers(buf);
	
	//Write RESPONSE
	n = send(sockfd, RESPONSE_OK, 2, 0);

	if (n < 0)
	{
		free(headers);
		close_socket(sockfd);
		die("Error sending");
	}
		
	fp = fopen(headers->filename, "wb");
	
	if (fp == NULL)
	{
		printf("Error creating file %s", headers->filename);
	}
	else 
	{
		printf("Fetching file: %s %zu bytes", headers->filename, headers->size);
		printf(NEWLINE);
		printf(NEWLINE);
		fflush(stdout);

		while(fetched < headers->size)
		{
			n = recv(sockfd, filebuf, sizeof(filebuf), 0);
			fwrite(filebuf, n, 1, fp);

			if (n < 0)
			{
				remove(headers->filename);
				printf("Error fetching file");
				break;
			}

			fetched += n;
			
			if ((i % 100) == 0 || fetched >= headers->size)
				print_progress(headers->size, fetched);	
			
			i++;
		}

		fclose(fp);
	}

	free(headers);
	close_socket(sockfd);
}
