namespace DF {
    [CCode (cheader_filename = "input.h")]
    namespace Input {
        [CCode (cname = "update_input_states")]
        void update(float delta);
        [CCode (cname = "get_input_state")]
        void get_state(ushort id);
        [CCode (cname = "set_input_state")]
        void set_state(ushort id, bool down);
        [CCode (cname = "set_axis")]
        void set_axis(ushort id, bool is_x, float val, bool analog, bool emulated);

        [CCode (cname = "get_axis")]
        void get_axis(ushort id, out float x, out float y);
        [CCode (cname = "get_axis_x")]
        float get_axis_x(ushort id);
        [CCode (cname = "get_axis_y")]
        float get_axis_y(ushort id);
    }
}
