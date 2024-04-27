#include "../../include/scop.h"

void handle_auto_rotate(t_obj_model *model)
{
	u8 x = has_flag(model->status, STATUS_ROTATE_X);
	u8 y = has_flag(model->status, STATUS_ROTATE_Y);
	u8 z = has_flag(model->status, STATUS_ROTATE_Z);

	if (!x && !y && !z) {
		set_shader_var_mat4(model->shader_id, "model", model->rotation);
		return;
	}
	if (x) {
		rotate_object_around_center(model, VEC3_ROTATEX, 2.0f, model->shader_id);
	} if (y) {
		rotate_object_around_center(model, VEC3_ROTATEY, 2.0f, model->shader_id);
	} if (z) {
		rotate_object_around_center(model, VEC3_ROTATEZ, 2.0f, model->shader_id);
	} 
}

/**
 * @brief Main loop of the program
 * @param model model structure
 * @param win window structure
*/
void main_loop(t_obj_model *model, GLFWwindow *win) 
{
    while (!glfwWindowShouldClose(win)) {
        /* clear gl render context*/
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		handle_auto_rotate(model);
		update_camera(&model->cam, model->shader_id);

		/* bind vertex array */
		glBindVertexArray(model->vao);
		glDrawElements(GL_TRIANGLES, (model->tri_size * 3), GL_UNSIGNED_INT, 0);

		/* unbind vertex array */
		glBindVertexArray(0);
		/* swap buff to display */
		glfwSwapBuffers(win);
        /* Event handling */
        glfwPollEvents();
		handle_input(model);
    }
}
