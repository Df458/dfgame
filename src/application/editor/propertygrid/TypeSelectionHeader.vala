using Gtk;

namespace DFGame.PropertyGrid {
    /**
     * Widget for selecting the type of an element in a {@link PropertyGrid}
     */
    internal class TypeSelectionHeader : Bin {
        /**
         * The currently-selected element type
         */
        public Element? current_element { get; private set; }

        /**
         * The {@link MultiElement} that provides this widget's options
         */
        public MultiElement source_element { get; private set; }

        /**
         * Called when the selected element type changes
         */
        public signal void type_changed (Element e);

        public TypeSelectionHeader (MultiElement m, Element? e) {
            source_element = m;
            current_element = e;

            combo = new ComboBoxText ();
            foreach (Element child in source_element.get_choice_elements ()) {
                combo.append (child.name, child.display_name);
            }

            if (current_element != null) {
                combo.active_id = e.name;
            }

            combo.changed.connect (on_type_changed);

            add (combo);
        }

        private void on_type_changed () {
            current_element = source_element.get_choice_element (combo.active_id);
            type_changed (current_element);
        }

        private ComboBoxText combo;
    }
}
