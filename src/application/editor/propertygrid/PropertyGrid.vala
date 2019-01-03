using DFGame.Core;
using DFGame.PropertyGrid.Editors;
using Gtk;
using Xml;

namespace DFGame.PropertyGrid {
    // Widget for viewing/editing objects with properties
    public class PropertyGrid : Bin {
        public signal void value_changed (string data);
        public IPropertyBuilder builder { get; set; }

        construct {
            properties_list = new ListBox ();
            properties_list.selection_mode = SelectionMode.NONE;
            properties_list.set_header_func (listbox_header_separator);
            properties_list.set_sort_func (sort_properties);

            this.add (properties_list);
        }

        // Loads type information from an XSD file
        public bool load_schema (Xml.Doc* doc) {
            if (doc == null || doc->children == null) {
                Logger.error ("Failed to read schema");
                return false;
            }

            for (var node = doc->children->first_element_child (); node != null; node = node->next_element_sibling ()) {
                PropertyType prop = null;

                switch (node->name) {
                    case XSD_COMPLEX_TYPE:
                        prop = new PropertyType.complex (node);
                    break;
                    case XSD_SIMPLE_TYPE:
                        prop = new SimpleType (node);
                    break;
                }

                if (prop != null) {
                    types.set (prop.type_name, prop);
                }
            }

            // Update property type references
            foreach (var prop in types.values) {
                prop.update_types (this);
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
                PropertyType parent = types.get (node->name);

                if (parent != null) {
                    foreach (string prop in parent.prop_names) {
                        add_property (parent.type_name, prop, node->get_prop (prop));
                    }
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
        // If value is null, the attribute's default value will be used
        public bool add_property (string owner, string name, string? value) {
            PropertyType owner_type = types.get (owner);
            Attribute attr;

            if (owner_type.try_get_attr (out attr, name)) {
                PropertyEditor editor = builder.create_editor (owner, attr, value ?? attr.default_value ?? "");
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

        // Get a PropertyType from the type name
        public bool try_get_prop_type (string name, out PropertyType prop_type) {
            prop_type = types.get (name);
            return prop_type != null;
        }

        private void on_update_value (PropertyEditor editor, string value) {
            if (!frozen) {
                value_changed ("<%s %s=\"%s\"/>".printf (editor.owner_name, editor.property_name, value));
            }
        }

        // Determine the ordering of two properties based on the ordering given to them
        private int sort_properties (ListBoxRow row1, ListBoxRow row2) {
            PropertyEditor e1 = row1.get_child () as PropertyEditor;
            PropertyEditor e2 = row2.get_child () as PropertyEditor;

            if (e1 == null) {
                return -1;
            } else if (e1 == null) {
                return 1;
            }

            return (int)(e1.attribute.sort_index > e2.attribute.sort_index)
                   - (int)(e1.attribute.sort_index < e2.attribute.sort_index);
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
