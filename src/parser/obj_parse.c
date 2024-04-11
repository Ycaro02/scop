#include "../../include/scop.h"


static void display_double_char(char **str)
{
	u32 i = 0;
	while (str[i] != NULL) {
		ft_printf_fd(1, "%s\n", str[i]);
		i++;
	}
}

static int8_t parse_obj_file(char *path)
{
	// t_obj_file *obj = NULL;
	char **file = NULL;
	if ((file = load_file(path)) == NULL) {
		return (0);
	}
	display_double_char(file);
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
