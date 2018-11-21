using Gtk;
using Xml;

namespace DFGame {
    // Represents a property on an object
    public class PropertyType {
        public string name { get; private set; }

        public PropertyType(string type_name) {
            name = type_name;
        }

        public void prepare(void* node_dat) {
            for(Xml.Node* node = node_dat; node != null; node = node->next) {
                if(node->type == ElementType.ELEMENT_NODE && node->name == "attribute") {
                    props.set(node->get_prop("name"), new PropertyType(node->get_prop("type")));
                }
            }
        }
        public bool try_get_prop(out PropertyType prop, string name) {
            if(!props.has_key(name)) {
                prop = null;
                return false;
            }

            prop = props.get(name);

            return true;
        }

        private Gee.HashMap<string, PropertyType> props = new Gee.HashMap<string, PropertyType>();
    }

    // Widget for editing properties
    public class PropertyEditor : Box {
        public string display_name { get; private set; }
        public string property_name { get; private set; }
        public string owner_name { get; private set; }

        private string _current_value = null;
        public string current_value {
            get { return _current_value; }
            set {
                if(_current_value != value) {
                    _current_value = value;
                    set_value(_current_value);
                }
            }
        }

        public string default_value { get; private set; }

        public signal void value_changed(string data);
        private signal void set_value(string data);

        construct {
            orientation = Orientation.HORIZONTAL;
            spacing = 6;
        }
        public PropertyEditor(string owner, string name, PropertyType prop, string value) {
            owner_name = owner;
            property_name = name;
            display_name = property_name.replace("_", " ").strip();
            display_name.data[0] = display_name.get(0).toupper();

            name_label = new Label(display_name);
            name_label.halign = Align.END;
            pack_start(name_label);

            reset_button = new Button.from_icon_name("edit-clear-symbolic");
            reset_button.get_style_context().add_class("flat");
            reset_button.clicked.connect(reset);
            reset_revealer = new Revealer();
            reset_revealer.add(reset_button);
            pack_end(reset_revealer, false, false);

            switch(prop.name) {
                case "xs:float":
                    var spin = new SpinButton.with_range(-1000000, 1000000, 1);
                    spin.digits = 2;
                    spin.value_changed.connect(() => { handle_value_changed(spin.text); });
                    set_value.connect(set_value_float);
                    widget = spin;
                break;
                case "xs:Boolean":
                    var check = new CheckButton();
                    check.toggled.connect(() => { handle_value_changed(check.active ? "true" : "false"); });
                    set_value.connect(set_value_bool);
                    widget = check;
                break;
                case "df:color4":
                    var color = new ColorButton();
                    color.show_editor = true;
                    color.color_set.connect((s) => { handle_value_changed("#"
                        + write_color(color.rgba.red)
                        + write_color(color.rgba.green)
                        + write_color(color.rgba.blue)
                        + write_color(color.rgba.alpha)); });
                    set_value.connect(set_value_color);
                    widget = color;
                break;
                default:
                    var entry = new Entry();
                    entry.activate.connect(() => { handle_value_changed(entry.text); });
                    entry.focus_out_event.connect(() => { handle_value_changed(entry.text); return true; });
                    set_value.connect(set_value_default);
                    widget = entry;
                break;
            }

            current_value = value;
            default_value = current_value; // Set from current_value, in case the format has changed

            set_is_changed(false);

            widget.halign = Align.START;
            pack_start(widget, false, true);
        }

        // Adds the label/editing widget to the provided size groups
        internal void set_groups(SizeGroup label_group, SizeGroup widget_group) {
            label_group.add_widget(name_label);
            widget_group.add_widget(widget);
        }

        // Resets the value to the default
        public void reset() {
            current_value = default_value;
            set_is_changed(false);

            value_changed(current_value);
        }

        private void handle_value_changed(string value) {
            _current_value = value;
            set_is_changed(current_value != default_value);

            value_changed(value);
        }

        private void set_value_float(string value) {
            var spin = widget as SpinButton;
            spin.value = double.parse(value);
        }

        private void set_value_bool(string value) {
            var check = widget as CheckButton;
            check.active = value.down() == "true";
        }

        private void set_value_color(string value) {
            var color = widget as ColorButton;

            var rgba = Gdk.RGBA() {
                red = parse_color(value, 1),
                    green = parse_color(value, 3),
                    blue = parse_color(value, 5),
                    alpha = parse_color(value, 7)
            };
            color.rgba = rgba;
        }

        private void set_value_default(string value) {
            var entry = widget as Entry;
            entry.text = value;
        }

        private void set_is_changed(bool changed) {
            name_label.attributes = new Pango.AttrList();
            name_label.attributes.insert(Pango.attr_weight_new(changed ? Pango.Weight.BOLD : Pango.Weight.NORMAL));
            reset_revealer.reveal_child = changed;
        }

        private Revealer reset_revealer;
        private Button reset_button;
        private Label name_label;
        private Widget widget;
    }

    // Widget for viewing/editing objects with properties
    public class PropertyGrid : Bin {
        public signal void value_changed(string data);

        construct {
            properties_list = new ListBox();
            properties_list.selection_mode = SelectionMode.NONE;
            properties_list.set_header_func(listbox_header_separator);

            this.add(properties_list);
        }

        // Loads type information from an XSD file
        public bool load_schema(Xml.Doc* doc) {
            if(doc == null || doc->children == null) {
                // TODO Error
                return false;
            }

            for(var node = doc->children->children; node != null; node = node->next) {
                if(node->type == ElementType.ELEMENT_NODE) {
                    switch(node->name) {
                        case "complexType":
                            string name = node->get_prop("name");
                            var prop = new PropertyType("complexType");
                            prop.prepare(node->children);
                            types.set(name, prop);
                            break;
                        case "attribute":
                            // TODO: Parse and prepare an attribute property
                            break;
                    }
                }
            }

            return false;
        }
        public bool load_schema_from_resource(string path) {
            Xml.ParserCtxt parser = new Xml.ParserCtxt();
            try {
                var schema = resources_lookup_data(path, ResourceLookupFlags.NONE);
                var doc = parser.read_memory((char[])schema.get_data(), schema.length, "");
                return load_schema(doc);
            } catch(GLib.Error e) {
                // TODO: Error
            }

            return false;
        }
        // Loads properties and builds editing widgets based on the provided XML
        public bool load_data(string data) {
            Xml.ParserCtxt parser = new Xml.ParserCtxt();
            var doc = parser.read_memory(data.to_utf8(), data.length, "");

            if(doc == null) {
                // TODO Error
                return false;
            }

            for(var node = doc->children; node != null; node = node->next) {
                for(var attr = node->properties; attr != null; attr = attr->next) {
                    add_property(node->name, attr->name, node->get_prop(attr->name));
                }
            }

            show_all();

            return false;
        }
        // Updates existing properties with new values in the provided XML
        public bool update_data(string data) {
            Xml.ParserCtxt parser = new Xml.ParserCtxt();
            var doc = parser.read_memory(data.to_utf8(), data.length, "");

            if(doc == null) {
                // TODO Error
                return false;
            }

            // Freeze notifications to avoid sending the new data back
            frozen = true;
            for(var node = doc->children; node != null; node = node->next) {
                for(var attr = node->properties; attr != null; attr = attr->next) {
                    update_property(node->name, attr->name, node->get_prop(attr->name));
                }
            }
            frozen = false;

            show_all();

            return false;
        }

        // Adds a property editing widget for an object with the given property data
        public bool add_property(string owner, string name, string value) {
            PropertyType owner_type = types.get(owner);
            PropertyType prop_type;

            if(owner_type.try_get_prop(out prop_type, name)) {
                PropertyEditor editor = new PropertyEditor(owner, name, prop_type, value);
                editor.value_changed.connect(on_update_value);
                editor.set_groups(label_group, control_group);
                properties_list.add(editor);
                widgets[name] = editor;
            }

            return false;
        }
        // Updates the value of a property
        public bool update_property(string owner, string name, string value) {
            PropertyEditor p = widgets.get(name);
            if(p != null) {
                p.current_value = value;
            }

            return false;
        }

        private void on_update_value(PropertyEditor editor, string value) {
            if(!frozen) {
                value_changed("<%s %s=\"%s\"/>".printf(editor.owner_name, editor.property_name, value));
            }
        }

        private bool frozen = false;
        private ListBox properties_list;
        private SizeGroup label_group = new SizeGroup(SizeGroupMode.BOTH);
        private SizeGroup control_group = new SizeGroup(SizeGroupMode.BOTH);
        private Gee.HashMap<string, PropertyType> types = new Gee.HashMap<string, PropertyType>();
        private Gee.HashMap<string, PropertyEditor> widgets = new Gee.HashMap<string, PropertyEditor>();
    }

    public string write_color(double value) {
        int v = (int)(value * 255);
        return v.to_string("%02x");
    }

    public double parse_color(string s, int offset) {
        int c = (s.get(offset).xdigit_value() * 16) + s.get(offset + 1).xdigit_value();

        return c / 255.0;
    }

    public void listbox_header_separator(Gtk.ListBoxRow row, Gtk.ListBoxRow? brow) {
        if(brow != null && row.get_header() == null) {
            row.set_header(new Gtk.Separator(Gtk.Orientation.HORIZONTAL));
        }
    }
}
