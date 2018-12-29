using DFGame.Core;
using Gee;
using Xml;

namespace DFGame.PropertyGrid {
    // Holds additional XSD information about an element
    public class Annotation {
        // Help string for displaying in UI
        public string documentation { get; private set; }

        // The hints set in this annotation
        public HashMap<string, string> hints { get; default = new HashMap<string, string> (); }

        public Annotation (Xml.Node* node_dat) {
            for (Xml.Node* node = node_dat->first_element_child ();
                node != null;
                node = node->next_element_sibling ()) {
                switch (node->name) {
                    case XSD_DOCUMENTATION:
                        documentation = node->get_content ();
                    break;
                    case DF_HINT:
                        string key = node->get_prop ("key");
                        if (key != null) {
                            hints[key] = node->get_content ();
                        } else {
                            Logger.warn ("Hint \"%s\" has no key", node->get_content ());
                        }
                    break;
                    default:
                        Logger.warn ("Unrecognized annotation data %s", node->name);
                        break;
                }
            }
        }

        // Try to get the hint value set at key
        public bool try_get_hint (string key, out string hint) {
            hint = hints[key];

            return hint != null;
        }
    }
}
