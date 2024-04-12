#include "../../include/scop.h"

/**
 *	@brief Face node free
 *	@param node node to free
*/
void free_face_node(void *node)
{
	if (((t_face_node *)node)->other) {
		free(((t_face_node *)node)->other);
	}
	free(node);
}

/**
 *	@brief Free obj file structure
 *	@param obj obj file to free
*/
void free_obj_file(t_obj_file *obj)
{
	if (obj->o) {
		free(obj->o);
	} if (obj->mtllib) {
		free(obj->mtllib);
	} if (obj->usemtl) {
		free(obj->usemtl);
	} if (obj->vertex) {
		ft_lstclear(&obj->vertex, free);
	} if (obj->face) {
		ft_lstclear(&obj->face, free_face_node);
	} if (obj->vertex) {
		lst_clear(&obj->vertex, free);
	}
}


/**
 * @brief Check if a string is a valid token
 * @param to_check string to check
 * @return 0 if not valid, else the value of the token
*/
static u16 is_valid_token(char *to_check)
{
	char *tokens[] = {
		TOKEN_COMMENT,
		TOKEN_OBJ,
		TOKEN_SMOOTH,
		TOKEN_VERTEX,
		TOKEN_VT,
		TOKEN_VN,
		TOKEN_MTLLIB,
		TOKEN_USEMT,
		TOKEN_FACE,
		NULL
	};
	for (u32 i = 0; tokens[i]; ++i) {
		if (ft_strcmp(to_check, tokens[i]) == 0) {
			// ft_printf_fd(1, ORANGE"Token %s is valid, val %d"RESET, to_check, (1 << i));
			return (1 << i);
		}
	}
	return (0);

}

/**
 * @brief Get string after token
 * @param to_fill_ptr string to fill
 * @param line line to parse
 * @return TRUE if success, else FALSE
*/
static s8 get_str_after_token(char **to_fill_ptr, char **line)
{
	if (line[0] == NULL || *line[0] == '\0' || line[1] != NULL) {
		ft_printf_fd(2, RED"Error: Invalid object name\n"RESET);
		display_double_char(line);
		return (FALSE);
	}
	*to_fill_ptr = ft_strdup(line[0]);
	return (TRUE);
}

/**
 * @brief Line to vec3_u32
 * @param line line to parse
 * @param other_val ptr on u32, set to the number of other value
 * @return t_vec3_u32
*/
t_vec3_u32 line_to_vec3_u32(char **line, u32 *other_val)
{
	t_vec3_u32 vec = {0, 0, 0};
	u32 size;
	if ((size = double_char_size(line)) < 3) {
		ft_printf_fd(2, RED"Error: Invalid f option\n"RESET);
		display_double_char(line);
		return (vec);
	} else if (size > 3) {
		*other_val = size - 3;
	}
	
	if (!str_is_digit(line[0]) || !str_is_digit(line[1]) || !str_is_digit(line[2])) {
		ft_printf_fd(2, RED"Error: Invalid vertex\n"RESET);
		display_double_char(line);
		return (vec);
	}
	/* need to check value, maybe count number of vertex/vertice */
	vec.x = array_to_uint32(line[0]);
	vec.y = array_to_uint32(line[1]);
	vec.z = array_to_uint32(line[2]);
	return (vec);
}


/**
 * @brief Line to face
 * @param file obj file
 * @param line line to parse
 * @return TRUE if success, else FALSE
*/
u8 line_to_face(t_obj_file *file, char **line)
{
	t_face_node *face;
	u32			other_val = 0;
	t_vec3_u32	vec = line_to_vec3_u32(line, &other_val);
	
	face = ft_calloc(1, sizeof(t_face_node));
	if (!face) {
		ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
		return (FALSE);
	}

	if (other_val) {
		// ft_printf_fd(2, ORANGE" Other val detected [%u]"RESET, other_val);
		face->other = ft_calloc(sizeof(u32), other_val);
		for (u32 i = 0; i < other_val; i++ ) {
			face->other[i] = array_to_uint32(line[i + 3]); /* need to parse val */
			// ft_printf_fd(2, PURPLE" idx: |%u| val: |%u|"RESET, i, face->other[i]);
		}
	}
	face->vec = CREATE_VEC3(u32, vec.x, vec.y, vec.z);
	ft_lstadd_back(&file->face, ft_lstnew(face));
	// ft_printf_fd(1, CYAN"\nFace: ");
	// DISPLAY_VEC3(u32, face->vec);
	// ft_printf_fd(1, RESET);
	return (TRUE);

}

/**
 * @brief Line to vertex
 * @param line line to parse
 * @return allocated t_vec3_float ptr , NULL if fail
*/
static t_vec3_float *line_to_vertex(char **line)
{
	t_vec3_float *vertex = NULL;
	if (double_char_size(line) != 3) {
		ft_printf_fd(2, RED"Error: Invalid vertex\n"RESET);
		display_double_char(line);
		return (NULL);
	}
	
	if (!str_is_float(line[0]) || !str_is_float(line[1]) || !str_is_float(line[2])) {
		ft_printf_fd(2, RED"Error: Invalid vertex\n"RESET);
		display_double_char(line);
		return (NULL);
	}

	if ((vertex = (t_vec3_float *)malloc(sizeof(t_vec3_float))) == NULL) {
		ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
		return (NULL);
	}
	vertex->x = ft_atof(line[0]);
	vertex->y = ft_atof(line[1]);
	vertex->z = ft_atof(line[2]);
	return (vertex);
}

/**
 * @brief Add vertex node to linked list
 * @param list ptr on list head
 * @param line line to parse
 * @return TRUE if success, else FALSE
*/
static u8 add_vertex_node(t_list **list, char **line)
{
	t_vec3_float *vertex = line_to_vertex(line);
	if (!vertex) {
		return (FALSE);
	}
	ft_lstadd_back(list, ft_lstnew(vertex));
	return (TRUE);
}

/**
 * @brief Handle smooth str
 * @param str str to parse
 * @return TRUE if success, else FALSE
*/
u8 handle_smooth_str(char *str)
{
	if (ft_strncmp(str, "on", ft_strlen("on")) == 0) {
		return (TRUE);
	} else if (ft_strncmp(str, "off", ft_strlen("off")) == 0) {
		return (FALSE);
	} else { /* error need to be catch */
		return (2);
	}
}

/**
 * @brief Handle line by token
 * @param file obj file
 * @param line line to parse
 * @param token token value
 * @return TRUE if success, else FALSE
*/
static u8 handle_line_by_token(t_obj_file *file, char **line, u16 token)
{
	char *tmp = NULL;

	switch (token) {
		case ENUM_COMMENT:
			break;
		case ENUM_OBJ:
			if (!get_str_after_token(&file->o, &line[1])) {
				return (FALSE);
			}
			ft_printf_fd(1, PINK"\nObject name: %s"RESET, file->o);
			break;
		case ENUM_SMOOTH:
			if (!get_str_after_token(&tmp, &line[1])) {
				return (FALSE);
			}
			file->smooth = handle_smooth_str(tmp);
			ft_printf_fd(1, PINK"\nSmooth name: %s, val %u\n"RESET, tmp, file->smooth);
			free(tmp);
			break;
		case ENUM_VERTEX:
			if (!add_vertex_node(&file->vertex, &line[1])) {
				return (FALSE) ;
			}
			break;
		case ENUM_VT:
			break;
		case ENUM_VN:
			break;
		case ENUM_MTLLIB:
			if (!get_str_after_token(&file->mtllib, &line[1])) {
				return (FALSE);
			}
			ft_printf_fd(1, CYAN"\nMTLIB name: %s"RESET, file->mtllib);
			break;
		case ENUM_USEMTL:
			if (!get_str_after_token(&file->usemtl, &line[1])) {
				return (FALSE);
			}
			ft_printf_fd(1, ORANGE"\nUsemtl name: %s"RESET, file->usemtl);
			break;
		case ENUM_F:
			line_to_face(file, &line[1]);
			break;
		default:
			ft_printf_fd(2, RED"Error: Invalid token %s\n"RESET, line[0]);
			// display_double_char(line);
			return (FALSE);
	}
	return (TRUE);
}


void  display_vertex_lst(t_list *lst)
{
	ft_printf_fd(1, "Vertex list\n");
	for (t_list *current = lst; current; current = current->next) {
		t_vec3_float vec = *(t_vec3_float *)current->content;
		DISPLAY_VEC3(float, vec)
	}
}



int8_t parse_obj_file(char *path)
{
	t_obj_file obj;
	char **file = NULL;
	uint16_t token = 0;

	ft_bzero(&obj, sizeof(t_obj_file));
	if ((file = load_file(path)) == NULL) {
		return (0);
	}

	for (u32 i = 0; file[i]; ++i) {
		// ft_printf_fd(1, YELLOW"\nLine %d:"RESET" "BLUE"|%s|"RESET, i, file[i]);
		char **trim = ft_split_trim(file[i], ' ');
		if (trim != NULL)  {
			if ((token = is_valid_token(trim[0])) == 0) {
				ft_printf_fd(2, RED"Error: Invalid token %s\n"RESET, trim[0]);
				free_double_char(trim);
				return (0);
			}
			handle_line_by_token(&obj, trim, token);
			free_double_char(trim);
		}
	}
	vertext_list_toarray(obj.vertex, ft_lstsize(obj.vertex));

	free_obj_file(&obj);
	free_double_char(file);
	return (1);
}

t_vec3_float *vertext_list_toarray(t_list *lst, u32 lst_size)
{
	t_vec3_float	*array = ft_calloc(sizeof(t_vec3_float), lst_size);
	u32				i = 0;
	
	if (!array) {
		return (NULL);
	}
	for (t_list *current = lst; current; current = current->next) {
		// array[i] = *(t_vec3_float *)current->content;
		ft_memcpy(&array[i], current->content, sizeof(t_vec3_float));
		i++;
	}

	ft_printf_fd(1, "Vertex list to array\n");
	for (i = 0; i < lst_size; i++) {
		DISPLAY_VEC3(float, array[i]);
	}

	return (array);
}

// static void init_gl_vertex_buffer(t_obj_file *obj)
// {
// 	GLuint vbo;
// 	glGenBuffers(1, &vbo);
// 	glBindBuffer(GL_ARRAY_BUFFER, vbo);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec3_float) * ft_lstsize(obj->vertex)
// 		, (list to array here obj->vertex), GL_STATIC_DRAW);
// }


// int main()
// {
// 	if (!parse_obj_file("rsc/42.obj")) {
// 		ft_printf_fd(2, "Error\n");
// 		return (1);
// 	}
// 	return (0);
// }
