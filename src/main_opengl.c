#include "../include/scop.h"

/* TO REMOVE need to implement my own parser */
#define STB_IMAGE_IMPLEMENTATION
#include "../../lib/stb/stb_image.h"

GLuint init_openGL_texture(t_obj_model* model, u8 *data, u32 width, u32 height)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	/* Set the texture wrapping parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	/* Set texture filtering parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// Set texture filtering to GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	/* Load the texture */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	GLuint texture_loc = glGetUniformLocation(model->shader_id, "texture1");
	glUniform1i(texture_loc, 0);

	return (texture);
}


/**
 * @brief Load a texture from a file
 * @param path path to the texture file
 * @return u8* return the texture data
*/
u8 *brut_load_texture(char *path, t_obj_model *model)
{
	int width, height, nrChannels;
	u8 *data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (!data) {
		ft_printf_fd(2, RED"Error: Failed to load texture\n"RESET);
		return (NULL);
	}
	ft_printf_fd(1, "Texture loaded: %s\n", path);

	// u8 *newd = resize_image(data, width, height, 128, 128); //
	ft_printf_fd(1, PINK"Width: %d, Height: %d, Channels: %d\n"RESET, width, height, nrChannels);
	init_openGL_texture(model, data, width, height);
	return (data);
}



/**
 * @brief Initialize the openGL context
 * @return GLFWwindow* return the window struct pointer
*/
GLFWwindow *init_openGL_context() 
{
    GLFWwindow *win;
	int version = 0;
	

    if (!glfwInit())
        return (NULL);

	/* Enable 8x antialiasing */
    // glfwWindowHint(GLFW_SAMPLES, 8);

    win = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Window", NULL, NULL);
    if (!win) {
        return (NULL);
    }

	/* Init openGL context on current thread */
    glfwMakeContextCurrent(win);

	/* 
	 * Load all openGL function pointer with glad
	 * gladLoaderLoadGLLoader returns the version of glad
	*/

 	if (!(version = gladLoaderLoadGL())) {
        ft_printf_fd(2, "Error: Failed to initialize Glad\n");
        glfwDestroyWindow(win);
        glfwTerminate();
        return (NULL);
    }

	ft_printf_fd(1, ORANGE"Glad Version %d.%d\n"RESET, GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

	/* Enable multisampling for antialiasing */
    glEnable(GL_MULTISAMPLE);
	/* Enable depth testing */
	glEnable(GL_DEPTH_TEST);

	/* Enable blending */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    return (win);
}


void handle_auto_rotate(t_obj_model *model)
{
	u8 x = has_flag(model->status, STATUS_ROTATE_X);
	u8 y = has_flag(model->status, STATUS_ROTATE_Y);
	u8 z = has_flag(model->status, STATUS_ROTATE_Z);

	if (!x && !y && !z) {
		set_shader_var_mat4(model->shader_id, "model", model->rotation);
		return;
	}
	if (x) {
		rotate_object_around_center(model, VEC3_ROTATEX, 2.0f, model->shader_id);
	} if (y) {
		rotate_object_around_center(model, VEC3_ROTATEY, 2.0f, model->shader_id);
	} if (z) {
		rotate_object_around_center(model, VEC3_ROTATEZ, 2.0f, model->shader_id);
	} 
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

		handle_auto_rotate(model);
		update_camera(&model->cam, model->shader_id);

		/* bind vertex array */
		glBindVertexArray(model->vao);
		glDrawElements(GL_TRIANGLES, (model->tri_size * 3), GL_UNSIGNED_INT, 0);

		/* unbind vertex array */
		glBindVertexArray(0);
		/* swap buff to display */
		glfwSwapBuffers(win);
        /* Event handling */
        glfwPollEvents();
		handle_input(model);
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

void set_shader_var_int(GLuint shader_id, char *name, int value)
{
	GLint loc = glGetUniformLocation(shader_id, name);
	glUniform1i(loc, value);
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

    win = init_openGL_context();
    if (!win) {
        ft_printf_fd(2, "Error: init_openGL_context\n");
        glfwTerminate();
        return (1);
    }

	/* Init model value */
	model->status = STATUS_ROTATE_Y;
	model->win_ptr = win;
	model->shader_id = load_shader(model);
	init_gl_triangle_array(model);
	ft_printf_fd(1, CYAN"Triangle number: %u\n"RESET, model->tri_size);

	// brut_load_texture(TEXTURE_MANDATORY_PATH, model);
	brut_load_texture(TEXTURE_BRICK_PATH, model);

	set_shader_var_int(model->shader_id, "activeTexture", 0);

    main_loop(model, win);
	glfw_destroy(win, model);
	return (0);
}