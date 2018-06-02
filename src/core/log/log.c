// Log category, used to filter logs
#define LOG_CATEGORY "Core"

#include "log.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WINDOWS
#include <windows.h>
#elif __GNUC__
#define _GNU_SOURCE
#include <signal.h>
#include <unistd.h>
#endif

static const char* const log_level_names[4] =
{
    "INFO",
    "WARN",
    "ERROR",
    "FATAL",
};

static log_handler* current_handler;
static FILE* current_file = 0;

static const char* const log_format = "[%s] %s:%d, LOG LEVEL %s: %s\n";

void _log(const char* file, uint32 line, const char* category, log_level level, const char* message, ...)
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

    free(format);

    fprintf(current_file, final_message);

    call_event(current_handler, file, line, level, final_message);
    free(final_message);

    if(level == LOG_FATAL)
        exit(1);
}

void _log_va(const char* file, uint32 line, const char* category, log_level level, const char* message, va_list args)
{
    if(!current_file)
        current_file = stderr;

    if(level > LOG_FATAL) {
        warn("Invalid log level %d received. Level reset to WARN.", level);
        level = 0;
    }
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

    va_list tempargs;
	va_copy(tempargs, args);
    length = vsnprintf(0, 0, format, tempargs);
    va_end(tempargs);
	++length;
    char* final_message = malloc(sizeof(char) * length);
    if(!format) {
        fprintf(current_file, "%s:%d, LOG LEVEL FATAL: Cannot handle message: Out of memory. Message follows:\n", __FILE__, __LINE__);
        va_copy(tempargs, args);
        vfprintf(current_file, format, args);
        va_end(tempargs);

        exit(1);
    }

	va_copy(tempargs, args);
    vsnprintf(final_message, length, format, args);
	va_end(tempargs);

    free(format);

    fprintf(current_file, final_message);

    call_event(current_handler, file, line, level, final_message);
    free(final_message);

    if(level == LOG_FATAL) {
#ifdef WINDOWS
    DEBUGBREAK();
#elif __GNUC__
    raise(SIGTRAP);
#endif
        exit(1);
    }
}

void register_log_handler(log_handler* handler)
{
    bind_event(current_handler, handler);
}

void register_log_file(FILE* file)
{
    current_file = file;
}

#undef LOG_CATEGORY
