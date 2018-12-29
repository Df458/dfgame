using DFGame.Application;
using DFGame.Core;
using Gtk;

namespace DFGame.PropertyGrid.Editors {
    // Property Editor for 2d vectors
    public class Vec2PropertyEditor : PropertyEditor {
        public Vec2PropertyEditor (string owner, Attribute attr, string value) {
            base (owner, attr, value);
        }
        protected override Widget? create_editor_widget () {
            entry = new Vec2Entry ();
            entry.value_changed.connect (() => {
                handle_value_changed ("%f %f".printf (entry.x, entry.y));
            });
            return entry;
        }

        protected override void set_value (string value) {
            string[] strs = value.split (" ");

            // Ensure that the correct number of values exist
            if (strs.length != 2) {
                Logger.error ("Expected 2 values for vector, got %d", strs.length);
            } else {
                entry.set_vector ({ float.parse (strs[0]), float.parse (strs[1]) });
            }
        }

        private Vec2Entry entry;
    }
    // Property Editor for 3d vectors
    public class Vec3PropertyEditor : PropertyEditor {
        public Vec3PropertyEditor (string owner, Attribute attr, string value) {
            base (owner, attr, value);
        }
        protected override Widget? create_editor_widget () {
            entry = new Vec3Entry ();
            entry.value_changed.connect (() => {
                handle_value_changed ("%f %f %f".printf (entry.x, entry.y, entry.z));
            });
            return entry;
        }

        protected override void set_value (string value) {
            string[] strs = value.split (" ");

            // Ensure that the correct number of values exist
            if (strs.length != 3) {
                Logger.error ("Expected 3 values for vector, got %d", strs.length);
            } else {
                entry.set_vector ({ float.parse (strs[0]), float.parse (strs[1]), float.parse (strs[2]) });
            }
        }

        private Vec3Entry entry;
    }
    // Property Editor for 4d vectors
    public class Vec4PropertyEditor : PropertyEditor {
        public Vec4PropertyEditor (string owner, Attribute attr, string value) {
            base (owner, attr, value);
        }
        protected override Widget? create_editor_widget () {
            entry = new Vec4Entry ();
            entry.value_changed.connect (() => {
                handle_value_changed ("%f %f %f %f".printf (entry.x, entry.y, entry.z, entry.w));
            });
            return entry;
        }

        protected override void set_value (string value) {
            string[] strs = value.split (" ");

            // Ensure that the correct number of values exist
            if (strs.length != 4) {
                Logger.error ("Expected 4 values for vector, got %d", strs.length);
            } else {
                entry.set_vector ({
                    float.parse (strs[0]),
                    float.parse (strs[1]),
                    float.parse (strs[2]),
                    float.parse (strs[3])
                });
            }
        }

        private Vec4Entry entry;
    }
}
