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
			ft_printf_fd(1, ORANGE"Token %s is valid, val %d"RESET, to_check, (1 << i));
			return (1 << i);
		}
	}
	return (0);

}

static int8_t parse_obj_file(char *path)
{
	// t_obj_file *obj = NULL;
	char **file = NULL;
	if ((file = load_file(path)) == NULL) {
		return (0);
	}

	for (u32 i = 0; file[i]; ++i) {
		ft_printf_fd(1, YELLOW"\nLine %d: "RESET, i);
		char **trim = ft_split_trim(file[i], ' ');
		if (trim != NULL && trim[0] != NULL)  {
			if (is_valid_token(trim[0]) == 0) {
				ft_printf_fd(2, RED"Error: Invalid token %s\n"RESET, trim[0]);
				free_double_char(trim);
				return (0);
			}
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
