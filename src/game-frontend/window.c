#include "window.h"
#include "../common/util.h"

GLFWwindow* main_window = 0;
GLFWframebuffersizefun user_size_func = 0;

void sizeCallback(GLFWwindow* win, int width, int height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    if(user_size_func)
        user_size_func(win, width, height);
}

// TODO: Make the opengl version more dynamic
GLFWwindow* create_main_window(uint16_t width, uint16_t height, const char* title)
{
    if(!glfwInit()) {
        fatal("Failed to intialize GLFW.");
    }

    if(main_window) {
        error("Trying to create a main window, but one already exists");
        return main_window;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    main_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if(!main_window)
        fatal("Failed to create the main window");
    glfwSetFramebufferSizeCallback(main_window, sizeCallback);
    glfwMakeContextCurrent(main_window);

	glewExperimental = 1;
	if(glewInit() != GLEW_OK) {
        fatal("Failed to initialize GLEW");
    }
    glGetError(); // Because GLEW is silly. <http://stackoverflow.com/questions/20034615/why-does-glewinit-result-in-gl-invalid-enum-after-making-some-calls-to-glfwwin>
    GLuint VAO; // FIXME: Stupid Hack. <http://stackoverflow.com/questions/13403807/glvertexattribpointer-raising-gl-invalid-operation>
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    return main_window;
}

GLFWwindow* get_main_window()
{
    if(!main_window)
        warn("Can't get the main window, it doesn't exist.");

    return main_window;
}

void set_window_size_func(GLFWframebuffersizefun func)
{
    user_size_func = func;
}

void cleanup_main_window()
{
    glfwDestroyWindow(main_window);
    main_window = NULL;
    glfwTerminate();
}
