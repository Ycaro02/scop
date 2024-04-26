#include "../../include/scop.h"

/**
 * @brief Display vertex list
 * @param lst vertex list
*/
void  display_vertex_lst(t_list *lst)
{
	ft_printf_fd(1, "Vertex list\n");
	for (t_list *current = lst; current; current = current->next) {
		vec3_f32 *vec = (vec3_f32 *)current->content;
		VECTOR_FLOAT_DISPLAY(3, (*vec))
	}
}

/**
 * @brief [ DEBUG ] Print element bufer data (triangle give to opengl)
*/
void print_elem_data(t_obj_model *model) {
	vec3_u32* bufferData = malloc(sizeof(vec3_u32) * model->tri_size);

	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(vec3_u32) * model->tri_size, bufferData);

	for (u32 i = 0; i < model->tri_size; i++) {
		ft_printf_fd(1, PINK"Element %u: x = %u, y = %u, z = %u\n"RESET, i, bufferData[i][0], bufferData[i][1], bufferData[i][2]);
	}
	free(bufferData);
}

void print_vertex_data(t_obj_model *model) {
    vec3_f32* bufferData = malloc(sizeof(vec3_f32) * model->v_size);

    glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3_f32) * model->v_size, bufferData);

    for (u32 i = 0; i < model->v_size; i++) {
        ft_printf_fd(1, ORANGE"Vertex %u: x = %f, y = %f, z = %f\n"RESET, i, bufferData[i][0], bufferData[i][1], bufferData[i][2]);
    }
    free(bufferData);
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
	if (model->colors) {
		free(model->colors);
	}
	if (model->texture_coord) {
		free(model->texture_coord);
	}
	if (model->obj_face) {
		free_obj_face_lst(&model->obj_face);
	}
	if (model->material) {
		if (model->material->newmtl) {
			free(model->material->newmtl);
		}
		free(model->material);
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
	/* First vertex array, contain only vertex file data */
	model->vertex = list_to_array(obj_file->vertex, ft_lstsize(obj_file->vertex), sizeof(vec3_f32));
	model->v_size = ft_lstsize(obj_file->vertex);

	/* Cut face to get triangle list of vec3_u32 represent all triangle vertex index */
	t_list *triangle_lst = quadra_to_triangle(obj_file->face);
	if (!triangle_lst) {
		ft_printf_fd(2, RED"Error: Failed to convert quadra to triangle\n"RESET);
		free_obj_file(obj_file);
		return (NULL);
	}

	/* Convert this triangle list into array */
	model->tri_face = list_to_array(triangle_lst, ft_lstsize(triangle_lst), sizeof(vec3_u32));
	model->tri_size = ft_lstsize(triangle_lst);

	/* Build new vertex/index list with all triangle */
	t_list	*idx_lst = NULL;
	t_list	*vertex_lst = get_all_triangle_vertex(model, &idx_lst);
	u32		vertex_size = ft_lstsize(vertex_lst);

	/* Convert vertex_lst to new vertex array */
	// free(model->vertex);
	model->vertex = list_to_array(vertex_lst, vertex_size, sizeof(vec3_f32));
	model->v_size = ft_lstsize(vertex_lst);


	/* Convert idx_lst to new tri_face array */
	free(model->tri_face);
	model->tri_face = list_to_array(idx_lst, ft_lstsize(idx_lst), sizeof(u32));
	model->tri_size = ft_lstsize(triangle_lst);

	/* Need to build face data structure contain all new vertex/index  */
	model->obj_face = get_all_face_vertex(obj_file, model);
	// (void)test;
	// free(model->vertex);
	// model->vertex = list_to_array(test->, vertex_size, sizeof(vec3_f32));


	ft_lstclear(&idx_lst, free);
	ft_lstclear(&vertex_lst, free);
	ft_lstclear(&triangle_lst, free);
	free_obj_file(obj_file);
	return (model);
}

/**
 *	@brief Get random number % max
 *	@param max max value
*/
u32     gener_u32(u32 max)
{
    u32 res = 0;
	int fd = open("/dev/urandom", O_RDONLY); 
	if (fd != -1) {
		char buff[4] = {0};
		int len = read(fd, &buff, 4);
		if (len != -1) {
            res = (*(u32 *)buff);
            close(fd);
		}
	}
	return (res % max);
}

/**
 * @brief Create VBO
 * @param size size of the buffer
 * @param data data to fill the buffer
 * @return vbo uint value
*/
GLuint create_VBO(u32 size, void *data)
{
	GLuint	vbo;

	/* create and fill vbo */
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	return (vbo);
}

/**
 * @brief Init gl triangle array
 * @param model obj model
 * @return vao uint value
*/
void init_gl_triangle_array(t_obj_model *model)
{
	/* Create new vao Vertex Array Object */
    glGenVertexArrays(1, &model->vao);
    glBindVertexArray(model->vao);

	/* init Vertex Buffer Object */
	model->vbo = create_VBO((sizeof(vec3_f32) * model->v_size), model->vertex);

	/* Hard build color for each vertex */
	build_material_texture(model);

	hard_build_color(model);
	GLuint color_vbo = create_VBO(model->v_size * sizeof(vec3_f32), model->colors);

	GLuint texCoords_vbo = create_VBO(model->v_size * sizeof(vec2_f32), model->texture_coord);

    /* create and fill ebo Element Buffer Objects */
    glGenBuffers(1, &model->ebo);
	
	/* Bind EBO to GL element array */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, TRIANGLE_DSIZE(model), model->tri_face, GL_STATIC_DRAW);
	// print_elem_data(model);

	/* Bind the vertex buffer */
	glBindBuffer(GL_ARRAY_BUFFER, model->vbo);
	/* Config new vertex attr for vertices */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3_f32), (void*)0);
	glEnableVertexAttribArray(0);  /* Enable vertex attr */
	
	/* Bind the color buffer */
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	
	/* Config new vertex attr for colors */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3_f32), (void*)0);
	glEnableVertexAttribArray(1);  /* Enable color attr */
	// print_vertex_data(model);

	/* Config texture attr for vertex */
	glBindBuffer(GL_ARRAY_BUFFER, texCoords_vbo);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vec2_f32), (void*)0);
	glEnableVertexAttribArray(2);

    /* Unlink vao */
    glBindVertexArray(0);

}
