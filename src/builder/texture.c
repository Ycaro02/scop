#include "../../include/scop.h"


FT_INLINE u8 vec_unused_axe(vec3_f32 vec1, vec3_f32 vec2, vec3_f32 vec3, u32 field) {
	return (float_equal(vec1[field], vec2[field]) && float_equal(vec1[field], vec3[field]) && float_equal(vec2[field], vec3[field]));
}

u32 check_unused_field(vec3_f32 *vec)
{
	if (vec_unused_axe(vec[0], vec[1], vec[2], X_FIELD)) {
		return (X_UNUSED);
	} else if (vec_unused_axe(vec[0], vec[1], vec[2], Y_FIELD)) {
		return (Y_UNUSED);
	} else if (vec_unused_axe(vec[0], vec[1], vec[2], Z_FIELD)) {
		return (Z_UNUSED);
	}
	return (0);
}

f32 count_abs_field(vec3_f32 *vec, u32 field)
{
	f32 count = 0;
	if (!float_equal(vec[0][field], vec[1][field])) {
		count += fabs(vec[0][field] - vec[1][field]);
	} if (!float_equal(vec[0][field], vec[2][field])) {
		count += fabs(vec[0][field] - vec[2][field]);
	}
	return (count);
}

u32 get_most_unsignifiant_field(vec3_f32 *vec)
{
	u32 field = Y_FIELD;
	f32 y_count = count_abs_field(vec, Y_FIELD);
	f32 z_count = count_abs_field(vec, Z_FIELD);

	if (y_count < z_count) {
		field = Z_FIELD;
	} 
	return (field);
}

void calculate_texture_coord(t_obj_face *face, vec2_f32 *texCoords, u32 r) {
	t_list *vertex_lst = face->vertex;
    vec3_f32 vertex[6];

	ft_bzero(vertex, sizeof(vec3_f32) * 6);
	for (u32 i = 0; i < face->size; i++) {
		vec3_f32 *v = vertex_lst->content;
		ft_memcpy(vertex[i], *v, sizeof(vec3_f32));
		// ft_printf_fd(1, ORANGE"vertex [%u] x %f, y %f, z %f\n"RESET, i, vertex[i][0], vertex[i][1], vertex[i][2]);
		vertex_lst = vertex_lst->next;
	}

	u32 unused_axis = check_unused_field(vertex);
	ft_printf_fd(1, YELLOW"Final unused_axis %u\n"RESET, unused_axis);

	// u32 first_val = 2; /* z */
	u32 first_val = 0;
	if (unused_axis == X_UNUSED) {
		first_val = 2;
	} else if (unused_axis == Z_UNUSED) {
		first_val = 0;
	}

	u32 second_val = 1; /* y */
	if (unused_axis == Y_UNUSED) {
		second_val = 2;
	}

	if (unused_axis == 0) {
		first_val = 0; /* Always keep x */
		second_val = get_most_unsignifiant_field(vertex); /* Need to choice for y or z */
	}

	texCoords[0][0] = vertex[0][first_val] * r ; texCoords[0][1] = vertex[0][second_val] * r ;  // Top left
	texCoords[1][0] = vertex[1][first_val] * r ; texCoords[1][1] = vertex[1][second_val] * r ;  // Top right
	texCoords[2][0] = vertex[2][first_val] * r ; texCoords[2][1] = vertex[2][second_val] * r ;  // Bottom
	if (face->size != 3) {
		texCoords[3][0] = vertex[3][first_val] * r ; texCoords[3][1] = vertex[3][second_val] * r ;  // Top left
		texCoords[4][0] = vertex[4][first_val] * r ; texCoords[4][1] = vertex[4][second_val] * r ;  // Bottom
		texCoords[5][0] = vertex[5][first_val] * r ; texCoords[5][1] = vertex[5][second_val] * r ;  // Bottom
	}
}

/* Brut texture build when no texture data provided in obj file */
u8 build_material_texture(t_obj_model *model)
{
    model->texture_coord = malloc(model->v_size * sizeof(vec2_f32));

    if (!model->texture_coord) {
        ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
        return (FALSE);
    }

	u32 i = 1;
    for (t_list *face_lst = model->obj_face; face_lst; face_lst = face_lst->next) {
		t_obj_face *face_node = face_lst->content;
        calculate_texture_coord(face_node, &model->texture_coord[i], 1);
		i += face_node->size;
    }
	
	// for (u32 i = 0; i < model->v_size; i++) {
	// 	ft_printf_fd(1, GREEN"texture %u x %f, y %f\n"RESET, i, model->texture_coord[i][0], model->texture_coord[i][1]);
	// }
    return (TRUE);
}