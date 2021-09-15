#include "file_ops.h"

file_dat* insert_file(file_dat* arr, int idx, dirent *de){
  if(strlen(de->d_name) > NAME_MAX){
    fprintf(stderr, "File name excedes limit of %d chars: %d chars.\n", NAME_MAX, (int)strlen(de->d_name));
    exit(EXIT_FAILURE);
  }

  file_dat* file = malloc(sizeof(file_dat));
  struct stat st;

  stat(de->d_name, &st);
  file->fName = (char*)malloc(sizeof(char)*(int)strlen(de->d_name));
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
  if(c > FILE_MAX){
    fprintf(stderr, "Error opening dir: %s\n", strerror(errno));
    fprintf(stderr, "Directory couldn't be opened.\n");
    exit(EXIT_FAILURE);
  }
  printf( "-----------------------------------------\n" );
}

void load_files(file_dat* arr, DIR* d){
  char cwd[NAME_MAX + 1];
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
    if(c > FILE_MAX){
      fprintf(stderr, "Limit of %d files exceded: %d files.\n", FILE_MAX, c);
      exit(EXIT_FAILURE);
    }
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
  printf("\n\t\tN Next\n\t\tP Previous\n\t\tH Hide Operations");
  printf("\n\t\tX Change Editor\n");
  printf( "-----------------------------------------\n" );
}

void edit_file(file_dat* arr, char* editor){
  int n, c = 0;
  char* in = (char*)malloc(sizeof(char)*(NAME_MAX + (int)strlen(editor) + 1));

  while(arr[c].fName != NULL){ c++;}
  if(c == 0){
    fprintf(stderr, "\nNo files in directory.\n\n");
    printf("Press enter to continue.");
    while(getchar() != '\n');
    return;
  }

  printf("Enter file number: ");
  while(!scanf("%d", &n) || (n >= c || n < 0)){
    fprintf(stderr, "Not a file number. Reenter file number: ");
    scanf("%*[^\n]");
  }
  getchar();

  sprintf(in, "%s %s", editor, arr[n].fName);
  system("clear");

  if(system(in)){
    fprintf(stderr, "\nFile could not be opened using %s\n", editor);
    printf("Check if the selected file can be opened with the editor.\n\n");
    printf("Press enter to continue.");
    while(getchar() != '\n');  
  }

  free(in);
}

void run_program(file_dat* arr){
  int n, c = 0;
  char* params = (char*)malloc(sizeof(char)*ARGS_MAX);
  
  while(arr[c].fName != NULL){ c++;}
  if(c == 0){
    fprintf(stderr, "\nNo files in directory.\n\n");
    printf("Press enter to continue.");
    while(getchar() != '\n');
    return;
  }

  printf("Enter file number: ");
  while(!scanf("%d", &n) || (n >= c || n < 0)){
    fprintf(stderr, "Not a file number. Reenter file number: ");
    scanf("%*[^\n]");
  }
  getchar();

  printf("Arguments (If none press enter): ");
  fgets(params, ARGS_MAX, stdin);
  system("clear");

  char* in = (char*)malloc(sizeof(char)*(int)(strlen("./ ") + strlen(arr[n].fName) + strlen(params)));
  sprintf(in, "./%s %s", arr[n].fName, params);

  if(system(in)){
    fprintf(stderr, "\nProgram could not be executed.\n");
    printf("Check if the selected file is executable and file permissions.\n\n");
    printf("Press enter to continue.");
    while(getchar() != '\n');
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
  (C &= (A->date.tm_mon == B->date.tm_mon)) && (A->date.tm_mday < B->date.tm_mday) ? true : 
  (C &= (A->date.tm_mday == B->date.tm_mday)) && (A->date.tm_hour < B->date.tm_hour) ? true : 
  (C &= (A->date.tm_hour == B->date.tm_hour)) && (A->date.tm_min < B->date.tm_min) ? true : 
  (C &= (A->date.tm_min == B->date.tm_min)) && (A->date.tm_sec < B->date.tm_sec) ? true : false;
}

void sort(file_dat* arr){
  int i = 0, n = 0;
  char args[ARGS_MAX + 1];

  while(arr[n].fName != NULL){ n++;}
  if(n == 0){
    fprintf(stderr, "\nNo files in directory.\n\n");
    printf("Press enter to continue.");
    while(getchar() != '\n');
    return;
  }
  printf("Sort by size or modified date (S/D): ");

  fgets(args, ARGS_MAX, stdin);
  while(args[1] != '\n'){
      fprintf(stderr, "Invalid input. Reenter Operation: ");
      fgets(args, ARGS_MAX, stdin);
  }

  while(true){
    if(toupper(args[0]) == 'S' || toupper(args[0]) == 'D'){
      if(toupper(args[0]) == 'S'){
        qsort(arr, n, sizeof(file_dat), cmp_size);
        printf("Least to Greatest or Greatest to Least (L/G): ");
      } else if(toupper(args[0]) == 'D'){
        qsort(arr, n, sizeof(file_dat), cmp_date);
        printf("Newest to Oldest or Oldest to Newest (N/O): ");
      }
      break;
    } else{
        fprintf(stderr, "Invalid input. Reenter Operation: ");
        fgets(args, ARGS_MAX, stdin);
        while(args[1] != '\n'){
          fprintf(stderr, "Invalid input. Reenter Operation: ");
          fgets(args, ARGS_MAX, stdin);
        }
    }
  }

  fgets(args, ARGS_MAX, stdin);
  while(args[1] != '\n'){
      fprintf(stderr, "Invalid input. Reenter Operation: ");
      fgets(args, ARGS_MAX, stdin);
  }

  while(true){
    while(args[1] != '\n'){
      fprintf(stderr, "Invalid input. Reenter Operation: ");
      fgets(args, ARGS_MAX, stdin);
    }

    if(toupper(args[0]) == 'L' || toupper(args[0]) == 'N')
      return;
    else if(toupper(args[0]) == 'G' || toupper(args[0]) == 'O'){
      for(; i < n/2; i++){
        file_dat file = arr[i];
        arr[i] = arr[n - i - 1];
        arr[n - i - 1] = file;
      }
      return;
    } else{
        fprintf(stderr, "Invalid input. Reenter Operation: ");
        fgets(args, ARGS_MAX, stdin);
    }
  }
}


void change_dir(file_dat** arr, DIR* d, char* dirName){ 
  int i = 0, c = 0, n;
  dirent* de;
  char cwd[NAME_MAX + 1];
  char* temp;

  if((d = opendir(".")) == NULL){
    fprintf(stderr, "Error opening dir: %s\n", strerror(errno));
    fprintf(stderr, "Directory couldn't be opened.\n");
    exit(EXIT_FAILURE);
  }
  while((de = readdir(d)))  //  Counting number of directories
    if(de->d_type == __DT_DIR)
      c++;
  closedir(d);

  printf("Enter directory number: ");
  while(!scanf("%d", &n) || (n >= c || n < 0)){
    fprintf(stderr, "Not a directory number. Reenter directory number: ");
    scanf("%*[^\n]");
  }
  getchar();

  opendir(".");
  while((de = readdir(d)) && i <= n){ //  Moves pointer to dir user selected
    if(de->d_type == __DT_DIR){
      i++;
      temp = de->d_name;
    }
  }

  system("clear");
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

  file_dat* new_files = malloc(sizeof(file_dat)*(FILE_MAX + 1));
  load_files(new_files, d);
  *arr = new_files;
}
void change_editor(char* editor){
  int c;
  FILE* fp;
  
  printf("Enter editor: ");
  fgets(editor, NAME_MAX, stdin);
  c = 0;
  while(editor[c] != '\n'){ c++;}
  editor[c] = '\0';

  fp = fopen(".editor", "w+");
  fputs(editor, fp);
  fclose(fp);
}