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

// void init
