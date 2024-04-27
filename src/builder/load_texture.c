#include "../../include/scop.h"

GLuint init_openGL_texture(t_obj_model* model, u8 *data, u32 width, u32 height, u16 texture_type)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	/* Set the texture wrapping parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	/* Set texture filtering parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// Set texture filtering to GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	GLfloat anisotropy_level = 8.0f;  // adjust this value to your preference
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, anisotropy_level);
	/* Load the texture */
	glTexImage2D(GL_TEXTURE_2D, 0, texture_type, width, height, 0, texture_type, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	GLuint texture_loc = glGetUniformLocation(model->shader_id, "texture1");
	glUniform1i(texture_loc, 0);

	return (texture);
}


/**
 * @brief Load a texture from a file
 * @param path path to the texture file
 * @return u8* return the texture data
*/
s8 brut_load_texture(char *path, t_obj_model *model)
{
	int width, height, type;
	// u8 *data = stbi_load(path, &width, &height, type, 0);
	u8 *data = parse_bmp_file(path, &width, &height, &type);


	if (!data) {
		ft_printf_fd(2, RED"Error: Failed to load texture\n"RESET);
		return (FALSE);
	}
	// ft_printf_fd(1, "Texture loaded: %s\n", path);
	// ft_printf_fd(1, PINK"Width: %d, Height: %d, Channels: %d\n"RESET, width, height, *type);
	if (type == 3) {
		type = GL_RGB;
	} else if (type == 4) {
		type = GL_RGBA;
	} else {
		ft_printf_fd(2, RED"Error: Unknow texture type only RGB or RGBA supported\n"RESET);
		free(data);
		return (FALSE);
	}
	init_openGL_texture(model, data, width, height, type);
	free(data);
	return (TRUE);
}