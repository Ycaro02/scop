#include "../include/scop.h"

void check_struct_size(char *str_test, u32 struct_size, u32 wanted_size)
{
	if (struct_size == wanted_size) {
		ft_printf_fd(1, GREEN"sizeof(%s) == %u)\n"RESET, str_test, wanted_size);
	} else {
		ft_printf_fd(1, RED"sizeof(%s) != %u)\n"RESET, str_test, wanted_size);
	}
}

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

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	// glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// glClearColor(200.0f, 0.0f, 0.0f, 1.0f);
	ft_printf_fd(1, "GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    return (win);
}



void main_loop(t_obj_model *model, GLFWwindow *win) 
{
    while (!glfwWindowShouldClose(win)) {
        /* clear gl render context*/
        glClear(GL_COLOR_BUFFER_BIT);

		/* Camera view need to work on */
		set_shader_var_vec4(model->shader_id, "myColor", (t_vec4_float){0.0f, 0.7f, 0.7f, 1.0f});

		glUseProgram(model->shader_id);

		glBindVertexArray(model->vao);
	
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ebo);
	
		glDrawElements(GL_TRIANGLES, (model->tri_size * 3), GL_UNSIGNED_INT, 0);
		
		/* unbind vertex array */
		glBindVertexArray(0);
        
		/* swap buff to display */
		glfwSwapBuffers(win);
        /* Event */
        glfwPollEvents();
    }
}

static void glfw_destroy(GLFWwindow *win)
{
	glfwDestroyWindow(win);
	glfwTerminate();
}

int main(int argc, char **argv)
{
    GLFWwindow *win;
    
	if (argc != 2) { /* need to check file */
		ft_printf_fd(2, "Usage: %s <obj_file>\n", argv[0]);
		return (1);
	}

	t_obj_model *model = parse_obj_file(argv[1]);

	if (!model) {
		ft_printf_fd(2, "Error parse 42.obj\n");
		return (1);
	}
    win = init_glfw();
    if (!win) {
        ft_printf_fd(2, "Error: Failed to init glfw\n");
        glfwTerminate();
        return (1);
    }

	// init_gl_vertex_buffer(model);
	// init_gl_index_buffer(model);
	// draw_obj_model(model->tri_size * 3);
	model->shader_id = load_shader(model);
	init_gl_triangle_array(model);
	ft_printf_fd(1, "tri_size: %u\n", model->tri_size);


	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(model->shader_id);
	t_camera cam = create_camera(45.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	update_camera(&cam, model->shader_id);


    main_loop(model, win);
	free_obj_model(model);
	glfw_destroy(win);
	return (0);
}
