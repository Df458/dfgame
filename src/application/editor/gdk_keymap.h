#ifndef APPLICATION_KEYMAP_H
#define APPLICATION_KEYMAP_H
#include "application/input_id.h"

#include <gdk/gdkkeysyms.h>

const uint32 dfgame_to_native_key[] = {
    [0 ... K_LAST]           = GDK_KEY_VoidSymbol,
    [K_SPACE]                = GDK_KEY_space,
    [K_APOSTROPHE]           = GDK_KEY_apostrophe,
    [K_COMMA]                = GDK_KEY_comma,
    [K_MINUS]                = GDK_KEY_minus,
    [K_PERIOD]               = GDK_KEY_period,
    [K_FORWARD_SLASH]        = GDK_KEY_slash,
    [K_0]                    = GDK_KEY_0,
    [K_1]                    = GDK_KEY_1,
    [K_2]                    = GDK_KEY_2,
    [K_3]                    = GDK_KEY_3,
    [K_4]                    = GDK_KEY_4,
    [K_5]                    = GDK_KEY_5,
    [K_6]                    = GDK_KEY_6,
    [K_7]                    = GDK_KEY_7,
    [K_8]                    = GDK_KEY_8,
    [K_9]                    = GDK_KEY_9,
    [K_SEMICOLON]            = GDK_KEY_semicolon,
    [K_EQUALS]               = GDK_KEY_equal,
    [K_A]                    = GDK_KEY_a,
    [K_B]                    = GDK_KEY_b,
    [K_C]                    = GDK_KEY_c,
    [K_D]                    = GDK_KEY_d,
    [K_E]                    = GDK_KEY_e,
    [K_F]                    = GDK_KEY_f,
    [K_G]                    = GDK_KEY_g,
    [K_H]                    = GDK_KEY_h,
    [K_I]                    = GDK_KEY_i,
    [K_J]                    = GDK_KEY_j,
    [K_K]                    = GDK_KEY_k,
    [K_L]                    = GDK_KEY_l,
    [K_M]                    = GDK_KEY_m,
    [K_N]                    = GDK_KEY_n,
    [K_O]                    = GDK_KEY_o,
    [K_P]                    = GDK_KEY_p,
    [K_Q]                    = GDK_KEY_q,
    [K_R]                    = GDK_KEY_r,
    [K_S]                    = GDK_KEY_s,
    [K_T]                    = GDK_KEY_t,
    [K_U]                    = GDK_KEY_u,
    [K_V]                    = GDK_KEY_v,
    [K_W]                    = GDK_KEY_w,
    [K_X]                    = GDK_KEY_x,
    [K_Y]                    = GDK_KEY_y,
    [K_Z]                    = GDK_KEY_z,
    [K_LEFT_SQUARE_BRACKET]  = GDK_KEY_bracketleft,
    [K_BACK_SLASH]           = GDK_KEY_backslash,
    [K_RIGHT_SQUARE_BRACKET] = GDK_KEY_bracketright,
    [K_GRAVE]                = GDK_KEY_grave,
    [K_INSERT]               = GDK_KEY_Insert,
    [K_DELETE]               = GDK_KEY_Delete,
    [K_HOME]                 = GDK_KEY_Home,
    [K_END]                  = GDK_KEY_End,
    [K_PAGE_UP]              = GDK_KEY_Page_Up,
    [K_PAGE_DOWN]            = GDK_KEY_Page_Down,
    [K_ESCAPE]               = GDK_KEY_Escape,
    [K_BACKSPACE]            = GDK_KEY_BackSpace,
    [K_ENTER]                = GDK_KEY_Return,
    [K_TAB]                  = GDK_KEY_Tab,
    [K_LEFT]                 = GDK_KEY_Left,
    [K_RIGHT]                = GDK_KEY_Right,
    [K_UP]                   = GDK_KEY_Up,
    [K_DOWN]                 = GDK_KEY_Down,
    [K_F1]                   = GDK_KEY_F1,
    [K_F2]                   = GDK_KEY_F2,
    [K_F3]                   = GDK_KEY_F3,
    [K_F4]                   = GDK_KEY_F4,
    [K_F5]                   = GDK_KEY_F5,
    [K_F6]                   = GDK_KEY_F6,
    [K_F7]                   = GDK_KEY_F7,
    [K_F8]                   = GDK_KEY_F8,
    [K_F9]                   = GDK_KEY_F9,
    [K_F10]                  = GDK_KEY_F10,
    [K_F11]                  = GDK_KEY_F11,
    [K_F12]                  = GDK_KEY_F12,
    [K_LEFT_SHIFT]           = GDK_KEY_Shift_L,
    [K_LEFT_CONTROL]         = GDK_KEY_Control_L,
    [K_LEFT_ALT]             = GDK_KEY_Alt_L,
    [K_RIGHT_SHIFT]          = GDK_KEY_Shift_R,
    [K_RIGHT_CONTROL]        = GDK_KEY_Control_R,
    [K_RIGHT_ALT]            = GDK_KEY_Alt_R,
};

const uint32 dfgame_to_native_mouse_button[] = {
    [0 ... MB_LAST] = 0,
    [MB_LEFT] = 1,
    [MB_MIDDLE] = 2,
    [MB_RIGHT] = 3,
};

#endif
