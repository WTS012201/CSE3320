#include "file_ops.h"

file_dat* insert_file(file_dat* arr, int idx, dirent *de){
  if(strlen(de->d_name) > NAME_MAX){
    fprintf(stderr, "File name excedes limit of %d chars: %d chars.\n", NAME_MAX, strlen(de->d_name));
    exit(EXIT_FAILURE);
  }

  file_dat* in = malloc(sizeof(file_dat));
  struct stat st;

  stat(de->d_name, &st);
  in->fName = (char*)malloc(sizeof(char)*strlen(de->d_name));
  strcpy(in->fName, de->d_name);
  in->size = st.st_size;
  in->date = NULL;
  arr[idx] = *in;

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
    if(de->d_type == __DT_DIR)
      printf( " ( %d Directory:  %s ) \n", c++, de->d_name);	  
  }
  closedir( d );
  printf( "-----------------------------------------\n" );
}

void load_files(file_dat* arr, DIR* d, dirent* de){
  int c = 0;
  if((d = opendir( "." )) == NULL){
    fprintf(stderr, "Current working directory could not be opened.\n");
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
  int i, n;
  char params[FILE_MAX];

  printf("Enter file number: ");
  scanf("%d", &n);

  printf("Arguments (If none press enter): ");
  while((params[i++] = getchar()) != '\n'){}  getchar();

  char* in = (char*)malloc(sizeof(char)*(strlen("./ ") + strlen(arr[n].fName) + ARGS_MAX));
  sprintf(in, "./%s %s", arr[n].fName, params);
  printf("%s\n", in);
  system("clear");

  if(system(in)){
    fprintf(stderr, "\nProgram could not be executed.\n");
    printf("Check if the selected file is executable and file permissions.\n\n");
    printf("Press enter to continue.");
    while(getchar() != '\n'){}  getchar();
  }
  
  free(in);
}