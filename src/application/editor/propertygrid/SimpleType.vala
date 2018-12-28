using DFGame.Core;
using Xml;

namespace DFGame.PropertyGrid {
    // Wraps a primitive type with special handling
    public class SimpleType : PropertyType {
        // The restrictions placed on this type
        public Restriction restriction { get; private set; default = null; }

        // The name of the underlying primitive type
        public override string primitive_name { get {
            if(restriction != null) {
                return restriction.base_name;
            }

            return name;
        } }

        public SimpleType (Xml.Node* node_dat) {
            name = "simpleType";
            type_name = node_dat->get_prop ("name");
            prepare (node_dat);
        }

        public override void prepare (Xml.Node* node_dat) {
            for (Xml.Node* node = node_dat->first_element_child ();
                node != null;
                node = node->next_element_sibling ()) {
                switch (node->name) {
                    case XSD_ANNOTATION:
                        annotation = new Annotation (node);
                    break;
                    case XSD_LIST:
                        // TODO
                    break;
                    case XSD_RESTRICTION:
                        restriction = new Restriction (node);
                    break;
                    case XSD_UNION:
                        // TODO
                    break;
                    default:
                        Logger.warn ("Unrecognized simpletype content %s", node->name);
                    break;
                }
            }
        }
    }
}
