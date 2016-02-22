#include <dfgame-game-front.h>

GLFWwindow* window;

void loop(float delta)
{
    glClear(GL_COLOR_BUFFER_BIT);
    if(glfwWindowShouldClose(window))
        main_loop_request_end();
    glfwSwapBuffers(window);
}

int main()
{
    window = create_main_window(1024, 768, "DFGame Project");

    main_loop_begin(loop);

    cleanup_main_window();
    return 0;
}
