#ifndef APPLICATION_GDK_INPUT_H
#define APPLICATION_GDK_INPUT_H
#include "application/input.h"

#include <gdk/gdk.h>

bool on_button_press(GdkEventButton* event);
bool on_button_release(GdkEventButton* event);
bool on_key_press(GdkEventKey* event);
bool on_key_release(GdkEventKey* event);
bool on_mouse_motion(GdkEventMotion* event);
bool on_scroll(GdkEventScroll* event);
void on_enter(double x, double y);

#endif
