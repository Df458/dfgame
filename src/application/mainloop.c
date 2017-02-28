// Log category, used to filter logs
#define LOG_CATEGORY "Application"

#include "mainloop.h"

#include "memory/alloc.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Represents a loop manually-interruptable loop
typedef struct mainloop {
    bool is_running;
    bool end_requested;
    loop_func func;
}* mainloop;

// Creates a new mainloop with the given content
mainloop mainloop_new(loop_func func) {
    mainloop loop = salloc(sizeof(mainloop));
    loop->is_running = false;
    loop->end_requested = false;
    loop->func = func;

    return loop;
}

// Frees the mainloop. NOTE: Don't call this function. Use the macro without
// the leading _ instead, as it also NULLs your pointer.
void _mainloop_free(mainloop loop) {
    sfree(loop);
}

// Runs the mainloop synchronously.
void mainloop_run(mainloop loop) {
    loop->is_running = true;
    float dt = 0;

    glfwSetTime(0);
    while(!loop->end_requested) {
        dt = glfwGetTime();
        glfwSetTime(0);
        glfwPollEvents();
        loop->func(loop, dt);
    }

    loop->is_running = false;
}

// Tells the loop to not continue.
void mainloop_request_end(mainloop loop) {
    loop->end_requested = true;
}

// Gets whether or not the loop is running.
bool mainloop_is_running(mainloop loop) {
    return loop->is_running;
}
