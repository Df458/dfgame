using DFGame.Core;
using DFGame.PropertyGrid.Editors;
using Gtk;
using Xml;

namespace DFGame.PropertyGrid {
    /**
     * Widget for viewing/editing objects with properties
     *
     * This is handled via XML serialization
     */
    public class PropertyGrid : Bin {
        /**
         * The builder used to construct this list's {@link[IPropertyEditor]) children
         */
        public IPropertyBuilder builder {
            get { return _builder; }
            set {
                if (_builder != value) {
                    _builder = value;

                    foreach (PropertyList list in lists.values) {
                        list.builder = _builder;
                    }
                }
            }
        }
        private IPropertyBuilder _builder;

        /**
         * Called when the value of an editor changes
         *
         * @param data The xml data, as a string
         */
        public signal void value_changed (string data);

        construct {
            properties_list = new PropertyList (label_group, control_group);
            properties_list.expanded = true;
            this.add (properties_list);
        }

        /**
         * Loads properties and builds editing widgets based on the provided XML
         */
        public bool load_data (string data) {
            Xml.ParserCtxt parser = new Xml.ParserCtxt ();
            var doc = parser.read_memory (data.to_utf8 (), data.length, "");

            if (doc == null) {
                Logger.error ("Failed to read xml data");
                return false;
            }

            if (doc->children != null) {
                PropertyType parent = types.get (doc->children->name);

                if (parent != null) {
                    properties_list.label = parent.type_name;
                    init_property_list (ref properties_list, parent, doc->children, parent.type_name);
                }
            }

            show_all ();

            return false;
        }

        /**
         * Loads type information from an XSD document
         */
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

        /**
         * Loads type information from an XSD file embedded as a GResource
         */
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

        /**
         * Get a {@link[PropertyType]} from the type name
         */
        public bool try_get_prop_type (string name, out PropertyType prop_type) {
            prop_type = types.get (name);
            return prop_type != null;
        }

        /**
         * Updates existing properties with new values in the provided XML
         */
        public bool update_data (string data) {
            Xml.ParserCtxt parser = new Xml.ParserCtxt ();
            var doc = parser.read_memory (data.to_utf8 (), data.length, "");

            if (doc == null) {
                Logger.error ("Failed to read xml data");
                return false;
            }

            // Freeze notifications to avoid sending the new data back
            set_frozen (true);


            if (doc->children != null) {
                var to_process = new Gee.ArrayQueue<Xml.Node*> ();
                var paths = new Gee.ArrayQueue<string> ();
                to_process.offer_head (doc->children);
                paths.offer_head (doc->children->name);

                while (!to_process.is_empty) {
                    Xml.Node* node = to_process.poll_head ();
                    string path = paths.poll_head ();

                    PropertyList list = lists[path];
                    if (list != null) {
                        for (var attr = node->properties; attr != null; attr = attr->next) {
                            list.update_property (attr->name, node->get_prop (attr->name));
                        }

                        for (var child = node->children; child != null; child = child->next) {
                            to_process.offer_head (child);
                            paths.offer_head (path + "/" + child->name);
                        }
                    }
                }
            }

            set_frozen (false);

            show_all ();

            return false;
        }

        /**
         * Create a new {@link[PropertyList]}, and initialize it with the provided XML
         */
        private PropertyList create_property_list (Attribute attr, Xml.Node* node, string path) {
            PropertyList list = new PropertyList (label_group, control_group, attr);

            init_property_list (ref list, attr.prop_type, node, path);

            return list;
        }

        /**
         * Initialize a {@link[PropertyList]} with the provided XML
         */
        private void init_property_list (ref PropertyList list, PropertyType prop_type, Xml.Node* node, string path) {
            list.builder = builder;

            foreach (string prop in prop_type.prop_names) {
                Attribute attr;
                if (prop_type.try_get_attr (out attr, prop)) {
                    PropertyType child_type = attr.prop_type;

                    if (child_type != null) {
                        if (child_type is SimpleType || child_type is PrimitiveType) {
                            list.add_property (prop_type, prop, node->get_prop (prop));
                        } else {
                            list.add_child (create_property_list (attr, node, path + "/" + prop));
                        }
                    }
                }
            }

            list.value_changed.connect ((s) => { value_changed (s); });
            list.prop_path = path;
            lists[path] = list;
        }

        /**
         * Freeze/unfreeze notifications on all child {@link[PropertyList]}s
         */
        private void set_frozen (bool frozen) {
            foreach (PropertyList list in lists.values) {
                list.frozen = frozen;
            }
        }

        private PropertyList properties_list;
        private SizeGroup label_group = new SizeGroup (SizeGroupMode.BOTH);
        private SizeGroup control_group = new SizeGroup (SizeGroupMode.BOTH);
        private Gee.HashMap<string, PropertyType> types = new Gee.HashMap<string, PropertyType> ();
        private Gee.HashMap<string, PropertyList> lists = new Gee.HashMap<string, PropertyList> ();
    }
}
