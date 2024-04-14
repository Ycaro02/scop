#include "../../include/scop.h"


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
    matrix[11] = -1.0f; // colonne 3, ligne 4
    matrix[14] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane); // colonne 4, ligne 3
}

t_camera create_camera_view(t_obj_model *model)
{
	t_camera camera = {0};
	// t_vec3_float target = {0.0f, 0.0f, 0.0f}; /* target dir */

	camera.pos = CREATE_VEC3(float, 0.0f, 0.0f, 3.0f); /* camera pos */
	camera.dir = CREATE_VEC3(float, 0.0f, 0.0f, -1.0f); /* camera dir */
	camera.up = CREATE_VEC3(float, 0.0f, 1.0f, 0.0f); /* camera up */

	DISPLAY_VEC3(float, camera.pos);
	// t_vec3_float direction = normalize_flaot_vec3(SUB_VEC3(float, camera.pos, target));
	t_vec3_float direction = normalize_flaot_vec3(camera.dir);
	t_vec3_float right = normalize_flaot_vec3(cross_vec3(camera.up, direction));
	t_vec3_float up = cross_vec3(direction, right);


	camera.view[0] = CREATE_VEC4(float, right.x, right.y, right.z, 0.0f);
	camera.view[1] = CREATE_VEC4(float, up.x, up.y, up.z, 0.0f);
	camera.view[2] = CREATE_VEC4(float, direction.x, direction.y, direction.z, 0.0f);
	camera.view[3] = CREATE_VEC4(float, -camera.pos.x, -camera.pos.y, -camera.pos.z, 1.0f);

	GLint viewLoc = glGetUniformLocation(model->vertex_shader_id, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (GLfloat*)camera.view);

	GLint modelLoc = glGetUniformLocation(model->vertex_shader_id, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)(model->identity_mat4));

	float projection[16];
	
	create_perspective_projection_matrix(projection, 45.0f\
		, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

	GLint projectionLoc = glGetUniformLocation(model->vertex_shader_id, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (GLfloat*)projection);

	for (u32 i = 0; i < 16; i++) {
		float *view = ((float *)camera.view) + i;
		float *identity = ((float *)(model->identity_mat4)) + i;
		float *model = ((float *)projection) + i;

		ft_printf_fd(1, ORANGE"view[%u]: %f\n"RESET, i, *view);
		ft_printf_fd(1, CYAN"identity[%u]: %f\n"RESET, i, *identity);
		ft_printf_fd(1, PINK"model[%u]: %f\n"RESET, i, *model);
	}


	for (u32 i = 0; i < 4; i++) {
		// DISPLAY_VEC4(float, camera.view[i]);
		DISPLAY_VEC4(float, model->identity_mat4[i]);
	}

	return (camera);
}
