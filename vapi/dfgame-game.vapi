namespace DF {
    [CCode (cheader_filename = "input.h")]
    namespace Input {
        [CCode (cname = "set_up_state")]
        void set_up_state(bool state);
        [CCode (cname = "set_down_state")]
        void set_down_state(bool state);
        [CCode (cname = "set_left_state")]
        void set_left_state(bool state);
        [CCode (cname = "set_right_state")]
        void set_right_state(bool state);
        [CCode (cname = "set_action_state")]
        void set_action_state(bool state);
        [CCode (cname = "set_confirm_state")]
        void set_confirm_state(bool state);
        [CCode (cname = "set_cancel_state")]
        void set_cancel_state(bool state);
        [CCode (cname = "set_menu_state")]
        void set_menu_state(bool state);
        [CCode (cname = "set_quit_state")]
        void set_quit_state(bool state);
        [CCode (cname = "update_input_states")]
        void update();
    }
}
