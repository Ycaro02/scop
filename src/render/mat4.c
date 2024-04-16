#include "../../include/scop.h"

/**
 * @brief Create a 4x4 matrix from 4 vectors of 4 float 
 * @return pointer on allocated vec4_float
*/
// vec4_float *create_mat4(vec4_float a, vec4_float b, vec4_float c, vec4_float d)
// {
// 	vec4_float *vec = ft_calloc(4, sizeof(vec4_float));
// 	if (!vec) {
// 		ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
// 		return (NULL);
// 	}
// 	ft_memcpy(&vec[0], &a, sizeof(vec4_float));
// 	ft_memcpy(&vec[1], &b, sizeof(vec4_float));
// 	ft_memcpy(&vec[2], &c, sizeof(vec4_float));
// 	ft_memcpy(&vec[3], &d, sizeof(vec4_float));
// 	return (vec);
// }

/**
 * @brief Create a 4x4 identity matrix
 * @return pointer on allocated vec4_float
*/
// vec4_float *create_mat4_identity()
// {
// 	vec4_float *vec = ft_calloc(4, sizeof(vec4_float));
// 	if (!vec) {
// 		ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
// 		return (NULL);
// 	}
// 	vec[0] = CREATE_VEC4(float, 1.0, 0.0, 0.0, 0.0);
// 	vec[1] = CREATE_VEC4(float, 0.0, 1.0, 0.0, 0.0);
// 	vec[2] = CREATE_VEC4(float, 0.0, 0.0, 1.0, 0.0);
// 	vec[3] = CREATE_VEC4(float, 0.0, 0.0, 0.0, 1.0);

// 	return (vec);
// }
