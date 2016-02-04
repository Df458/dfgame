namespace DF
{
[CCode (cheader_filename="io_util.h")]
namespace IO
{
    [CCode (cname="get_base_resource_path")]
        unowned string? get_path();

    [CCode (cname="get_unique_resource_name")]
        string get_unique_name(string? location, string name);

    [CCode (cname="construct_extended_resource_path")]
        string make_path(string? location, string name);

    [CCode (cname="get_extension")]
        unowned string? get_extension(string filename);

    [CCode (cname="path_to_content_path")]
        string? path_to_content_path(string path);
}

[CCode (cheader_filename="util.h")]
namespace Logger
{
    [CCode (cname = "log_level", cprefix = "LOG_", has_type_id = false)]
    public enum LogLevel {
        INFO,
        WARNING,
        ERROR,
        FATAL
    }
    [CCode (cname = "_log")]
    public void log(string file, uint line, LogLevel level, string message, ...);
    [CCode (cname = "info")]
    public void log_info(string message, ...);
    [CCode (cname = "warn")]
    public void log_warning(string message, ...);
    [CCode (cname = "error")]
    public void log_error(string message, ...);
    [CCode (cname = "fatal")]
    public void log_fatal(string message, ...);
    [CCode (cname = "stub")]
    public void stub();

    [CCode (cname = "log_handler", has_target = "false")]
    public delegate void LogHandler(string file, uint line, LogLevel level, string message);

    [CCode (cname = "register_log_handler")]
    public void register_log_handler(LogHandler handler);
}
}
