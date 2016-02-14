#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

#include "loop.h"

loop_func iter;
bool continue_loop;
float dt;

void main_loop_begin(loop_func f)
{
    continue_loop = true;
    dt = 0;

    glfwSetTime(0);
    while(continue_loop) {
        dt = glfwGetTime();
        glfwSetTime(0);
        glfwPollEvents();
        iter(dt);
    }
}

void main_loop_request_end()
{
    continue_loop = false;
}
