#ifndef LOGGER_H
#define LOGGER_H

#include "types.h"

#define LOG_BUFFER_SIZE 2000

typedef enum {
    LOG_INFO,
    LOG_OK,
    LOG_WARN,
    LOG_ERROR
} log_level_t;

void logger_init();
void log_message(log_level_t level, const char* message);
void log_info(const char* message);
void log_ok(const char* message);
void log_warn(const char* message);
void log_error(const char* message);
const char* logger_get_buffer();
size_t logger_get_buffer_size();

#endif
