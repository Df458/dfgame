using DFGame.PropertyGrid.Editors;
using Gtk;

namespace DFGame.PropertyGrid {
    /**
     * Represents a list of property editors in a {@link[PropertyGrid]}'s hierarchy
     */
    internal class PropertyList : Bin, IPropertyNode {
        /**
         * The builder used to construct this list's {@link[IPropertyEditor]} children
         */
        public IPropertyBuilder builder { get; set; }

        /**
         * Expands/collapses this widget's {@link[Expander]}
         */
        public bool expanded {
            get { return expander.expanded; }
            set { expander.expanded = value; }
        }

        /**
         * Whether or not this list should deliver change events
         */
        public bool frozen { get; set; }

        /**
         * The path to the property in xml.
         *
         * Each parent tag/attribute is represented by a string in the format tag/tag/tag/...
         */
        public string prop_path { get; set; }

        /**
         * Hint for a parent list's sorting
         */
        public uint sort_index { get; protected set; default = 0; }

        /**
         * The label displayed in the header of the list
         */
        public string label {
            get { return expander.label; }
            set { expander.label = value; }
        }

        /**
         * Called when the value of an editor changes
         *
         * @param data The xml data, as a string
         */
        public signal void value_changed (string data);

        construct {
            expander = new Expander (null);
            expander.label_fill = true;

            properties_list = new ListBox ();
            properties_list.selection_mode = SelectionMode.NONE;
            properties_list.set_header_func (listbox_header_separator);
            properties_list.set_sort_func (sort_properties);
            properties_list.margin_start = 12;

            expander.add (properties_list);
            this.add (expander);
        }

        public PropertyList (SizeGroup lg, SizeGroup cg, Attribute? attr = null) {
            label_group = lg;
            control_group = cg;

            if (attr != null) {
                attribute = attr;
                sort_index = attr.sort_index;
                expander.label = attr.display_name;
            }
        }

        /**
         * Adds a new property editor to the list
         *
         * @param owner_type The type to get the property from
         * @param name The name of the property
         * @param value The value of the property. If the value is null, the default value will be used.
         */
        public void add_property (PropertyType owner_type, string name, string? value) {
            Attribute attr;

            if (owner_type.try_get_attr (out attr, name)) {
                PropertyEditor editor = builder.create_editor (
                    owner_type.type_name,
                    attr,
                    value ?? attr.default_value ?? "");

                editor.value_changed.connect (on_update_value);
                editor.set_groups (label_group, control_group);
                properties_list.add (editor);
                widgets[name] = editor;
            }
        }

        /**
         * Updates the value of a property
         *
         * @param name The name of the property
         * @param value The new value of the property
         */
        public bool update_property (string name, string value) {
            PropertyEditor p = widgets.get (name);
            if (p != null) {
                p.current_value = value;
            }

            return false;
        }

        /**
         * Add a child PropertyList
         */
        public void add_child (PropertyList list) {
            properties_list.add (list);
        }

        private void on_update_value (PropertyEditor editor, string value) {
            if (!frozen) {
                var sb = new StringBuilder ();

                string[] tokens = prop_path.split ("/");
                for (int i = 0; i < tokens.length - 1; ++i) {
                    sb.append_printf ("<%s>", tokens[i]);
                }

                sb.append_printf ("<%s %s=\"%s\"/>".printf (editor.owner_name, editor.property_name, value));

                for (int i = 0; i < tokens.length - 1; ++i) {
                    sb.append_printf ("</%s>", tokens[i]);
                }

                value_changed (sb.str);
            }
        }

        /** 
         * Determine the ordering of two properties based on the ordering given to them
         */
        private int sort_properties (ListBoxRow row1, ListBoxRow row2) {
            IPropertyNode e1 = row1.get_child () as IPropertyNode;
            IPropertyNode e2 = row2.get_child () as IPropertyNode;

            if (e1 == null) {
                return -1;
            } else if (e2 == null) {
                return 1;
            }

            return (int)(e1.sort_index > e2.sort_index)
                   - (int)(e1.sort_index < e2.sort_index);
        }

        /**
         * Function to add a separator between rows in this widget's {@link[ListBox]}
         */
        private void listbox_header_separator (Gtk.ListBoxRow row, Gtk.ListBoxRow? brow) {
            if (brow != null && row.get_header () == null) {
                row.set_header (new Gtk.Separator (Gtk.Orientation.HORIZONTAL));
            }
        }

        private SizeGroup label_group;
        private SizeGroup control_group;
        private Expander expander;
        private ListBox properties_list;
        private Gee.HashMap<string, PropertyEditor> widgets = new Gee.HashMap<string, PropertyEditor> ();
        private Attribute attribute;
    }
}
