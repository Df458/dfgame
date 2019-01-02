using Gtk;

namespace DFGame.PropertyGrid.Editors {
    public class IntPropertyEditor : PropertyEditor {
        public IntPropertyEditor (string owner, Attribute attr, string value) {
            base (owner, attr, value);
        }
        protected override Widget? create_editor_widget () {
            double min = -1000000;
            double max = 1000000;

            // Check for restrictions
            if (prop_type is SimpleType) {
                SimpleType simple = (SimpleType)prop_type;
                if (simple.restriction != null) {
                    // Digit count
                    if (simple.restriction.total_digits != null) {
                        // TODO
                    }

                    // Minimum value
                    if (simple.restriction.min_inclusive != null) {
                        min = simple.restriction.min_inclusive;
                    } else if (simple.restriction.min_exclusive != null) {
                        // SpinButton range is inclusive, so we calculate
                        // the closest valid number based on the digit count
                        min = simple.restriction.min_exclusive + 1;
                    }

                    // Maximum value
                    if (simple.restriction.max_inclusive != null) {
                        max = simple.restriction.max_inclusive;
                    } else if (simple.restriction.max_exclusive != null) {
                        // SpinButton range is inclusive, so we calculate
                        // the closest valid number based on the digit count
                        max = simple.restriction.max_exclusive - 1;
                    }
                }
            }

            spin = new SpinButton.with_range (min, max, 1);
            spin.value_changed.connect (() => { handle_value_changed (spin.text); });
            return spin;
        }

        protected override void set_value (string value) {
            spin.value = int.parse (value);
        }

        private SpinButton spin;
    }
}
