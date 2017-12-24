using Gtk;
using DFGame.Input;

namespace DFGame
{
    public class Viewport : GLArea
    {
        private uint _update_interval = 0;
        public uint update_interval { get { return _update_interval; } set { set_update_timer(value); } }

        construct
        {
            button_press_event.connect(on_button_press);
            button_release_event.connect(on_button_release);
            key_press_event.connect(on_key_press);
            key_release_event.connect(on_key_release);
            motion_notify_event.connect(on_mouse_motion);
            scroll_event.connect(on_scroll);

            events |= Gdk.EventMask.BUTTON_RELEASE_MASK | Gdk.EventMask.KEY_RELEASE_MASK | Gdk.EventMask.BUTTON_PRESS_MASK | Gdk.EventMask.KEY_PRESS_MASK | Gdk.EventMask.POINTER_MOTION_MASK | Gdk.EventMask.SCROLL_MASK;
            update_id = Signal.lookup("update_step", typeof(Viewport));
        }

        public bool update()
        {
            update_input();

            return !Signal.has_handler_pending(this, update_id, {}, false) || update_step(_update_interval > 0 ? 1.0f / update_interval : 0);
        }

        private void set_update_timer(uint fps)
            requires(fps < 1000) // Timer takes milliseconds, anything more will result in an interval of 0
        {
            _update_interval = fps;
            if(fps != 0) {
                update_timer = new TimeoutSource(1000 / fps);

                update_timer.set_callback(update);
                update_timer.attach(MainContext.default());
            } else {
                // TODO
            }
        }

        public signal bool update_step(float delta);

        private TimeoutSource update_timer;
        private uint update_id;
    }
}
