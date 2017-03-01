#include "log/log.h"
#include "mainloop.h"
#include "window.h"

GLFWwindow* win;

bool loop(mainloop l, float dt) {
    glClear(GL_COLOR_BUFFER_BIT);
    info("%f", dt);
    glfwSwapBuffers(win);
    return !glfwWindowShouldClose(win);
}

int main(int argc, char** argv) {
    win = window_new_default(1280, 720, "Test Window");

    mainloop l = mainloop_new(loop);
    mainloop_run(l);
    mainloop_free(l);

    window_free_final(win);

    return 0;
}
