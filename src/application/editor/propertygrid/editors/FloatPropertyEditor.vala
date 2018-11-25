using Gtk;

namespace DFGame {
    public class FloatPropertyEditor : PropertyEditor {
        public FloatPropertyEditor (string owner, string name, PropertyType prop, string value) {
            base (owner, name, prop, value);
        }
        protected override Widget? create_editor_widget () {
            spin = new SpinButton.with_range (-1000000, 1000000, 1);
            spin.digits = 2;
            spin.value_changed.connect (() => { handle_value_changed (spin.text); });
            return spin;
        }

        protected override void set_value (string value) {
            spin.value = double.parse (value);
        }

        private SpinButton spin;
    }
}
