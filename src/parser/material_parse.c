#include "../../include/scop.h"

void display_material_data(t_material_file *file)
{
	ft_printf_fd(1, CYAN"\nMaterial name: %s\n"RESET, file->newmtl);
	ft_printf_fd(1, CYAN"Ambient color: %f %f %f\n"RESET, file->ka[0], file->ka[1], file->ka[2]);
	ft_printf_fd(1, CYAN"Diffuse color: %f %f %f\n"RESET, file->kd[0], file->kd[1], file->kd[2]);
	ft_printf_fd(1, CYAN"Specular color: %f %f %f\n"RESET, file->ks[0], file->ks[1], file->ks[2]);
	ft_printf_fd(1, CYAN"Specular exponent: %f\n"RESET, file->ns);
	ft_printf_fd(1, CYAN"Optical density: %f\n"RESET, file->ni);
	ft_printf_fd(1, CYAN"Dissolve factor: %f\n"RESET, file->d);
	ft_printf_fd(1, CYAN"Illumination model: %d\n"RESET, file->illum);
}


static u8 line_to_vec3_f32(char **line, vec3_f32 vec)
{
	if (double_char_size(line) != 3) {
		ft_printf_fd(2, RED"Error: Invalid vector in mtl file\n"RESET);
		display_double_char(line);
		return (FALSE);
	}

	 if (!str_is_float(line[0]) || !str_is_float(line[1]) || !str_is_float(line[2])) {
		ft_printf_fd(2, RED"Error: Invalid vertex\n"RESET);
		display_double_char(line);
		return (FALSE);
	}
	vec[0] = ft_atof(line[0]);
	vec[1] = ft_atof(line[1]);
	vec[2] = ft_atof(line[2]);
	return (TRUE);
}

static u8 line_to_float(char **line, float *val)
{
	if (double_char_size(line) != 1) {
		ft_printf_fd(2, RED"Error: Invalid float in mtl file\n"RESET);
		display_double_char(line);
		return (FALSE);
	}
	if (!str_is_float(line[0])) {
		ft_printf_fd(2, RED"Error: Invalid float in mtl file\n"RESET);
		display_double_char(line);
		return (FALSE);
	}
	*val = ft_atof(line[0]);
	return (TRUE);
}

static u8  material_line_by_token(t_material_file *file, char **line, u16 token)
{
	if (token == ENUM_MTL_NEWMTL) {
		file->newmtl = ft_strdup(line[0]);
	} else if (token == ENUM_MTL_KA) {
		line_to_vec3_f32(line, file->ka);
	} else if (token == ENUM_MTL_KD) {
		line_to_vec3_f32(line, file->kd);
	} else if (token == ENUM_MTL_KS) {
		line_to_vec3_f32(line, file->ks);
	} else if (token == ENUM_MTL_NS) {
		line_to_float(line, &file->ns);
	} else if (token == ENUM_MTL_NI) {
		line_to_float(line, &file->ni);
	} else if (token == ENUM_MTL_D) {
		line_to_float(line, &file->d) ;
	}  else if (token == ENUM_MTL_ILLUM) {
		file->illum = array_to_uint32(line[0]); /* to check */
	} else if (token == ENUM_MTL_COMMENT) {
		ft_printf_fd(1, CYAN"MTL comment: %s\n"RESET, line[0]);
	} else {
		return (FALSE);
	}
	return (TRUE);
}


t_material_file *parse_mtl_file(char *path)
{
	char			*valid_tokens[] = MATERIAL_TOKEN_ARRAY; /* Array of valid token for obj file */
	char			**file_content = NULL; /* Array of string from file */
	t_material_file	*file = NULL;	/* Material file structure */
	uint16_t		token = 0;		/* Token value */

	if ((file_content = sstring_load_file(path)) == NULL) {
		return (NULL);
	} else if ((file = ft_calloc(sizeof(t_material_file), 1)) == NULL) {
		ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
		return (NULL);
	}

	for (u32 i = 0; file_content[i]; ++i) {
		char **trim = ft_split_trim(file_content[i], ' ');
		if (trim != NULL)  {
			if ((token = is_valid_token(valid_tokens, trim[0])) == 0) {
				ft_printf_fd(2, RED"Error: Invalid token %s\n"RESET, trim[0]);
				free_double_char(trim);
				return (NULL);
			}
			material_line_by_token(file, &trim[1], token);
			free_double_char(trim);
		}
	}
	free_double_char(file_content);
	return (file);
}

