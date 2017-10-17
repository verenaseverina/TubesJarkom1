#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "file.h"

long get_size(FILE* &file)
{
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    return size;
}

void open_file(const char* &filename, FILE* &file, const char* mode)
{
    file = fopen(filename, mode); // open in binary mode

    if(file == NULL) 
    {
        perror("Error opening file");
        exit(1);
    }
}

void read_file(const char* filename, long &file_size, char* &file_buffer)
{
	FILE* file;
	open_file(filename, file, "rb");
    
    file_size = get_size(file); // get file size
	file_buffer = (char*) malloc(sizeof(char) * file_size); // allocate buffer for file

	size_t read_size = fread(file_buffer, 1, file_size, file); // read file
    
    if(read_size != file_size)
    {
        perror("Error read file"); // check if read file size equal to file size
        exit(1);
    }
		
	fclose(file);
}

void write_file(const char* filename, long &file_size, char* &file_buffer)
{
	FILE* file;
	open_file(filename, file, "wb"); // open in binary mode

	fwrite(file_buffer, 1, file_size, file); // write file
	fclose(file);
	free(file_buffer); // free buffer
}