using DFGame.Core;
using Gtk;
using Xml;

namespace DFGame {
    // Represents a property on an object
    public class PropertyType {
        public string name { get; private set; }

        public PropertyType (string type_name) {
            name = type_name;
        }

        public void prepare (void* node_dat) {
            for (Xml.Node* node = node_dat; node != null; node = node->next) {
                if (node->type == ElementType.ELEMENT_NODE && node->name == "attribute") {
                    props.set (node->get_prop ("name"), new PropertyType (node->get_prop ("type")));
                }
            }
        }
        public bool try_get_prop (out PropertyType prop, string name) {
            if (!props.has_key (name)) {
                prop = null;
                return false;
            }

            prop = props.get (name);

            return true;
        }

        private Gee.HashMap<string, PropertyType> props = new Gee.HashMap<string, PropertyType> ();
    }

    // Widget for viewing/editing objects with properties
    public class PropertyGrid : Bin {
        public signal void value_changed (string data);
        public IPropertyBuilder builder { get; set; }

        construct {
            properties_list = new ListBox ();
            properties_list.selection_mode = SelectionMode.NONE;
            properties_list.set_header_func (listbox_header_separator);

            this.add (properties_list);
        }

        // Loads type information from an XSD file
        public bool load_schema (Xml.Doc* doc) {
            if (doc == null || doc->children == null) {
                Logger.error ("Failed to read schema");
                return false;
            }

            for (var node = doc->children->children; node != null; node = node->next) {
                if (node->type == ElementType.ELEMENT_NODE) {
                    switch (node->name) {
                        case "complexType":
                            string name = node->get_prop ("name");
                            var prop = new PropertyType ("complexType");
                            prop.prepare (node->children);
                            types.set (name, prop);
                            break;
                        case "attribute":
                            // TODO: Parse and prepare an attribute property
                            break;
                    }
                }
            }

            return false;
        }
        public bool load_schema_from_resource (string path) {
            Xml.ParserCtxt parser = new Xml.ParserCtxt ();
            try {
                var schema = resources_lookup_data (path, ResourceLookupFlags.NONE);
                var doc = parser.read_memory ((char[])schema.get_data (), schema.length, "");
                return load_schema (doc);
            } catch (GLib.Error e) {
                Logger.error ("Failed to read schema from path %s", path);
            }

            return false;
        }
        // Loads properties and builds editing widgets based on the provided XML
        public bool load_data (string data) {
            Xml.ParserCtxt parser = new Xml.ParserCtxt ();
            var doc = parser.read_memory (data.to_utf8 (), data.length, "");

            if (doc == null) {
                Logger.error ("Failed to read xml data");
                return false;
            }

            for (var node = doc->children; node != null; node = node->next) {
                for (var attr = node->properties; attr != null; attr = attr->next) {
                    add_property (node->name, attr->name, node->get_prop (attr->name));
                }
            }

            show_all ();

            return false;
        }
        // Updates existing properties with new values in the provided XML
        public bool update_data (string data) {
            Xml.ParserCtxt parser = new Xml.ParserCtxt ();
            var doc = parser.read_memory (data.to_utf8 (), data.length, "");

            if (doc == null) {
                Logger.error ("Failed to read xml data");
                return false;
            }

            // Freeze notifications to avoid sending the new data back
            frozen = true;
            for (var node = doc->children; node != null; node = node->next) {
                for (var attr = node->properties; attr != null; attr = attr->next) {
                    update_property (node->name, attr->name, node->get_prop (attr->name));
                }
            }
            frozen = false;

            show_all ();

            return false;
        }

        // Adds a property editing widget for an object with the given property data
        public bool add_property (string owner, string name, string value) {
            PropertyType owner_type = types.get (owner);
            PropertyType prop_type;

            if (owner_type.try_get_prop (out prop_type, name)) {
                PropertyEditor editor = builder.create_editor (owner, name, prop_type, value);
                editor.value_changed.connect (on_update_value);
                editor.set_groups (label_group, control_group);
                properties_list.add (editor);
                widgets[name] = editor;
            }

            return false;
        }
        // Updates the value of a property
        public bool update_property (string owner, string name, string value) {
            PropertyEditor p = widgets.get (name);
            if (p != null) {
                p.current_value = value;
            }

            return false;
        }

        private void on_update_value (PropertyEditor editor, string value) {
            if (!frozen) {
                value_changed ("<%s %s=\"%s\"/>".printf (editor.owner_name, editor.property_name, value));
            }
        }

        private bool frozen = false;
        private ListBox properties_list;
        private SizeGroup label_group = new SizeGroup (SizeGroupMode.BOTH);
        private SizeGroup control_group = new SizeGroup (SizeGroupMode.BOTH);
        private Gee.HashMap<string, PropertyType> types = new Gee.HashMap<string, PropertyType> ();
        private Gee.HashMap<string, PropertyEditor> widgets = new Gee.HashMap<string, PropertyEditor> ();
    }

    public void listbox_header_separator (Gtk.ListBoxRow row, Gtk.ListBoxRow? brow) {
        if (brow != null && row.get_header () == null) {
            row.set_header (new Gtk.Separator (Gtk.Orientation.HORIZONTAL));
        }
    }
}
