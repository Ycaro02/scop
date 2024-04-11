#include "../../include/scop.h"

void display_double_char(char **str)
{
	u32 i = 0;
	while (str[i] != NULL) {
		ft_printf_fd(1, "%s\n", str[i]);
		i++;
	}
}


static uint16_t is_valid_token(char *to_check)
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

static s8 handle_line_by_token(t_obj_file *file, char **line, uint16_t token)
{
	(void)file;
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
