using Gtk;

namespace DFGame.PropertyGrid.Editors {
    // Property Editor for enumerated types
    public class EnumPropertyEditor : PropertyEditor {
        public EnumPropertyEditor (string owner, Attribute attr, string value) {
            base (owner, attr, value);
        }
        protected override Widget? create_editor_widget () {
            combo = new ComboBoxText ();

            // Add enum values
            if (prop_type is SimpleType) {
                SimpleType simple = (SimpleType)prop_type;
                if (simple.restriction != null && simple.restriction.enum_values.length > 0) {
                    foreach (RestrictionEnum e in simple.restriction.enum_values) {
                        combo.append (e.value, e.display_name);
                    }
                }
            }

            combo.changed.connect (() => { handle_value_changed (combo.get_active_id ()); });

            return combo;
        }

        protected override void set_value (string value) {
            combo.active_id = value;
        }

        private ComboBoxText combo;
    }
}
