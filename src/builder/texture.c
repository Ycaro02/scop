#include "../../include/scop.h"

#define X_UNUSED 1U
#define Y_UNUSED 2U
#define Z_UNUSED 4U

#define X_FIELD 0
#define Y_FIELD 1
#define Z_FIELD 2

FT_INLINE u8 vec_unused_axe(vec3_f32 vec1, vec3_f32 vec2, vec3_f32 vec3, u32 field)
{
	return (float_equal(vec1[field], vec2[field]) && float_equal(vec1[field], vec3[field]));
}

u32 check_unused_field(vec3_f32 *vec)
{
	// if (float_equal(vec[0][0], vec[1][0]) && float_equal(vec[0][0], vec[2][0])) {
	// 	return (X_UNUSED);
	// } else if (float_equal(vec[0][1], vec[1][1]) && float_equal(vec[0][1], vec[2][1])) {
	// 	return (Y_UNUSED);
	// } else if (float_equal(vec[0][2], vec[1][2]) && float_equal(vec[0][2], vec[2][2])) {
	// 	return (Z_UNUSED);
	// }
	if (vec_unused_axe(vec[0], vec[1], vec[2], 0)) {
		return (X_UNUSED);
	} else if (vec_unused_axe(vec[0], vec[1], vec[2], 1)) {
		return (Y_UNUSED);
	} else if (vec_unused_axe(vec[0], vec[1], vec[2], 2)) {
		return (Z_UNUSED);
	}
	return (0);
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

	u32 unused = check_unused_field(vertex);
	ft_printf_fd(1, YELLOW"Final unused %u\n"RESET, unused);

	// u32 first_val = 2; /* z */
	u32 first_val = 0;
	if (unused == X_UNUSED) {
		first_val = 2;
	} else if (unused == Z_UNUSED) {
		first_val = 0;
	}

	u32 second_val = 1; /* y */
	if (unused == Y_UNUSED) {
		second_val = 2;
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

/* r is repeat factor */
// void calculate_texture_coord(t_obj_face *face, vec2_f32 *texCoords, u32 r) {
// 	t_list *vertex_lst = face->vertex;
//     vec3_f32 vertex[6];

// 	ft_bzero(vertex, sizeof(vec3_f32) * 6);
	
// 	for (u32 i = 0; i < face->size; i++) {
// 		vec3_f32 *v = vertex_lst->content;
// 		ft_memcpy(vertex[i], *v, sizeof(vec3_f32));
// 		ft_printf_fd(1, ORANGE"vertex [%u] x %f, y %f, z %f\n"RESET, i, vertex[i][0], vertex[i][1], vertex[i][2]);
// 		vertex_lst = vertex_lst->next;
// 	}

// 	if (face->size == 3) {
//         // Triangle face
//         texCoords[0][0] = r * 0; texCoords[0][1] = r * 0;  // Top left
//         texCoords[1][0] = r * 1; texCoords[1][1] = r * 0;  // Top right
//         texCoords[2][0] = r * 0; texCoords[2][1] = r * 1;  // Bottom
// 		return ;
//     }

// 	/* Diagonale only */
// 	texCoords[0][0] = r * 0; texCoords[0][1] = r * 0;  // Top left of first triangle
// 	texCoords[1][0] = r * 1; texCoords[1][1] = r * 0;  // Top right of first triangle
// 	texCoords[2][0] = r * 1; texCoords[2][1] = r * 1;  // Bottom right of first triangle
// 	texCoords[3][0] = r * 0; texCoords[3][1] = r * 0;  // Top left of second triangle (same as top left of first triangle)
// 	texCoords[4][0] = r * 1; texCoords[4][1] = r * 1;  // Bottom right of second triangle (same as bottom right of first triangle)
// 	texCoords[5][0] = r * 0; texCoords[5][1] = r * 1;  // Bottom left of second triangle

// 	// Quadrilateral face, split into two triangles
// 	texCoords[0][0] = r * 0; texCoords[0][1] = r * 0;  // Top left of first triangle
// 	texCoords[1][0] = r * 1; texCoords[1][1] = r * 0;  // Top right of first triangle
// 	texCoords[2][0] = r * 0; texCoords[2][1] = r * 1;  // Bottom of first triangle
// 	texCoords[3][0] = r * 0; texCoords[3][1] = r * 0;  // Top left of second triangle (same as top left of first triangle)
// 	texCoords[4][0] = r * 1; texCoords[4][1] = r * 0;  // Top right of second triangle (same as top right of first triangle)
// 	texCoords[5][0] = r * 1; texCoords[5][1] = r * 1;  // Bottom of second triangle
// }