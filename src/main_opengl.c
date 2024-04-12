#include "../include/scop.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	(void)scancode, (void)mode;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

GLFWwindow *init_glfw() 
{
    GLFWwindow *win;
	int version = 0;


    if (!glfwInit())
        return (NULL);
    win = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Window", NULL, NULL);
    if (!win) {
        return (NULL);
    }
    glfwMakeContextCurrent(win);

	glfwSetKeyCallback(win, key_callback);

 	if (!(version = gladLoaderLoadGL())) {
        ft_printf_fd(2, "Error: Failed to initialize Glad\n");
        glfwDestroyWindow(win);
        glfwTerminate();
        return NULL;
    }

	// gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(200.0f, 0.0f, 0.0f, 1.0f);
	ft_printf_fd(1, "GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
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
    
	(void)argc, (void)argv;

	if (!parse_obj_file("rsc/42.obj")) {
		ft_printf_fd(2, "Error parse 42.obj\n");
		return (1);
	}



    win = init_glfw();
    if (!win) {
        ft_printf_fd(2, "Error: Failed to init glfw\n");
        glfwTerminate();
        return (1);
    }
    main_loop(win);
	glfwDestroyWindow(win);
    glfwTerminate();
}
