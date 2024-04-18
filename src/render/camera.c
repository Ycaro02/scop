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
	u32 vec3_size = sizeof(vec3_f32);
	ft_vec_copy(camera.position, (vec3_f32){0.0f, 0.0f, 3.0f}, vec3_size);
	ft_vec_copy(camera.target, (vec3_f32){0.0f, 0.0f, -1.0f}, vec3_size);
	ft_vec_copy(camera.up, (vec3_f32){0.0f, 1.0f, 0.0f}, vec3_size);


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

    make_rotatation(rotation, deg_to_rad(angle), axis);
    mat4_mult_vec3(rotation, camera->target, 1.0f, camera->target);

}

t_camera init_custom_camera() 
{
    t_camera camera;

    // init cam position
    camera.position[0] = 5.483057f;
    camera.position[1] = 0.00000f;
    camera.position[2] = 1.265557f;

    // init cam target
    camera.target[0] = -3.340549f;
    camera.target[1] = 0.00000f;
    camera.target[2] = 1.130818f;

    // init up vector
    camera.up[0] = 0.00000f;
    camera.up[1] = 1.00000f;
    camera.up[2] = 0.00000f;

    /* init view mat4 */
    float view[16] = {
        0.015268f, 0.00000f, 0.999883f, 0.00000f,
        0.00000f, 1.00000f, 0.00000f, 0.00000f,
        -0.999883f, 0.00000f, 0.015268f, 0.00000f,
        1.181691f, 0.00000f, -5.501741f, 1.00000f
    };
    ft_memcpy(camera.view, view, sizeof(view));

    /* init proj mat4 */
    float projection[16] = {
        2.414213f, 0.00000f, 0.00000f, 0.00000f,
        0.00000f, 2.414213f, 0.00000f, 0.00000f,
        0.00000f, 0.00000f, -1.002002f, -1.00000f,
        0.00000f, 0.00000f, -0.200200f, 0.00000f
    };
    ft_memcpy(camera.projection, projection, sizeof(projection));

    /* init model mat4 */
    float model[16] = {
        1.00000f, 0.00000f, 0.00000f, 0.00000f,
        0.00000f, 1.00000f, 0.00000f, 0.00000f,
        0.00000f, 0.00000f, 1.00000f, 0.00000f,
        0.00000f, 0.00000f, 0.00000f, 1.00000f
    };
    ft_memcpy(camera.model, model, sizeof(model));

    return (camera);
}


void rotate_object(t_camera* camera, vec3_f32 rotate_vec, float angle, GLuint shader_id) 
{
    mat4_f32 rotation;

    /* Create rotation matrix */
    make_rotatation(rotation, deg_to_rad(angle), rotate_vec);

    /* Multiply model matrix by rotation matrix */
    mat_mult(camera->model, rotation, camera->model);

	/* Update shader model matrix */
	set_shader_var_mat4(shader_id, "model", camera->model);
}
