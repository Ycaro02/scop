#include "../../include/scop.h"

/**
 * @brief Create a new camera
 * @param fov field of view
 * @param aspect_ratio aspect ratio
 * @param near near plane
 * @param far far plane
 * @return new camera
*/
t_camera create_camera(float fov, float aspect_ratio, float near, float far) 
{
    t_camera camera;

    /* Initialize position, target and up vector */
	// CREATE_VEC3(0.0f, 0.0f, 3.0f, camera.position);
	// CREATE_VEC3(0.0f, 0.0f, -1.0f, camera.target);
	// CREATE_VEC3(0.0f, 1.0f, 0.0f, camera.up);

    /* init camera position */
    CREATE_VEC3(23.756206f, 0.00000f, 2.155274f, camera.position);
    /* init camera target */
    CREATE_VEC3(14.963508f, 0.00000f, 1.405361f, camera.target);
    /* init up vector */
    CREATE_VEC3(0.00000f, 1.00000f, 0.00000f, camera.up);

    /* Compute view martice */
    update_view_matrice(camera.position, camera.target, camera.up, camera.view);

    /* Compute projection matrice */
    get_perspective_mat4(deg_to_rad(fov), aspect_ratio, near, far, camera.projection);
    return (camera);
}

/**
 * @brief Update camera
 * @param camera camera to update
 * @param shader_id shader id
*/
void update_camera(t_camera* camera, GLuint shader_id) 
{
    update_view_matrice(camera->position, camera->target, camera->up, camera->view);
    set_shader_var_mat4(shader_id, "view", camera->view);
    // set_shader_var_mat4(shader_id, "model", camera->model); /* need to be updated */
    set_shader_var_mat4(shader_id, "projection", camera->projection);
}

/**
 * @brief Move camera forward
 * @param camera camera to move
 * @param distance distance to move
*/
void move_camera_forward(t_camera* camera, float distance) 
{
    vec3_f32 direction;

    vec3_sub(camera->target, camera->position, direction); /* tocheck */
    vec3_normalize(direction);
    vec3_scale(direction, distance, direction);
    VECTOR_ADD(float, 3, camera->position, direction);
    VECTOR_ADD(float, 3, camera->target, direction);
}

/**
 * @brief Move camera backward
 * @param camera camera to move
 * @param distance distance to move
*/
void move_camera_backward(t_camera* camera, float distance) {
    move_camera_forward(camera, -distance);
}

/**
 * @brief Move camera left
 * @param camera camera to move
 * @param distance distance to move
 * @param axis axis to move
*/
void rotate_camera(t_camera* camera, float angle, vec3_f32 axis) {
    mat4_f32 rotation;

    /* Create rotation matrix */
	make_rotation(rotation, deg_to_rad(angle), axis);

    /* Rotate the direction vector from the position to the target */
    vec3_f32 direction;
	vec3_sub(camera->target, camera->position, direction);
	mat4_mult_vec3(rotation, direction, 1.0f, direction);

    /* Update the target based on the rotated direction */
	vec3_add(camera->position, direction, camera->target);
}

/**
 * @brief Move camera up
 * @param camera camera to move
 * @param distance distance to move
*/
void move_camera_up(t_camera* camera, float distance) 
{
    vec3_f32 direction, right, up_movement, up = {0.0f, 1.0f, 0.0f};;
    
	/* Compute direction vector */
	vec3_sub(camera->target, camera->position, direction);
	
	/* Compute right vector and normalise it */
    vec3_cross(direction, up, right);
	vec3_normalize(right);

	/* Compute up movement vector, normalise and scale it */
	vec3_cross(right, direction, up_movement);
	vec3_normalize(up_movement);
	vec3_scale(up_movement, distance, up_movement);

    vec3_add(camera->position, up_movement, camera->position); /* move up camera */
    vec3_add(camera->target, up_movement, camera->target); /* move up target */
}

/* Hard code camera postition */
void reset_camera(t_obj_model *model) 
{
    t_camera *camera = &model->cam;

    /* init camera position */
    CREATE_VEC3(23.756206f, 0.00000f, 2.155274f, camera->position);
    /* init camera target */
    CREATE_VEC3(14.963508f, 0.00000f, 1.405361f, camera->target);
    /* init up vector */
    CREATE_VEC3(0.00000f, 1.00000f, 0.00000f, camera->up);

    /* init model rotation mat4 */
	mat_identity(model->rotation);

	update_view_matrice(camera->position, camera->target, camera->up, camera->view);
}


/**
 * @brief Rotate object
 * @param camera camera context
 * @param rotate_vec vector used to rotate
 * @param angle angle used to rotate
 * @param shader_id shader id for update model matrix
*/
void rotate_object(t_obj_model *model, vec3_f32 rotate_vec, float angle, GLuint shader_id) 
{
    mat4_f32 rotation;

    /* Create rotation matrix */
    make_rotation(rotation, deg_to_rad(angle), rotate_vec);

    /* Multiply model matrix by rotation matrix */
    mat_mult(rotation, model->rotation, model->rotation);

	/* Update shader model matrix */
	set_shader_var_mat4(shader_id, "model", model->rotation);
}

/**
 * @brief Get object center
 * @param m object model
 * @param center center vector
*/
void get_obj_center(t_obj_model* m, vec3_f32 center) {
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
 * @brief Create a translation matrix
 * @param mat matrix to update
 * @param translation translation vector
*/
void make_translation(mat4_f32 mat, vec3_f32 translation) {
	mat_identity(mat);
	mat[3][0] = translation[0];
	mat[3][1] = translation[1];
	mat[3][2] = translation[2];
}

/**
 * @brief Multiply mat and translated matrix
 * @param mat matrix to update
 * @param translation translation vector
 * 
*/
void mat_mult_translation(mat4_f32 mat, vec3_f32 translation) {
	mat4_f32 translation_mat;
	make_translation(translation_mat, translation);
	mat_mult(translation_mat, mat, mat);
}

/**
 * @brief Negate a vector
 * @param dest destination vector
 * @param vec vector to negate
*/
void vec3_negate(vec3_f32 dest, vec3_f32 vec) {
	dest[0] = -vec[0];
	dest[1] = -vec[1];
	dest[2] = -vec[2];
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
    make_translation(translation_to_origin, obj_center_neg);

    /* Apply rotate */
    make_rotation(rotation, deg_to_rad(angle), rotate_vec);

    /* Update obj model */
    mat_mult(translation_to_origin, m->rotation, m->rotation);		/* Origin translate */
    mat_mult(rotation, m->rotation, m->rotation);					/* Apply rotate */
    mat_mult_translation(m->rotation, obj_center);					/* Translate back */

    /* Update model matrix in shader */
    set_shader_var_mat4(shader_id, "model", m->rotation);
}
