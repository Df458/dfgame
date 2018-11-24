[CCode(cheader_filename = "core/log/log.h")]
namespace DFGame.Core {
    [CCode(cname = "log_level", cprefix = "LOG_", has_type_id = false)]
    public enum LogLevel {
        CORE,
        WARNING,
        ERROR,
        FATAL
    }

    public class Logger {
        [CCode(cname = "info")]
        [PrintfFormat]
        public static void info(string message, ...);

        [CCode(cname = "warn")]
        [PrintfFormat]
        public static void warn(string message, ...);

        [CCode(cname = "error")]
        [PrintfFormat]
        public static void error(string message, ...);

        [CCode(cname = "fatal")]
        [NoReturn]
        [PrintfFormat]
        public static void fatal(string message, ...);

        [CCode (cname = "log_handler_func", has_target = false)]
        public delegate void LogCallback(uint16 width, uint16 height, void* user);
        [CCode (cname = "log_handler", destroy_function = "", has_type_id = false)]
        public struct LogCallbackData {
            [CCode (cname = "func", has_target = false)]
            LogCallback cb;
            [CCode (cname = "user")]
            void* user;
        }

        [CCode (cname = "register_log_handler")]
        public void register_handler(LogCallbackData cb);
    }
}
