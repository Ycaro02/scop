#include "../include/glfw3.h"
#include "../libft/libft.h"

GLFWwindow *init_glfw() 
{
    GLFWwindow *win;

    if (!glfwInit())
        return (NULL);
    win = glfwCreateWindow(640, 480, "OpenGL Window", NULL, NULL);
    if (!win) {
        glfwTerminate();
        return (NULL);
    }
    glfwMakeContextCurrent(win);
    return (win);
}

static void main_loop(GLFWwindow *win) 
{
    while (!glfwWindowShouldClose(win)) {
        /* clear gl render context*/
        glClear(GL_COLOR_BUFFER_BIT);
        /* Swap display buff with bg buff*/
        glfwSwapBuffers(win);
        /* Event */
        glfwPollEvents();
    }
}

int main(int argc, char **argv)
{
    GLFWwindow *win;
    
    win = init_glfw();
    if (!win) {
        ft_printf_fd(2, "Error: Failed to init glfw\n");
        return (1);
    }
    main_loop(win);
    glfwTerminate();
}
