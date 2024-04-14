#include "../../include/scop.h"

t_vec4_float *create_mat4(t_vec4_float a, t_vec4_float b, t_vec4_float c, t_vec4_float d)
{
	t_vec4_float *vec = ft_calloc(4, sizeof(t_vec4_float));
	if (!vec) {
		ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
		return (NULL);
	}
	ft_memcpy(&vec[0], &a, sizeof(t_vec4_float));
	ft_memcpy(&vec[1], &b, sizeof(t_vec4_float));
	ft_memcpy(&vec[2], &c, sizeof(t_vec4_float));
	ft_memcpy(&vec[3], &d, sizeof(t_vec4_float));
	return (vec);
}

t_vec4_float *create_mat4_identity()
{
	t_vec4_float *vec = ft_calloc(4, sizeof(t_vec4_float));
	if (!vec) {
		ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
		return (NULL);
	}
	vec[0] = CREATE_VEC4(float, 1.0, 0.0, 0.0, 0.0);
	vec[1] = CREATE_VEC4(float, 0.0, 1.0, 0.0, 0.0);
	vec[2] = CREATE_VEC4(float, 0.0, 0.0, 1.0, 0.0);
	vec[3] = CREATE_VEC4(float, 0.0, 0.0, 0.0, 1.0);


	ft_printf_fd(1, "identity = %d\n"PINK, vec[0].x);
	DISPLAY_VEC4(float, vec[0]);
	DISPLAY_VEC4(float, vec[1]);
	DISPLAY_VEC4(float, vec[2]);
	DISPLAY_VEC4(float, vec[3]);
	ft_printf_fd(1, RESET);
	return (vec);
}
