#define LOG_CATEGORY "Audio"

#include "audio_log.h"

#include "log/log.h"

void _LogALError(const char* file, unsigned line, const char* function, const char* log_category, ALuint err) {
    const char* message;
    log_level severity = LOG_WARNING;
    switch(err) {
        case AL_INVALID_NAME:
            message = "Invalid Name";
            severity = LOG_ERROR;
        break;
        case AL_INVALID_ENUM:
            message = "Invalid Enum";
            severity = LOG_ERROR;
        break;
        case AL_INVALID_VALUE:
            message = "Invalid Value";
            severity = LOG_ERROR;
        break;
        case AL_INVALID_OPERATION:
            message = "Invalid Operation";
            severity = LOG_ERROR;
        break;
        case AL_OUT_OF_MEMORY:
            message = "Out of Memory";
            severity = LOG_FATAL;
        break;
        default:
            _log(file, line, function, log_category, severity, "OpenAL error: Received error code 0x%0X. Consider adding a custom message to handle it", err);
            message = "Unknown Error";
    }

    _log(file, line, function, log_category, severity, "OpenAL error: %s", message);
}
