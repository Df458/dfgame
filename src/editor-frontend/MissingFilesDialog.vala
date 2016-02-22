using Gtk;
using DF.IO;

public class MissingFilesDialog : Dialog
{
    private Label message_label;

    private TreeView           missing_list;
    private TreeStore          missing_data;
    private ScrolledWindow     missing_scroll;
    private CellRendererToggle toggle_renderer;
    private TreeViewColumn     col_keep;

    private enum MissingTreeColumn
    {
        ID = 0,
        SRCPATH,
        DESTDISP,
        DESTNAME,
        DESTPATH,
        KEEP,
        COLUMN_COUNT
    }

    public MissingFilesDialog(Window win)
    {
        this.set_transient_for(win);
        this.window_position = WindowPosition.CENTER;
        this.set_default_size(800, 600);
        init_content();
        connect_signals();
        populate_data();
        this.show_all();
    }

    public void apply()
    {
        missing_data.foreach(apply_iter);
    }

    private void init_content()
    {
        message_label  = new Label("Several game assets were either moved or deleted while the application was not running.\nPlease select their new locations or remove them.");
        missing_data   = new TreeStore(MissingTreeColumn.COLUMN_COUNT, typeof(int), typeof(string), typeof(string), typeof(string), typeof(string?), typeof(bool));
        missing_list   = new TreeView.with_model(missing_data);
        missing_scroll = new ScrolledWindow(null, null);

        missing_data.set_sort_column_id(MissingTreeColumn.ID, SortType.ASCENDING);
        toggle_renderer = new CellRendererToggle();

        col_keep                 = new TreeViewColumn.with_attributes("Keep",         toggle_renderer,          "active", MissingTreeColumn.KEEP);
        TreeViewColumn col_src   = new TreeViewColumn.with_attributes("Missing File", new CellRendererText(),   "text",   MissingTreeColumn.SRCPATH);
        TreeViewColumn col_dest  = new TreeViewColumn.with_attributes("Replacement",  new CellRendererText(),   "text",   MissingTreeColumn.DESTDISP, "sensitive", MissingTreeColumn.KEEP);
        missing_list.append_column(col_keep);
        missing_list.append_column(col_src);
        missing_list.append_column(col_dest);

        this.add_buttons("Confirm", 0, "Exit", 1);
        this.set_default_response(0);

        missing_scroll.add(missing_list);
        this.get_content_area().margin_top    = 18;
        this.get_content_area().margin_bottom = 18;
        this.get_content_area().spacing       = 12;
        this.get_content_area().add(message_label);
        this.get_content_area().pack_start(missing_scroll, true, true);
    }

    private void connect_signals()
    {
        toggle_renderer.toggled.connect((path) =>
        {
            bool keep;
            string? extension = null;
            string src = "";
            TreeIter? iter = null;
            TreePath tpath = new TreePath.from_string(path);
            missing_data.get_iter(out iter, tpath);
            if(iter == null)
                return;
            missing_data.get(iter, MissingTreeColumn.KEEP, out keep, MissingTreeColumn.SRCPATH, out src, -1);
            extension = get_extension(src);
            if(!keep) {
                Popover select_pop = new Popover(missing_list);
                Gdk.Rectangle rect;
                missing_list.get_cell_area(tpath, col_keep, out rect);
                select_pop.set_pointing_to(rect);
                ResourceSelector select_widget = new ResourceSelector("Select a replacement", extension);
                select_pop.add(select_widget);
                select_pop.modal = true;
                select_widget.respond.connect((response) =>
                {
                    select_pop.hide();
                    ResourceEntry entry = select_widget.get_selected();
                    string disp = "";
                    if(entry.path != null)
                        disp = entry.path + "/";
                    disp += entry.name;
                    missing_data.set(iter, MissingTreeColumn.KEEP, true, MissingTreeColumn.DESTDISP, disp, MissingTreeColumn.DESTNAME, entry.name, MissingTreeColumn.DESTPATH, entry.path, -1);
                });
                select_pop.show_all();
            } else {
                missing_data.set(iter, MissingTreeColumn.KEEP, false, MissingTreeColumn.DESTDISP, "None", -1);
            }
        });
    }

    private void populate_data()
    {
        int32 entry_count = get_failed_count();
        int32[] entries = get_failed_ids();

        for(int i = 0; i < entry_count; ++i) {
            TreeIter iter;
            string? path = get_path_from_id(entries[i], true);
            string srcpath = "";
            if(path != null)
                srcpath = path + "/";
            srcpath += get_name_from_id(entries[i], true);
            missing_data.insert_with_values(out iter, null, -1, MissingTreeColumn.ID, entries[i], MissingTreeColumn.SRCPATH, srcpath, MissingTreeColumn.KEEP, false, MissingTreeColumn.DESTDISP, "None", -1);
        }
    }

    private bool apply_iter(TreeModel model, TreePath path, TreeIter iter)
    {
        int id;
        bool keep;
        string? dest_path;
        string dest_name;
        model.get(iter, MissingTreeColumn.ID, out id, MissingTreeColumn.KEEP, out keep, MissingTreeColumn.DESTNAME, out dest_name, MissingTreeColumn.DESTPATH, out dest_path, -1);
        if(keep) {
            move_id(id, dest_path, dest_name);
        } else {
            delete_id(id);
        }
        return false;
    }
}
