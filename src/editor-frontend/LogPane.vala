using Gtk;

namespace DF {

public static LogPane? active_pane = null;
public void log_to_pane(string file, uint line, Logger.LogLevel level, string message)
{
    if(active_pane != null)
        active_pane.write_log_to_file(file, line, level, message);
}

public class LogPane : Box
{
    private Box          action_box;
    private ComboBoxText log_selector;
    private CheckButton  hide_low_check;
    private SearchEntry  filter_entry;

    private TreeView        log_view;
    private TreeStore       log_data;
    private TreeModelFilter log_filter;

    private enum LogTreeColumn
    {
        TIME = 0,
        LOCATION,
        LEVEL,
        MESSAGE,
        COLUMN_COUNT
    }

    private const string LogLevels[] =
    {
        "Info",
        "Warning",
        "Error",
        "Fatal"
    };

    public LogPane()
    {
        this.orientation = Orientation.VERTICAL;
        init_structure();
        init_content();
        this.show_all();
    }

    public void write_log_to_file(string file, uint line, Logger.LogLevel level, string message)
    {
        TreeIter iter;
        log_data.append(out iter, null);
        log_data.set(iter, LogTreeColumn.TIME, new DateTime.now_local().to_string(), LogTreeColumn.LOCATION, "%s:%u".printf(file.offset(file.last_index_of_char('/') + 1), line), LogTreeColumn.LEVEL, LogLevels[level], LogTreeColumn.MESSAGE, message);
        // TODO: Write this to a file
    }

    public void set_active()
    {
        active_pane = this;
        Logger.register_log_handler(log_to_pane);
        Logger.log_info("Log pane is now active");
    }

    private void init_structure()
    {
        action_box = new Box(Orientation.HORIZONTAL, 12);
        action_box.margin = 6;

        this.add(action_box);
    }

    private void init_content()
    {
        filter_entry   = new SearchEntry();
        log_selector   = new ComboBoxText();
        hide_low_check = new CheckButton.with_label("Hide low-priority entries");

        log_data                    = new TreeStore(LogTreeColumn.COLUMN_COUNT, typeof(string), typeof(string), typeof(string), typeof(string));
        log_view                    = new TreeView.with_model(log_data);
        ScrolledWindow scroll       = new ScrolledWindow(null, null);
        TreeViewColumn col_time     = new Gtk.TreeViewColumn.with_attributes("Time", new CellRendererText(), "text", LogTreeColumn.TIME, null);
        TreeViewColumn col_location = new Gtk.TreeViewColumn.with_attributes("Location", new CellRendererText(), "text", LogTreeColumn.LOCATION, null);
        TreeViewColumn col_level    = new Gtk.TreeViewColumn.with_attributes("Level", new CellRendererText(), "text", LogTreeColumn.LEVEL, null);
        TreeViewColumn col_message  = new Gtk.TreeViewColumn.with_attributes("Message", new CellRendererText(), "text", LogTreeColumn.MESSAGE, null);

        log_data.set_sort_column_id(LogTreeColumn.MESSAGE, SortType.ASCENDING);
        log_view.insert_column(col_time, -1);
        log_view.insert_column(col_location, -1);
        log_view.insert_column(col_level, -1);
        log_view.insert_column(col_message, -1);

        filter_entry.placeholder_text = "Search\u2026";

        scroll.add(log_view);
        this.pack_end(scroll, true, true);

        action_box.add(filter_entry);
        action_box.add(hide_low_check);
        action_box.add(log_selector);
    }
}

}
