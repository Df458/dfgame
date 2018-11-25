using Gtk;
namespace DFGame {
    public class BoolPropertyEditor : PropertyEditor {
        public BoolPropertyEditor (string owner, string name, PropertyType prop, string value) {
            base (owner, name, prop, value);
        }
        protected override Widget? create_editor_widget () {
            check = new CheckButton ();
            check.toggled.connect (() => { handle_value_changed (check.active ? "true" : "false"); });
            return check;
        }

        protected override void set_value (string value) {
            check.active = value.down () == "true";
        }

        private CheckButton check;
    }
}
