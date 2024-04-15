#include "../../include/scop.h"

void set_shader_var_vec4(GLuint shader_id, char *var_name, t_vec4_float vec)
{
	glUseProgram(shader_id);
	
	GLint var_loc = glGetUniformLocation(shader_id, var_name);
	glUniform4f(var_loc, vec.x, vec.y, vec.z, vec.w);

}

void set_shader_var_mat4(GLuint shader_id, char *var_name, t_vec4_float *mat4)
{
	glUseProgram(shader_id);
	
	GLint var_loc = glGetUniformLocation(shader_id, var_name);
	glUniformMatrix4fv(var_loc, 1, GL_FALSE, (GLfloat *)mat4);
}
