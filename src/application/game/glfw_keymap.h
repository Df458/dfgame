#ifndef APPLICATION_KEYMAP_H
#define APPLICATION_KEYMAP_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "input_id.h"
#include "types.h"

const uint32 native_to_dfgame_key[] = {
    [0 ... GLFW_KEY_LAST]    = K_INVALID,
    [GLFW_KEY_SPACE]         = K_SPACE,
    [GLFW_KEY_APOSTROPHE]    = K_APOSTROPHE,
    [GLFW_KEY_COMMA]         = K_COMMA,
    [GLFW_KEY_MINUS]         = K_MINUS,
    [GLFW_KEY_PERIOD]        = K_PERIOD,
    [GLFW_KEY_SLASH]         = K_FORWARD_SLASH,
    [GLFW_KEY_0]             = K_0,
    [GLFW_KEY_1]             = K_1,
    [GLFW_KEY_2]             = K_2,
    [GLFW_KEY_3]             = K_3,
    [GLFW_KEY_4]             = K_4,
    [GLFW_KEY_5]             = K_5,
    [GLFW_KEY_6]             = K_6,
    [GLFW_KEY_7]             = K_7,
    [GLFW_KEY_8]             = K_8,
    [GLFW_KEY_9]             = K_9,
    [GLFW_KEY_SEMICOLON]     = K_SEMICOLON,
    [GLFW_KEY_EQUAL]         = K_EQUALS,
    [GLFW_KEY_A]             = K_A,
    [GLFW_KEY_B]             = K_B,
    [GLFW_KEY_C]             = K_C,
    [GLFW_KEY_D]             = K_D,
    [GLFW_KEY_E]             = K_E,
    [GLFW_KEY_F]             = K_F,
    [GLFW_KEY_G]             = K_G,
    [GLFW_KEY_H]             = K_H,
    [GLFW_KEY_I]             = K_I,
    [GLFW_KEY_J]             = K_J,
    [GLFW_KEY_K]             = K_K,
    [GLFW_KEY_L]             = K_L,
    [GLFW_KEY_M]             = K_M,
    [GLFW_KEY_N]             = K_N,
    [GLFW_KEY_O]             = K_O,
    [GLFW_KEY_P]             = K_P,
    [GLFW_KEY_Q]             = K_Q,
    [GLFW_KEY_R]             = K_R,
    [GLFW_KEY_S]             = K_S,
    [GLFW_KEY_T]             = K_T,
    [GLFW_KEY_U]             = K_U,
    [GLFW_KEY_V]             = K_V,
    [GLFW_KEY_W]             = K_W,
    [GLFW_KEY_X]             = K_X,
    [GLFW_KEY_Y]             = K_Y,
    [GLFW_KEY_Z]             = K_Z,
    [GLFW_KEY_LEFT_BRACKET]  = K_LEFT_SQUARE_BRACKET,
    [GLFW_KEY_BACKSLASH]     = K_BACK_SLASH,
    [GLFW_KEY_RIGHT_BRACKET] = K_RIGHT_SQUARE_BRACKET,
    [GLFW_KEY_GRAVE_ACCENT]  = K_GRAVE,
    [GLFW_KEY_INSERT]        = K_INSERT,
    [GLFW_KEY_DELETE]        = K_DELETE,
    [GLFW_KEY_HOME]          = K_HOME,
    [GLFW_KEY_END]           = K_END,
    [GLFW_KEY_PAGE_UP]       = K_PAGE_UP,
    [GLFW_KEY_PAGE_DOWN]     = K_PAGE_DOWN,
    [GLFW_KEY_ESCAPE]        = K_ESCAPE,
    [GLFW_KEY_BACKSPACE]     = K_BACKSPACE,
    [GLFW_KEY_ENTER]         = K_ENTER,
    [GLFW_KEY_TAB]           = K_TAB,
    [GLFW_KEY_LEFT]          = K_LEFT,
    [GLFW_KEY_RIGHT]         = K_RIGHT,
    [GLFW_KEY_UP]            = K_UP,
    [GLFW_KEY_DOWN]          = K_DOWN,
    [GLFW_KEY_F1]            = K_F1,
    [GLFW_KEY_F2]            = K_F2,
    [GLFW_KEY_F3]            = K_F3,
    [GLFW_KEY_F4]            = K_F4,
    [GLFW_KEY_F5]            = K_F5,
    [GLFW_KEY_F6]            = K_F6,
    [GLFW_KEY_F7]            = K_F7,
    [GLFW_KEY_F8]            = K_F8,
    [GLFW_KEY_F9]            = K_F9,
    [GLFW_KEY_F10]           = K_F10,
    [GLFW_KEY_F11]           = K_F11,
    [GLFW_KEY_F12]           = K_F12,
    [GLFW_KEY_LEFT_SHIFT]    = K_LEFT_SHIFT,
    [GLFW_KEY_LEFT_CONTROL]  = K_LEFT_CONTROL,
    [GLFW_KEY_LEFT_ALT]      = K_LEFT_ALT,
    [GLFW_KEY_RIGHT_SHIFT]   = K_RIGHT_SHIFT,
    [GLFW_KEY_RIGHT_CONTROL] = K_RIGHT_CONTROL,
    [GLFW_KEY_RIGHT_ALT]     = K_RIGHT_ALT,
};

const uint32 dfgame_to_native_key[] = {
    [0 ... K_LAST]           = GLFW_KEY_UNKNOWN,
    [K_SPACE]                = GLFW_KEY_SPACE,
    [K_APOSTROPHE]           = GLFW_KEY_APOSTROPHE,
    [K_COMMA]                = GLFW_KEY_COMMA,
    [K_MINUS]                = GLFW_KEY_MINUS,
    [K_PERIOD]               = GLFW_KEY_PERIOD,
    [K_FORWARD_SLASH]        = GLFW_KEY_SLASH,
    [K_0]                    = GLFW_KEY_0,
    [K_1]                    = GLFW_KEY_1,
    [K_2]                    = GLFW_KEY_2,
    [K_3]                    = GLFW_KEY_3,
    [K_4]                    = GLFW_KEY_4,
    [K_5]                    = GLFW_KEY_5,
    [K_6]                    = GLFW_KEY_6,
    [K_7]                    = GLFW_KEY_7,
    [K_8]                    = GLFW_KEY_8,
    [K_9]                    = GLFW_KEY_9,
    [K_SEMICOLON]            = GLFW_KEY_SEMICOLON,
    [K_EQUALS]               = GLFW_KEY_EQUAL,
    [K_A]                    = GLFW_KEY_A,
    [K_B]                    = GLFW_KEY_B,
    [K_C]                    = GLFW_KEY_C,
    [K_D]                    = GLFW_KEY_D,
    [K_E]                    = GLFW_KEY_E,
    [K_F]                    = GLFW_KEY_F,
    [K_G]                    = GLFW_KEY_G,
    [K_H]                    = GLFW_KEY_H,
    [K_I]                    = GLFW_KEY_I,
    [K_J]                    = GLFW_KEY_J,
    [K_K]                    = GLFW_KEY_K,
    [K_L]                    = GLFW_KEY_L,
    [K_M]                    = GLFW_KEY_M,
    [K_N]                    = GLFW_KEY_N,
    [K_O]                    = GLFW_KEY_O,
    [K_P]                    = GLFW_KEY_P,
    [K_Q]                    = GLFW_KEY_Q,
    [K_R]                    = GLFW_KEY_R,
    [K_S]                    = GLFW_KEY_S,
    [K_T]                    = GLFW_KEY_T,
    [K_U]                    = GLFW_KEY_U,
    [K_V]                    = GLFW_KEY_V,
    [K_W]                    = GLFW_KEY_W,
    [K_X]                    = GLFW_KEY_X,
    [K_Y]                    = GLFW_KEY_Y,
    [K_Z]                    = GLFW_KEY_Z,
    [K_LEFT_SQUARE_BRACKET]  = GLFW_KEY_LEFT_BRACKET,
    [K_BACK_SLASH]           = GLFW_KEY_BACKSLASH,
    [K_RIGHT_SQUARE_BRACKET] = GLFW_KEY_RIGHT_BRACKET,
    [K_GRAVE]                = GLFW_KEY_GRAVE_ACCENT,
    [K_INSERT]               = GLFW_KEY_INSERT,
    [K_DELETE]               = GLFW_KEY_DELETE,
    [K_HOME]                 = GLFW_KEY_HOME,
    [K_END]                  = GLFW_KEY_END,
    [K_PAGE_UP]              = GLFW_KEY_PAGE_UP,
    [K_PAGE_DOWN]            = GLFW_KEY_PAGE_DOWN,
    [K_ESCAPE]               = GLFW_KEY_ESCAPE,
    [K_BACKSPACE]            = GLFW_KEY_BACKSPACE,
    [K_ENTER]                = GLFW_KEY_ENTER,
    [K_TAB]                  = GLFW_KEY_TAB,
    [K_LEFT]                 = GLFW_KEY_LEFT,
    [K_RIGHT]                = GLFW_KEY_RIGHT,
    [K_UP]                   = GLFW_KEY_UP,
    [K_DOWN]                 = GLFW_KEY_DOWN,
    [K_F1]                   = GLFW_KEY_F1,
    [K_F2]                   = GLFW_KEY_F2,
    [K_F3]                   = GLFW_KEY_F3,
    [K_F4]                   = GLFW_KEY_F4,
    [K_F5]                   = GLFW_KEY_F5,
    [K_F6]                   = GLFW_KEY_F6,
    [K_F7]                   = GLFW_KEY_F7,
    [K_F8]                   = GLFW_KEY_F8,
    [K_F9]                   = GLFW_KEY_F9,
    [K_F10]                  = GLFW_KEY_F10,
    [K_F11]                  = GLFW_KEY_F11,
    [K_F12]                  = GLFW_KEY_F12,
    [K_LEFT_SHIFT]           = GLFW_KEY_LEFT_SHIFT,
    [K_LEFT_CONTROL]         = GLFW_KEY_LEFT_CONTROL,
    [K_LEFT_ALT]             = GLFW_KEY_LEFT_ALT,
    [K_RIGHT_SHIFT]          = GLFW_KEY_RIGHT_SHIFT,
    [K_RIGHT_CONTROL]        = GLFW_KEY_RIGHT_CONTROL,
    [K_RIGHT_ALT]            = GLFW_KEY_RIGHT_ALT,
};

const uint32 native_to_dfgame_mouse_button[] = {
    [0 ... GLFW_MOUSE_BUTTON_LAST] = MB_INVALID,
    [GLFW_MOUSE_BUTTON_LEFT] = MB_LEFT,
    [GLFW_MOUSE_BUTTON_MIDDLE] = MB_MIDDLE,
    [GLFW_MOUSE_BUTTON_RIGHT] = MB_RIGHT,
};

const uint32 dfgame_to_native_mouse_button[] = {
    [0 ... MB_LAST] = GLFW_MOUSE_BUTTON_LAST,
    [MB_LEFT] = GLFW_MOUSE_BUTTON_LEFT,
    [MB_MIDDLE] = GLFW_MOUSE_BUTTON_MIDDLE,
    [MB_RIGHT] = GLFW_MOUSE_BUTTON_RIGHT,
};

#endif