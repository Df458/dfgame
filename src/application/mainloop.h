#ifndef DF_APPLICATION_MAINLOOP
#define DF_APPLICATION_MAINLOOP

#include "mainloop.hd"

// Creates a new mainloop with the given content.
mainloop mainloop_new(loop_func func);

// Frees the mainloop and sets it to NULL to make it harder to double-free.
#define mainloop_free(loop) { _mainloop_free(loop); loop = NULL; }

// Frees the mainloop. NOTE: Don't call this function. Use the macro without
// the leading _ instead, as it also NULLs your pointer.
void _mainloop_free(mainloop loop);

// Runs the mainloop synchronously.
void mainloop_run(mainloop loop);

// Tells the loop to not continue.
void mainloop_request_end(mainloop loop);

// Gets whether or not the loop is running.
bool mainloop_is_running(mainloop loop);

#endif
