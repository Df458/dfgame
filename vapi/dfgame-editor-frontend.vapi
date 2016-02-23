/* dfgame-editor-frontend.vapi generated by valac 0.30.0, do not modify. */

namespace DF {
	[CCode (cheader_filename = "dfgame-editor-front.h")]
	public class MissingFilesDialog : Gtk.Dialog {
		public MissingFilesDialog (Gtk.Window win);
		public void apply ();
	}
	[CCode (cheader_filename = "dfgame-editor-front.h")]
	public class ResourceSelector : Gtk.Box {
		public ResourceSelector (string display_title, string? desired_extension = null, bool should_allow_multiple = false, bool show_cancel = false);
		public void add_extension (string ext);
		public void generate_list ();
		public ResourceEntry get_selected ();
		public Gee.ArrayList<ResourceEntry> get_selected_full ();
		public signal void respond (Gtk.ResponseType response);
	}
	[CCode (cheader_filename = "dfgame-editor-front.h")]
	public class Viewport : Gtk.GLArea {
		public bool first_run;
		public Viewport ();
		public virtual bool initialize ();
		public virtual bool prepare ();
		public virtual bool render_view ();
		public signal void ready ();
	}
	[CCode (cheader_filename = "dfgame-editor-front.h")]
	public static GLib.File file_from_resource (string? ext, string name, bool new_file = false);
	[CCode (cheader_filename = "dfgame-editor-front.h")]
	public static bool file_is_content (GLib.File file);
	[CCode (cheader_filename = "dfgame-editor-front.h")]
	public static GLib.File get_content_directory ();
}
[CCode (cheader_filename = "dfgame-editor-front.h")]
public enum AssetType {
	NONE,
	ACTOR,
	MAP,
	SPRITE,
	TILESET
}
[CCode (cheader_filename = "dfgame-editor-front.h")]
public class ProjectFilePane : Gtk.Box {
	public int icon_size;
	public ProjectFilePane ();
	public string? get_selected_path ();
	public signal void file_import ();
	public signal void file_new (AssetType type);
	public signal void file_opened (string? resource_path, string resource_name);
}
[CCode (cheader_filename = "dfgame-editor-front.h")]
public class NewProjectDialog {
	public NewProjectDialog (Gtk.Window? parent, string default_path);
	public void destroy ();
	public void display ();
	public void respond (int response_id);
	public signal void response_selected (bool created, string? path = null);
}
[CCode (cheader_filename = "dfgame-editor-front.h")]
public class ResourceModel : GLib.ListModel, GLib.Object {
	public ResourceModel ();
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
[CCode (cheader_filename = "dfgame-editor-front.h")]
public interface AssetEditor : AssetViewer, Gtk.Widget {
	public abstract bool create (ResourceEntry entry);
	public abstract bool save ();
}
[CCode (cheader_filename = "dfgame-editor-front.h")]
public interface AssetViewer : Gtk.Bin {
	public abstract bool load (ResourceEntry entry);
}
