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
 * @brief Add vertex node to linked list
 * @param list ptr on list head
 * @param line line to parse
 * @return TRUE if success, else FALSE
*/
static u8 add_vertex_node(t_list **list, char **line)
{
	vec3_float *vertex = line_to_vertex_node(line);
	if (!vertex) {
		return (FALSE);
	}
	ft_lstadd_back(list, ft_lstnew(vertex));
	return (TRUE);
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
			if (!get_str_after_token(&file->o, line)) {
				return (FALSE);
			}
			ft_printf_fd(1, PINK"\nObject name: %s"RESET, file->o);
			break;
		case ENUM_SMOOTH:
			if (!get_str_after_token(&tmp, line)) {
				return (FALSE);
			}
			file->smooth = handle_smooth_str(tmp);
			ft_printf_fd(1, PINK"\nSmooth name: %s, val %u\n"RESET, tmp, file->smooth);
			free(tmp);
			break;
		case ENUM_VERTEX:
			if (!add_vertex_node(&file->vertex, line)) {
				return (FALSE) ;
			}
			break;
		case ENUM_VT:
			break;
		case ENUM_VN:
			break;
		case ENUM_MTLLIB:
			if (!get_str_after_token(&file->mtllib, line)) {
				return (FALSE);
			}
			ft_printf_fd(1, CYAN"\nMTLIB name: %s"RESET, file->mtllib);
			break;
		case ENUM_USEMTL:
			if (!get_str_after_token(&file->usemtl, line)) {
				return (FALSE);
			}
			ft_printf_fd(1, ORANGE"\nUsemtl name: %s"RESET, file->usemtl);
			break;
		case ENUM_F:
			line_to_face(file, line);
			break;
		default:
			ft_printf_fd(2, RED"Error: Invalid token %s\n"RESET, line[0]);
			// display_double_char(line);
			return (FALSE);
	}
	return (TRUE);
}



/**
 * @brief Parse obj file
 * @param path path to obj file
 * @return pointer on allocated t_obj_model
*/
t_obj_model *parse_obj_file(char *path)
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
				return (NULL);
			}
			handle_line_by_token(&obj, &trim[1], token);
			free_double_char(trim);
		}
	}
	free_double_char(file);

	t_obj_model *model = init_obj_model(&obj);

	return (model);
}