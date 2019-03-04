using Gtk;

namespace DFGame.PropertyGrid {
    /**
     * Widget for editing a collection in a {@link PropertyGrid}
     */
    internal class MultiPropertyHeader : Bin {
        /**
         * The associated element
         */
        public MultiElement element { get; construct set; }

        /**
         * Called when a new element is requested
         */
        public signal void add_request (Element elem);

        /**
         * Called when the user wants to clear the list
         */
        public signal void clear_request ();

        public MultiPropertyHeader (PropertyList l, MultiElement e) {
            Object (element: e);
            list = l;

            Box box = new Box (Orientation.HORIZONTAL, 6);

            add_button = new Button.from_icon_name ("list-add-symbolic");
            add_button.tooltip_text = "Add a new entry to this list";
            add_button.clicked.connect (on_add);

            clear_button = new Button.from_icon_name ("list-remove-symbolic");
            clear_button.tooltip_text = "Clear this list";
            clear_button.clicked.connect (on_clear);

            add_pop = new Popover (add_button);

            Grid pop_grid = new Grid ();
            pop_grid.column_spacing = 6;
            pop_grid.row_spacing = 6;
            pop_grid.margin = 6;

            type_box = new ComboBoxText ();
            foreach (Element child in element.get_choice_elements ()) {
                type_box.append (child.name, child.display_name);
            }
            type_box.changed.connect (on_type_changed);

            confirm_button = new Button.with_label ("Create");
            confirm_button.sensitive = false;
            confirm_button.clicked.connect (on_confirm);

            pop_grid.attach (new Label ("Type"), 0, 0);
            pop_grid.attach (type_box, 1, 0);
            pop_grid.attach (confirm_button, 0, 1, 2);
            add_pop.add (pop_grid);

            box.pack_start (add_button, false, false);
            box.pack_start (clear_button, false, false);

            add (box);
            update ();
            show_all ();
        }

        /**
         * Update the UI in response to a change in the list
         */
        public void update () {
            uint count = list.child_count;

            add_button.sensitive = count < element.max_occurs || element.max_occurs == 0;
            clear_button.sensitive = count > element.min_occurs;
        }

        private void on_add () {
            var choices = element.get_choice_elements ();
            if (choices.size == 1) {
                var iter = choices.iterator ();
                if (iter.next ()) {
                    add_request (iter.get ());
                }
            } else {
                type_box.active = -1;
                add_pop.popup ();
                add_pop.show_all ();
            }
        }

        private void on_clear () {
            clear_request ();
        }

        private void on_confirm () {
            add_pop.hide ();

            Element child_element = element.get_choice_element (type_box.active_id);
            if (child_element != null) {
                add_request (child_element);
            }
        }

        private void on_type_changed () {
            confirm_button.sensitive = type_box.active_id != null;
        }

        private Button add_button;
        private Button clear_button;
        private Popover add_pop;
        private ComboBoxText type_box;
        private Button confirm_button;
        private PropertyList list;
    }
}
