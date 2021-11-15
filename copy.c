#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<errno.h>
#include<assert.h>

#define MAX_BUF_SIZE 16
#define MAX_FILENAME_SIZE 200

void create_file_copy(const char *infile_name, char *outfile_name) 
{

  if(outfile_name == NULL) {
    char filename[MAX_FILENAME_SIZE];
    outfile_name = filename;
    char *prefix_str = "copy_";
    assert(strlen(infile_name) <= MAX_FILENAME_SIZE - strlen(prefix_str));
    memcpy(filename, prefix_str, strlen(prefix_str));
    memcpy(filename + strlen(prefix_str), infile_name,
        strlen(infile_name));
  }


  char buf[MAX_BUF_SIZE];
  FILE *in_stream, *out_stream;

  if((in_stream = fopen(infile_name, "r")) == NULL) {
    fprintf(stderr, "ERROR: can't open file %s: %s\n", infile_name,
            strerror(errno));
    exit(1);
  }

  if((out_stream = fopen(outfile_name, "w+")) == NULL) {
    fprintf(stderr, "ERROR: can't open file %s: %s\n", outfile_name,
            strerror(errno));
    fclose(in_stream);
    exit(1);
  }
  printf("file: %s\n", outfile_name);

  while(!feof(in_stream)) {
    size_t nchar_read, nchar_write;

    if((nchar_read = fread(buf, sizeof(char), MAX_BUF_SIZE, in_stream)) < MAX_BUF_SIZE) {
      if(feof(in_stream)) {
        // let it be written to out_stream
      } else if(ferror(in_stream)) {
        fprintf(stderr, "ERROR: error reading file %s\n", infile_name);
        fclose(in_stream);
        fclose(out_stream);
        exit(1);
      }
    }
    if((nchar_write = fwrite(buf, sizeof(char), nchar_read, out_stream)) < nchar_read) {
      if(feof(out_stream)) {
        printf("INFO: reached end of file %s\n", outfile_name);
        break;
      } else if(ferror(out_stream)) {
        fprintf(stderr, "ERROR: error writing file %s\n", outfile_name);
        fclose(in_stream);
        fclose(out_stream);
        exit(1);
      }
    }
  }

  fclose(in_stream);
  fclose(out_stream);
}


int main(int argc, char **argv)
{
  if(argc > 3 || argc < 2) {
    fprintf(stderr, "ERROR: Usage: %s <src_file> [<target_file>]\n", *argv);
    exit(1);
  }

  char *infile_name = *(argv+1);
  if(argc == 2)
    create_file_copy(infile_name, NULL);
  else if(argc == 3)
    create_file_copy(infile_name, *(argv+2));

  return 0;
}
