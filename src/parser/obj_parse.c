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
		TOKEN_F,
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
	double res = 0, neg = 1;

	if (!str) {
		return (0);
	} else if (*str == '-') {
		neg = -1;
		str++;
	}

	while (*str >= '0' && *str <= '9') {
		res = res * 10 + *str - '0';
		str++;
	}
	if (*str && *str == '.') {
		str++;
		double dec = 0.1;
		while (*str && *str >= '0' && *str <= '9') {
			res += (*str - '0') * dec;
			dec *= 0.1;
			str++;
		}
	}
	return ((res * neg));
}


static u8 add_vertex_node(t_list **list, char **line)
{
	t_vec3_float *vertex = NULL;
	if (double_char_size(line) != 3) {
		ft_printf_fd(2, RED"Error: Invalid vertex\n"RESET);
		display_double_char(line);
		return (FALSE);
	}
	
	if (!str_is_float(line[0]) || !str_is_float(line[1]) || !str_is_float(line[2])) {
		ft_printf_fd(2, RED"Error: Invalid vertex\n"RESET);
		display_double_char(line);
		return (FALSE);
	}

	if ((vertex = (t_vec3_float *)malloc(sizeof(t_vec3_float))) == NULL) {
		ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
		return (FALSE);
	}
	vertex->x = ft_atof(line[0]);
	vertex->y = ft_atof(line[1]);
	vertex->z = ft_atof(line[2]);
	// printf(GREEN"\nVertex: |%s = %f| |%s = %f| |%s = %f|"RESET, line[0], vertex->x, line[1], vertex->y, line[2], vertex->z);
	ft_lstadd_back(list, ft_lstnew(vertex));
	return (TRUE);
}

static s8 handle_line_by_token(t_obj_file *file, char **line, u16 token)
{
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
			break;
		case ENUM_VERTEX:
			if (!add_vertex_node(&file->v, &line[1])) {
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
			break;
		default:
			ft_printf_fd(2, RED"Error: Invalid token %s\n"RESET, line[0]);
			// display_double_char(line);
			return (0);
	}
	return (1);
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
	lst_clear(&obj->v, free);
}

static int8_t parse_obj_file(char *path)
{
	t_obj_file obj;
	char **file = NULL;
	uint16_t token = 0;
	if ((file = load_file(path)) == NULL) {
		return (0);
	}

	for (u32 i = 0; file[i]; ++i) {
		ft_printf_fd(1, YELLOW"\nLine %d:"RESET" "BLUE"|%s|"RESET, i, file[i]);
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
	display_vertex_lst(obj.v);
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