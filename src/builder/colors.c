#include "../../include/scop.h"

/**
 * @brief Hard build color for each triangle
 * @param model obj model
*/
u8 hard_build_color(t_obj_model *model) 
{
	vec3_f32 colors[] = {
		{1.0f, 0.0f, 0.0f},  /* Red */
		{0.0f, 1.0f, 0.0f},  /* Green */
		{0.0f, 0.0f, 1.0f},  /* Blue */
		{1.0f, 1.0f, 0.0f},  /* Yellow */
		{1.0f, 0.0f, 1.0f},  /* Magenta */
		{0.0f, 1.0f, 1.0f},  /* Cyan */
		{1.0f, 0.5f, 0.0f},  /* Orange */
		{0.5f, 0.0f, 0.5f},  /* Purple */
		{0.5f, 0.5f, 0.5f},  /* Grey */
		{0.0f, 0.5f, 0.0f},  /* Dark Green */
		{0.5f, 0.0f, 0.0f},  /* Dark Red */
		{0.0f, 0.0f, 0.5f},  /* Dark Blue */
		{0.5f, 0.5f, 0.0f},  /* Olive */
		{0.5f, 0.0f, 1.0f},  /* Violet */
		{0.0f, 0.5f, 0.5f},  /* Teal */
		{0.5f, 0.5f, 1.0f},  /* Light Blue */
		{1.0f, 0.5f, 0.5f},  /* Light Red */
		{0.5f, 1.0f, 0.5f},  /* Light Green */
		{1.0f, 1.0f, 0.5f},  /* Light Yellow */
		{0.5f, 1.0f, 1.0f},  /* Light Cyan */
		{1.0f, 0.5f, 1.0f},  /* Pink */
		{0.7f, 0.3f, 0.3f},  /* Brown */
		{0.3f, 0.7f, 0.3f},  /* Forest Green */
		{0.3f, 0.3f, 0.7f},  /* Navy Blue */
		{0.7f, 0.7f, 0.3f},  /* Khaki */
		{0.7f, 0.3f, 0.7f},  /* Fuchsia */
		{0.3f, 0.7f, 0.7f},  /* Aqua */
		{0.7f, 0.7f, 0.7f},  /* Light Grey */
		{0.3f, 0.3f, 0.3f},  /* Dark Grey */
		{0.6f, 0.4f, 0.2f}   /* Bronze */
	};
	/* Usefull trick */
    u32 num_colors = sizeof(colors) / sizeof(colors[0]);

    model->colors = malloc(model->v_size * sizeof(vec3_f32));
    if (!model->colors) {
        ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
        return (FALSE);
    }
    for (u32 i = 0; i < model->v_size; i++) {
        ft_vec_copy(model->colors[i], colors[i % num_colors], sizeof(vec3_f32));
    }

	return (TRUE);
}
