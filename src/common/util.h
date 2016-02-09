#ifndef DFGAME_UTIL_H
#define DFGAME_UTIL_H

#include <errno.h>
#include <string.h>

typedef enum
{
    LOG_INFO,    // Extra debugging info
    LOG_WARNING, // A slight issue, or something that could cause problems later on.
    LOG_ERROR,   // A more serious issue, but something that (hopefully) won't result in a crash.
    LOG_FATAL    // This kills the program immediately. Reserved for errors that will guarantee a crash, such as null pointers or out of memory errors.
} log_level;

typedef void (*log_handler)(const char*, unsigned, log_level, char*);

void _log(const char* file, unsigned line, log_level level, char* const message, ...);
#define info(message, ...)  _log(__FILE__, __LINE__, LOG_INFO,    message, ## __VA_ARGS__)
#define warn(message, ...)  _log(__FILE__, __LINE__, LOG_WARNING, message, ## __VA_ARGS__)
#define error(message, ...) _log(__FILE__, __LINE__, LOG_ERROR,   message, ## __VA_ARGS__)
#define fatal(message, ...) _log(__FILE__, __LINE__, LOG_FATAL,   message, ## __VA_ARGS__)
#define error_code() error("Error code %d recieved: %s", errno, strerror(errno))
#define nulltest(ptr) if(!ptr) fatal("Unexpected null pointer")
#define stub(ret) warn("%s stub!", __FUNCTION__); return ret

void register_log_handler(log_handler handler);

unsigned char* loadFileContents(const char* const filepath);

#define DEGTORAD 0.01745329251
#define RADTODEG 57.2957795131

#endif
