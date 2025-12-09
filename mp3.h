#include<stdio.h>
#ifndef MP3_H
#define MP3_H


typedef struct
{
    char tag[5];
    int frame_size;
    char flag[3];
    char frame_data[1024];
} song;

void view_mp3(char *file);
void edit_mp3(char *file, char *option, char *new_data, song *s);
void get_help(char *argv);
int convert_size(unsigned char *bytes,int ver);
void read_flags(song *s,FILE *fptr);
void read_size(song *s,FILE *fptr,int ver);
void read_tag(song *s,FILE *fptr);
void read_data(song *s,FILE *fptr);
void print_frame(song *s,FILE *fp);
void write_size(unsigned char *size_bytes,int size,unsigned char ver);

#endif