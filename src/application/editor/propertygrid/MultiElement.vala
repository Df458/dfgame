using DFGame.Core;
using Gee;
using Xml;

namespace DFGame.PropertyGrid {
    /**
     * Represents a collection of child elements
     */
    public abstract class MultiElement : Element {
        /**
         * The max number of elements that this can contain.
         * If this value is 0, the upper bound is unset.
         */
        public uint max_occurs { get; private set; default = 0; }

        /**
         * The min number of elements that this can contain.
         * If this value is 0, the lower bound is unset.
         */
        public uint min_occurs { get; private set; default = 0; }

        protected MultiElement (Xml.Node* node_dat, uint index = 0) {
            base (index);
        }

        /**
         * Gets a supported child element matching a given name.
         */
        public Element get_choice_element (string name) {
            return choice_elements.get (name);
        }

        /**
         * Gets all supported child elements.
         */
        public Collection<Element> get_choice_elements () {
            return choice_elements.values;
        }

        /**
         * Update references to types from the contents of a {@link PropertyGrid}
         */
        public override void update_types (PropertyGrid props) {
            foreach (Element e in choice_elements.values) {
                e.update_types (props);
            }
        }

        /**
         * Prepare the content of this object from XML
         */
        protected override void prepare (Xml.Node* node_dat) {
            base.prepare (node_dat);

            string max_str = node_dat->get_prop (XSD_MAX_OCCURS);
            if (max_str != null) {
                max_occurs = int.parse (max_str);
            }

            string min_str = node_dat->get_prop (XSD_MIN_OCCURS);
            if (min_str != null) {
                min_occurs = int.parse (min_str);
            }
        }

        /**
         * Contains all supported child elements
         */
        protected Gee.HashMap<string, Element> choice_elements = new Gee.HashMap<string, Element> ();
    }
}
