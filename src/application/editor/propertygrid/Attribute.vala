using DFGame.Core;
using Xml;

namespace DFGame.PropertyGrid {
    // Represents an XSD attribute
    public class Attribute : Object {
        // The name to display in UI and documentation
        public string display_name { get; private set; }

        // The name of the attribute
        public string name { get; private set; }

        // The type name of the attribute
        public string type_name { get; private set; }

        // The default value to use when nothing is provided
        public string default_value { get; private set; }

        // The attribute's property type
        public PropertyType prop_type { get; internal set; }

        // Annotation data
        public Annotation annotation { get; private set; }

        // The order that this should appear in the UI
        public uint sort_index { get; construct; }

        public Attribute (Xml.Node* node_dat, uint index = 0) {
            Object (sort_index: index);

            name = node_dat->get_prop ("name");
            display_name = node_dat->get_ns_prop (DF_DISPLAY_NAME, DF_NS) ?? make_display_name (name);
            type_name = node_dat->get_prop ("type");
            default_value = node_dat->get_prop ("default") ?? "";

            for (Xml.Node* node = node_dat->first_element_child ();
                node != null;
                node = node->next_element_sibling ()) {
                switch (node->name) {
                    case XSD_ANNOTATION:
                        annotation = new Annotation (node);
                    break;
                    default:
                        Logger.warn ("Unrecognized attribute content %s", node->name);
                    break;
                }
            }
        }

        // Try to get the hint value set at key
        public bool try_get_hint (string key, out string hint) {
            hint = null;
            if (annotation != null && annotation.try_get_hint (key, out hint)) {
                return true;
            }

            return prop_type.try_get_hint (key, out hint);
        }

        // Format a name for display
        private string make_display_name (string name) {
            // Split on underscores to form words
            string display_name = name.replace ("_", " ").strip ();

            // Capitalize the first character
            if (display_name.length > 0) {
                display_name.data[0] = display_name.get (0).toupper ();
            }

            return display_name;
        }
    }
}
