#include "../../include/scop.h"


/**
 * @brief Vertex list to array
 * @param lst vertex list
 * @param lst_size size of the list
 * @return allocated t_vec3_float array, NULL if fail
*/
t_vec3_float	*vertex_list_toarray(t_list *lst, u32 lst_size) 
{
	u32 i = 0;
	t_vec3_float *array = ft_calloc(sizeof(t_vec3_float), lst_size);

	if (!array) {
		ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
		return (NULL);
	}
	for (t_list *current = lst; current; current = current->next) {
		ft_memcpy(&array[i], current->content, sizeof(t_vec3_float));
		i++;
	}
	return (array);
}

/**
 * @brief Triangle list to array
 * @param lst triangle list
 * @param lst_size size of the list
 * @return allocated t_vec3_u32 array, NULL if fail
*/
t_vec3_u32	*triangle_list_to_array(t_list *lst, u32 lst_size) 
{
	u32 i = 0;
	t_vec3_u32 *array = ft_calloc(sizeof(t_vec3_u32), lst_size);

	if (!array) {
		ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
		return (NULL);
	}
	for (t_list *current = lst; current; current = current->next) {
		ft_memcpy(&array[i], current->content, sizeof(t_vec3_u32));
		i++;
	}
	return (array);
}


/**
 * @brief Display vertex list
 * @param lst vertex list
*/
void  display_vertex_lst(t_list *lst)
{
	ft_printf_fd(1, "Vertex list\n");
	for (t_list *current = lst; current; current = current->next) {
		t_vec3_float vec = *(t_vec3_float *)current->content;
		DISPLAY_VEC3(float, vec)
	}
}

/**
 * @brief Free obj model
 * @param model obj model to free
*/
void free_obj_model(t_obj_model *model)
{
	if (model->vertex) {
		free(model->vertex);
	}
	if (model->tri_face) {
		free(model->tri_face);
	}
	free(model);
}

/**
 * @brief Init obj model
 * @param obj_file obj file data structure
 * @return pointer on allocated t_obj_model
 * 
*/
t_obj_model *init_obj_model(t_obj_file *obj_file)
{
	t_obj_model *model = ft_calloc(1, sizeof(t_obj_model));
	if (!model) {
		ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
		free_obj_file(obj_file);
		return (NULL);
	}

	model->vertex = vertex_list_toarray(obj_file->vertex, ft_lstsize(obj_file->vertex));
	model->v_size = ft_lstsize(obj_file->vertex);

	t_list *triangle_lst = quadra_to_triangle(obj_file->face);
	if (!triangle_lst) {
		ft_printf_fd(2, RED"Error: Failed to convert quadra to triangle\n"RESET);
		free_obj_file(obj_file);
		return (NULL);
	}

	model->tri_face = triangle_list_to_array(triangle_lst, ft_lstsize(triangle_lst));
	model->tri_size = ft_lstsize(triangle_lst);


	free_obj_file(obj_file);
	return (model);
}

void print_vertex_data(t_obj_model *model) {
    t_vec3_float* bufferData = malloc(sizeof(t_vec3_float) * model->v_size);

    glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(t_vec3_float) * model->v_size, bufferData);

    for (u32 i = 0; i < model->v_size; i++) {
        ft_printf_fd(1, ORANGE"Vertex %u: x = %f, y = %f, z = %f\n"RESET, i, bufferData[i].x, bufferData[i].y, bufferData[i].z);
    }
    free(bufferData);
}

void init_gl_vertex_buffer(t_obj_model *model)
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec3_float) * model->v_size
		, model->vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(t_vec3_float), (void*)0);
	print_vertex_data(model);
}

static void check_struct_size(char *str_test, u32 struct_size, u32 wanted_size)
{
	if (struct_size == wanted_size) {
		ft_printf_fd(1, GREEN"sizeof(%s) == %u)\n"RESET, str_test, wanted_size);
	} else {
		ft_printf_fd(1, RED"sizeof(%s) != %u)\n"RESET, str_test, wanted_size);
	}
}


/* TOTEST */
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

    // for (t_list *current = triangle_list; current; current = current->next) {
    //     t_vec3_u32 *triangle = current->content;
    //     ft_printf_fd(1, ORANGE"Triangle: %u, %u, %u\n"RESET, triangle->x, triangle->y, triangle->z);
    // }
	return (triangle_list);
}
