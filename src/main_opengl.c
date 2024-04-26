#include "../include/scop.h"
#include "parser/parse_bmp.h"

GLuint init_openGL_texture(t_obj_model* model, u8 *data, u32 width, u32 height, u16 texture_type)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	/* Set the texture wrapping parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	/* Set texture filtering parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// Set texture filtering to GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	GLfloat anisotropy_level = 8.0f;  // adjust this value to your preference
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, anisotropy_level);
	/* Load the texture */
	glTexImage2D(GL_TEXTURE_2D, 0, texture_type, width, height, 0, texture_type, GL_UNSIGNED_BYTE, data);
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
	int width, height, type;
	// u8 *data = stbi_load(path, &width, &height, type, 0);
	u8 *data = parse_bmp_file(path, &width, &height, &type);


	if (!data) {
		ft_printf_fd(2, RED"Error: Failed to load texture\n"RESET);
		return (NULL);
	}
	// ft_printf_fd(1, "Texture loaded: %s\n", path);
	// ft_printf_fd(1, PINK"Width: %d, Height: %d, Channels: %d\n"RESET, width, height, *type);
	if (type == 3) {
		type = GL_RGB;
	} else if (type == 4) {
		type = GL_RGBA;
	}
	init_openGL_texture(model, data, width, height, type);
	return (data);
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


int main(int argc, char **argv)
{
    GLFWwindow	*win;
	t_obj_model	*model;
    

	if (argc < 2 || argc > 3) { /* need to check file */
		ft_printf_fd(2, "Usage: %s <obj_file> [texture_path]\n", argv[0]);
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
	model->cam = create_camera(70.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

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
	model->tex_intensity = 0.0f;
	init_gl_triangle_array(model);
	ft_printf_fd(1, CYAN"Triangle number: %u\n"RESET, model->tri_size);

	brut_load_texture(argv[2], model);
	set_shader_var_float(model->shader_id, "textureIntensity", model->tex_intensity);
    main_loop(model, win);
	glfw_destroy(win, model);
	return (0);
}