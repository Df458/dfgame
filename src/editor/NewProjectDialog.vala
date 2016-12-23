using Gtk;

public class NewProjectDialog
{
    Dialog dialog;
    FileChooserDialog fc_dialog;

    Grid main_grid;

    Label name_label;
    Label location_label;
    Entry name_entry;
    Entry location_entry;
    EntryBuffer name_buffer;
    EntryBuffer location_buffer;

    public NewProjectDialog(Window? parent, string default_path)
    {
        dialog = new Dialog.with_buttons("Create a New Project", parent, DialogFlags.MODAL | DialogFlags.DESTROY_WITH_PARENT, "Cancel", ResponseType.REJECT, "Create", ResponseType.ACCEPT, null);
        dialog.set_response_sensitive(1, true);
        Button button_create = (Button)dialog.get_widget_for_response(ResponseType.ACCEPT);
        button_create.get_style_context().add_class(STYLE_CLASS_SUGGESTED_ACTION);
        button_create.sensitive = false;
        fc_dialog = new FileChooserDialog("Select a location for your project", dialog, FileChooserAction.SELECT_FOLDER, "Cancel", ResponseType.REJECT, "Select", ResponseType.ACCEPT, null);
        fc_dialog.set_transient_for(dialog);

        main_grid = new Grid();
        main_grid.margin_top = 12;
        main_grid.margin_bottom = 12;
        main_grid.margin_start = 18;
        main_grid.margin_end = 18;
        main_grid.set_row_spacing(6);
        main_grid.set_column_spacing(12);

        name_label = new Gtk.Label("Project Name");
        location_label = new Gtk.Label("Project Location");
        name_buffer = new EntryBuffer();
        location_buffer = new EntryBuffer();
        name_entry = new Gtk.Entry.with_buffer(name_buffer);
        location_entry = new Gtk.Entry.with_buffer(location_buffer);

        name_label.justify = Justification.RIGHT;
        location_label.justify = Justification.RIGHT;
        name_entry.secondary_icon_tooltip_text = "This field is required";
        name_entry.secondary_icon_sensitive = false;
        location_entry.primary_icon_name = "document-open-symbolic";
        location_entry.primary_icon_tooltip_text = "Choose a folder\u2026";
        location_entry.secondary_icon_tooltip_text = "This field is required";
        location_entry.secondary_icon_sensitive = false;
        location_entry.text = default_path;

        main_grid.attach(name_label, 0, 0);
        main_grid.attach(location_label, 0, 1);
        main_grid.attach(name_entry, 1, 0);
        main_grid.attach(location_entry, 1, 1);
        dialog.get_content_area().pack_start(main_grid, true, true);

        name_buffer.inserted_text.connect(() => {
            if(name_entry.text_length > 0 && location_entry.text_length > 0)
                button_create.sensitive = true;
            name_entry.secondary_icon_name = "";
        });
        name_buffer.deleted_text.connect(() => {
            if(name_entry.text_length == 0) {
                button_create.sensitive = false;
                name_entry.secondary_icon_name = "dialog-warning-symbolic";
            }
        });
        location_buffer.inserted_text.connect(() => {
            if(name_entry.text_length > 0 && location_entry.text_length > 0)
                button_create.sensitive = true;
            location_entry.secondary_icon_name = "";
        });
        location_buffer.deleted_text.connect(() => {
            if(location_entry.text_length == 0) {
                button_create.sensitive = false;
                location_entry.secondary_icon_name = "dialog-warning-symbolic";
                }
        });
        location_entry.icon_press.connect(() => {
            select_base_folder();
        });
        dialog.response.connect(respond);
        fc_dialog.response.connect((id) => {
            if(id == ResponseType.ACCEPT)
                location_entry.text = fc_dialog.get_file().get_path();
            fc_dialog.hide();
        });
    }

    public void display()
    {
        dialog.show_all();
    }

    public void respond(int response_id)
    {
        if(response_id == ResponseType.CLOSE)
            return;
        if(response_id == ResponseType.ACCEPT)
            response_selected(true, location_entry.text + "/" + name_entry.text);
        else
            response_selected(false);
        dialog.destroy();
    }

    public void destroy()
    {
        dialog.destroy();
    }

    public signal void response_selected(bool created, string? path = null);


    private void select_base_folder()
    {
        fc_dialog.show_all();
        fc_dialog.run();
    }
}
