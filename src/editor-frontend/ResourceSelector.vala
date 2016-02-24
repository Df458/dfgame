using Gtk;
namespace DF {

// TODO: Update the way objects are selected to match the mockup
public class ResourceSelector : Box
{
    string  title;
    Gee.TreeSet<string> extensions;
    bool    multi_select;
    bool    can_cancel;

    // Structural Widgets
    Box response_box;

    // Content Widgets
    Label         title_label;
    SearchEntry   filter;
    ListBox       list;
    ResourceModel list_data;
    Button        select_button;
    Button        cancel_button;

    public ResourceSelector(string display_title, string? desired_extension = null, bool should_allow_multiple = false, bool show_cancel = false)
    {
        this.orientation = Orientation.VERTICAL;
        this.spacing = 6;
        this.margin = 12;
        this.title = display_title;
        this.extensions = new Gee.TreeSet<string>();
        if(desired_extension != null)
            this.extensions.add(desired_extension);
        this.multi_select = should_allow_multiple;
        this.can_cancel = show_cancel;

        init_structure();
        init_content();
        connect_signals();
    }

    public ResourceEntry get_selected()
    {
        int index = list.get_selected_row().get_index();

        return list_data.get_item(index) as ResourceEntry;
    }

    public Gee.ArrayList<ResourceEntry> get_selected_full()
    {
        Gee.ArrayList<ResourceEntry> resource_list = new Gee.ArrayList<ResourceEntry>();

        // TODO: Get all rows and return their matching data
        Logger.stub();

        return resource_list;
    }

    public void add_extension(string ext)
    {
        this.extensions.add(ext);
    }

    public signal void respond(ResponseType response);

    private void init_structure()
    {
        response_box = new Box(Orientation.HORIZONTAL, 6);

        this.pack_end(response_box, false, false);
    }

    private void init_content()
    {
        title_label = new Label("<b>" + title + "</b>");
        filter = new SearchEntry();
        list = new ListBox();
        select_button = new Button.with_label("Select");
        cancel_button = new Button.with_label("Cancel");

        title_label.use_markup = true;
        generate_list();
        if(multi_select)
            list.selection_mode = SelectionMode.MULTIPLE;
        else
            list.selection_mode = SelectionMode.SINGLE;
        list.set_filter_func(list_filter);

        select_button.sensitive = false;

        this.pack_start(title_label, false, false);
        this.pack_start(filter, false, false);
        this.pack_start(list, true, true);
        response_box.pack_start(select_button, true, false);
        if(can_cancel)
            response_box.pack_end(cancel_button, true, false);
    }

    public void generate_list()
    {
        list_data = new ResourceModel();
        int add_position = iterate_directory(File.new_for_path(IO.get_path()));
        list.bind_model(list_data, create_list_widget);

        for(int i = 0; i < add_position; ++i)
            list.get_row_at_index(i).set_activatable(false);
        Image add_icon = new Image.from_icon_name("list-add-symbolic", IconSize.BUTTON);
        list.add(add_icon);
        ListBoxRow row = list.get_row_at_index(add_position);
        row.selectable = false;
        row.activatable = true;
        list.row_activated.connect(() =>
        {
            Logger.stub();
        });
    }

    private int iterate_directory(File f, int current_count = 0)
    {
        FileInfo info;
        int count = current_count;
        try {
            info = f.query_info ("standard::*", 0);
            FileEnumerator enumerator = f.enumerate_children (FileAttribute.STANDARD_NAME, 0);

            FileInfo file_info;
            while ((file_info = enumerator.next_file ()) != null) {
                string name = file_info.get_name();
                if(file_info.get_file_type () == FileType.DIRECTORY) {
                    count = iterate_directory(f.resolve_relative_path(name), count);
                    continue;
                }

                string ext = name.substring(name.last_index_of_char('.') + 1);
                if(extensions.size == 0 || extensions.contains(ext)) {
                    list_data.append(f.resolve_relative_path(name));
                    count++;
                }
            }
        } catch (Error e) {
            stderr.printf ("Error reading project directory: %s\n", e.message);
            return count;
        }

        return count;
    }

    private void connect_signals()
    {
        list.row_selected.connect(() => { select_button.set_sensitive(true); });
        select_button.clicked.connect(() => { this.respond(ResponseType.ACCEPT); });
        cancel_button.clicked.connect(() => { this.respond(ResponseType.CANCEL); });
        filter.search_changed.connect(() => { list.invalidate_filter(); });
    }

    private Widget create_list_widget(Object obj)
    {
        ResourceEntry entry = obj as ResourceEntry;
        Label label = new Label(entry.name);
        label.set_justify(Justification.LEFT);
        label.set_halign(Align.START);
        return label;
    }

    private bool list_filter(ListBoxRow row)
    {
        if(row.get_child() is Image)
            return true;
        Label label = row.get_child() as Label;

        return filter.get_text_length() <= 0 || label.get_text().down().contains(filter.get_text().down());
    }
}
}
