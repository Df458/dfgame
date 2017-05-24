#ifndef CORE_CHECK_H
#define CORE_CHECK_H
#include "log/log.h"
#include "types.h"

bool _check(const char* file, uint32 line, const char* category, bool value, log_level level, const char* message, ...);
#define check_warn(v, m, ...) _check(__FILE__, __LINE__, LOG_CATEGORY, v, LOG_WARNING, m, ## __VA_ARGS__)
#define check_error(v, m, ...) _check(__FILE__, __LINE__, LOG_CATEGORY, v, LOG_ERROR, m, ## __VA_ARGS__)
#define check_break(v, m, ...) { if(_check(__FILE__, __LINE__, LOG_CATEGORY, v, LOG_ERROR, m, ## __VA_ARGS__)) break; }
#define check_return(v, m, r, ...) { if(_check(__FILE__, __LINE__, LOG_CATEGORY, v, LOG_ERROR, m, ## __VA_ARGS__)) return r; }
#define check_kill(v, m, ...) _check(__FILE__, __LINE__, LOG_CATEGORY, v, LOG_FATAL, m, ## __VA_ARGS__)

#endif
