namespace DF
{
[CCode (cheader_filename="io_util.h", cheader_filename="asset_registry.h")]
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

    [CCode (cname="resource_loader", has_target=false)]
    public delegate void* resource_loader(string? location, string name);

    [CCode (cname="resources_init")]
    public bool init(resource_loader loader);
    [CCode (cname="resources_cleanup")]
    public void cleanup();

    [CCode (cname="load_anonymous_resource")]
    public void* load(string? location, string name);

    [CCode (cname="get_id_from_resource")]
    public uint32 get_id_from_resource(string? location, string name, bool should_create);

    [CCode (cname="get_name_from_id")]
    public string? get_name_from_id(uint32 id, bool force_invalid = false);

    [CCode (cname="get_path_from_id")]
    public string? get_path_from_id(uint32 id, bool force_invalid = false);

    [CCode (cname="move_resource")]
    public void move_resource(string? prev_location, string prev_name, string? next_location, string next_name);

    [CCode (cname="delete_resource")]
    public void delete_resource(string? resource_location, string resource_name);

    [CCode (cname="move_id")]
    public void move_id(uint32 id, string? next_location, string next_name);

    [CCode (cname="delete_id")]
    public void delete_id(uint32 id);

    [CCode (cname="get_failed_count")]
    public int32 get_failed_count();

    [CCode (cname="get_failed_ids")]
    public int32[] get_failed_ids();
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
