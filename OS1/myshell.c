//  Name: William Sigala
//  CSE 3320
//  Lab 1

#include "file_ops.h"

int main(void) {
  DIR* d = opendir(".");
  file_dat* files = malloc(sizeof(file_dat)*(FILE_MAX + 1));
  FILE* fp = fopen(".editor", "r");

  int c = 0, idx = 0, num_files = 5;
  bool hide = false;
  char* editor = (char*)malloc(sizeof(char)*(NAME_MAX));
  char* editorPath = (char*)malloc(sizeof(char)*(NAME_MAX));
  char* args = (char*)malloc(sizeof(char)*(ARGS_MAX));
  char dirName[NAME_MAX + 1];

  if(getcwd(dirName, sizeof(dirName)) == NULL){
    fprintf(stderr, "Error opening dir: %s\n", strerror(errno));
    fprintf(stderr, "Current directory could not be opened.\n");
    exit(EXIT_FAILURE);
  }
  strcpy(editorPath, dirName);
  strcat(editorPath, "/.editor");

  //  I know this wasn't a requirement but I went ahead and checked if the user has xdg-open.
  //  If not, they can change the editor and it will save to .editor so that
  //  they don't have to change it everytime.

  if(fp == NULL){                   //  Checks if .editor exists
    if(system("xdg-open") == 256){  //  Checks if xdg-open is on the system.
      fp = fopen(".editor", "w");
      editor = "xdg-open";
      fputs(editor, fp);        
      fclose(fp);
    } else
      change_editor(editor, editorPath);        //  If system doesn't have xdg-open, ask the user to set editor.
  } else{                           //  Load editor
    fp = fopen(".editor", "r");
    while((editor[c] = getc(fp)) != '\n' && (editor[c] != EOF)){ c++;}
    editor[c] = '\0';
    fclose(fp);
  }

  load_files(files, d);

  while(true){
    fflush(stdout);
    system("clear");
    display_time();
    display_dirs(d, dirName);
    display_files(files, num_files, &idx);
    if(!hide) display_options();

    printf("Enter Operation: ");
    fgets(args, ARGS_MAX, stdin);
    while(args[1] != '\n'){
      fprintf(stderr, "Invalid input. Reenter Operation: ");
      fgets(args, ARGS_MAX, stdin);
    }

    while(true){
      switch(toupper(args[0])){
        case 'E':
          edit_file(files, editor);
          break;
        case 'R':
          run_program(files);
          break;
        case 'C':
          change_dir(&files, d, dirName);
          idx = 0;
          break;
        case 'S':
          sort(files);
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
        case 'X':
          change_editor(editor, editorPath);
          break;
        default:
          fprintf(stderr, "Invalid input. Reenter Operation: ");
          fgets(args, ARGS_MAX, stdin);
          continue;
      }
    break;
    }
  }

  free(files);
  free(editor);
}