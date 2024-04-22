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

	model->vertex = list_to_array(obj_file->vertex, ft_lstsize(obj_file->vertex), sizeof(vec3_f32));
	model->v_size = ft_lstsize(obj_file->vertex);

	t_list *triangle_lst = quadra_to_triangle(obj_file->face);
	if (!triangle_lst) {
		ft_printf_fd(2, RED"Error: Failed to convert quadra to triangle\n"RESET);
		free_obj_file(obj_file);
		return (NULL);
	}

	// model->tri_face = triangle_list_to_array(triangle_lst, ft_lstsize(triangle_lst));
	model->tri_face = list_to_array(triangle_lst, ft_lstsize(triangle_lst), sizeof(vec3_u32));
	model->tri_size = ft_lstsize(triangle_lst);

	ft_lstclear(&triangle_lst, free);

	// for (t_list *current = triangle_lst; current; current = current->next) {
	// 	vec3_u32 *vec = (vec3_u32 *)current->content;
	// 	VECTOR_UINT_DISPLAY(3, (*vec))
	// }

	free_obj_file(obj_file);
	return (model);
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
 * @brief Hard build color for each triangle
 * @param model obj model
*/
u8 hard_build_color(t_obj_model *model) 
{
	vec3_f32 colors[] = {
		{1.0f, 0.0f, 0.0f},  /* Red */
		{0.0f, 1.0f, 0.0f},  /* Green */
		{0.0f, 0.0f, 1.0f},  /* Blue */
		{1.0f, 1.0f, 0.0f},  /* Yellow */
		{1.0f, 0.0f, 1.0f},  /* Magenta */
		{0.0f, 1.0f, 1.0f},  /* Cyan */
		{1.0f, 0.5f, 0.0f},  /* Orange */
		{0.5f, 0.0f, 0.5f},  /* Purple */
		{0.5f, 0.5f, 0.5f},  /* Grey */
		{0.0f, 0.5f, 0.0f},  /* Dark Green */
		{0.5f, 0.0f, 0.0f},  /* Dark Red */
		{0.0f, 0.0f, 0.5f},  /* Dark Blue */
		{0.5f, 0.5f, 0.0f},  /* Olive */
		{0.5f, 0.0f, 1.0f},  /* Violet */
		{0.0f, 0.5f, 0.5f},  /* Teal */
		{0.5f, 0.5f, 1.0f},  /* Light Blue */
		{1.0f, 0.5f, 0.5f},  /* Light Red */
		{0.5f, 1.0f, 0.5f},  /* Light Green */
		{1.0f, 1.0f, 0.5f},  /* Light Yellow */
		{0.5f, 1.0f, 1.0f},  /* Light Cyan */
		{1.0f, 0.5f, 1.0f},  /* Pink */
		{0.7f, 0.3f, 0.3f},  /* Brown */
		{0.3f, 0.7f, 0.3f},  /* Forest Green */
		{0.3f, 0.3f, 0.7f},  /* Navy Blue */
		{0.7f, 0.7f, 0.3f},  /* Khaki */
		{0.7f, 0.3f, 0.7f},  /* Fuchsia */
		{0.3f, 0.7f, 0.7f},  /* Aqua */
		{0.7f, 0.7f, 0.7f},  /* Light Grey */
		{0.3f, 0.3f, 0.3f},  /* Dark Grey */
		{0.6f, 0.4f, 0.2f}   /* Bronze */
	};
	/* Usefull trick */
    u32 num_colors = sizeof(colors) / sizeof(colors[0]);

    model->colors = malloc(model->v_size * sizeof(vec3_f32));
    if (!model->colors) {
        ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
        return (FALSE);
    }
    for (u32 i = 0; i < model->v_size; i++) {
        ft_vec_copy(model->colors[i], colors[i % num_colors], sizeof(vec3_f32));
    }

	return (TRUE);
}

void calculate_texture_coord(vec3_f32 vertex, vec2_f32 texCoord, u32 max_img) {
    // Convert the Cartesian coordinates to polar coordinates
    float theta = atan2(vertex[2], vertex[0]);
    float z = vertex[1];

    // Normalize theta to be between 0 and 1
    theta = (theta + FT_PI) / (2 * FT_PI);
    //  Normalize z value
    z = (z + 1) / 2;
    // Clamp values between 0 and 1
    theta = fmaxf(0, fminf(theta, 1));
    z = fmaxf(0, fminf(z, 1));
    // Use theta and z as texture coordinates
    texCoord[0] = theta * max_img;
    texCoord[1] = z * max_img;
}

/* Brut texture build when no texture data provided in obj file */
u8 build_material_texture(t_obj_model *model) 
{
    model->texture_coord = malloc(model->v_size * sizeof(vec2_f32));

    if (!model->texture_coord) {
        ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
        return (FALSE);
    }

	for (u32 i = 0; i < model->v_size; i++) {
		// Get the indices of the vertices of the i-th triangle
		calculate_texture_coord(model->vertex[i], model->texture_coord[i], 1);
	}
	for (u32 i = 0; i < model->v_size; i++) {
		ft_printf_fd(1, GREEN"texture %u x %f, y %f\n"RESET, i, model->texture_coord[i][0], model->texture_coord[i][1]);
	}
    return (TRUE);
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

	GLuint texCoords_vbo = create_VBO(model->tri_size * sizeof(vec2_f32), model->texture_coord);

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
