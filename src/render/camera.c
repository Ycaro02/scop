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
	CREATE_VEC3(0.0f, 0.0f, 3.0f, camera.position);
	CREATE_VEC3(0.0f, 0.0f, -1.0f, camera.target);
	CREATE_VEC3(0.0f, 1.0f, 0.0f, camera.up);

    /* Init identity matrice 4x4 */
    mat_identity(camera.model); /* ft version need to rework name */

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
    set_shader_var_mat4(shader_id, "model", camera->model);
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
t_camera init_custom_camera() 
{
    t_camera camera;

    /* init camera position */
    CREATE_VEC3(11.782312f, 0.00000f, 1.361747f, camera.position);
    /* init camera target */
    CREATE_VEC3(2.958718f, 0.00000f, 1.227008f, camera.target);
    /* init up vector */
    CREATE_VEC3(0.00000f, 1.00000f, 0.00000f, camera.up);

    /* init view mat4 */
    CREATE_VEC4(0.015268f, 0.00000f, 0.999883f, 0.00000f, camera.view[0]);
    CREATE_VEC4(0.00000f, 1.00000f, 0.00000f, 0.00000f, camera.view[1]);
    CREATE_VEC4(-0.999883f, 0.00000f, 0.015268f, 0.00000f, camera.view[2]);
    CREATE_VEC4(1.181690f, 0.00000f, -11.801730f, 1.00000f, camera.view[3]);

    /* init projection mat4 */
    CREATE_VEC4(2.414213f, 0.00000f, 0.00000f, 0.00000f, camera.projection[0]);
    CREATE_VEC4(0.00000f, 2.414213f, 0.00000f, 0.00000f, camera.projection[1]);
    CREATE_VEC4(0.00000f, 0.00000f, -1.002002f, -1.00000f, camera.projection[2]);
    CREATE_VEC4(0.00000f, 0.00000f, -0.200200f, 0.00000f, camera.projection[3]);

    /* init model mat4 */
	mat_identity(camera.model);
    return (camera);
}


/**
 * @brief Rotate object
 * @param camera camera context
 * @param rotate_vec vector used to rotate
 * @param angle angle used to rotate
 * @param shader_id shader id for update model matrix
*/
void rotate_object(t_camera* camera, vec3_f32 rotate_vec, float angle, GLuint shader_id) 
{
    mat4_f32 rotation;

    /* Create rotation matrix */
    make_rotation(rotation, deg_to_rad(angle), rotate_vec);

    /* Multiply model matrix by rotation matrix */
    mat_mult(rotation, camera->model, camera->model);

	/* Update shader model matrix */
	set_shader_var_mat4(shader_id, "model", camera->model);
}
