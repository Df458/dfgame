namespace DFGame
{
    [CCode (cheader_filename = "gdk_input.h")]
    namespace Input
    {
        [CCode (cname = "on_button_press")]
            public static bool on_button_press(Gdk.EventButton event);
        [CCode (cname = "on_button_release")]
            public static bool on_button_release(Gdk.EventButton event);
        [CCode (cname = "on_key_press")]
            public static bool on_key_press(Gdk.EventKey event);
        [CCode (cname = "on_key_release")]
            public static bool on_key_release(Gdk.EventKey event);
        [CCode (cname = "on_mouse_motion")]
            public static bool on_mouse_motion(Gdk.EventMotion event);
        [CCode (cname = "on_scroll")]
            public static bool on_scroll(Gdk.EventScroll event);
        [CCode (cname = "on_enter")]
            public static bool on_enter(double x, double y);
        [CCode (cname = "update_input")]
            public static void update_input();
    }

    [CCode (cheader_filename = "renderpass.h")]
    namespace Framebuffer
    {
        [CCode (cname = "renderpass_null_response_func", has_target = false)]
        public delegate void Callback(void* pass, void* user);
        [CCode (cname = "renderpass_null_response", destroy_function = "", has_type_id = false)]
        struct CallbackData
        {
            [CCode (cname = "func", has_target = false)]
            Callback cb;
            [CCode (cname = "user")]
            void* user;
        }
        [CCode (cname = "set_default_renderpass_response")]
        void set_callback(CallbackData cb);
    }
}
