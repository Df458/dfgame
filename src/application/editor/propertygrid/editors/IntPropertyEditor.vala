using Gtk;

namespace DFGame.PropertyGrid.Editors {
    public class IntPropertyEditor : PropertyEditor {
        public IntPropertyEditor (string owner, Attribute attr, string value) {
            base (owner, attr, value);
        }
        protected override Widget? create_editor_widget () {
            double min = long.MIN;
            double max = long.MAX;

            switch (prop_type.primitive_name) {
                case "xs:int":
                    min = int.MIN;
                    max = int.MAX;
                    break;
                case "xs:negativeInteger":
                    min = int.MIN;
                    max = -1;
                    break;
                case "xs:nonNegativeInteger":
                    min = 0;
                    max = int.MAX;
                    break;
                case "xs:nonPositiveInteger":
                    min = int.MIN;
                    max = 0;
                    break;
                case "xs:positiveInteger":
                    min = 1;
                    max = int.MAX;
                    break;
                case "xs:short":
                    min = short.MIN;
                    max = short.MAX;
                    break;
                case "xs:unsignedLong":
                    min = ulong.MIN;
                    max = ulong.MAX;
                    break;
                case "xs:unsignedInt":
                    min = uint.MIN;
                    max = uint.MAX;
                    break;
                case "xs:unsignedShort":
                    min = ushort.MIN;
                    max = ushort.MAX;
                    break;
                case "xs:unsignedByte":
                    min = 0;
                    max = 0xFF;
                    break;
            }

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
            spin.width_chars = 3;
            spin.value_changed.connect (() => { handle_value_changed (spin.text); });
            return spin;
        }

        protected override void set_value (string value) {
            spin.value = int.parse (value);
        }

        private SpinButton spin;
    }
}
