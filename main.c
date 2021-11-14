#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<dirent.h>
#include<assert.h>

/* print contents of current directory recursively*/

char *concat_path(const char *a, const char *b) 
{
  char *dir_path = (char *) malloc(sizeof(char) * (strlen(a) + strlen(b)) + 1);;
  char *j = dir_path;
  while((*(j++) = *(a++)) != '\0') {}
  *(j-1) = '/';
  
  while((*(j++) = *(b++)) != '\0') {}
  *j = '\0';
  return dir_path;
}


void print_level(int level) {
  for(int i=0; i<level; i++) {
    printf("___ ");
  }
}


void print_dir_recursively(const char *dir_path, int level) 
{
  DIR *dirp = opendir(dir_path);
  if(dirp == NULL) {
    fprintf(stderr, "ERROR: couldn't open directory %s: %s\n",
            dir_path, strerror(errno));
    exit(1);
  }


  errno = 0;
  struct dirent *ent = readdir(dirp);
  while(ent != NULL) {
    if(ent->d_name[0] != '.') {
      print_level(level);

      if(ent->d_type == DT_DIR) {
        char *new_dir_path = concat_path(dir_path, ent->d_name);
        printf("dir: %s\n", ent->d_name);
        print_dir_recursively(new_dir_path, level+1);
        free(new_dir_path);
      }
      else if(ent->d_type == DT_REG) {
        printf("file: %s\n", ent->d_name);
      }
      else if(ent->d_type == DT_UNKNOWN) {
        printf("unknown: %s\n", ent->d_name);
      }
      else {
        printf("???: %s\n", ent->d_name);
      }
    }
    ent = readdir(dirp);
  }

  if(errno != 0) {
    fprintf(stderr, "ERROR: couldn't read directory %s: %s\n",
            dir_path, strerror(errno));
    exit(1);
  }

  closedir(dirp);
}


int main()
{
  const char *dir_path = "."; 
  print_dir_recursively(dir_path, 1);

  return 0;
}
