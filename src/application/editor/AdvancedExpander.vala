using DFGame.Core;
using Gtk;
using Gdk;

namespace DFGame.Application {
    /**
     * Widget that acts like {@link Expander}, but doesn't steal events from its header
     */
    public class AdvancedExpander : Bin {
        /**
         * A widget to display in the header
         */
        public Widget header_child {
            get { return _header_child; }
            set {
                if (_header_child != value) {
                    if (_header_child != null) {
                        header_box.remove (_header_child);
                    }

                    _header_child = value;

                    if (_header_child != null) {
                        header_box.add (_header_child);
                    }
                }
            }
        }
        private Widget _header_child;

        /**
         * The widget that serves as the expander's content
         */
        public Widget body_child {
            get { return _body_child; }
            set {
                if (_body_child != value) {
                    if (_body_child != null) {
                        revealer.remove (_body_child);
                    }

                    _body_child = value;

                    if (_body_child != null) {
                        revealer.add (_body_child);
                    }
                }
            }
        }
        private Widget _body_child;

        /**
         * The expander's label
         */
        public string label {
            get {
                if (label_widget != null) {
                    return label_widget.label;
                }
                return "";
            }
            set {
                if (label_widget != null) {
                    label_widget.label = value;
                }
            }
        }

        /**
         * Is the content expanded?
         */
        public bool expanded {
            get {
                if (revealer != null) {
                    return revealer.child_revealed;
                }
                return false;
            }
            set {
                if (revealer != null && expanded != value) {
                    revealer.reveal_child = value;
                    expanded_icon.icon_name = value ? "go-down-symbolic" : "go-next-symbolic";
                }
            }
        }

        public AdvancedExpander (string? label_text = null) {
            Box main_box = new Box (Orientation.VERTICAL, 0);
            Box sub_header_box = new Box (Orientation.HORIZONTAL, 6);
            EventBox event_box = new EventBox ();
            event_box.events = BUTTON_PRESS_MASK | BUTTON_RELEASE_MASK | LEAVE_NOTIFY_MASK;
            event_box.button_press_event.connect (on_button_press);
            event_box.button_release_event.connect (on_button_release);
            event_box.leave_notify_event.connect (on_leave);
            event_box.above_child = true;
            event_box.visible_window = false;

            header_box = new Box (Orientation.HORIZONTAL, 0);

            revealer = new Revealer ();
            revealer.transition_duration = 0;
            revealer.reveal_child = false;

            label_widget = new Label (label_text);
            label_widget.xalign = 0;

            expanded_icon = new Image.from_icon_name ("go-next-symbolic", IconSize.BUTTON);

            sub_header_box.pack_start (expanded_icon, false, false);
            sub_header_box.pack_start (label_widget, false, false);
            event_box.add (sub_header_box);
            header_box.pack_start (event_box, true, true);
            main_box.pack_start (header_box, false, false);
            main_box.pack_start (revealer, true, true);
            add (main_box);
        }

        private bool on_button_press (EventButton button) {
            if (button.button == BUTTON_PRIMARY) {
                button_state = true;
                return true;
            }
            return false;
        }

        private bool on_button_release (EventButton button) {
            if (button.button == BUTTON_PRIMARY && button_state) {
                expanded = !expanded;
                return true;
            }
            return false;
        }

        private bool on_leave (EventCrossing crossing) {
            button_state = false;
            return true;
        }

        private Box header_box;
        private Revealer revealer;
        private Label label_widget;
        private Image expanded_icon;
        private bool button_state;
    }
}
