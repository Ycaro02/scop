#include "../../include/scop.h"

/* need to go in libft linked list API */
/**
 * @brief List to array
 * @param lst pointer on linked list head
 * @param lst_size size of the list, number of node
 * @param type_size size of the type to copy
 * @return allocated t_vec3_u32 array, NULL if fail
*/
void	*list_to_array(t_list *lst, u32 lst_size, u32 type_size) 
{
	u32		i = 0;
	void	*array = ft_calloc(type_size, lst_size);

	if (!array) {
		ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
		return (NULL);
	}
	for (t_list *current = lst; current; current = current->next) {
		ft_memcpy(array + (i * type_size), current->content, type_size);
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

	// model->vertex = vertex_list_toarray(obj_file->vertex, ft_lstsize(obj_file->vertex));
	model->vertex = list_to_array(obj_file->vertex, ft_lstsize(obj_file->vertex), sizeof(t_vec3_float));
	model->v_size = ft_lstsize(obj_file->vertex);

	t_list *triangle_lst = quadra_to_triangle(obj_file->face);
	if (!triangle_lst) {
		ft_printf_fd(2, RED"Error: Failed to convert quadra to triangle\n"RESET);
		free_obj_file(obj_file);
		return (NULL);
	}

	// model->tri_face = triangle_list_to_array(triangle_lst, ft_lstsize(triangle_lst));
	model->tri_face = list_to_array(triangle_lst, ft_lstsize(triangle_lst), sizeof(t_vec3_u32));
	model->tri_size = ft_lstsize(triangle_lst);


	// load_shader();

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

// void init_gl_vertex_buffer(t_obj_model *model)
// {
// 	GLuint vbo;
// 	glGenBuffers(1, &vbo);
// 	glBindBuffer(GL_ARRAY_BUFFER, vbo);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec3_float) * model->v_size
// 		, model->vertex, GL_STATIC_DRAW);
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(t_vec3_float), (void*)0);
	
// 	// print_vertex_data(model);
// 	glBindBuffer(GL_ARRAY_BUFFER, 0);  /* unbind */
// }

// void init_gl_index_buffer(t_obj_model *model)
// {
// 	GLuint ebo;
// 	glGenBuffers(1, &ebo);
// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(t_vec3_u32) * model->tri_size
// 		, model->tri_face, GL_STATIC_DRAW);
// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  /* unbind */
// }


void load_shader()
{
	const char *vertex_shader = "#version 330 core\nlayout(location = 0) in vec3 aPos;\nvoid main() {\n gl_Position = vec4(aPos, 1.0);\n}";
	const char *fragment_shader = "#version 330 core\nout vec4 FragColor;\nvoid main() {\nFragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n}";
	/* create shader */
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	/* compile shader */
	glShaderSource(vertex_shader_id, 1, &vertex_shader, NULL);
	glCompileShader(vertex_shader_id);
	glShaderSource(fragment_shader_id, 1, &fragment_shader, NULL);
	glCompileShader(fragment_shader_id);

	GLuint shader_program = glCreateProgram();
	
	/* Attach and link shader program  */
	glAttachShader(shader_program, vertex_shader_id);
	glAttachShader(shader_program, fragment_shader_id);
	glLinkProgram(shader_program);

	glUseProgram(shader_program);

	/* delete ressource */
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);
}

GLuint init_gl_triangle_array(t_obj_model *model)
{
	/* Create new vao */
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /* create and fill vbo */
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec3_float) * model->v_size, model->vertex, GL_STATIC_DRAW);

   /* Config new vertex attr */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(t_vec3_float), (void*)0);
    glEnableVertexAttribArray(0);  // N'oubliez pas d'activer l'attribut de vertex à l'index 0

    /* create and fill ebo */
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(t_vec3_u32) * model->tri_size, model->tri_face, GL_STATIC_DRAW);

    /* Unlink vao */
    glBindVertexArray(0);
	ft_printf_fd(1, "VAO: %u\n", VAO);

	return (VAO);
	/* draw */
	// glBindVertexArray(VAO);
	// glDrawElements(GL_TRIANGLES, model->tri_size, GL_UNSIGNED_INT, 0);
	// glBindVertexArray(0);
}
