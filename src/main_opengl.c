#include "../include/scop.h"




/**
 * @brief Set key callback for the window
 * @param window 
 * @param key keycode receive from glfw
 * @param scancode unused
 * @param action action receive from glfw	
 * @param mode unused
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) 
{
	(void)scancode, (void)mode;
	t_obj_model *model = glfwGetWindowUserPointer(window);
	static u8 fill_mode = 1;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
		return ;
	}
	/* Zoom */
	if (key == GLFW_KEY_W && action >= GLFW_PRESS) {
		move_camera_forward(&model->cam, 1.0f);
	}
	/* Unzoom */ 
	else if (key == GLFW_KEY_S && action >= GLFW_PRESS) {
		move_camera_backward(&model->cam, 1.0f);
	}
	/* Rotate horizontal camera +1 */
	else if (key == GLFW_KEY_A && action >= GLFW_PRESS) {
		rotate_camera(&model->cam, CAM_MOVE_ANGLE, VEC3_ROTATEX);
	} 
	/* Rotate horizontale camera -1 */
	else if (key == GLFW_KEY_D && action >= GLFW_PRESS) {
		rotate_camera(&model->cam, -CAM_MOVE_ANGLE, VEC3_ROTATEX);
	}
	/* Up camera */ 
	else if (key == GLFW_KEY_Q && action >= GLFW_PRESS) {
		move_camera_up(&model->cam, 1.0f);
	}
	/* Down camera */
	else if (key == GLFW_KEY_E && action >= GLFW_PRESS) {
		move_camera_up(&model->cam, -1.0f);
	}
	/* Reset cam */
	else if (key == GLFW_KEY_ENTER && action >= GLFW_PRESS) {
		reset_camera(model);
	} 
	/* Display cam data */
	else if (key == GLFW_KEY_SPACE && action >= GLFW_PRESS) {
		display_camera_value(&model->cam);
	} 
	/* Change polygon mode */
	else if (key == GLFW_KEY_P && action >= GLFW_PRESS) {
		fill_mode = !fill_mode;
		/* This working cause GL_FILL is GL_LINE + 1*/
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE + fill_mode);
	} 
	/* Rotate object left*/
	else if (key == GLFW_KEY_LEFT && action >= GLFW_PRESS) {
		rotate_object_around_center(model, VEC3_ROTATEX, ROTATE_ANGLE, model->shader_id);
	} 
	/* Rotate object right*/
	else if (key == GLFW_KEY_RIGHT && action >= GLFW_PRESS) {
		rotate_object_around_center(model, VEC3_ROTATEX, -ROTATE_ANGLE, model->shader_id);
	} 
	/* Rotate object up*/
	else if (key == GLFW_KEY_UP && action >= GLFW_PRESS) {
		rotate_object_around_center(model, VEC3_ROTATEY, ROTATE_ANGLE, model->shader_id);
	} 
	/* Rotate object down*/
	else if (key == GLFW_KEY_DOWN && action >= GLFW_PRESS) {
		rotate_object_around_center(model, VEC3_ROTATEY, -ROTATE_ANGLE, model->shader_id);
	} 
	/* Rotate object Z up */
	else if (key == GLFW_KEY_PAGE_UP && action >= GLFW_PRESS) {
		rotate_object_around_center(model, VEC3_ROTATEZ, ROTATE_ANGLE, model->shader_id);
	} 
	/* Rotate object Z down */
	else if (key == GLFW_KEY_PAGE_DOWN && action >= GLFW_PRESS) {
		rotate_object_around_center(model, VEC3_ROTATEZ, -ROTATE_ANGLE, model->shader_id);
	} else if (key == GLFW_KEY_R && action >= GLFW_PRESS) {
		reverse_flag(&model->status, STATUS_ROTATE_X);
	}
}

/**
 * @brief Initialize the openGL context
 * @param model model structure
 * @return GLFWwindow* return the window struct pointer
*/
GLFWwindow *init_openGL_context(t_obj_model *model) 
{
    GLFWwindow *win;
	int version = 0;

    if (!glfwInit())
        return (NULL);

	/* Enable 4x antialiasing */
    glfwWindowHint(GLFW_SAMPLES, 8);

    win = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Window", NULL, NULL);
    if (!win) {
        return (NULL);
    }
    glfwMakeContextCurrent(win);

	/* Set the user pointer to the model, can get it with glfwGetWindowUserPointer(win); */
	glfwSetWindowUserPointer(win, model);
	glfwSetKeyCallback(win, key_callback);

 	if (!(version = gladLoaderLoadGL())) {
        ft_printf_fd(2, "Error: Failed to initialize Glad\n");
        glfwDestroyWindow(win);
        glfwTerminate();
        return NULL;
    }

	/* Enable multisampling for antialiasing */
    glEnable(GL_MULTISAMPLE);
	/* Enable depth testing */
	glEnable(GL_DEPTH_TEST);

	/* Enable blending */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	// glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	ft_printf_fd(1, "Glad Version %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    return (win);
}

/**
 * @brief Main loop of the program
 * @param model model structure
 * @param win window structure
*/
void main_loop(t_obj_model *model, GLFWwindow *win) 
{
    while (!glfwWindowShouldClose(win)) {
        /* clear gl render context*/
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (has_flag(model->status, STATUS_ROTATE_X)) {
			rotate_object_around_center(model, VEC3_ROTATEX, 2.0f, model->shader_id);
		} else {
			set_shader_var_mat4(model->shader_id, "model", model->rotation);
		}
		/* Use the shader */
		glUseProgram(model->shader_id); /* useless ? */
		update_camera(&model->cam, model->shader_id);
		glBindVertexArray(model->vao);
		glDrawElements(GL_TRIANGLES, (model->tri_size * 3), GL_UNSIGNED_INT, 0);
		/* unbind vertex array */
		glBindVertexArray(0);
		/* swap buff to display */
		glfwSwapBuffers(win);
        /* Event handling */
        glfwPollEvents();
    }
}

/**
 * @brief Destroy glfw and free the model
 * @param win window struct
 * @param model model struct
*/
static void glfw_destroy(GLFWwindow *win, t_obj_model *model)
{

	ft_printf_fd(2, RED"Destroying glfw\n"RESET);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &model->vao);
	glDeleteProgram(model->shader_id);
	gladLoaderUnloadGL();
	glfwDestroyWindow(win);
	glfwTerminate();
	free_obj_model(model);
}

int main(int argc, char **argv)
{
    GLFWwindow	*win;
	t_obj_model	*model;
    

	if (argc != 2) { /* need to check file */
		ft_printf_fd(2, "Usage: %s <obj_file>\n", argv[0]);
		return (1);
	}

	model = parse_obj_file(argv[1]);
	if (!model) {
		ft_printf_fd(2, "Error parse 42.obj\n");
		return (1);
	}

	/* init model rotation mat4 */
	mat4_identity(model->rotation);

	/* Init camera structure */
	model->cam = create_camera(45.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	model->status = STATUS_ROTATE_X;

    win = init_openGL_context(model);
    if (!win) {
        ft_printf_fd(2, "Error: init_openGL_context\n");
        glfwTerminate();
        return (1);
    }

	model->shader_id = load_shader(model);
	init_gl_triangle_array(model);
	ft_printf_fd(1, "tri_size: %u\n", model->tri_size);

	/* set this with event handle */
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    main_loop(model, win);
	glfw_destroy(win, model);
	return (0);
}
