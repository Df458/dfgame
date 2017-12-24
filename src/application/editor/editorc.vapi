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
        [CCode (cname = "on_key_release")]
            public static bool on_mouse_motion(Gdk.EventMotion event);
        [CCode (cname = "on_key_release")]
            public static bool on_scroll(Gdk.EventScroll event);
        [CCode (cname = "update_input")]
            public static void update_input();
    }
}
