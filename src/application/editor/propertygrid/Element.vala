using DFGame.Core;
using Gee;
using Xml;

namespace DFGame.PropertyGrid {
    /**
     * Represents an <element/> XSD tag, or any other tag that results in an XML node
     *
     * For a regular node, use {@link TypeElement}.
     * For a list of potential options, use {@link Choice}
     */
    public abstract class Element : Object {
        /**
         * Annotation data
         */
        public Annotation annotation { get; protected set; }

        /**
         * The name to display in UI and documentation
         */
        public string display_name { get; protected set; }

        /**
         * The name of the attribute
         */
        public string name { get; protected set; }

        /**
         * The order that this should appear in the UI
         */
        public uint sort_index { get; construct; }

        public Element (uint index = 0) {
            Object (sort_index: index);
        }

        /**
         * Gets the child attributes
         *
         * The default implementation returns nothing
         */
        public virtual Collection<Attribute> get_attributes () {
            return new ArrayList<Attribute> ();
        }

        /**
         * Gets the child elements
         *
         * The default implementation returns nothing
         */
        public virtual Collection<Element> get_elements () {
            return new ArrayList<Element> ();
        }

        /**
         * Update references to types from the contents of a {@link PropertyGrid}
         */
        public virtual void update_types (PropertyGrid props) {
            foreach (var attr in get_attributes ()) {
                PropertyType prop_type;
                if (props.try_get_prop_type (attr.type_name, out prop_type)) {
                    attr.prop_type = prop_type;
                } else {
                    attr.prop_type = new PrimitiveType (attr.type_name);
                }
            }

            foreach (var elem in get_elements ()) {
                elem.update_types (props);
            }
        }

        /**
         * Format a name for display
         */
        protected string make_display_name (string name) {
            // Split on underscores to form words
            string display_name = name.replace ("_", " ").strip ();

            // Capitalize the first character
            if (display_name.length > 0) {
                display_name.data[0] = display_name.get (0).toupper ();
            }

            return display_name;
        }

        /**
         * Prepare the content of this object from XML
         */
        protected virtual void prepare (Xml.Node* node) {
            name = node->get_prop ("name");
            display_name = node->get_ns_prop (DF_DISPLAY_NAME, DF_NS) ?? make_display_name (name);
        }
    }
}
