#include "../../include/scop.h"

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
 * @brief Line to vertex
 * @param line line to parse
 * @return allocated t_vec3_float ptr , NULL if fail
*/
t_vec3_float *line_to_vertex_node(char **line)
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

