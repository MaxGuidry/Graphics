#include"ReadFile.h"
#include <cstdio>

const char* FileRead(const char * FileName)
{
	FILE * fp;
	fopen_s(&fp, FileName, "r");
	if (fp == nullptr)
	{
		perror("file not found");
	}
	fseek(fp, 0, SEEK_END);
	size_t num = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	const int size = num + 1;
	char * buffer = new char[size];
	size_t nm = fread(buffer, sizeof(char), num + 1, fp);
	buffer[nm] = '\0';
	fclose(fp);

	return buffer;
}
