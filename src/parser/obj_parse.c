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
	vec3_f32 *vertex = line_to_vertex_node(line);
	if (!vertex) {
		return (FALSE);
	}
	ft_lstadd_back(list, ft_lstnew(vertex));
	return (TRUE);
}



/**
 * @brief Handle line by token
 * @param file obj file
 * @param line line to parse
 * @param token token value
 * @return TRUE if success, else FALSE
*/
static u8 obj_line_by_token(t_obj_file *file, char **line, u16 token)
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
			ft_printf_fd(2, PURPLE"Default case line %s\n"RESET, line[0]);
			// display_double_char(line);
			return (FALSE);
	}
	return (TRUE);
}


vec3_f32 *vec3_f32_new(float x, float y, float z)
{
	vec3_f32 *vec = (vec3_f32 *)malloc(sizeof(vec3_f32));
	if (vec == NULL) {
		return (NULL);
	}
	CREATE_VEC3(x, y, z, (*vec));
	return (vec);
}

/**
 * @brief Parse obj file
 * @param path path to obj file
 * @return pointer on allocated t_obj_model
*/
t_obj_model *parse_obj_file(char *path)
{
	char *valid_tokens[] = OBJ_TOKEN_ARRAY;
	t_obj_file obj;
	char **file = NULL;
	uint16_t token = 0;

	ft_bzero(&obj, sizeof(t_obj_file));
	if ((file = sstring_load_file(path)) == NULL) {
		return (0);
	}

	/* Add origin vertex index 0 */
	ft_lstadd_front(&obj.vertex, ft_lstnew(vec3_f32_new(0.0f, 0.0f, 0.0f)));

	for (u32 i = 0; file[i]; ++i) {
		// ft_printf_fd(1, YELLOW"\nLine %d:"RESET" "BLUE"|%s|"RESET, i, file[i]);
		char **trim = ft_split_trim(file[i], ' ');
		if (trim != NULL)  {
			if ((token = is_valid_token(valid_tokens, trim[0])) == 0) {
				ft_printf_fd(2, RED"Error: Invalid token %s\n"RESET, trim[0]);
				free_double_char(trim);
				free_double_char(file);
				free_obj_file(&obj);
				return (NULL);
			}
			obj_line_by_token(&obj, &trim[1], token);
			free_double_char(trim);
		}
	}
	free_double_char(file);

	t_sstring sstr = fill_sstring(MODEL_PATH);

	concat_sstring(&sstr, obj.mtllib);

	ft_printf_fd(1, CYAN"\nMaterial file: %s\n"RESET, sstr.data);


	t_material_file *mtl = NULL;

	if (obj.mtllib) {
		mtl = parse_mtl_file(sstr.data);
		if (!mtl) {
			ft_printf_fd(1, GREEN"\nInvalid Material file %s\n"RESET, sstr.data);
		}
	}

	t_obj_model *model = init_obj_model(&obj);

	if (!model){
		ft_printf_fd(2, RED"Error: Failed to init obj model\n"RESET);
		return (NULL);
	}
	model->material = mtl;



	return (model);
}