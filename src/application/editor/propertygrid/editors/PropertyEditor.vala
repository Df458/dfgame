using Gtk;

namespace DFGame {
    // Widget for editing properties
    public abstract class PropertyEditor : Box {
        public string display_name { get; private set; }
        public string property_name { get; private set; }
        public string owner_name { get; private set; }

        private string _current_value = null;
        public string current_value {
            get { return _current_value; }
            set {
                if (_current_value != value) {
                    _current_value = value;
                    set_value (_current_value);
                }
            }
        }

        public string default_value { get; private set; }

        public signal void value_changed (string data);
        /* private signal void set_value (string data); */

        construct {
            orientation = Orientation.HORIZONTAL;
            spacing = 6;
        }
        public PropertyEditor (string owner, string name, PropertyType prop, string value) {
            owner_name = owner;
            property_name = name;
            display_name = property_name.replace ("_", " ").strip ();
            display_name.data[0] = display_name.get (0).toupper ();

            name_label = new Label (display_name);
            name_label.halign = Align.END;
            pack_start (name_label);

            reset_button = new Button.from_icon_name ("edit-clear-symbolic");
            reset_button.get_style_context ().add_class ("flat");
            reset_button.clicked.connect (reset);
            reset_revealer = new Revealer ();
            reset_revealer.add (reset_button);
            pack_end (reset_revealer, false, false);

            widget = create_editor_widget ();

            current_value = value;
            default_value = current_value; // Set from current_value, in case the format has changed

            set_is_changed (false);

            widget.halign = Align.START;
            pack_start (widget, false, true);
        }

        // Adds the label/editing widget to the provided size groups
        internal void set_groups (SizeGroup label_group, SizeGroup widget_group) {
            label_group.add_widget (name_label);
            widget_group.add_widget (widget);
        }

        // Resets the value to the default
        public void reset () {
            current_value = default_value;
            set_is_changed (false);

            value_changed (current_value);
        }

        protected abstract Widget? create_editor_widget ();
        protected abstract void set_value (string data);

        protected void handle_value_changed (string value) {
            _current_value = value;
            set_is_changed (current_value != default_value);

            value_changed (value);
        }

        private void set_is_changed (bool changed) {
            name_label.attributes = new Pango.AttrList ();
            name_label.attributes.insert (Pango.attr_weight_new (changed ? Pango.Weight.BOLD : Pango.Weight.NORMAL));
            reset_revealer.reveal_child = changed;
        }

        private Revealer reset_revealer;
        private Button reset_button;
        private Label name_label;
        private Widget widget;
    }

    public class GenericPropertyEditor : PropertyEditor {
        public GenericPropertyEditor (string owner, string name, PropertyType prop, string value) {
            base (owner, name, prop, value);
        }
        protected override Widget? create_editor_widget () {
            entry = new Entry ();
            entry.activate.connect (() => { handle_value_changed (entry.text); });
            entry.focus_out_event.connect (() => { handle_value_changed (entry.text); return true; });
            return entry;
        }

        protected override void set_value (string value) {
            entry.text = value;
        }

        private Entry entry;
    }
}
