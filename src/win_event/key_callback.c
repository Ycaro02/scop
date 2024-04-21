#include "../../include/scop.h"

/* Escapte Key : GLFW_KEY_ESCAPE */
void act_escape(t_obj_model *model) {
    glfwSetWindowShouldClose(model->win_ptr, GL_TRUE);
}

/* Zoom */
void act_zoom(t_obj_model *model) {
    move_camera_forward(&model->cam, 1.0f);
}

/* Unzoom */
void act_unzoom(t_obj_model *model) {
    move_camera_backward(&model->cam, 1.0f);
}

/* Rotate horizontal camera +1 */
void act_rotate_horizontal_plus(t_obj_model *model) {
    rotate_camera(&model->cam, CAM_MOVE_ANGLE, VEC3_ROTATEX);
}

/* Rotate horizontal camera -1 */
void act_rotate_horizontal_minus(t_obj_model *model) {
    rotate_camera(&model->cam, -CAM_MOVE_ANGLE, VEC3_ROTATEX);
}

/* Up camera */
void act_up_camera(t_obj_model *model) {
    move_camera_up(&model->cam, 1.0f);
}

/* Down camera */
void act_down_camera(t_obj_model *model) {
    move_camera_up(&model->cam, -1.0f);
}

/* Reset cam */
void act_reset_camera(t_obj_model *model) {
    reset_camera(model);
}

/* Display cam data */
void act_display_camera_value(t_obj_model *model) {
    display_camera_value(&model->cam);
}

/* Change polygon mode */
void act_change_polygon_mode(t_obj_model *model) {
    static u8 fill_mode = 1;
	
	(void)model;
    fill_mode = !fill_mode;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE + fill_mode);
}

/* Rotate object left */
void act_rotate_object_left(t_obj_model *model) {
	rotate_object_around_center(model, VEC3_ROTATEX, ROTATE_ANGLE, model->shader_id);
}

/* Rotate object right */
void act_rotate_object_right(t_obj_model *model) {
	rotate_object_around_center(model, VEC3_ROTATEX, -ROTATE_ANGLE, model->shader_id);
}

/* Rotate object up */
void act_rotate_object_up(t_obj_model *model) {
	rotate_object_around_center(model, VEC3_ROTATEY, ROTATE_ANGLE, model->shader_id);
}

/* Rotate object down */
void act_rotate_object_down(t_obj_model *model) {
	rotate_object_around_center(model, VEC3_ROTATEY, -ROTATE_ANGLE, model->shader_id);
}

/* Rotate object Z up */
void act_rotate_object_z_up(t_obj_model *model) {
	rotate_object_around_center(model, VEC3_ROTATEZ, ROTATE_ANGLE, model->shader_id);
}

/* Stop/Start auto X rotation */
void act_stop_start_auto_x_rotation(t_obj_model *model) {
	reverse_flag(&model->status, STATUS_ROTATE_X);
}

/**
 * @brief Set key callback for the window
 * @param window 
 * @param key keycode receive from glfw
 * @param scancode unused
 * @param action action receive from glfw	
 * @param mode unused
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	(void)scancode, (void)mode;
	t_key_action	key_actions[] = {
		{GLFW_KEY_ESCAPE, act_escape},
		{GLFW_KEY_W, act_zoom},
		{GLFW_KEY_S, act_unzoom},
		{GLFW_KEY_A, act_rotate_horizontal_plus},
		{GLFW_KEY_D, act_rotate_horizontal_minus},
		{GLFW_KEY_Q, act_up_camera},
		{GLFW_KEY_E, act_down_camera},
		{GLFW_KEY_ENTER, act_reset_camera},
		{GLFW_KEY_SPACE, act_display_camera_value},
		{GLFW_KEY_P, act_change_polygon_mode},
		{GLFW_KEY_LEFT, act_rotate_object_left},
		{GLFW_KEY_RIGHT, act_rotate_object_right},
		{GLFW_KEY_UP, act_rotate_object_up},
		{GLFW_KEY_DOWN, act_rotate_object_down},
		{GLFW_KEY_PAGE_UP, act_rotate_object_z_up},
		{GLFW_KEY_R, act_stop_start_auto_x_rotation},
	};
	t_obj_model  	*model = NULL;
	u32 			max = (sizeof(key_actions) / sizeof(t_key_action));
	if (!window) {
		return ;
	}
	
	model = glfwGetWindowUserPointer(window);
	for (u32 i = 0; i < max; i++) {
		if (key_actions[i].key == key && action >= GLFW_PRESS) {
			key_actions[i].action(model);
		}
	}
}