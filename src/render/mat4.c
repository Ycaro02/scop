#include "../../include/scop.h"

/**
 * @brief Create a 4x4 matrix from 4 vectors of 4 float 
 * @return pointer on allocated vec4_f32
*/
// vec4_f32 *create_mat4(vec4_f32 a, vec4_f32 b, vec4_f32 c, vec4_f32 d)
// {
// 	vec4_f32 *vec = ft_calloc(4, sizeof(vec4_f32));
// 	if (!vec) {
// 		ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
// 		return (NULL);
// 	}
// 	ft_memcpy(&vec[0], &a, sizeof(vec4_f32));
// 	ft_memcpy(&vec[1], &b, sizeof(vec4_f32));
// 	ft_memcpy(&vec[2], &c, sizeof(vec4_f32));
// 	ft_memcpy(&vec[3], &d, sizeof(vec4_f32));
// 	return (vec);
// }

/**
 * @brief Create a 4x4 identity matrix
 * @return pointer on allocated vec4_f32
*/
// vec4_f32 *create_mat4_identity()
// {
// 	vec4_f32 *vec = ft_calloc(4, sizeof(vec4_f32));
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
