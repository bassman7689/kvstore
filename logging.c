#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "logging.h"

void log_message(const char *log_level, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  time_t rawtime;
  time(&rawtime);
  char buf[80];
  strftime(buf, 80, "%m/%d/%Y%l:%M:%S %p", gmtime(&rawtime));
  fprintf(stderr, "%s %s: ", buf, log_level);
  vprintf(fmt, args);
  va_end(args);
}
