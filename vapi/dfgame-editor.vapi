namespace DF {
    [CCode (cheader_filename = "project.h")]
    namespace Editor {
        [CCode (cname="create_project")]
        bool create_project(string path);
        [CCode (cname="load_project")]
        bool load_project(string path);
        [CCode (cname="get_project_name")]
        unowned string? get_loaded_project_name();
        [CCode (cname="get_project_path")]
        unowned string? get_loaded_project_path();
    }
}
