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
	if (model->identity_mat4) {
		free(model->identity_mat4);
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

char *load_shader_file(char *path)
{
	char **file = load_file(path);
	char *shader_src = ft_strdup("");


	if (!file) {
		ft_printf_fd(2, RED"Error: Failed to load shader file\n"RESET);
		return (NULL);
	}
	for (u32 i = 0; file[i]; ++i) {
		shader_src = ft_strjoin_free(shader_src, file[i], 'f');
		shader_src = ft_strjoin_free(shader_src, "\n", 'f');
	}

	free_double_char(file);

	ft_printf_fd(1, "Shader src: %s\n", shader_src);
	return (shader_src);
}

GLuint load_shader(t_obj_model *model)
{
	// const char *vertex_shader = "#version 330 core\nlayout (location = 0) in vec3 aPos;\nuniform mat4 model;\nuniform mat4 view;\nuniform mat4 projection;\nvoid main()\n{gl_Position = projection * view * model * vec4(aPos, 1.0);}";
	// char *vertex_shader = load_shader_file(VERTEX_SHADER_PATH);
	char *vertex_shader = load_shader_file(NEW_VERTEX_SHADER);
	
	char *fragment_shader = load_shader_file(FRAGMENT_SHADER_PATH);
	
	/* create shader */
	GLuint frag_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag_pixel_shader = glCreateShader(GL_FRAGMENT_SHADER);

	/* compile shader */
	glShaderSource(frag_vertex_shader, 1, (const char **)&vertex_shader, NULL);
	glCompileShader(frag_vertex_shader);

	glShaderSource(frag_pixel_shader, 1, (const char **)&fragment_shader, NULL);
	glCompileShader(frag_pixel_shader);

	model->shader_id = glCreateProgram();
	
	/* Attach and link shader program  */
	glAttachShader(model->shader_id , frag_vertex_shader);
	glAttachShader(model->shader_id , frag_pixel_shader);
	glLinkProgram(model->shader_id );

	GLint succes = 0;
	glGetProgramiv(model->shader_id , GL_LINK_STATUS, &succes);
	if (!succes) {
		GLchar data[1024];
		ft_bzero(data, 1024);
		glGetProgramInfoLog(model->shader_id , 512, NULL, data);
		ft_printf_fd(2, "Shader program log: %s\n", data);
	} else {
		ft_printf_fd(1, "Shader program linked\n");
	}

	glUseProgram(model->shader_id);

	/* delete shader tocheck */
	// glDeleteShader(frag_vertex_shader);
	// glDeleteShader(frag_pixel_shader);

	/* delete ressource */
	free(vertex_shader);
	free(fragment_shader);
	return (model->shader_id);
}


GLuint init_gl_vertex_buffer(t_obj_model *model)
{
	/* create and fill vbo */
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec3_float) * model->v_size, model->vertex, GL_STATIC_DRAW);

  

	return (vbo);
}

/**
 * @brief Print element buufer data (triangle give to opengl)
*/
void print_elem_data(t_obj_model *model) {
	t_vec3_u32* bufferData = malloc(sizeof(t_vec3_u32) * model->tri_size);

	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(t_vec3_u32) * model->tri_size, bufferData);

	for (u32 i = 0; i < model->tri_size; i++) {
		ft_printf_fd(1, PINK"Element %u: x = %u, y = %u, z = %u\n"RESET, i, bufferData[i].x, bufferData[i].y, bufferData[i].z);
	}
	free(bufferData);
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
	model->vbo = init_gl_vertex_buffer(model);

    /* create and fill ebo Element Buffer Objects */
    glGenBuffers(1, &model->ebo);
	/* Bind EBO to GL alement array */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(t_vec3_u32) * model->tri_size, model->tri_face, GL_STATIC_DRAW);
	
	/*print here*/
	print_elem_data(model);

 /* Config new vertex attr */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(t_vec3_float), (void*)0);
	glEnableVertexAttribArray(0);  /* Enable vertex attr */
	/*print here*/
	print_vertex_data(model);

    /* Unlink vao */
    glBindVertexArray(0);

}
