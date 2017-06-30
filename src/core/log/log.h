#ifndef DF_CORE_LOG
#define DF_CORE_LOG

// If the log category is unset, set it to the default
#ifndef LOG_CATEGORY
#define LOG_CATEGORY "GENERIC"
#endif // LOG_CATEGORY

#include "types.h"
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

// log_level: Represents the severity of a log entry.
// NOTE: A LOG_FATAL message will cause the program to immediately halt.
typedef enum log_level {
    LOG_INFO = 0, // Extra debugging info

    LOG_WARNING,  // A slight issue, or something that could cause problems
                  // later on.

    LOG_ERROR,    // A more serious issue, something that needs to break out of
                  // a function but (hopefully) won't result in a crash.

    LOG_FATAL     // This kills the program immediately. Reserved for errors
                  // that will guarantee a crash, such as memory errors.
} log_level;

event(log_handler, const char*, uint32, log_level, const char*);

void _log(const char* file, uint32 line, const char* category, log_level level, const char* message, ...);
void _log_va(const char* file, uint32 line, const char* category, log_level level, const char* message, va_list args);
#define info(message, ...)  _log(__FILE__, __LINE__, LOG_CATEGORY, LOG_INFO,    message, ## __VA_ARGS__)
#define warn(message, ...)  _log(__FILE__, __LINE__, LOG_CATEGORY, LOG_WARNING, message, ## __VA_ARGS__)
#define error(message, ...) _log(__FILE__, __LINE__, LOG_CATEGORY, LOG_ERROR,   message, ## __VA_ARGS__)
#define fatal(message, ...) _log(__FILE__, __LINE__, LOG_CATEGORY, LOG_FATAL,   message, ## __VA_ARGS__)

// TODO: This code is OS-specific. We'll want to block it from compiling on
// certain targets
#define error_code() error("Error code %d recieved: %s", errno, strerror(errno))

#define nulltest(ptr) if(!ptr) fatal("Unexpected null pointer")
#define stub(ret) { warn("%s stub!", __FUNCTION__); return ret; }

void register_log_handler(log_handler* handler);
void register_log_file(FILE* file);

#endif // DF_CORE_LOG
