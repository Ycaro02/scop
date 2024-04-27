#include "../include/scop.h"

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
	model->tex_intensity = 0.0f;
	init_gl_triangle_array(model);
	ft_printf_fd(1, CYAN"Triangle number: %u\n"RESET, model->tri_size);

	if (argc == 3) {
		brut_load_texture(argv[2], model);
	} else {
		brut_load_texture(TEXTURE_BRICK_PATH, model);
	}
	set_shader_var_float(model->shader_id, "textureIntensity", model->tex_intensity);
    main_loop(model, win);
	glfw_destroy(win, model);
	return (0);
}