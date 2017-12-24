#ifndef APPLICATION_GDK_INPUT_H
#define APPLICATION_GDK_INPUT_H
#include <gdk/gdk.h>

#include "input.h"

bool on_button_press(GdkEventButton* event);
bool on_button_release(GdkEventButton* event);
bool on_key_press(GdkEventKey* event);
bool on_key_release(GdkEventKey* event);
bool on_mouse_motion(GdkEventMotion* event);
bool on_scroll(GdkEventScroll* event);

#endif
