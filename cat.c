#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<errno.h>

#define MAX_BUF_SIZE 16

/* there is no limit on no. of cmd-line args */
int main(int argc, char **argv)
{
  if(argc == 1) {
    fprintf(stderr, "ERROR: Usage: %s <file1> [<file2>]\n", *argv);
    exit(1);
  } else {
    for(int i=1; i<argc; i++) {
      char *fname = *(argv + i);
      char buf[MAX_BUF_SIZE] = {0};
      FILE *fstream;
      if((fstream = fopen(fname, "r")) == NULL) {
        fprintf(stderr, "ERROR: couldn't open file %s: %s\n", fname, strerror(errno));
        exit(1);
      }


      while(!feof(fstream)) {
        memset(buf, 0, MAX_BUF_SIZE);

        // if I read MAX_BUF_SIZE, there is garbage printed on stdout; why?
        // making it MAX_BUF_SIZE - 1, alongwith memset buf to 0 gives expected output on stdout
        // why?
        size_t nchar_read = fread(buf, sizeof(char), MAX_BUF_SIZE-1, fstream);
        if(nchar_read < MAX_BUF_SIZE-1) {
          if(feof(fstream)) {
            // do nothing
          } else if(ferror(fstream)) {
            fprintf(stderr, "ERROR: couldn't read file %s: %s\n", fname, strerror(errno));
            fclose(fstream);
            exit(1);
          }
        }
        printf("%s", buf);
      }
      fclose(fstream);
    }
  }

  return 0;
}
