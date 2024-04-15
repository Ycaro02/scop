#include "../../include/scop.h"

t_camera create_camera(float fov, float aspect_ratio, float near, float far) 
{
    t_camera camera;

    // Initialiser la position, la cible et le vecteur "up" de la caméra
    glm_vec3_copy((vec3){0.0f, 0.0f, 3.0f}, camera.position);
    glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera.target);
    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera.up);

    // Initialiser la matrice de modèle à l'identité
    glm_mat4_identity(camera.model);

    // Calculer la matrice de vue
    glm_lookat(camera.position, camera.target, camera.up, camera.view);

    // Calculer la matrice de projection
    glm_perspective(glm_rad(fov), aspect_ratio, near, far, camera.projection);

    return camera;
}

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

void move_camera_forward(t_camera* camera, float distance) 
{
    vec3 direction;
    glm_vec3_sub(camera->target, camera->position, direction);
    glm_vec3_normalize(direction);
    glm_vec3_scale(direction, distance, direction);
    glm_vec3_add(camera->position, direction, camera->position);
    glm_vec3_add(camera->target, direction, camera->target);
}

void move_camera_backward(t_camera* camera, float distance) {
    move_camera_forward(camera, -distance);
}

void rotate_camera(t_camera* camera, float angle, vec3 axis) {
    mat4 rotation;
    glm_rotate_make(rotation, glm_rad(angle), axis);
    glm_mat4_mulv3(rotation, camera->target, 1.0f, camera->target);
}