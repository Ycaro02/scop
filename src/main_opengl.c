#include "../include/scop.h"

void check_struct_size(char *str_test, u32 struct_size, u32 wanted_size)
{
	if (struct_size == wanted_size) {
		ft_printf_fd(1, GREEN"sizeof(%s) == %u)\n"RESET, str_test, wanted_size);
	} else {
		ft_printf_fd(1, RED"sizeof(%s) != %u)\n"RESET, str_test, wanted_size);
	}
}

void display_camera_value(t_camera *cam)
{
	ft_printf_fd(1, CYAN"Camera position: %f %f %f\n", cam->position[0], cam->position[1], cam->position[2]);
	ft_printf_fd(1, "Camera target: %f %f %f\n", cam->target[0], cam->target[1], cam->target[2]);
	ft_printf_fd(1, "Camera up: %f %f %f\n", cam->up[0], cam->up[1], cam->up[2]);
	ft_printf_fd(1, "Camera view: \n");
	for (u32 i = 0; i < 4; i++) {
		ft_printf_fd(1, "%f %f %f %f\n", cam->view[i][0], cam->view[i][1], cam->view[i][2], cam->view[i][3]);
	}
	ft_printf_fd(1, "Camera projection: \n");
	for (u32 i = 0; i < 4; i++) {
		ft_printf_fd(1, "%f %f %f %f\n", cam->projection[i][0], cam->projection[i][1], cam->projection[i][2], cam->projection[i][3]);
	}
	ft_printf_fd(1, RESET);
}




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
	}


}

GLFWwindow *init_openGL_context(t_obj_model *model) 
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

	/* Set the user pointer to the model, can get it with glfwGetWindowUserPointer(win); */
	glfwSetWindowUserPointer(win, model);

	glfwSetKeyCallback(win, key_callback);

 	if (!(version = gladLoaderLoadGL())) {
        ft_printf_fd(2, "Error: Failed to initialize Glad\n");
        glfwDestroyWindow(win);
        glfwTerminate();
        return NULL;
    }

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	ft_printf_fd(1, "GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    return (win);
}


void main_loop(t_obj_model *model, GLFWwindow *win) 
{
    while (!glfwWindowShouldClose(win)) {
        /* clear gl render context*/
        glClear(GL_COLOR_BUFFER_BIT);

		// auto_y_rotate(model->cam.model, 0.02f);


		/* 
		if (m->rotate) {
			rotate_object_around_center(model, VEC3_ROTATEX, 2.0f, model->shader_id);
		} else {
			set_shader_var_mat4(model->shader_id, "model", model->rotation);
		}
		*/
		
		rotate_object_around_center(model, VEC3_ROTATEX, 2.0f, model->shader_id);
	    // set_shader_var_mat4(model->shader_id, "model", model->rotation);

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

static void glfw_destroy(GLFWwindow *win)
{
	gladLoaderUnloadGL();
	glfwDestroyWindow(win);
	glfwTerminate();
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
	mat_identity(model->rotation);

	/* Init camera structure */
	model->cam = create_camera(45.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

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
	free_obj_model(model);
	glfw_destroy(win);
	return (0);
}
