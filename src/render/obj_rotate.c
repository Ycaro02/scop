#include "../../include/scop.h"

/**
 * @brief Get object center
 * @param m object model
 * @param center center vector
*/
void get_obj_center(t_obj_model* m, vec3_f32 center) 
{
    vec3_f32 total = {0.0f};

    for (u32 i = 0; i < m->v_size; i++) {
        total[0] += m->vertex[i][0];
        total[1] += m->vertex[i][1];
        total[2] += m->vertex[i][2];
    }
    center[0] = total[0] / m->v_size;
    center[1] = total[1] / m->v_size;
    center[2] = total[2] / m->v_size;
}

/**
 * @brief Rotate object around center
 * @param m object model
 * @param rotate_vec vector used to rotate
 * @param angle angle used to rotate
 * @param shader_id shader id for update model matrix
*/
void rotate_object_around_center(t_obj_model* m, vec3_f32 rotate_vec, float angle, GLuint shader_id) 
{
    vec3_f32 obj_center, obj_center_neg;
    mat4_f32 translation_to_origin, rotation;
    
	/* Find model center */
	get_obj_center(m, obj_center);
    vec3_negate(obj_center_neg, obj_center);

	/* Translate position, place computed center at the origin */
    mat4_translattion(translation_to_origin, obj_center_neg);

    /* Apply rotate */
    mat4_rotate(rotation, deg_to_rad(angle), rotate_vec);

    /* Update obj model */
    mat4_mult(translation_to_origin, m->rotation, m->rotation);		/* Origin translate */
    mat4_mult(rotation, m->rotation, m->rotation);					/* Apply rotate */
    mat4_mult_translation(m->rotation, obj_center);					/* Translate back */

    /* Update model matrix in shader */
    set_shader_var_mat4(shader_id, "model", m->rotation);
}
