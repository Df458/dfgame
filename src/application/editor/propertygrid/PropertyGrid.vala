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
         * The builder used to construct this list's {@link Editors.PropertyEditor} children
         */
        public IPropertyBuilder builder {
            get { return _builder; }
            set {
                if (_builder != value) {
                    _builder = value;

                    if (properties_list != null) {
                        properties_list.builder = _builder;
                    }
                }
            }
        }
        private IPropertyBuilder _builder;

        /**
         * Text to display when the property grid is empty
         */
        public string placeholder_label {
            get { return _placeholder_label; }
            set {
                if (value != _placeholder_icon) {
                    _placeholder_icon = value;

                    if (empty_label != null) {
                        empty_label.label = value;
                    }
                }
            }
        }
        private string _placeholder_label;

        /**
         * Image to display when the property grid is empty
         */
        public string placeholder_icon {
            get { return _placeholder_icon; }
            set {
                if (value != _placeholder_icon) {
                    _placeholder_icon = value;

                    if (empty_icon != null) {
                        empty_icon.icon_name = value;
                    }
                }
            }
        }
        private string _placeholder_icon;

        /**
         * Called when the value of an editor changes
         *
         * @param data The xml data
         */
        public signal void value_changed (Xml.Node* data);

        construct {
            property_stack = new Stack ();

            Box box = new Box (Orientation.VERTICAL, 6);
            box.valign = Align.CENTER;
            empty_icon = new Image ();
            empty_icon.icon_size = IconSize.DIALOG;
            empty_icon.sensitive = false;
            empty_label = new Label (null);
            empty_label.sensitive = false;
            box.add (empty_icon);
            box.add (empty_label);
            property_stack.add_named (box, EMPTY_NAME);

            add (property_stack);

            show_all ();

            property_stack.visible_child_name = EMPTY_NAME;
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
                Element elem = new TypeElement.with_type (types.get (doc->children->name));

                properties_list = new PropertyList (elem, doc->children, builder);
                properties_list.expanded = true;

                Widget? w = property_stack.get_child_by_name (PROPERTIES_NAME);
                if (w != null) {
                    property_stack.remove (w);
                }

                property_stack.add_named (properties_list, PROPERTIES_NAME);
                properties_list.value_changed.connect (on_list_value_changed);
            }

            show_all ();
            property_stack.visible_child_name = PROPERTIES_NAME;

            return false;
        }

        /**
         * Loads type information from an XSD document
         */
        public bool load_schema (Xml.Doc* doc, string? path = null) {
            if (doc == null || doc->children == null) {
                Logger.error ("Failed to read schema");
                return false;
            }


            var loaded = new Gee.HashSet<string> ();
            var to_load = new Gee.ArrayQueue<Xml.Doc*> ();

            if (path != null) {
                loaded.add (path);
            }
            to_load.add (doc);

            while (!to_load.is_empty) {
                Xml.Doc* current_doc = to_load.poll ();

                for (var node = current_doc->children->first_element_child ();
                        node != null;
                        node = node->next_element_sibling ()) {
                    PropertyType prop = null;

                    switch (node->name) {
                        case XSD_COMPLEX_TYPE:
                            prop = new PropertyType.complex (node);
                        break;
                        case XSD_SIMPLE_TYPE:
                            prop = new SimpleType (node);
                        break;
                        case XSD_INCLUDE:
                            string include_path = node->get_prop ("schemaLocation");
                            if (include_path != null && loaded.add (include_path)) {
                                Xml.Doc* include_doc = load_doc_from_uri (include_path);
                                if (doc != null) {
                                    to_load.add (include_doc);
                                }
                            }
                        break;
                    }

                    if (prop != null) {
                        types.set (prop.type_name, prop);
                    }
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
            Xml.Doc* doc = load_doc_from_uri ("resource://%s".printf (path));
            if (doc != null) {
                return load_schema (doc);
            }

            return false;
        }

        /**
         * Get a {@link PropertyType} from the type name
         */
        public bool try_get_prop_type (string name, out PropertyType prop_type) {
            prop_type = types.get (name);
            return prop_type != null;
        }

        /**
         * Load an XML document from the provided URI
         */
        private Xml.Doc* load_doc_from_uri (string uri) {
            Xml.ParserCtxt parser = new Xml.ParserCtxt ();
            try {
                if (uri.index_of ("file://") == 0) {
                    return parser.read_file (uri.substring (7));
                } else if (uri.index_of ("resource://") == 0) {
                    var schema = resources_lookup_data (uri.substring (11), ResourceLookupFlags.NONE);

                    if (schema != null) {
                        return parser.read_memory ((char[])schema.get_data (), schema.length, "");
                    } else {
                        Logger.error ("Failed to locate resource: %s", uri);
                    }
                }
            } catch (GLib.Error e) {
                Logger.error ("Failed to read schema from uri %s", uri);
            }

            return null;
        }

        /**
         * Updates existing properties with new values in the provided XML
         */
        public void update_data (string data) {
            Xml.ParserCtxt parser = new Xml.ParserCtxt ();
            var doc = parser.read_memory (data.to_utf8 (), data.length, "");

            if (doc == null) {
                Logger.error ("Failed to read xml data");
                return;
            }

            // Freeze notifications to avoid sending the new data back
            set_frozen (true);

            if (doc->children != null
                && properties_list != null
                && properties_list.element.name == doc->children->name) {
                properties_list.update_data (doc->children);
            }

            set_frozen (false);

            show_all ();
        }

        private void on_list_value_changed (Xml.Node* node) {
            value_changed (node);
        }

        /**
         * Freeze/unfreeze notifications on all child {@link PropertyList}s
         */
        private void set_frozen (bool frozen) {
            if (properties_list != null) {
                properties_list.frozen = frozen;
            }
        }

        private PropertyList properties_list;
        private Gee.HashMap<string, PropertyType> types = new Gee.HashMap<string, PropertyType> ();

        private Stack property_stack;
        private Image empty_icon;
        private Label empty_label;

        private const string EMPTY_NAME = "empty";
        private const string PROPERTIES_NAME = "properties";
    }
}
