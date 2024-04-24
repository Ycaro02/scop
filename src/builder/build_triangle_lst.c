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


void display_triangle_data(t_list *vertex_lst, t_list *idx_lst)
{
	u32 nb = 0;
	for (t_list *current = vertex_lst; current; current = current->next) {
		if (nb == 0) {
			current = current->next;
			nb++;
		}
		vec3_f32 *vec = (vec3_f32 *)current->content;
		ft_printf_fd(1, PURPLE"Vertex %u, %f %f %f", nb, (*vec)[0], (*vec)[1], (*vec)[2]);
		if (nb != 0 && nb % 3 == 0) {
			ft_printf_fd(1, "\n");
		} else {
			ft_printf_fd(1, " -> ");
		}
		nb++;
	}
	nb = 0;
	for (t_list *current = idx_lst; current; current = current->next) {
		u32 *val = current->content;
		ft_printf_fd(1, ORANGE"nb %u, |%u|"RESET,nb, *val);
		if (nb != 0 && nb % 3 == 0) {
			ft_printf_fd(1, "\n");
		} else {
			ft_printf_fd(1, " ->");
		}
		nb++;
	}
}

/*This free model->vertex */
t_list *get_all_triangle_vertex(t_obj_model *model, t_list **idx_lst_ptr) 
{
	t_list *vertex_lst = NULL;
	t_list **idx_lst = idx_lst_ptr;
	u32 idx_for_lst = 1;
	vec3_f32 *hard_origin = ft_calloc(sizeof(vec3_f32), 1);
	if (!hard_origin) {
		ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
		return (NULL);
	}
	ft_lstadd_back(&vertex_lst, ft_lstnew(hard_origin)); 
	
	for (u32 i = 0; i < model->tri_size; i++) {
		for (u32 j = 0; j < 3; j++) {
			u32 vertex_index = model->tri_face[i][j];		
			vec3_f32 *vertex = ft_calloc(sizeof(vec3_f32), 1);
			if (!vertex){
				ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
				return (NULL);
			}
//			ft_printf_fd(1, "Vertex index %u\n", vertex_index);
			ft_vec_copy(*vertex, model->vertex[vertex_index], sizeof(vec3_f32));
			ft_lstadd_back(&vertex_lst, ft_lstnew(vertex));
			u32 *idx = ft_calloc(sizeof(u32), 1);
			if (!idx) {
				ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
				return (NULL);
			}
			*idx = idx_for_lst;
			ft_lstadd_back(idx_lst, ft_lstnew(idx));
			idx_for_lst++;
		}
	}
	// display_triangle_data(vertex_lst, *idx_lst);
	free(model->vertex);
	return (vertex_lst);
}
