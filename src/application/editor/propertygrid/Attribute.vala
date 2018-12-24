using DFGame.Core;
using Xml;

namespace DFGame.PropertyGrid {
    // Represents an XSD attribute
    public class Attribute {
        // The name to display in UI and documentation
        public string display_name { get; private set; }

        // The name of the attribute
        public string name { get; private set; }

        // The type name of the attribute
        public string type_name { get; private set; }

        // The attribute's property type
        public PropertyType prop_type { get; internal set; }

        // Annotation data
        public Annotation annotation { get; private set; }

        public Attribute (Xml.Node* node_dat) {
            name = node_dat->get_prop ("name");
            display_name = node_dat->get_ns_prop (DF_DISPLAY_NAME, DF_NS) ?? make_display_name (name);
            type_name = node_dat->get_prop ("type");

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
