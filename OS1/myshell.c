//  Name: William Sigala
//  CSE 3320
//  Lab 1

#include "file_ops.h"

int main(void) {
  
  DIR* d;
  dirent* de;
  file_dat* files = malloc(sizeof(file_dat)*FILE_MAX);
  int idx = 0;
  int const num_files = 5;
  bool hide = false;
  char* editor = (char*)malloc(sizeof(char)*strlen("xdg-open"));  //  May want to change this later

  editor = "xdg-open";
  if(!system(editor)){
    fprintf(stderr, "\nCould not find an editor.\n");
    exit(EXIT_FAILURE);
  }
  load_files(files, d, de);

  while(true){
    fflush(stdout);
    system("clear");
    display_time();
    display_dirs(d, de);
    display_files(files, num_files, &idx);
    if(!hide) display_options();

    printf("Enter Operation: ");
    switch(toupper(getchar())){
      case 'E':
        edit_file(files, editor);
        break;
      case 'R':
        run_program(files);
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