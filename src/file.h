#pragma once

void file_size(FILE* &file);
void open_file(const char* &filename, FILE* &file);
void read_file(const char* filename, long &file_size, char* &file_buffer);
void write_file(const char* filename, long &file_size, char* &file_buffer);