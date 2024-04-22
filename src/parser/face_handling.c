#include "../../include/scop.h"

/**
 *	@brief Convert to triangle, build new linked list with only triangle point
 *	for the Opengl render
 *	@param face_node_lst linked list of face node
 *	@return new linked list with only triangle point
*/
t_list *quadra_to_triangle(t_list *face_node_lst)
{
    t_list		*triangle_list = NULL;
	t_face_node	*node = NULL;

    for (t_list *current = face_node_lst; current; current = current->next) {
		node = current->content;
		vec3_u32 *triangle = ft_calloc(1, sizeof(vec3_u32));
		if (!triangle) {
			ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
			return (NULL);
		}
		/* save first triangle in new list */
		ft_memcpy(triangle, &node->vec, sizeof(vec3_u32));
		ft_lstadd_back(&triangle_list, ft_lstnew(triangle));
		if (node->other) {
            u32 other_size = 1; /* hardcode need to count*/
        	/* for each other value build accordate subtriangle and it to new list */
            for (u32 i = 0; i < other_size; i++) {
            	vec3_u32 *sub_triangle = ft_calloc(1, sizeof(vec3_u32));
                if (!sub_triangle) {
                    ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
                    return (NULL);
                }
				(*sub_triangle)[0] = node->vec[0];
                if (i == 0) {
                    (*sub_triangle)[1] = node->vec[2];
                    (*sub_triangle)[2] = node->other[i];
                } else  {
                    (*sub_triangle)[1] = node->other[i - 1];
                    (*sub_triangle)[2] = node->other[i];
				}
                ft_lstadd_back(&triangle_list, ft_lstnew(sub_triangle));
            }
            // free(node->other);
            // node->other = NULL;
        }
    }

	check_struct_size("vec3_u32", sizeof(vec3_u32), 12);
	check_struct_size("vec3_f32", sizeof(vec3_f32), 12);
	check_struct_size("vec3_f64", sizeof(vec3_f64), 24);
	check_struct_size("t_vec4_f32", sizeof(vec4_f32), 16);
    // for (t_list *current = triangle_list; current; current = current->next) {
    //     vec3_u32 *triangle = current->content;
    //     ft_printf_fd(1, ORANGE"Triangle: %u, %u, %u\n"RESET, triangle->x, triangle->y, triangle->z);
    // }
	return (triangle_list);
}
