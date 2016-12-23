namespace DF.IO
{
public File get_content_directory()
{
    return File.new_for_path(DF.IO.get_path());
}

/*
 * This function takes a resource extension and filename and creates a GIO File
 * from the path returned by construct_extended_resource_path.
 */
public File file_from_resource(string? ext, string name, bool new_file = false)
{
    string path = make_path(ext, name);
    if(new_file)
        path = make_path(ext, DF.IO.get_unique_name(ext, name));
    File f = File.new_for_path(path);
    return f;
}

/*
 * Returns whether or not a file is part of the current project's content
 */
public bool file_is_content(File file)
{
    string? path = file.get_path();
    string cpath = get_path();

    if(path != null && path.length > cpath.length) {
        if(path.substring(0, cpath.length) == cpath) {
            return true;
        }
    }

    g_free(path);
    return false;
}
}
