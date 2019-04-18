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

void signal_handler(int signum)
{
	printf("\r\nBye!\r\n");
	fflush(stdout);
	
	//Exit
	exit(signum);
}

int main(int argc, char *argv[]) 
{
	//Register signal hanler
	signal(SIGINT, signal_handler);

#ifdef WINDOWS
	WSADATA wsa;
	
	if (WSAStartup( MAKEWORD(2,2), &wsa) != 0)
	{
		WSACleanup();
		die("Error winsock init");
	}
#endif
	
	show_info();	

	if (argc < 2)
	{
		show_help();
	}
	else if (strcmp(argv[1], "send") == 0)
	{
		send_file(argc, argv);
	}
	else if (strcmp(argv[1], "get") == 0) 
	{
		get_file(argc, argv);
	}
	else if (strcmp(argv[1], "help") == 0)
	{
		show_help();
	}
	else 
	{
		show_help();
	}

#ifdef WINDOWS
	WSACleanup();
#endif

	printf(NEWLINE);
	printf(NEWLINE);
	fflush(stdout);
	
	return 0;
}
