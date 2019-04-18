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

void show_info()
{
	printf(NEWLINE);
	printf("Oink");
	printf(" - (c) Fredrik Blank 2019");
	printf(" v%s", VER);
	printf(NEWLINE);
	printf("----------------------------------");
	printf(NEWLINE);
	printf(NEWLINE);
	fflush(stdout);
}

void show_help()
{
	printf("Wait for file: ./oink get");
	printf(NEWLINE);
	printf(NEWLINE);
	printf("Send file: ./oink send [filename] [ip]");
	printf(NEWLINE);
	printf("e.g. ./oink send ~/Desktop/MyFile 100.10.10.0");
	printf(NEWLINE);
	printf(NEWLINE);
	printf("Oink uses tcp port %d", PORT);
	fflush(stdout);
}

char *get_filename(const char *path)
{
	int i = 0;
	char *filename;
	size_t len;
	
	for (i = strlen(path); i > 0; i--) 
	{
		char t = path[i];
		if (t == '\\' || t == '/')
			break;
	}
	
	if (i > 0) i++;
	
	len = (strlen(path) - i);
	filename = (char *)xmalloc(len + 1);
	memcpy(filename, &path[i], len);
	filename[len] = '\0';
	
	return filename;
}

char *make_headers(size_t size, const char *filepath)
{
	char str_size[64];
	char headers[128];
	
	memset(&headers, 0, sizeof(headers));
	sprintf(str_size, "%zu", size);
	strcpy(headers, str_size);
	strcat(headers, NEWLINE);
	strcat(headers, filepath);
	
	return strdup(headers);
}

struct headers *parse_headers(char *buf)
{
	struct headers *headers;
	char *token;
	char *str_size;
	char *str_name;
	int i = 0;
	size_t size = 0;
	
	token = strtok(buf, NEWLINE);
	
	while(token != NULL) 
	{
		if (i == 0)
			str_size = strdup(token);
		if (i == 1)
			str_name = strdup(token);
		
		token = strtok(NULL, NEWLINE);
		i++;
	}
	
	size = strtoul(str_size, NULL, 10);
	free(str_size);
	
	headers = (struct headers *)xmalloc(sizeof(struct headers));
	headers->size = size;
	headers->filename = str_name;
	
	return headers;
}

void print_progress(size_t size, size_t part)
{
	static size_t proc = 0;
	proc = 100 * part / size;

	printf("\r");
	printf("Progress: %zu (%zu%%)", part, proc);
	fflush(stdout);
}

size_t get_filesize(const char *filepath)
{
#ifdef WINDOWS
    HANDLE hFile = CreateFile(
		filepath, 
		GENERIC_READ, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL
	);
		
    if (hFile == INVALID_HANDLE_VALUE)
        return 0;

    LARGE_INTEGER size;
    if (!GetFileSizeEx(hFile, &size))
    {
        CloseHandle(hFile);
        return 0;
    }

    CloseHandle(hFile);
	
    return (size_t)size.QuadPart;
#else
	size_t size = 0;
	struct stat fileStat;
	
	if ((stat(filepath, &fileStat)) >= 0)
	{
		size = (size_t)fileStat.st_size;
	}
	
	return size;
#endif
}

void die(const char *msg)
{
	printf("%s", msg);
	printf(NEWLINE);
	exit(1);
}

void *xmalloc(size_t size)
{
	void *mem;
	mem = malloc(size);

	if (mem == NULL) 
		die("Error allocating memory");
	
	return mem;
}
