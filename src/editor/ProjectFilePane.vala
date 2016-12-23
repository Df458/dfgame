using Gtk;
using DF;

public enum AssetType
{
    NONE = 0,
    ACTOR,
    MAP,
    SPRITE,
    TILESET
}

public class ProjectFilePane : Box
{
    private Box             control_box;
    private Button          import_button;
    private SearchEntry     filter_entry;

    private TreeStore       file_data;
    private Paned           view_paned;
    private TreeView        sidebar_view;
    private TreeModelFilter sidebar_filter;
    private IconView        icon_view;
    private TreeModelFilter view_filter;
    ScrolledWindow          sidebar_scroll;
    ScrolledWindow          icon_scroll;

    private Overlay         view_overlay;
    private Scale           icon_scale;

    private Gtk.Menu content_menu;
    private Gtk.Menu content_file_menu;
    Gee.TreeMap<File, FileMonitor> project_monitors;

    public int icon_size = 5;

    /*
     * This enum assigns numerical values to each column of the project's
     * treemodel.
     */
    private enum ProjectTreeColumn
    {
        FILENAME = 0,
        URI,
        ICON_PIXBUF,
        SIDEBAR_FILTER,
        ICON_NAME,
        COLUMN_COUNT
    }

    public ProjectFilePane()
    {
        this.orientation = Orientation.VERTICAL;
        this.spacing = 6;
        this.margin_top = 6;
        this.set_homogeneous(false);
        project_monitors = new Gee.TreeMap<File, FileMonitor>();

        init_structure();
        init_content();
        connect_signals();
        add_actions();
        init_menus();

        add_project_directory(IO.get_content_directory(), null);
        TreePath path = new TreePath.from_indices(0);
        sidebar_view.expand_to_path(path);
        sidebar_view.set_cursor(path, null, false);
    }

    /*
     * This function returns the current sub-path of the resource directory
     */
    public string? get_selected_path()
    {
        // TODO: Cache this value
        string path_string = "";
        TreePath path;

        // We use the icon view and convert, because the tree view might be
        // deselected
        path = view_filter.virtual_root;

        // If nothing is selected, default to the base path
        if(path == null || path.get_depth() == 1)
            return null;

        while(path.get_depth() > 1) {
            string temp_name;
            TreeIter iter;
            file_data.get_iter(out iter, path);
            file_data.get(iter, ProjectTreeColumn.FILENAME, out temp_name);
            if(path_string != "")
                path_string = temp_name + "/" + path_string;
            else
                path_string = temp_name;
            path.up();
        }

        return path_string;
    }

    /*
     * This signal indicates that a file has been opened
     */
    public signal void file_opened(string? resource_path, string resource_name);

    /*
     * This signal indicates that the import dialog should be opened
     */
    public signal void file_import();

    /*
     * This signal indicates that a file should be created
     */
    public signal void file_new(AssetType type);

    /*
     * Initializes the structural widgets
     */
    private void init_structure()
    {
        control_box = new Box(Orientation.HORIZONTAL, 6);
        view_paned = new Paned(Orientation.HORIZONTAL);
        view_overlay = new Overlay();
        sidebar_scroll = new ScrolledWindow(null, null);
        icon_scroll = new ScrolledWindow(null, null);

        control_box.margin_start = 12;
        control_box.margin_end = 12;
        view_paned.position = 128;

        view_paned.pack1(sidebar_scroll, false, false);
        view_overlay.add(icon_scroll);
        view_paned.pack2(view_overlay, true, false);
        this.pack_start(control_box, false, false);
        this.pack_end(view_paned, true, true);
    }

    /*
     * Initializes display and interactive widgets
     */
    private void init_content()
    {
        import_button = new Button.with_label("Import");
        filter_entry = new SearchEntry();
        sidebar_view = new TreeView();
        icon_view = new IconView();
        icon_scale = new Scale.with_range(Orientation.HORIZONTAL, 1, 6, 1);

        // Tree data stuff
        file_data = new TreeStore(ProjectTreeColumn.COLUMN_COUNT, typeof(string), typeof(string), typeof(Gdk.Pixbuf), typeof(bool), typeof(string));
        file_data.set_sort_column_id(ProjectTreeColumn.FILENAME, SortType.ASCENDING);
        sidebar_filter = new TreeModelFilter(file_data, null);
        sidebar_filter.set_visible_column(ProjectTreeColumn.SIDEBAR_FILTER);
        sidebar_view.set_model(sidebar_filter);
        sidebar_view.set_headers_visible(false);

        build_view_filter();

        TreeViewColumn col_name = new Gtk.TreeViewColumn.with_attributes("Name", new CellRendererText(), "text", ProjectTreeColumn.FILENAME, null);
        sidebar_view.insert_column(col_name, -1);
        icon_view.text_column = ProjectTreeColumn.FILENAME;
        icon_view.pixbuf_column = ProjectTreeColumn.ICON_PIXBUF;
        icon_view.item_width = 64;
        foreach(weak CellRenderer cell in icon_view.get_cells()) {
            icon_view.set_cell_data_func(cell, iconview_cell_data);
        }

        icon_scale.restrict_to_fill_level = true;
        icon_scale.draw_value = false;
        icon_scale.add_mark(4, PositionType.BOTTOM, null);
        icon_scale.halign = Align.END;
        icon_scale.valign = Align.END;
        icon_scale.hexpand = false;
        icon_scale.vexpand = false;
        icon_scale.width_request = 128;
        icon_scale.set_value(4);
        icon_scale.get_style_context().add_class(STYLE_CLASS_OSD);
        icon_scale.margin = 6;

        import_button.image = new Image.from_icon_name("document-open-symbolic", IconSize.SMALL_TOOLBAR);
        import_button.get_style_context().add_class(STYLE_CLASS_SUGGESTED_ACTION);
        filter_entry.placeholder_text = "Search\u2026";
        filter_entry.sensitive = false;

        control_box.pack_start(filter_entry, false, false);
        control_box.pack_start(import_button, false, false);
        sidebar_scroll.add(sidebar_view);
        icon_scroll.add(icon_view);
        view_overlay.add_overlay(icon_scale);
    }

    /*
     * Connects functions to signals on added widgets
     */
    private void connect_signals()
    {
        sidebar_view.cursor_changed.connect(() =>
        {
            TreePath path;
            TreeIter iter;
            string uri;
            sidebar_view.get_cursor(out path, null);
            if(path == null)
                return;
            sidebar_filter.get_iter(out iter, path);
            sidebar_filter.get(iter, ProjectTreeColumn.URI, out uri);
            path = sidebar_filter.convert_path_to_child_path(path);
            view_filter.get_iter(out iter, path);

            build_view_filter(path);
        });

        icon_view.item_activated.connect((path) =>
        {
            TreeIter iter;
            string file_uri;
            string file_name;
            bool is_folder = false;
            view_filter.get_iter(out iter, path);
            view_filter.get(iter, ProjectTreeColumn.FILENAME, out file_name, ProjectTreeColumn.URI, out file_uri, ProjectTreeColumn.SIDEBAR_FILTER, out is_folder);
            if(is_folder) {
                TreePath new_path = sidebar_filter.convert_child_path_to_path(view_filter.convert_path_to_child_path(path));
                sidebar_view.expand_to_path(new_path);
                sidebar_view.set_cursor(new_path, null, false);
            } else {
                file_opened(get_selected_path(), file_name);
            }
        });
        CellRendererText text = (CellRendererText)icon_view.get_cells().nth_data(0);
        text.edited.connect((path, new_text) =>
        {
            TreeIter iter;
            string file_name;
            string file_uri;
            view_filter.get_iter(out iter, new TreePath.from_string(path));
            view_filter.get(iter, ProjectTreeColumn.FILENAME, out file_name, ProjectTreeColumn.URI, out file_uri);
            if(file_name == new_text) {
                text.editable = false;
                return;
            }

            File f = File.new_for_uri(file_uri);
            try {
                f = f.set_display_name(new_text);
            } catch(Error e) {
                Logger.log_warning("Error renaming file: %s", e.message);
            }
        });

        icon_view.button_press_event.connect((event) =>
        {
            if(event.button == 3) {
                TreePath path = icon_view.get_path_at_pos((int)event.x, (int)event.y);
                if(path != null) {
                    icon_view.select_path(path);
                    icon_view.set_cursor(path, null, false);
                } else {
                    icon_view.unselect_all();
                }
                icon_view.popup_menu();
            }
            return false;
        });
        icon_view.popup_menu.connect(() =>
        {
            if(icon_view.get_selected_items().length() == 0)
                content_menu.popup(null, null, null, 0, Gtk.get_current_event_time());
            else
                content_file_menu.popup(null, null, null, 0, Gtk.get_current_event_time());
            return false;
        });

        icon_scale.value_changed.connect(() =>
        {
            int new_value = (int)icon_scale.get_value();
            if(new_value == 4)
                new_value = 5;
            icon_view.item_width = new_value * 10 + 16;
            icon_size = new_value;
        });

        // TODO: File Previews
        import_button.clicked.connect(() => { file_import(); });
    }

    /*
     * Adds actions for use with menus and shortcuts
     */
    private void add_actions()
    {
        // Main group
        SimpleActionGroup content_group = new SimpleActionGroup();
        SimpleAction act_import = new SimpleAction("import", null);
        act_import.activate.connect(() => { file_import(); });
        act_import.set_enabled(true);
        content_group.add_action(act_import);
        SimpleAction act_open = new SimpleAction("open", null);
        act_open.activate.connect(() =>
        {
            icon_view.selected_foreach((view, path) =>
            {
                view.item_activated(path);
            });
        });
        content_group.add_action(act_open);
        SimpleAction act_rename = new SimpleAction("rename", null);
        act_rename.activate.connect(() =>
        {
            TreePath path = null;
            CellRenderer renderer;
            CellRendererText text = (CellRendererText)icon_view.get_cells().nth_data(0);
            icon_view.get_cursor(out path, out renderer);
            if(path != null) {
                text.editable = true;
                icon_view.set_cursor(path, text, true);
                text.editable = false;
            }
        });
        content_group.add_action(act_rename);
        SimpleAction act_delete = new SimpleAction("delete", null);
        act_delete.activate.connect(() =>
        {
            icon_view.selected_foreach((view, path) =>
            {
                TreeIter iter;
                string file_uri;
                view_filter.get_iter(out iter, path);
                view_filter.get(iter, ProjectTreeColumn.URI, out file_uri);
                // Get the file
                File file = File.new_for_uri(file_uri);
                try {
                    file.delete();
                } catch(Error e) {
                    Logger.log_warning("Can't delete file %s: %s", file_uri, e.message);
                }
            });
        });
        content_group.add_action(act_delete);
        view_paned.insert_action_group("content", content_group);

        // Main group for creating assets
        SimpleActionGroup content_new_group = new SimpleActionGroup();
        SimpleAction act_new_folder = new SimpleAction("folder", null);
        act_new_folder.activate.connect(() =>
        {
            try {
                File new_folder = IO.file_from_resource(get_selected_path(), "Untitled", true);
                new_folder.make_directory();
            } catch(Error e) {
                Logger.log_warning("Can't make directory: %s", e.message);
            }
        });
        content_new_group.add_action(act_new_folder);
        SimpleAction act_new_actor = new SimpleAction("actor", null);
        act_new_actor.activate.connect(() =>
        {
            file_new(AssetType.ACTOR);
        });
        content_new_group.add_action(act_new_actor);
        SimpleAction act_new_map = new SimpleAction("map", null);
        act_new_map.activate.connect(() =>
        {
            file_new(AssetType.MAP);
        });
        content_new_group.add_action(act_new_map);
        SimpleAction act_new_sprite = new SimpleAction("sprite", null);
        act_new_sprite.activate.connect(() =>
        {
            file_new(AssetType.SPRITE);
        });
        content_new_group.add_action(act_new_sprite);
        SimpleAction act_new_tileset = new SimpleAction("tileset", null);
        act_new_tileset.activate.connect(() =>
        {
            file_new(AssetType.TILESET);
        });
        content_new_group.add_action(act_new_tileset);
        view_paned.insert_action_group("new", content_new_group);
    }

    /*
     * Creates and attaches menus to widgets
     */
    private void init_menus()
    {
        GLib.Menu content_model = new GLib.Menu();
        content_model.append("Import File", "content.import");
        content_model.append("New Folder", "new.folder");

        GLib.Menu new_sub = new GLib.Menu();
        new_sub.append("Actor", "new.actor");
        new_sub.append("Map", "new.map");
        new_sub.append("Sprite", "new.sprite");
        new_sub.append("Tileset", "new.tileset");
        content_model.append_submenu("New", new_sub);

        GLib.Menu content_file_model = new GLib.Menu();
        content_file_model.append("Open", "content.open");
        content_file_model.append("Rename", "content.rename");
        content_file_model.append("Delete", "content.delete");

        content_menu = new Gtk.Menu.from_model(content_model);
        content_menu.attach_to_widget(view_paned, null);
        content_file_menu = new Gtk.Menu.from_model(content_file_model);
        content_file_menu.attach_to_widget(view_paned, null);
    }

    private void build_view_filter(TreePath? path = null)
    {
        view_filter = new TreeModelFilter(file_data, path);
        // TODO: This will need changing when the view becomes switchable
        icon_view.set_model(view_filter);
    }

    /*
     * This function handles changes to the files in the project.
     * It is responsible for adding/removing files from the view, as well as
     * updating the asset registry.
     */
    private void monitor_event(File src, File? dest, FileMonitorEvent type)
    {
        TreeIter? file_iter = get_iter_for_file(src);

        switch(type) {
            case FileMonitorEvent.RENAMED:
                if(src.query_file_type(FileQueryInfoFlags.NONE) == FileType.DIRECTORY) {
                    FileMonitor? m = null;
                    project_monitors.unset(src, out m);
                    if(m != null)
                        project_monitors.set(dest, m);
                }

                if(file_iter == null)
                    break;
                file_data.set(file_iter, ProjectTreeColumn.FILENAME,  dest.get_basename());
                file_data.set(file_iter, ProjectTreeColumn.URI,       dest.get_uri());
                ResourceEntry prev_entry = new ResourceEntry(src);
                ResourceEntry next_entry = new ResourceEntry(dest);
                DF.IO.move_resource(prev_entry.path, prev_entry.name, next_entry.path, next_entry.name);
                break;

            case FileMonitorEvent.CREATED:
            case FileMonitorEvent.MOVED_IN:
                if(file_iter == null) {
                    try {
                        TreeIter? parent_iter = get_iter_for_file(src.get_parent());
                        add_project_file(src.query_info("standard::*", FileQueryInfoFlags.NONE), parent_iter);
                    } catch(Error e) {
                        Logger.log_warning("Can't get file info: %s", e.message);
                    }
                }
                break;

            case FileMonitorEvent.DELETED:
                if(src.query_file_type(FileQueryInfoFlags.NONE) == FileType.DIRECTORY) {
                    FileMonitor? m = null;
                    project_monitors.unset(src, out m);
                    if(m != null) {
                        m.cancel();
                        m.unref();
                    }

                    FileInfo info;
                    try {
                        info = src.query_info ("standard::*", 0);
                        FileEnumerator enumerator = src.enumerate_children (FileAttribute.STANDARD_NAME, 0);

                        FileInfo file_info;
                        while ((file_info = enumerator.next_file ()) != null) {
                            monitor_event(enumerator.get_child(file_info), null, FileMonitorEvent.DELETED);
                        }
                    } catch (Error e) {
                        DF.Logger.log_error("Error reading project directory: %s\n", e.message);
                    }
                }
                ResourceEntry deleted_entry = new ResourceEntry(src);
                DF.IO.delete_resource(deleted_entry.path, deleted_entry.name);
                if(file_iter != null) {
                    file_data.remove(ref file_iter);
                }
                break;

            case FileMonitorEvent.MOVED_OUT:
                if(dest.query_file_type(FileQueryInfoFlags.NONE) == FileType.DIRECTORY) {
                    FileMonitor? m = null;
                    project_monitors.unset(src, out m);
                    if(m != null) {
                        m.cancel();
                        m.unref();
                    }

                    FileInfo info;
                    try {
                        info = dest.query_info ("standard::*", 0);
                        FileEnumerator enumerator = dest.enumerate_children (FileAttribute.STANDARD_NAME, 0);

                        FileInfo file_info;
                        while ((file_info = enumerator.next_file ()) != null) {
                            File next_file = enumerator.get_child(file_info);
                            string new_path = src.get_path().concat("/").concat(dest.get_relative_path(next_file));
                            monitor_event(File.new_for_path(new_path), next_file, FileMonitorEvent.MOVED_OUT);
                        }
                    } catch (Error e) {
                        DF.Logger.log_error("Error reading project directory: %s\n", e.message);
                    }
                }
                ResourceEntry moved_entry = new ResourceEntry(src);
                ResourceEntry new_entry = new ResourceEntry(dest);
                if(!IO.file_is_content(dest)) {
                    IO.delete_resource(moved_entry.path, moved_entry.name);
                } else {
                    IO.move_resource(moved_entry.path, moved_entry.name, new_entry.path, new_entry.name);
                }
                file_data.remove(ref file_iter);
                break;
        }
    }

    private void add_project_directory(File f, TreeIter? parent_iter)
    {
        TreeIter iter;
        file_data.append(out iter, parent_iter);
        try {
            FileMonitor? m = f.monitor_directory(FileMonitorFlags.WATCH_MOVES);
            m.set_rate_limit(1000);
            m.changed.connect(monitor_event);
            if(m != null)
                project_monitors.set(f, m);
        } catch(IOError e) {
            Logger.log_warning("Can't watch content directory: %s");
        }

        file_data.set(iter, ProjectTreeColumn.FILENAME, f.get_basename(), ProjectTreeColumn.URI, f.get_uri(), ProjectTreeColumn.SIDEBAR_FILTER, true, ProjectTreeColumn.ICON_NAME, "folder");
        FileInfo info;
        try {
            info = f.query_info ("standard::*", 0);
            FileEnumerator enumerator = f.enumerate_children (FileAttribute.STANDARD_NAME, 0);

            FileInfo file_info;
            while ((file_info = enumerator.next_file ()) != null) {
                add_project_file(file_info, iter);
            }
        } catch (Error e) {
            DF.Logger.log_error("Error reading project directory: %s\n", e.message);
            return;
        }
    }

    /*
     * This function adds a new file to the treestore representing the content
     * directory
     */
    private void add_project_file(FileInfo info, TreeIter? parent_iter)
    {
        string filename = info.get_name();
        if(info.get_is_hidden() || filename[0] == '.')
            return;
        if(info.get_file_type () == FileType.DIRECTORY && parent_iter != null) {
            string uri;
            file_data.get(parent_iter, ProjectTreeColumn.URI, out uri);
            File f = File.new_for_uri(uri);

            add_project_directory(f.resolve_relative_path (info.get_name ()), parent_iter);
            return;
        }

        TreeIter temp_iter;
        file_data.append(out temp_iter, parent_iter);

        string name = "text-x-generic";
        string extension = DF.IO.get_extension(filename);
        // TODO: Use custom non-placeholder icons
        switch(extension) {
            case "png":
                name = "image-x-generic";
                break;
            case "actr":
                name = "avatar-default-symbolic";
                break;
            case "map":
                name = "mark-location";
                break;
        }

        string parent_uri;
        if(parent_iter == null)
            parent_uri = IO.get_content_directory().get_uri();
        else
            file_data.get(parent_iter, ProjectTreeColumn.URI, out parent_uri);
        file_data.set(temp_iter, ProjectTreeColumn.FILENAME, info.get_name(), ProjectTreeColumn.URI, parent_uri + "/" + info.get_name(), ProjectTreeColumn.SIDEBAR_FILTER, false, ProjectTreeColumn.ICON_NAME, name);
    }

    private void iconview_cell_data(CellLayout layout, CellRenderer cell, TreeModel model, TreeIter iter)
    {
        if(cell is CellRendererPixbuf) {
            string icon_name = "folder";
            model.get(iter, ProjectTreeColumn.ICON_NAME, out icon_name);
            ((CellRendererPixbuf)cell).icon_name = icon_name;
            ((CellRendererPixbuf)cell).stock_size = icon_size;
        } else if(cell is CellRendererText) {
            string title = "";
            model.get(iter, ProjectTreeColumn.FILENAME, out title);
            ((CellRendererText)cell).text = title;
        }
    }

    private TreeIter? get_iter_for_file(File infile)
    {
        TreeIter? search_iter = null;
        string uri = infile.get_uri();
        file_data.foreach((model, path, iter) =>
        {
            string? uri_present = null;
            file_data.get(iter, ProjectTreeColumn.URI, out uri_present);
            if(uri == uri_present) {
                search_iter = iter;
                return true;
            }
            return false;
        });
        return search_iter;
    }
}
