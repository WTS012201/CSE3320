#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define FILE_MAX 1024
#define NAME_MAX 2048

struct list{
  char* fName;
};

typedef struct list list;
typedef struct dirent dirent;

list* insert_file(list* arr, int idx, char* fName){
  if(strlen(fName) > NAME_MAX)
    fprintf(stderr, "File name excedes limit of 2048 chars: %d chars.\n", strlen(fName));

  list* temp = malloc(sizeof(list));
  temp->fName = (char*)malloc(sizeof(char)*strlen(fName));
  strcpy(temp->fName, fName);
  arr[idx] = *temp;
  
  return arr;
}

void display_time(){
      time_t t = time( NULL );
      
      printf( "Time: %s\n", ctime( &t ));
      printf("-----------------------------------------------\n" );
}

void display_dirs(DIR* d, dirent* de){
  char s[256];
  d = opendir( "." );
  int c = 0;

  getcwd(s, 200);
  printf( "\nCurrent Directory: %s \n", s);
  while(de = readdir(d)){
    if(de->d_type == DT_DIR)
      printf( " ( %d Directory:  %s ) \n", c++, de->d_name);	  
  }
  closedir( d );
  printf( "-----------------------------------------\n" );
}

void display_files(DIR* d, dirent* de){
  char s[256];
  d = opendir( "." );
  int c = 0;

  while ((de = readdir(d))){                    
    if (de->d_type == DT_REG)
      printf( " ( %d File:  %s ) \n", c++, de->d_name);

      if(c % 5 == 0){
        printf( "Hit N for Next\n" );
        
      }
  }
  closedir( d );
  printf( "-----------------------------------------\n" );
}

void display_options(){

}

int main(void) {
  char s[256], cmd[256], c;
  DIR* d;
  dirent* de;

  while (1) {
    system("clear");
    display_time(d, de);
    display_dirs(d, de);
    display_files(d, de);
    /*
    //getchar( ); 
    c = getchar( );
    switch (c) {
      case 'q': exit(0); 
      case 'e': printf( "Edit what?:" );
                scanf( "%s", s );
                strcpy( cmd, "pico ");
                strcat( cmd, s );
                system( cmd );
                break;
      case 'r': printf( "Run what?:" );
                scanf( "%s", cmd );
                system( cmd );
                break;
      case 'c': printf( "Change To?:" );
                scanf( "%s", cmd );
                chdir( cmd );   
                break; 
    }
    */
  }
}
