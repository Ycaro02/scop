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
		t_vec3_u32 *triangle = ft_calloc(1, sizeof(t_vec3_u32));
		if (!triangle) {
			ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
			return (NULL);
		}
		/* save first triangle in new list */
		ft_memcpy(triangle, &node->vec, sizeof(t_vec3_u32));
		ft_lstadd_back(&triangle_list, ft_lstnew(triangle));
		if (node->other) {
            u32 other_size = 1; /* hardcode need to count*/
        	/* for each other value build accordate subtriangle and it to new list */
            for (u32 i = 0; i < other_size; i++) {
            	t_vec3_u32 *sub_triangle = ft_calloc(1, sizeof(t_vec3_u32));
                if (!sub_triangle) {
                    ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
                    return (NULL);
                }
				sub_triangle->x = node->vec.x;
                if (i == 0) {
                    sub_triangle->y = node->vec.z;
                    sub_triangle->z = node->other[i];
                } else  {
                    sub_triangle->y = node->other[i - 1];
                    sub_triangle->z = node->other[i];
				}
                ft_lstadd_back(&triangle_list, ft_lstnew(sub_triangle));
            }
            free(node->other);
            node->other = NULL;
        }
    }

	check_struct_size("t_vec3_u32", sizeof(t_vec3_u32), 12);
	check_struct_size("t_vec3_float", sizeof(t_vec3_float), 12);
	check_struct_size("t_vec3_double", sizeof(t_vec3_double), 24);
	check_struct_size("t_vec4_float", sizeof(t_vec4_float), 16);
    // for (t_list *current = triangle_list; current; current = current->next) {
    //     t_vec3_u32 *triangle = current->content;
    //     ft_printf_fd(1, ORANGE"Triangle: %u, %u, %u\n"RESET, triangle->x, triangle->y, triangle->z);
    // }
	return (triangle_list);
}
