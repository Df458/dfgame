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
            enter_notify_event.connect((ev) => { is_focus = true; });

            Framebuffer.set_callback({ framebuffer_callback, this });

            events = Gdk.EventMask.ALL_EVENTS_MASK;
            update_id = Signal.lookup("update_step", typeof(Viewport));
            render.connect(run_update);
        }

        public bool update()
        {
            update_input();

            queue_render();

            return should_continue;
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

        private bool run_update()
        {
            should_continue = !Signal.has_handler_pending(this, update_id, {}, false) || update_step(_update_interval > 0 ? 1.0f / update_interval : 0);

            return true;
        }

        private static void framebuffer_callback(void* pass, void* user)
        {
            (user as GLArea).attach_buffers();
        }

        public signal bool update_step(float delta);

        private TimeoutSource update_timer;
        private uint update_id;
        private bool should_continue;
    }
}
