//  Name: William Sigala
//  CSE 3320
//  Lab 1

#undef DT_DIR
#undef DT_REG

typedef enum{
  FILE_MAX = 1024,
  __DT_DIR = 4,
  __DT_REG = 8,
  NAME_MAX = 2048,
  ARGS_MAX = 2048,
  DATE_SIZE = 15
} defs;

//  For some reason my IDE was saying that DT_REG and DT_DIR were undefined and was giving me squiggles.
//  However, when I compile it runs just fine. I just redefined it above with the same enum values from dirent.h
//  because the squiggles were bothering me in case that was confusing.

#ifndef __FILE_OPS_H
#define __FILE_OPS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>

extern int errno ;

typedef struct dirent dirent;
typedef struct{
  char* fName;
  int size;
  struct tm date;
} file_dat;

file_dat* insert_file(file_dat* arr, int idx, dirent *de);
file_dat* change_dir(DIR* d, char* dirName);
void display_time();
void display_dirs(DIR* d, char* dirName);
void load_files(file_dat* arr, DIR* d);
void display_files(file_dat* arr, int amount, int *idx);
void display_options();
void edit_file(file_dat* arr, char* editor);
void run_program(file_dat* arr);
void change_editor(char* editor, char* editorPath);
void sort(file_dat* arr);
int get_file_amount();

#endif