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
  time_t t = time(NULL);
  
  printf( "Time: %s\n", ctime( &t ));
  printf("-----------------------------------------" );
}

void display_dirs(DIR* d, char* dirName){
  int c = 0;
  dirent* de;

  if((d = opendir(".")) == NULL){
    fprintf(stderr, "Error opening dir: %s\n", strerror(errno));
    fprintf(stderr, "Directory couldn't be opened.\n");
    exit(EXIT_FAILURE);
  }

  printf( "\nCurrent Directory: %s \n", dirName);

  while(de = readdir(d)){
    if(de->d_type == __DT_DIR)
      printf( " ( %d Directory:  %s ) \n", c++, de->d_name);	  
  }
  closedir(d);
  printf( "-----------------------------------------\n" );
}

void load_files(file_dat* arr, DIR* d){
  char cwd[NAME_MAX];
  int c = 0;
  dirent* de;

  if((d = opendir(".")) == NULL){
    fprintf(stderr, "Error opening dir: %s\n", strerror(errno));
    fprintf(stderr, "Directory couldn't be opened.\n");
    exit(EXIT_FAILURE);
  }

  while (de = readdir(d)){                    
    if (de->d_type == __DT_REG)
      insert_file(arr, c++, de);
  }
  closedir(d);
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
  int n, c = 0;
  char* in = (char*)malloc(sizeof(char)*(NAME_MAX + strlen(editor)));

  printf("Enter file number: ");
  scanf("%d", &n); getchar();

  while(arr[c].fName != NULL){ c++;}
  if(n >= c || n < 0){
    fprintf(stderr, "Invalid input. File number %d doesn't exist.\n", n);
    fprintf(stderr, "Press enter to continue.");
    while(getchar() != '\n'){}
    return;
  }

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
  int n, c = 0;
  char* params = (char*)malloc(sizeof(char)*ARGS_MAX);

  printf("Enter file number: ");
  scanf("%d", &n); getchar();
  
  while(arr[c].fName != NULL){ c++;}
  if(n >= c || n < 0){
    fprintf(stderr, "Invalid input. File number %d doesn't exist.\n", n);
    fprintf(stderr, "Press enter to continue.");
    while(getchar() != '\n'){}
    return;
  }

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
int cmp_date(const void* a, const void* b){
  const file_dat* A = a;
  const file_dat* B = b;
  bool C;
  
  return (A->date.tm_year < B->date.tm_year) ? true : 
  (C = (A->date.tm_year == B->date.tm_year)) && (A->date.tm_mon < B->date.tm_mon) ? true : 
  (C = C && (A->date.tm_mon == B->date.tm_mon)) && (A->date.tm_mday < B->date.tm_mday) ? true : 
  (C = C && (A->date.tm_mday == B->date.tm_mday)) && (A->date.tm_hour < B->date.tm_hour) ? true : 
  (C = C && (A->date.tm_hour == B->date.tm_hour)) && (A->date.tm_min < B->date.tm_min) ? true : 
  (C = C && (A->date.tm_min == B->date.tm_min)) && (A->date.tm_sec < B->date.tm_sec) ? true : false;

  return false;
}

void sort(file_dat* arr){
  int c = 0;

  while(arr[c].fName != NULL){ c++;}  //  Since fName is malloced, this is the easiest way to get the size of arr
  printf("Sort by size or date (S/D): ");
  qsort(arr, c, sizeof(file_dat), cmp_date);
  /*
  if(toupper(getchar()) == 'S'){
    qsort(arr, c, sizeof(file_dat), cmp_size);
  }else if(toupper(getchar()) == 'C'){
    qsort(arr, c, sizeof(file_dat), cmp_date);
  }*/
  
}

void change_dir(file_dat** arr, DIR* d, char* dirName){ 
  int i = 0, n;
  dirent* de;
  char cwd[NAME_MAX];
  char* temp;

  printf("Enter directory number: ");
  scanf("%d", &n);  getchar();
  system("clear");

  if(n < 0){
    fprintf(stderr, "Invalid input. Directory number %d doesn't exist.\n", n);
    fprintf(stderr, "Press enter to continue.");
    while(getchar() != '\n'){}
    return;
  }
  if((d = opendir(".")) == NULL){
    fprintf(stderr, "Error opening dir: %s\n", strerror(errno));
    fprintf(stderr, "Directory couldn't be opened.\n");
  }

  while((de = readdir(d)) && i <= n){
    if(de->d_type == __DT_DIR){
      i++;
      temp = de->d_name;
    }
  }

  closedir(d);
  chdir(temp);

  if(getcwd(cwd, NAME_MAX) == NULL){
    fprintf(stderr, "Error opening dir: %s\n", strerror(errno));
    fprintf(stderr, "Current directory could not be opened.\n");
    exit(EXIT_FAILURE);
  }
  
  for(i = 0; cwd[i] != '\0'; i++)
    dirName[i] = cwd[i];
  dirName[i] = '\0';

  file_dat* new_files = malloc(sizeof(file_dat)*FILE_MAX);
  load_files(new_files, d);
  *arr = new_files;
  
}