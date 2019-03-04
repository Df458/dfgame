using DFGame.Core;
using Gee;
using Xml;

namespace DFGame.PropertyGrid {
    /**
     * Represents an element that matches a {@link PropertyType}
     */
    public class TypeElement : Element {
        /**
         * The associated type
         */
        public PropertyType prop_type { get; private set; }

        public TypeElement (Xml.Node* node_dat, uint index = 0) {
            base (index);

            prepare (node_dat);

            if (prop_type_name == null) {
                prop_type = new PropertyType.complex (node_dat);
            }
        }

        public TypeElement.with_type (PropertyType type, uint index = 0) {
            base (index);

            prop_type = type;
            name = prop_type.type_name;
            display_name = make_display_name (name);
        }

        /**
         * Gets the child attributes
         */
        public override Collection<Attribute> get_attributes () {
            return prop_type.get_attributes ();
        }

        /**
         * Gets the child elements
         */
        public override Collection<Element> get_elements () {
            return prop_type.get_elements ();
        }

        /**
         * Update references to types from the contents of a {@link PropertyGrid}
         */
        public override void update_types (PropertyGrid props) {
            if (prop_type_name != null) {
                PropertyType prop;
                if (props.try_get_prop_type (prop_type_name, out prop)) {
                    prop_type = prop;
                } else {
                    prop_type = new PrimitiveType (prop_type_name);
                }
            } else {
                prop_type.update_types (props);
            }

            base.update_types (props);
        }

        /**
         * Prepare the content of this object from XML
         */
        protected override void prepare (Xml.Node* node) {
            base.prepare (node);

            prop_type_name = node->get_prop (XSD_TYPE);
        }

        /**
         * The name of the associated property
         */
        private string prop_type_name;
    }
}
