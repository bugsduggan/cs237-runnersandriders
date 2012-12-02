/*
 * file
 *
 * This takes a filename and reads the file into an array
 *
 * Tom Leaman (tom@tomleaman.co.uk)
 */

#include <stdlib.h>
#include <stdio.h>

typedef struct FileData {
  char* lines[];
  int num_lines;
}

void read_file(FileData* data, filename) {
  
}

FileData* FileData_get(char* filename) {
  FileData* data = malloc(sizeof(FileData));
  data->lines = NULL;
  data->num_lines = 0;
  read_file(data, filename);
  return data;
}

void FileData_destroy(FileData* data) {
  if (data) {
    if (data->lines) {
      free(data->lines);
    }
    free(data);
  }
}
