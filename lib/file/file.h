#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

char* file_read(const char* path, size_t* out_byte_size);