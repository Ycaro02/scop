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
t_list *get_all_triangle_vertex(t_obj_model *model, t_list **idx_lst_ptr) {
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
	display_triangle_data(vertex_lst, *idx_lst);
	free(model->vertex);
	return (vertex_lst);
}

void display_obj_face_lst(t_list *face_lst) {
	/* Display obj_face_lst */
	for (t_list *current = face_lst; current; current = current->next) {
		t_obj_face	*face = current->content;
		t_list		*curr_idx = face->idx;
		ft_printf_fd(1, CYAN"\nFace %u\n"RESET, face->id);
		for (t_list *curr_vertex = face->vertex; curr_vertex; curr_vertex = curr_vertex->next) {
			vec3_f32 *vec = curr_vertex->content;
			void *idx = curr_idx->content;
			ft_printf_fd(1, PINK"Vertex [%u] %f %f %f\n"RESET, *((u32 *)idx), (*vec)[0], (*vec)[1], (*vec)[2]);
			curr_idx = curr_idx->next;
		}
		ft_printf_fd(1, CYAN"ID: %u, Size %u\n"RESET, face->id, face->size);
	}
}

/**
 * @brief Extract face data
 * @param model obj model
 * @param face obj face
 * @param tri_idx triangle index
 * @return TRUE if success, FALSE otherwise (malloc failed)
*/
u8 extract_face_data(t_obj_model *model, t_obj_face *face, u32 tri_idx)
{
	vec3_u32	*tri_face = model->tri_face;

	for (u32 i = 0; i < 3; i++) {
		vec3_f32 *vertex = ft_calloc(sizeof(vec3_f32), 1);
		if (!vertex) {
			ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
			return (FALSE);
		}
		ft_vec_copy(*vertex, model->vertex[tri_face[tri_idx][i]], sizeof(vec3_f32));
		ft_lstadd_back(&face->vertex, ft_lstnew(vertex));
		u32 *idx = ft_calloc(sizeof(u32), 1);
		if (!idx) {
			ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
			return (FALSE);
		}
		*idx = tri_face[tri_idx][i];
		ft_lstadd_back(&face->idx, ft_lstnew(idx));
	} /* End for loop 3 */
	return (TRUE);
}

/**
 * @brief Increment tri_idx and face_size
 * @param tri_idx triangle index to increment by 1
 * @param face_size face size to increment by 3 (3 vertex per triangle)
*/
FT_INLINE void increment_tri_idx(u32 *tri_idx, u32 *face_size) {
	*tri_idx += 1;
	*face_size += 3;
}

t_list *get_all_face_vertex(t_obj_file *file, t_obj_model *model) {
	t_list 		*face_lst = NULL;
	u32			face_id = 1, tri_idx = 0;

	/* We need to loop on face lst to identify different face, and loop on tri_face to select right, vertex and idx
		- loop on face list logic
			- NODE = face list content, get the 3 first vertex/index in tri_face, cause face is minimum triangle (3 vertex)
			- if NODE->other is not null, get the next 3 vertex/index in tri_face, get the next triangle
	*/

	for (t_list *current = file->face; current; current = current->next) {
		t_face_node	*curr_node = current->content;
		t_obj_face	*face = ft_calloc(sizeof(t_obj_face), 1);
		u32			face_size = 0;
		
		if (!face) {
			ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
			return (NULL);
		}
		face->id = face_id;
		extract_face_data(model, face, tri_idx);
		increment_tri_idx(&tri_idx, &face_size);
		if (curr_node->other) {
			extract_face_data(model, face, tri_idx);
			increment_tri_idx(&tri_idx, &face_size);
		}
		face->size = face_size;
		ft_lstadd_back(&face_lst, ft_lstnew(face));
		face_id++;
	}

	/* Display data here */
	display_obj_face_lst(face_lst);

	return (face_lst);
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


	ft_lstclear(&vertex_lst, free);
	ft_lstclear(&triangle_lst, free);
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

void calculate_texture_coord(t_obj_face *face, vec2_f32 *texCoords, u32 repeat_fact) {
    if (face->size == 3) {
        // Triangle face
        texCoords[0][0] = repeat_fact * 0; texCoords[0][1] = repeat_fact * 0;  // Top left
        texCoords[1][0] = repeat_fact * 1; texCoords[1][1] = repeat_fact * 0;  // Top right
        texCoords[2][0] = repeat_fact * 0; texCoords[2][1] = repeat_fact * 1;  // Bottom
		return ;
    }
	// Quadrilateral face, split into two triangles
	texCoords[0][0] = repeat_fact * 0; texCoords[0][1] = repeat_fact * 0;  // Top left of first triangle
	texCoords[1][0] = repeat_fact * 1; texCoords[1][1] = repeat_fact * 0;  // Top right of first triangle
	texCoords[2][0] = repeat_fact * 0; texCoords[2][1] = repeat_fact * 1;  // Bottom of first triangle

	texCoords[3][0] = repeat_fact * 0; texCoords[3][1] = repeat_fact * 0;  // Top left of second triangle (same as top left of first triangle)
	texCoords[4][0] = repeat_fact * 1; texCoords[4][1] = repeat_fact * 0;  // Top right of second triangle (same as top right of first triangle)
	texCoords[5][0] = repeat_fact * 1; texCoords[5][1] = repeat_fact * 1;  // Bottom of second triangle
}

/* Brut texture build when no texture data provided in obj file */
u8 build_material_texture(t_obj_model *model) 
{
    model->texture_coord = malloc(model->v_size * sizeof(vec2_f32));

    if (!model->texture_coord) {
        ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
        return (FALSE);
    }

	u32 i = 0;
    for (t_list *face_lst = model->obj_face; face_lst; face_lst = face_lst->next) {
		t_obj_face *face_node = face_lst->content;
        // Calculez les coordonnées de texture pour chaque sommet de la face
        calculate_texture_coord(face_node, &model->texture_coord[i], 4);
		i += face_node->size;
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
