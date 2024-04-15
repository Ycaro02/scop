#include "../../include/scop.h"


void display_brut_camera_data (t_camera *camera, t_obj_model *model, GLfloat *projection)
{
	for (u32 i = 0; i < 16; i++) {
		GLfloat *view = ((GLfloat *)camera->view) + i;
		GLfloat *identity = ((GLfloat *)(model->identity_mat4)) + i;
		GLfloat *model = ((GLfloat *)projection) + i;

		ft_printf_fd(1, ORANGE"Idx[%u]view: %f,"RESET, i, *view);
		ft_printf_fd(1, CYAN" model: %f,"RESET, i, *identity);
		ft_printf_fd(1, PINK" projection: %f\n"RESET, i, *model);
	}
}

/* Vectoriel product */
t_vec3_float cross_vec3(t_vec3_float a, t_vec3_float b) {
	t_vec3_float result;
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return (result);
}

t_vec3_float normalize_flaot_vec3(t_vec3_float v) {
    float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (len == (float)0) {
		ft_printf_fd(1, RED"Error: Normalize by 0\n"RESET);
		return (v);
	}
    t_vec3_float result;
    result.x = v.x / len;
    result.y = v.y / len;
    result.z = v.z / len;
    return (result);
}

void create_perspective_projection_matrix(float matrix[16], float fov, float aspectRatio, float nearPlane, float farPlane)
{
    // Convertir l'angle de champ de vision de degrés en radians
    float fovRad = fov * (PI / 180.0f);

    // Calculer la tangente de la moitié de l'angle de champ de vision
    float tanHalfFov = tanf(fovRad / 2.0f);

    // Initialiser la matrice à zéro
    for (int i = 0; i < 16; i++)
        matrix[i] = 0.0f;

    // Remplir la matrice de projection
    matrix[0] = 1.0f / (aspectRatio * tanHalfFov); // colonne 1, ligne 1
    matrix[5] = 1.0f / tanHalfFov; // colonne 2, ligne 2
    matrix[10] = -(farPlane + nearPlane) / (farPlane - nearPlane); // colonne 3, ligne 3
    matrix[11] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane); // colonne 3, ligne 4
    matrix[14] = -1.0f; // colonne 4, ligne 3
    matrix[15] = 0.0f; // colonne 4, ligne 4
}
/* scalar product between to vec3 float 	*/
float dot_vec3(t_vec3_float v1, t_vec3_float v2) {
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

t_camera create_camera_view(t_obj_model *model)
{
    t_camera camera = {0};

    camera.pos = CREATE_VEC3(float, 0.0f, 0.0f, 3.0f); /* camera pos */
    camera.dir = CREATE_VEC3(float, 0.0f, 0.0f, -1.0f); /* camera dir */
    camera.up = CREATE_VEC3(float, 0.0f, 1.0f, 0.0f); /* camera up */

    t_vec3_float direction = normalize_flaot_vec3(camera.dir);
    t_vec3_float right = normalize_flaot_vec3(cross_vec3(camera.up, direction));
    t_vec3_float up = cross_vec3(direction, right);

    camera.view[0] = CREATE_VEC4(float, right.x, right.y, right.z, -dot_vec3(right, camera.pos));
    camera.view[1] = CREATE_VEC4(float, up.x, up.y, up.z, -dot_vec3(up, camera.pos));
    camera.view[2] = CREATE_VEC4(float, direction.x, direction.y, direction.z, -dot_vec3(direction, camera.pos));
    camera.view[3] = CREATE_VEC4(float, 0.0f, 0.0f, 0.0f, 1.0f);

	glUseProgram(model->shader_id);

    GLint viewLoc = glGetUniformLocation(model->shader_id, "view");
	if (viewLoc == -1)
		ft_printf_fd(2, RED"Error: glGetUniformLocation view failed\n"RESET);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (GLfloat*)camera.view);

    GLint modelLoc = glGetUniformLocation(model->shader_id, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)(model->identity_mat4));

    float projection[16];
    
    create_perspective_projection_matrix(projection, 70.0f\
        , (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    GLint projLoc = glGetUniformLocation(model->shader_id, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, (GLfloat*)projection);


	display_brut_camera_data(&camera, model, projection);

    return (camera);
}

