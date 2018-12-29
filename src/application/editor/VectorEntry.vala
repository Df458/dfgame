using Gtk;
using DFGame.Math;

namespace DFGame.Application {
    // Base class for vector editing widgets
    public abstract class VectorEntryBase : Bin {
        // The precision (decimal places) of the numerical entries
        public int precision {
            get {
                return _precision;
            } set {
                if (_precision != value) {
                    assert (value <= 6 && value >= 0);

                    _precision = value;

                    if (_precision == 0) {
                        format = "%d";
                    } else {
                        format = "%%g.%d".printf (_precision);
                    }
                }
            }
        }
        private int _precision = 0;

        // Has minimum?
        public bool has_min_value {
            get {
                return _has_min_value;
            } set {
                if (_has_min_value != value) {
                    _has_min_value = value;
                    limits_changed ();
                }
            }
        }
        private bool _has_min_value = false;

        // The minium allowed value, null means no minimum
        public float min_value {
            get {
                return _min_value;
            } set {
                if (!_has_min_value || _min_value != value) {
                    _min_value = value;
                    _has_min_value = true;
                    limits_changed ();
                }
            }
        }
        private float _min_value = 0;

        // Has maximum?
        public bool has_max_value {
            get {
                return _has_max_value;
            } set {
                if (_has_max_value != value) {
                    _has_max_value = value;
                    limits_changed ();
                }
            }
        }
        private bool _has_max_value = false;

        // The maxium allowed value, null means no maximum
        public float max_value {
            get {
                return _max_value;
            } set {
                if (!_has_max_value || _max_value != value) {
                    _max_value = value;
                    _has_max_value = true;
                    limits_changed ();
                }
            }
        }
        private float _max_value = 0;

        // The format string used to populate the entry text
        protected string format = "%d";

        // Try to get a numerical value from text
        protected bool try_get_value (string text, out float value) {
            if (precision > 0) {
                float new_val = 0;
                int res = text.scanf (format, &new_val);

                if (res != 0) {
                    value = clamp (new_val);
                    return true;
                }
            } else {
                int new_val = 0;
                int res = text.scanf (format, &new_val);

                if (res != 0) {
                    value = clamp (new_val);
                    return true;
                }
            }

            value = 0;
            return false;
        }
        // Clamp a numerical value to the min/max
        protected float clamp (float input) {
            if (has_min_value && min_value > input) {
                input = min_value;
            } else if (has_max_value && max_value < input) {
                input = max_value;
            }

            return input;
        }
        // Called when the min/max changes
        protected abstract void limits_changed ();
    }

    // Widget for editing a 2d vector
    [GtkTemplate (ui="/org/df458/DFGame/Application/Vec2Entry.ui")]
    public class Vec2Entry : VectorEntryBase {
        public signal void value_changed (Vec2 vec);

        public float x { get {
            return _vector.x;
        } set {
            update_value (ref _vector.x, value);
        }}
        public float y { get {
            return _vector.y;
        } set {
            update_value (ref _vector.y, value);
        }}

        public string x_icon { owned get {
            return x_entry.primary_icon_name;
        } set {
            x_entry.primary_icon_name = value;
        }}
        public string y_icon { owned get {
            return y_entry.primary_icon_name;
        } set {
            y_entry.primary_icon_name = value;
        }}

        construct {
            x_icon = "axis-x-symbolic";
            y_icon = "axis-y-symbolic";
        }

        public Vec2 get_vector () {
            return _vector;
        }
        public void set_vector (Vec2 vec) {
            if (_vector.x != vec.x || _vector.y != vec.y) {
                _vector = vec;

                update_entry_text ();
                value_changed (_vector);
            }
        }

        protected override void limits_changed () {
            x = clamp (x);
            y = clamp (y);
        }

        private Vec2 _vector;

        [GtkChild]
        private Entry x_entry;
        [GtkChild]
        private Entry y_entry;

        [GtkCallback]
        private void on_icon_press (Entry sender, EntryIconPosition pos, Gdk.EventButton button) {
            sender.grab_focus ();
        }

        [GtkCallback]
        private void on_x_commit () {
            float new_x;
            if (try_get_value (x_entry.text, out new_x)) {
                x = new_x;
                update_entry_text ();
            }
        }
        [GtkCallback]
        private void on_y_commit () {
            float new_y;
            if (try_get_value (y_entry.text, out new_y)) {
                y = new_y;
                update_entry_text ();
            }
        }

        [GtkCallback]
        private bool on_unfocus (Gtk.Widget w, Gdk.EventFocus focus) {
            w.activate ();

            return false;
        }

        private void update_value (ref float val, float new_val) {
            if (val != new_val) {
                val = new_val;
                update_entry_text ();
                value_changed (_vector);
            }
        }
        private void update_entry_text () {
            if (precision != 0) {
                x_entry.text = format.printf (_vector.x);
                y_entry.text = format.printf (_vector.y);
            } else {
                x_entry.text = format.printf ( (int)_vector.x);
                y_entry.text = format.printf ( (int)_vector.y);
            }
        }
    }

    // Widget for editing a 3d vector
    [GtkTemplate (ui="/org/df458/DFGame/Application/Vec3Entry.ui")]
    public class Vec3Entry : VectorEntryBase {
        public signal void value_changed (Vec3 vec);

        public float x { get {
            return _vector.x;
        } set {
            update_value (ref _vector.x, value);
        }}
        public float y { get {
            return _vector.y;
        } set {
            update_value (ref _vector.y, value);
        }}
        public float z { get {
            return _vector.z;
        } set {
            update_value (ref _vector.z, value);
        }}

        public string x_icon { owned get {
            return x_entry.primary_icon_name;
        } set {
            x_entry.primary_icon_name = value;
        }}
        public string y_icon { owned get {
            return y_entry.primary_icon_name;
        } set {
            y_entry.primary_icon_name = value;
        }}
        public string z_icon { owned get {
            return z_entry.primary_icon_name;
        } set {
            z_entry.primary_icon_name = value;
        }}

        construct {
            x_icon = "axis-x-symbolic";
            y_icon = "axis-y-symbolic";
            z_icon = "axis-z-symbolic";

            update_entry_text ();
        }

        public Vec3 get_vector () {
            return _vector;
        }
        public void set_vector (Vec3 vec) {
            if (_vector.x != vec.x || _vector.y != vec.y || _vector.z != vec.z) {
                _vector = vec;

                update_entry_text ();
                value_changed (_vector);
            }
        }

        protected override void limits_changed () {
            x = clamp (x);
            y = clamp (y);
            z = clamp (z);
        }

        [GtkChild]
        private Entry x_entry;
        [GtkChild]
        private Entry y_entry;
        [GtkChild]
        private Entry z_entry;

        private Vec3 _vector;

        [GtkCallback]
        private void on_icon_press (Entry sender, EntryIconPosition pos, Gdk.EventButton button) {
            sender.grab_focus ();
        }

        [GtkCallback]
        private void on_x_commit () {
            float new_x;
            if (try_get_value (x_entry.text, out new_x)) {
                x = new_x;
                update_entry_text ();
            }
        }
        [GtkCallback]
        private void on_y_commit () {
            float new_y;
            if (try_get_value (y_entry.text, out new_y)) {
                y = new_y;
                update_entry_text ();
            }
        }
        [GtkCallback]
        private void on_z_commit () {
            float new_z;
            if (try_get_value (z_entry.text, out new_z)) {
                z = new_z;
                update_entry_text ();
            }
        }

        [GtkCallback]
        private bool on_unfocus (Gtk.Widget w, Gdk.EventFocus focus) {
            w.activate ();

            return false;
        }

        private void update_value (ref float val, float new_val) {
            if (val != new_val) {
                val = new_val;
                update_entry_text ();
                value_changed (_vector);
            }
        }
        private void update_entry_text () {
            if (precision != 0) {
                x_entry.text = format.printf (_vector.x);
                y_entry.text = format.printf (_vector.y);
                z_entry.text = format.printf (_vector.z);
            } else {
                x_entry.text = format.printf ( (int)_vector.x);
                y_entry.text = format.printf ( (int)_vector.y);
                z_entry.text = format.printf ( (int)_vector.z);
            }
        }
    }

    // Widget for editing a 4d vector
    [GtkTemplate (ui="/org/df458/DFGame/Application/Vec4Entry.ui")]
    public class Vec4Entry : VectorEntryBase {
        public signal void value_changed (Vec4 vec);

        public float x { get {
            return _vector.x;
        } set {
            update_value (ref _vector.x, value);
        }}
        public float y { get {
            return _vector.y;
        } set {
            update_value (ref _vector.y, value);
        }}
        public float z { get {
            return _vector.z;
        } set {
            update_value (ref _vector.z, value);
        }}
        public float w { get {
            return _vector.w;
        } set {
            update_value (ref _vector.w, value);
        }}

        public string x_icon { owned get {
            return x_entry.primary_icon_name;
        } set {
            x_entry.primary_icon_name = value;
        }}
        public string y_icon { owned get {
            return y_entry.primary_icon_name;
        } set {
            y_entry.primary_icon_name = value;
        }}
        public string z_icon { owned get {
            return z_entry.primary_icon_name;
        } set {
            z_entry.primary_icon_name = value;
        }}
        public string w_icon { owned get {
            return w_entry.primary_icon_name;
        } set {
            w_entry.primary_icon_name = value;
        }}

        construct {
            x_icon = "axis-x-symbolic";
            y_icon = "axis-y-symbolic";
            z_icon = "axis-z-symbolic";
            w_icon = "axis-w-symbolic";

            update_entry_text ();
        }

        public Vec4 get_vector () {
            return _vector;
        }
        public void set_vector (Vec4 vec) {
            if (_vector.x != vec.x || _vector.y != vec.y || _vector.z != vec.z || _vector.w != vec.w) {
                _vector = vec;

                update_entry_text ();
                value_changed (_vector);
            }
        }

        protected override void limits_changed () {
            x = clamp (x);
            y = clamp (y);
            z = clamp (z);
            w = clamp (w);
        }

        [GtkChild]
        private Entry x_entry;
        [GtkChild]
        private Entry y_entry;
        [GtkChild]
        private Entry z_entry;
        [GtkChild]
        private Entry w_entry;

        private Vec4 _vector;

        [GtkCallback]
        private void on_icon_press (Entry sender, EntryIconPosition pos, Gdk.EventButton button) {
            sender.grab_focus ();
        }

        [GtkCallback]
        private void on_x_commit () {
            float new_x;
            if (try_get_value (x_entry.text, out new_x)) {
                x = new_x;
                update_entry_text ();
            }
        }
        [GtkCallback]
        private void on_y_commit () {
            float new_y;
            if (try_get_value (y_entry.text, out new_y)) {
                y = new_y;
                update_entry_text ();
            }
        }
        [GtkCallback]
        private void on_z_commit () {
            float new_z;
            if (try_get_value (z_entry.text, out new_z)) {
                z = new_z;
                update_entry_text ();
            }
        }
        [GtkCallback]
        private void on_w_commit () {
            float new_w;
            if (try_get_value (w_entry.text, out new_w)) {
                w = new_w;
                update_entry_text ();
            }
        }

        [GtkCallback]
        private bool on_unfocus (Gtk.Widget w, Gdk.EventFocus focus) {
            w.activate ();

            return false;
        }

        private void update_value (ref float val, float new_val) {
            if (val != new_val) {
                val = new_val;
                update_entry_text ();
                value_changed (_vector);
            }
        }
        private void update_entry_text () {
            if (precision != 0) {
                x_entry.text = format.printf (_vector.x);
                y_entry.text = format.printf (_vector.y);
                z_entry.text = format.printf (_vector.z);
                w_entry.text = format.printf (_vector.w);
            } else {
                x_entry.text = format.printf ( (int)_vector.x);
                y_entry.text = format.printf ( (int)_vector.y);
                z_entry.text = format.printf ( (int)_vector.z);
                w_entry.text = format.printf ( (int)_vector.w);
            }
        }
    }
}
