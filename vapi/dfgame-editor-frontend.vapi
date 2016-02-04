/* dfgame-editor-frontend.vapi generated by valac 0.30.0, do not modify. */

[CCode (cheader_filename = "dfgame-editor-front.h")]
public class ResourceSelector : Gtk.Box {
	public ResourceSelector (string display_title, string? desired_extension = null, bool should_allow_multiple = false, bool show_cancel = false);
	public ResourceEntry get_selected ();
	public Gee.ArrayList<ResourceEntry> get_selected_full ();
	public signal void respond (Gtk.ResponseType response);
}
[CCode (cheader_filename = "dfgame-editor-front.h")]
public class ResourceModel : GLib.ListModel, GLib.Object {
	public ResourceModel (string? desired_extension);
	public void append (GLib.File f);
	public GLib.Object? get_object (uint position);
}
[CCode (cheader_filename = "dfgame-editor-front.h")]
public class ResourceEntry : GLib.Object {
	public string name;
	public string? path;
	public ResourceEntry (GLib.File file);
	public GLib.File construct_file ();
	public string construct_path ();
	public ResourceEntry.from_entry (string? inpath, string inname, bool new_file = false);
}
