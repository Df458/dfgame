using DFGame.Core;
using Xml;

namespace DFGame.PropertyGrid {
    // Holds additional XSD information about an element
    public class Annotation {
        // Help string for displaying in UI
        public string documentation { get; private set; }

        public Annotation (Xml.Node* node_dat) {
            for (Xml.Node* node = node_dat->first_element_child ();
                node != null;
                node = node->next_element_sibling ()) {
                if (node->name == XSD_DOCUMENTATION) {
                    documentation = node->get_content ();
                } else {
                    Logger.warn ("Unrecognized annotation data %s", node->name);
                }
            }
        }
    }
}
