#ifndef LOGGING_H
#define LOGGING_H

#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_WARN 2
#define  LOG_LEVEL_ERROR 3

void log_message(const char *log_level, char *fmt, ...);

#define log_error(fmt, ...) log_message("ERR", fmt, ##__VA_ARGS__)

#if LOG_LEVEL > LOG_LEVEL_WARN
#define log_warn(fmt, ...)
#else
#define log_warn(fmt, ...)  log_message("WARN", fmt, ##__VA_ARGS__)
#endif

#if LOG_LEVEL > LOG_LEVEL_INFO
#define log_info(fmt, ...)
#else
#define log_info(fmt, ...)  log_message("INFO", fmt, ##__VA_ARGS__)
#endif

#if LOG_LEVEL > LOG_LEVEL_DEBUG
#define log_debug(fmt, ...)
#else
#define log_debug(fmt, ...) log_message("DEBUG", fmt, ##__VA_ARGS__)
#endif

#endif
