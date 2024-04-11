#include "../../include/scop.h"

void display_double_char(char **array)
{
	u32 i = 0;
	while (array[i] != NULL) {
		ft_printf_fd(1, "%s\n", array[i]);
		i++;
	}
}

u32 double_char_size(char **array)
{
	u32 i = 0;

	while (array[i] != NULL) {
		i++;
	}
	return (i);

}

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


static u8 str_is_float(char *str)
{
	u8 point_found = 0;

	if (!str) {
		return (FALSE);
	} else if (*str == '-') {
		str++;
	}
	while (*str) {
		if (*str == '.') {
			if (point_found) {
				return (FALSE);
			}
			point_found = 1;
		} else if (!ft_isdigit(*str)) {
			return (FALSE);
		}
		str++;
	}
	return (TRUE);

}

float ft_atof(char *str)
{
	/* Create result and neg var */
	double res = 0, neg = 1;
	/* decimal var for decimal parts compute */
	double dec = 0.1;

	if (!str) {
		return (0);
	} else if (*str == '-') { /* if first is '-' neg = -1 and skip it */
		neg = -1;
		str++;
	}

	/* like atoi just add digit ony by one with * 10 */
	while (*str >= '0' && *str <= '9') {
		res = res * 10 + *str - '0';
		str++;
	}
	/* check for decimal parts */
	if (*str && *str == '.') { 
		str++; /* skip dot */
		/* same logic than atoi, but /10 each iterate instead of *10 */
		while (*str && *str >= '0' && *str <= '9') {
			res += (*str - '0') * dec;
			dec *= 0.1;
			str++;
		}
	}
	return ((res * neg));
}

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

void free_face_node(void *node)
{
	if (((t_face_node *)node)->other) {
		free(((t_face_node *)node)->other);
	}
	free(node);
}

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

static u8 add_vertex_node(t_list **list, char **line)
{
	t_vec3_float *vertex = line_to_vertex(line);
	if (!vertex) {
		return (FALSE);
	}
	ft_lstadd_back(list, ft_lstnew(vertex));
	return (TRUE);
}

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


void free_obj_file(t_obj_file *obj)
{
	if (obj->o) {
		free(obj->o);
	}
	if (obj->mtllib) {
		free(obj->mtllib);
	}
	if (obj->usemtl) {
		free(obj->usemtl);
	}
	if (obj->vertex) {
		ft_lstclear(&obj->vertex, free);
	}
	if (obj->face) {
		ft_lstclear(&obj->face, free_face_node);
	}
	lst_clear(&obj->vertex, free);
}

static int8_t parse_obj_file(char *path)
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
		// display_double_char(trim);
	}
	// display_double_char(file);
	// display_vertex_lst(obj.vertex);
	free_obj_file(&obj);
	free_double_char(file);
	return (1);
}

int main()
{
	if (!parse_obj_file("rsc/42.obj")) {
		ft_printf_fd(2, "Error\n");
		return (1);
	}
	return (0);
}
