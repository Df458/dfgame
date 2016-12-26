// Log category, used to filter logs
#define LOG_CATEGORY "Core"

#include "log.h"

#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WINDOWS
#include <windows.h>
#elif __GNUC__
#define _GNU_SOURCE
#include <unistd.h>
#endif

static const char* const log_level_names[4] =
{
    "INFO",
    "WARN",
    "ERROR",
    "FATAL",
};

static log_handler current_handler = 0;
static FILE* current_file = 0;

static const char* const log_format = "[%s] %s:%d, LOG LEVEL %s: %s\n";

void _log(const char* file, unsigned line, const char* category, log_level level, char* const message, ...)
{
    if(!current_file)
        current_file = stderr;

    if(level > LOG_FATAL) {
        warn("Invalid log level %d received. Level reset to WARN.", level);
        level = 0;
    }
	va_list args;
	size_t length;

    // Get the length of the format string
    length = snprintf(0, 0, log_format, category, file, line, log_level_names[level], message);
    ++length;
    char* format = malloc(sizeof(char) * length);
    if(!format) {
        fprintf(current_file, "%s:%d, LOG LEVEL FATAL: Cannot write message: Out of memory\n Partial message text: %s\n", __FILE__, __LINE__, message);
        exit(1);
    }
    snprintf(format, length, log_format, category, file, line, log_level_names[level], message);

	va_start(args, message);
    length = vsnprintf(0, 0, format, args);
    va_end(args);
	++length;
    char* final_message = malloc(sizeof(char) * length);
    if(!format) {
        fprintf(current_file, "%s:%d, LOG LEVEL FATAL: Cannot handle message: Out of memory. Message follows:\n", __FILE__, __LINE__);
        va_start(args, message);
        vfprintf(current_file, format, args);
        va_end(args);

        exit(1);
    }

	va_start(args, message);
    vsnprintf(final_message, length, format, args);
	va_end(args);

    fprintf(current_file, final_message);

    if(current_handler != 0) 
        current_handler(file, line, level, message);
    else
        free(final_message);

    if(level == LOG_FATAL)
        exit(1);
}

// TODO: Pass in user pointer
void register_log_handler(log_handler handler)
{
    current_handler = handler;
}

void register_log_file(FILE* file)
{
    current_file = file;
}

unsigned int next_power_of_two(int num)
{
    if(num < 2)
        return 1;

    unsigned int n = 2;
    while(num > n) {
        n = n << 1;
    }

    return n;
}

#undef LOG_CATEGORY