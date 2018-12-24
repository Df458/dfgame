using Gtk;

namespace DFGame.PropertyGrid.Editors {
    public class ColorPropertyEditor : PropertyEditor {
        public ColorPropertyEditor (string owner, Attribute attr, string value) {
            base (owner, attr, value);
        }
        protected override Widget? create_editor_widget () {
            color = new ColorButton ();
            color.show_editor = true;
            color.color_set.connect ((s) => { handle_value_changed ("#"
                        + write_color (color.rgba.red)
                        + write_color (color.rgba.green)
                        + write_color (color.rgba.blue)
                        + write_color (color.rgba.alpha)); });
            return color;
        }

        protected override void set_value (string value) {
            var rgba = Gdk.RGBA () {
                red = parse_color (value, 1),
                green = parse_color (value, 3),
                blue = parse_color (value, 5),
                alpha = parse_color (value, 7)
            };
            color.rgba = rgba;
        }

        private string write_color (double value) {
            int v = (int)(value * 255);
            return v.to_string ("%02x");
        }
        private double parse_color (string s, int offset) {
            int c = (s.get (offset).xdigit_value () * 16) + s.get (offset + 1).xdigit_value ();

            return c / 255.0;
        }

        private ColorButton color;
    }
}
