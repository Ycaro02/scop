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
	
	/* CGLM version */
    // glm_vec3_copy((vec3){0.0f, 0.0f, 3.0f}, camera.position);
    // glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera.target);
    // glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera.up);

	/* NEW VERSION*/
	u32 vec3_size = sizeof(vec3_float);
	ft_vec_copy(camera.position, (vec3_float){0.0f, 0.0f, 3.0f}, vec3_size);
	ft_vec_copy(camera.target, (vec3_float){0.0f, 0.0f, -1.0f}, vec3_size);
	ft_vec_copy(camera.up, (vec3_float){0.0f, 1.0f, 0.0f}, vec3_size);

    /* Init identity matrice 4x4 */
    // glm_mat4_identity(camera.model);
    mat_identity(camera.model); /* ft version need to rework name */

    /* Compute view martice */
    // glm_lookat(camera.position, camera.target, camera.up, camera.view);
    glm_lookat_rh(camera.position, camera.target, camera.up, camera.view);

    /* Compute projection matrice */
    // glm_perspective(glm_rad(fov), aspect_ratio, near, far, camera.projection);
    glm_perspective_rh_no(glm_rad(fov), aspect_ratio, near, far, camera.projection);
    return (camera);
}

/**
 * @brief Update camera
 * @param camera camera to update
 * @param shader_id shader id
*/
void update_camera(t_camera* camera, GLuint shader_id) 
{
    glm_lookat_rh(camera->position, camera->target, camera->up, camera->view);
    // glm_lookat(camera->position, camera->target, camera->up, camera->view);

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
    vec3 direction;

    glm_vec3_sub(camera->target, camera->position, direction);
    glm_vec3_normalize(direction); /* tocheck */
    glm_vec3_scale(direction, distance, direction);
    glm_vec3_add(camera->position, direction, camera->position);
    glm_vec3_add(camera->target, direction, camera->target);

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
void rotate_camera(t_camera* camera, float angle, vec3 axis) {
    mat4 rotation;
    glm_rotate_make(rotation, glm_rad(angle), axis);
    glm_mat4_mulv3(rotation, camera->target, 1.0f, camera->target);
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
