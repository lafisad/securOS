#include "logger.h"
#include "string.h"
#include "../drivers/serial.h"

static char log_buffer[LOG_BUFFER_SIZE];
static size_t log_position = 0;

void logger_init() {
    memset(log_buffer, 0, LOG_BUFFER_SIZE);
    log_position = 0;
    serial_init();
    serial_write("[SERIAL] Logger initialized\n");
}

void log_message(log_level_t level, const char* message) {
    const char* prefix;
    switch(level) {
        case LOG_INFO: prefix = "[INFO] "; break;
        case LOG_OK:   prefix = "[OK]   "; break;
        case LOG_WARN: prefix = "[WARN] "; break;
        case LOG_ERROR: prefix = "[ERR]  "; break;
        default: prefix = "[????] "; break;
    }
    
    // Write to Serial
    serial_write(prefix);
    serial_write(message);
    serial_write("\n");
    
    // Add prefix to buffer
    size_t prefix_len = strlen(prefix);
    for (size_t i = 0; i < prefix_len && log_position < LOG_BUFFER_SIZE - 1; i++) {
        log_buffer[log_position++] = prefix[i];
    }
    
    // Add message to buffer
    size_t msg_len = strlen(message);
    for (size_t i = 0; i < msg_len && log_position < LOG_BUFFER_SIZE - 1; i++) {
        log_buffer[log_position++] = message[i];
    }
    
    // Add newline to buffer
    if (log_position < LOG_BUFFER_SIZE - 1) {
        log_buffer[log_position++] = '\n';
    }
    
    log_buffer[log_position] = '\0';
}

void log_info(const char* message) {
    log_message(LOG_INFO, message);
}

void log_ok(const char* message) {
    log_message(LOG_OK, message);
}

void log_warn(const char* message) {
    log_message(LOG_WARN, message);
}

void log_error(const char* message) {
    log_message(LOG_ERROR, message);
}

const char* logger_get_buffer() {
    return log_buffer;
}

size_t logger_get_buffer_size() {
    return log_position;
}
