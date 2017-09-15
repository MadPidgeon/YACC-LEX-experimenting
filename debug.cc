#include "debug.h"

int msglevel = 90;

void pmesg(int level, const char* format, ...) {
  va_list args;

  if (level <= msglevel) {
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
  }
}
