//  NEED TO HANDLE ERRORS BETTER WITH ERNNO. Check for out of bounds etc...
// CHANGE DIRENT PARAMS
#include "file_ops.h"

file_dat* insert_file(file_dat* arr, int idx, dirent *de){
  if(strlen(de->d_name) > NAME_MAX){
    fprintf(stderr, "File name excedes limit of %d chars: %d chars.\n", NAME_MAX, strlen(de->d_name));
    exit(EXIT_FAILURE);
  }

  file_dat* file = malloc(sizeof(file_dat));
  struct stat st;

  stat(de->d_name, &st);
  file->fName = (char*)malloc(sizeof(char)*strlen(de->d_name));
  strcpy(file->fName, de->d_name);
  file->size = st.st_size;
  file->date = *localtime(&(st.st_mtime));
  arr[idx] = *file;

  return arr;
}

void display_time(){
  time_t t = time( NULL );
  
  printf( "Time: %s\n", ctime( &t ));
  printf("-----------------------------------------" );
}

void display_dirs(DIR* d){
  char cwd[NAME_MAX];
  int c = 0;
  dirent* de;
  
  if(getcwd(cwd, sizeof(cwd)) == NULL){
    fprintf(stderr, "Current working directory could not be opened.\n");
    exit(EXIT_FAILURE);
  }

  if((d = opendir(cwd)) == NULL){
    fprintf(stderr, "Directory couldn't be determined or size conflict.\n");
    exit(EXIT_FAILURE);
  }

  printf( "\nCurrent Directory: %s \n", cwd);

  while(de = readdir(d)){
    if(de->d_type == __DT_DIR)
      printf( " ( %d Directory:  %s ) \n", c++, de->d_name);	  
  }
  closedir( d );
  printf( "-----------------------------------------\n" );
}

void load_files(file_dat* arr, DIR* d){
  char cwd[NAME_MAX];
  int c = 0;
  dirent* de;

  if(getcwd(cwd, sizeof(cwd)) == NULL){
    fprintf(stderr, "Current working directory could not be opened.\n");
    exit(EXIT_FAILURE);
  }

  if((d = opendir(cwd)) == NULL){
    fprintf(stderr, "Directory couldn't be determined or size conflict.\n");
    exit(EXIT_FAILURE);
  }

  while (de = readdir(d)){                    
    if (de->d_type == __DT_REG)
      insert_file(arr, c++, de);
  }
  closedir( d );
}

void display_files(file_dat* arr, int amount, int *idx){
  int c = *idx;

  for(; c < (amount + *idx); c++){
    if(arr[c].fName == NULL)
      break;
    if(arr[c].size < 1000)
      printf( " ( %d File:  %s | \tSize: %.3d Bytes | ", c, arr[c].fName, arr[c].size);
    else if(arr[c].size < 1000000)
      printf( " ( %d File:  %s | \tSize: %.3f KiB | ", c, arr[c].fName, (float)arr[c].size/1000);
    else if(arr[c].size < 1000000000)
      printf( " ( %d File:  %s | \tSize: %.3f MiB | ", c, arr[c].fName, (float)arr[c].size/1000000);
    else
      printf( " ( %d File:  %s | \tSize: %.3f GiB | ", c, arr[c].fName, (float)arr[c].size/1000000000);
    printf("\tDate: %d-%d-%d ", arr[c].date.tm_year + 1900, arr[c].date.tm_mon + 1, arr[c].date.tm_mday);
    printf("%d:%d:%d)\n", arr[c].date.tm_hour, arr[c].date.tm_min, arr[c].date.tm_sec);
  }
  printf( "-----------------------------------------\n" );
}

void display_options(){
  printf("Operation:\tE Edit\n\t\tR Run\n\t\tC Change Directory");
  printf("\n\t\tS Sort Directory file/date\n\t\tQ Quit");
  printf("\n\t\tN Next\n\t\tP Previous\n\t\tH Hide Operations\n");
  printf( "-----------------------------------------\n" );
}

void edit_file(file_dat* arr, char* editor){
  int n;
  char* in = (char*)malloc(sizeof(char)*(NAME_MAX + strlen(editor)));

  printf("Enter file number: ");
  scanf("%d", &n);
  sprintf(in, "%s %s", editor, arr[n].fName);
  system("clear");

  if(system(in)){
    fprintf(stderr, "\nFile could not be opened using %s\n", editor);
    printf("Check if the selected file can be opened with the editor.\n\n");
    printf("Press enter to continue.");
    while(getchar() != '\n'){}  getchar();
  }

  free(in);
}

void run_program(file_dat* arr){
  int n;
  char* params = (char*)malloc(sizeof(char)*ARGS_MAX);

  printf("Enter file number: ");
  scanf("%d", &n); getchar();
  
  printf("Arguments (If none press enter): ");
  fgets(params, ARGS_MAX, stdin);

  system("clear");

  char* in = (char*)malloc(sizeof(char)*(strlen("./ ") + strlen(arr[n].fName) + strlen(params)));
  sprintf(in, "./%s %s", arr[n].fName, params);

  if(system(in)){
    fprintf(stderr, "\nProgram could not be executed.\n");
    printf("Check if the selected file is executable and file permissions.\n\n");
    printf("Press enter to continue.");
    while(getchar() != '\n'){}
  }

  free(params);
  free(in);
}

int cmp_size(const void* a, const void* b){
  const file_dat* A = a;
  const file_dat* B = b;

  return (A->size - B->size);
}

void sort(file_dat* arr){
  int c = 0;

  while(arr[c].fName != NULL){ c++;}  //  Since fName is malloced, this is the easiest way to get the size of arr

  qsort(arr, c, sizeof(file_dat), cmp_size);
}
/*
void change_dir(DIR* d){ 
  int i, n;
  char cwd[NAME_MAX];
  dirent* de;

  printf("Enter directory number: ");
  scanf("%d", &n);

  if(getcwd(cwd, sizeof(cwd)) == NULL){
    fprintf(stderr, "Current working directory could not be opened.\n");
    exit(EXIT_FAILURE);
  }
  opendir(cwd);

  for(i = 0; i < n; i++)
    de = readdir(d);
  closedir(d);
  d = opendir(de->d_name);
  closedir(d);
}*/