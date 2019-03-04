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
         * Called when the value of an editor changes
         *
         * @param data The xml data
         */
        public signal void value_changed (Xml.Node* data);

        construct {
            property_stack = new Stack ();

            Box box = new Box (Orientation.VERTICAL, 6);
            empty_icon = new Image ();
            empty_label = new Label ("Test");
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
         * Get a {@link PropertyType} from the type name
         */
        public bool try_get_prop_type (string name, out PropertyType prop_type) {
            prop_type = types.get (name);
            return prop_type != null;
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
