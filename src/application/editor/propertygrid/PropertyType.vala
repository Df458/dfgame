using DFGame.Core;
using Xml;

namespace DFGame.PropertyGrid {
    // Represents a property on an object
    public class PropertyType {
        public string name { get; protected set; }
        public string type_name { get; protected set; }

        // The name of the underlying primitive type
        public virtual string primitive_name { get { return name; } }

        public Annotation annotation { get; protected set; }

        public PropertyType.complex (Xml.Node* node_dat) {
            name = "complexType";
            type_name = node_dat->get_prop ("name");
            prepare (node_dat);
        }

        public virtual void prepare (Xml.Node* node_dat) {
            for (Xml.Node* node = node_dat->first_element_child ();
                node != null;
                node = node->next_element_sibling ()) {
                switch (node->name) {
                    case XSD_ANNOTATION:
                        annotation = new Annotation (node);
                    break;
                    case XSD_ATTRIBUTE:
                        Attribute attr = new Attribute (node);
                        attrs.set (attr.name, attr);
                    break;
                    default:
                        Logger.warn ("Unrecognized property content %s", node->name);
                    break;
                }
            }
        }

        // Try to get an attribute from its name
        public bool try_get_attr (out Attribute attr, string name) {
            attr = attrs.get (name);

            return attr != null;
        }

        // Update the type data on attributes from the given property grid's registered data
        public void update_types (PropertyGrid props) {
            foreach (var attr in attrs.values) {
                PropertyType prop_type;
                if (props.try_get_prop_type (attr.type_name, out prop_type)) {
                    attr.prop_type = prop_type;
                } else {
                    attr.prop_type = new PrimitiveType (attr.type_name);
                }
            }
        }

        // All attributes on this property
        private Gee.HashMap<string, Attribute> attrs = new Gee.HashMap<string, Attribute> ();
    }

    // A PropertyType with no special information; used for built-in types
    public class PrimitiveType : PropertyType {
        public PrimitiveType (string in_name) {
            name = in_name;
            type_name = in_name;
        }
    }
}
