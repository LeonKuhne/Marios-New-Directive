#include "file.h"

char* file_read(const char* path, size_t* out_byte_size) {
  // open file
  FILE* file = fopen(path, "rb");
  if (!file) return nullptr;

  // get file size
  fseek(file, 0, SEEK_END);
  *out_byte_size = ftell(file);

  // read to buffer
  char* buffer = static_cast<char*>(malloc(*out_byte_size));
  fseek(file, 0, SEEK_SET);
  size_t read_size = fread(buffer, 1, *out_byte_size, file);

  // close file
  fclose(file);

  if (read_size != *out_byte_size) {
    free(buffer);
    return nullptr;
  }
  return buffer;
}