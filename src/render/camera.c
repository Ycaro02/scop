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
    glm_vec3_copy((vec3){0.0f, 0.0f, 3.0f}, camera.position);
    glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera.target);
    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera.up);

    /* Init identity matrice 4x4 */
    glm_mat4_identity(camera.model);

    /* Compute view martice */
    glm_lookat(camera.position, camera.target, camera.up, camera.view);

    /* Compute projection matrice */
    glm_perspective(glm_rad(fov), aspect_ratio, near, far, camera.projection);
    return (camera);
}

/**
 * @brief Update camera
 * @param camera camera to update
 * @param shader_id shader id
*/
void update_camera(t_camera* camera, GLuint shader_id) 
{
    glm_lookat(camera->position, camera->target, camera->up, camera->view);

    // Mettre à jour la matrice de vue dans le shader
    GLint viewLoc = glGetUniformLocation(shader_id, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (GLfloat*)camera->view);

    // Mettre à jour la matrice de projection dans le shader
    GLint projLoc = glGetUniformLocation(shader_id, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, (GLfloat*)camera->projection);

    // Mettre à jour la matrice de modèle dans le shader
    GLint modelLoc = glGetUniformLocation(shader_id, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat*)camera->model);
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
    glm_vec3_normalize(direction);
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