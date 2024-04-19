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
    glm_rotate_make(rotation, glm_rad(angle), axis);

    /* Rotate the direction vector from the position to the target */
    vec3_f32 direction;
    glm_vec3_sub(camera->target, camera->position, direction);
    glm_mat4_mulv3(rotation, direction, 1.0f, direction);

    /* Update the target based on the rotated direction */
    glm_vec3_add(camera->position, direction, camera->target);
}

void move_camera_up(t_camera* camera, float distance) 
{
    vec3_f32 direction;
    glm_vec3_sub(camera->target, camera->position, direction); // Calcul du vecteur direction
    vec3_f32 up = {0.0f, 1.0f, 0.0f}; // Vecteur vertical
    vec3_f32 right;
    glm_cross(direction, up, right); // Calcul du vecteur horizontal (perpendiculaire à la direction et vertical)
    glm_normalize(right); // Normalisation du vecteur horizontal

    vec3_f32 up_movement;
    glm_cross(right, direction, up_movement); // Calcul du vecteur de déplacement vertical (perpendiculaire à la direction et horizontal)
    glm_normalize(up_movement); // Normalisation du vecteur de déplacement vertical
    glm_vec3_scale(up_movement, distance, up_movement); // Mise à l'échelle du vecteur de déplacement vertical selon la distance spécifiée

    glm_vec3_add(camera->position, up_movement, camera->position); // Déplacement de la position de la caméra vers le haut
    glm_vec3_add(camera->target, up_movement, camera->target); // Déplacement de la cible de la caméra vers le haut
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


void get_obj_center(t_obj_model* m, vec3_f32 center) {
	vec3_f32 total = {0.0f};

	for (u32 i = 0; i < m->tri_size; i++) {
		total[0] += m->tri_face[i][0];
		total[1] += m->tri_face[i][1];
		total[2] += m->tri_face[i][2];
	}

	center[0] = total[0] / m->tri_size;
	center[1] = total[1] / m->tri_size;
	center[2] = total[2] / m->tri_size;
}

void make_translation(mat4_f32 mat, vec3_f32 translation) {
	mat_identity(mat);
	mat[3][0] = translation[0];
	mat[3][1] = translation[1];
	mat[3][2] = translation[2];
}


void mat_mult_translation(mat4_f32 mat, vec3_f32 translation) {
	mat4_f32 translation_mat;
	make_translation(translation_mat, translation);
	mat_mult(translation_mat, mat, mat);
}

void vec3_negate(vec3_f32 dest, vec3_f32 vec) {
	dest[0] = -vec[0];
	dest[1] = -vec[1];
	dest[2] = -vec[2];
}

void rotate_object_around_center(t_obj_model* m, vec3_f32 rotate_vec, float angle, GLuint shader_id) 
{
    // Étape 1 : Trouver le centre de l'objet
    vec3_f32 obj_center;
    vec3_f32 obj_center_neg;
    get_obj_center(m, obj_center);

	ft_printf_fd(2, "center: %f %f %f\n", obj_center[0], obj_center[1], obj_center[2]);

    vec3_negate(obj_center_neg, obj_center);
    // Étape 2 : Translater l'objet pour que son centre soit à l'origine
    mat4_f32 translation_to_origin;
    make_translation(translation_to_origin, obj_center_neg);

    // Étape 3 : Appliquer la rotation
    mat4_f32 rotation;
    make_rotation(rotation, deg_to_rad(angle), rotate_vec);

    // Mettre à jour la matrice de modèle de l'objet
    mat_mult(translation_to_origin, m->rotation, m->rotation);    // Translater à l'origine
    mat_mult(rotation, m->rotation, m->rotation);                 // Appliquer la rotation
    mat_mult_translation(m->rotation, obj_center);             // Translater de retour au centre

    // Étape 4 : Mettre à jour la matrice de modèle dans le shader
    set_shader_var_mat4(shader_id, "model", m->rotation);
}
