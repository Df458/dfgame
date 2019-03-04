using DFGame.Core;
using Xml;

namespace DFGame.PropertyGrid {
    /**
     * Represents a list of elements, matching any of several types
     */
    public class Choice : MultiElement {
        public Choice (Xml.Node* node_dat, uint index = 0) {
            base (node_dat, index);

            name = "choice";
            prepare (node_dat);
        }

        /**
         * Prepare the content of this object from XML
         */
        protected override void prepare (Xml.Node* node_dat) {
            base.prepare (node_dat);

            for (Xml.Node* node = node_dat->first_element_child ();
                node != null;
                node = node->next_element_sibling ()) {
                switch (node->name) {
                    case XSD_ANNOTATION:
                        annotation = new Annotation (node);
                    break;
                    case XSD_CHOICE:
                        Choice elem = new Choice (node);
                        foreach (Element e in elem.get_choice_elements ()) {
                            choice_elements[e.name] = e;
                        }
                    break;
                    case XSD_ELEMENT:
                        Element elem = new TypeElement (node);
                        choice_elements[elem.name] = elem;
                    break;
                    default:
                        Logger.warn ("Unrecognized property content %s", node->name);
                    break;
                }
            }
        }
    }
}
