//  Name: William Sigala
//  CSE 3320
//  Lab 1

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

#undef DT_DIR
#undef DT_REG

#define FILE_MAX 1024
#define DT_DIR 4
#define DT_REG 8

#ifndef NAME_MAX
#define NAME_MAX 2048
#endif

//  For some reason my IDE was saying that DT_REG and DT_DIR were undefined and was giving me squiggles.
//  However, when I compile it runs just fine. I just redefined it above with the same enum values from dirent.h
//  because the squiggles were bothering me in case that was confusing.

struct list{
  char* fName;
  int size;
  //struct tm* date;
};

typedef struct list list;
typedef struct dirent dirent;

list* insert_file(list* arr, int idx, dirent *de){
  if(strlen(de->d_name) > NAME_MAX){
    fprintf(stderr, "File name excedes limit of %d chars: %d chars.\n", NAME_MAX, strlen(de->d_name));
    exit(EXIT_FAILURE);
  }
  
  list* temp = malloc(sizeof(list));
  struct stat st;

  stat(de->d_name, &st);
  temp->fName = (char*)malloc(sizeof(char)*strlen(de->d_name));
  strcpy(temp->fName, de->d_name);
  temp->size = st.st_size;
  //temp->date = NULL;
  arr[idx] = *temp;

  return arr;
}

void display_time(){
  time_t t = time( NULL );
  
  printf( "Time: %s\n", ctime( &t ));
  printf("-----------------------------------------" );
}

void display_dirs(DIR* d, dirent* de){
  char cwd[NAME_MAX];
  int c = 0;
  if((d = opendir( "." )) == NULL){
    fprintf(stderr, "Current working directory could not be opened.\n");
    exit(EXIT_FAILURE);
  }

  if(getcwd(cwd, sizeof(cwd)) == NULL){
    fprintf(stderr, "Directory couldn't be determined or size conflict.\n");
    exit(EXIT_FAILURE);
  }
  printf( "\nCurrent Directory: %s \n", cwd);

  while(de = readdir(d)){
    if(de->d_type == DT_DIR)
      printf( " ( %d Directory:  %s ) \n", c++, de->d_name);	  
  }
  closedir( d );
  printf( "-----------------------------------------\n" );
}

void load_files(list* arr, DIR* d, dirent* de){
  int c = 0;
  if((d = opendir( "." )) == NULL){
    fprintf(stderr, "Current working directory could not be opened.\n");
    exit(EXIT_FAILURE);
  }

  while (de = readdir(d)){                    
    if (de->d_type == DT_REG)
      insert_file(arr, c++, de);
  }
  closedir( d );
}

void display_files(list* arr, int amount, int *idx){
  int c = *idx;

  for(; c < (amount + *idx); c++){
    if(arr[c].fName == NULL)
      break;

    if(arr[c].size < 1000)
      printf( " ( %d File:  %s \tSize: %.3d Bytes) \n", c, arr[c].fName, arr[c].size);
    else if(arr[c].size < 1000000)
      printf( " ( %d File:  %s \tSize: %.3f KiB)\n", c, arr[c].fName, (float)arr[c].size/1000);
    else if(arr[c].size < 1000000000)
      printf( " ( %d File:  %s \tSize: %.3f MiB)\n", c, arr[c].fName, (float)arr[c].size/1000000);
    else
      printf( " ( %d File:  %s \tSize: %.3f GiB) \n", c, arr[c].fName, (float)arr[c].size/1000000000);
      
  }
  printf( "-----------------------------------------\n" );
}

void display_options(){
  printf("Operation:\tE Edit\n\t\tR Run\n\t\tC Change Directory");
  printf("\n\t\tS Sort Directory listing\n\t\tQ Quit");
  printf("\n\t\tN Next\n\t\tP Previous\n\t\tH Hide Operations\n");
  printf( "-----------------------------------------\n" );
}

void edit_file(list* arr, char* editor){
  int n;
  char* temp = (char*)malloc(sizeof(char)*(NAME_MAX + strlen(editor)));

  printf("Enter file number: ");
  scanf("%d", &n);
  sprintf(temp, "%s %s", editor, arr[n].fName);
  system("clear");

  if(system(temp)){
    fprintf(stderr, "\nFile could not be opened using %s\n", editor);
    exit(EXIT_FAILURE);
  }
}

int main(void) {
  DIR* d;
  dirent* de;
  list* files = malloc(sizeof(list)*FILE_MAX);
  int idx = 0;
  int const num_files = 5;
  bool hide = false;
  char c;
  char* editor = (char*)malloc(sizeof(char)*strlen("xdg-open"));
  
  editor = "xdg-open";
  load_files(files, d, de);
  
  while(true){
    system("clear");
    display_time();
    display_dirs(d, de);
    display_files(files, num_files, &idx);
    if(!hide) display_options();

    printf("Enter Operation: ");
    switch(c = toupper(getchar())){
      case 'E':
        edit_file(files, editor);
        break;
      case 'R':
        
        break;
      case 'C':
        
        break;
      case 'S':
        
        break;
      case 'Q':
        system("clear");
        exit(EXIT_SUCCESS);
        break;
      case 'N':
        if(files[idx + num_files].fName != NULL)
          idx += num_files;
        break;
      case 'P':
        if(idx != 0)
          idx -= num_files;
        break;
      case 'H':
        hide = !hide;
        break;
    }
  }
}
