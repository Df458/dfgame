using DFGame.Core;
using Gee;
using Xml;

namespace DFGame.PropertyGrid {
    /**
     * Represents a property on an object
     */
    public class PropertyType {
        public string name { get; protected set; }
        public string type_name { get; protected set; }

        /**
         * The list of all names associated with child attributes
         */
        public Gee.Collection<string> prop_names { owned get { return attrs.keys; } }

        /**
         * The list of all names associated with child elements
         */
        public Gee.Collection<string> element_names { owned get { return elements.keys; } }

        /**
         * The name of the underlying primitive type
         */
        public virtual string primitive_name { get { return name; } }

        /**
         * Provides additional metadata
         */
        public Annotation annotation { get; protected set; }

        public PropertyType.complex (Xml.Node* node_dat) {
            name = "complexType";
            type_name = node_dat->get_prop ("name");
            prepare (node_dat);
        }

        public virtual void prepare (Xml.Node* node_dat) {
            uint current_index = 0;

            for (Xml.Node* node = node_dat->first_element_child ();
                node != null;
                node = node->next_element_sibling ()) {
                switch (node->name) {
                    case XSD_ANNOTATION:
                        annotation = new Annotation (node);
                    break;
                    case XSD_ATTRIBUTE:
                        Attribute attr = new Attribute (node, current_index);
                        current_index++;
                        attrs.set (attr.name, attr);
                    break;
                    case XSD_CHOICE:
                        Element elem = new Choice (node, current_index);
                        current_index++;
                        elements.set (elem.name, elem);
                    break;
                    case XSD_ELEMENT:
                        Element elem = new TypeElement (node, current_index);
                        current_index++;
                        elements.set (elem.name, elem);
                    break;
                    default:
                        Logger.warn ("Unrecognized property content %s", node->name);
                    break;
                }
            }
        }

        /**
         * Try to get an attribute from its name
         *
         * @param name The name of the attribute
         */
        public bool try_get_attr (out Attribute attr, string name) {
            attr = attrs.get (name);

            return attr != null;
        }

        /**
         * Try to get an element from its name
         *
         * @param name The name of the attribute
         */
        public bool try_get_element (out Element elem, string name) {
            elem = elements.get (name);

            return elem != null;
        }

        /**
         * Get all attributes contained in this type
         */
        public virtual Collection<Attribute> get_attributes () {
            return attrs.values;
        }

        /**
         * Get all elements contained in this type
         */
        public virtual Collection<Element> get_elements () {
            return elements.values;
        }

        /**
         * Update the type data on attributes from the given property grid's registered data
         */
        public void update_types (PropertyGrid props) {
            foreach (var attr in attrs.values) {
                PropertyType prop_type;
                if (props.try_get_prop_type (attr.type_name, out prop_type)) {
                    attr.prop_type = prop_type;
                } else {
                    attr.prop_type = new PrimitiveType (attr.type_name);
                }
            }

            foreach (var elem in elements.values) {
                elem.update_types (props);
            }
        }

        /**
         * Try to get the hint value set at key
         */
        public bool try_get_hint (string key, out string hint) {
            if (annotation == null) {
                hint = null;
                return false;
            }

            return annotation.try_get_hint (key, out hint);
        }

        /**
         * The list of all attributes on this property
         */
        private Gee.HashMap<string, Attribute> attrs = new Gee.HashMap<string, Attribute> ();

        /**
         * The list of all XSD elements in this type
         */
        private Gee.HashMap<string, Element> elements = new Gee.HashMap<string, Element> ();
    }

    /**
     * A {@link PropertyType} with no special information; used for built-in types
     */
    public class PrimitiveType : PropertyType {
        public PrimitiveType (string in_name) {
            name = in_name;
            type_name = in_name;
        }
    }
}
