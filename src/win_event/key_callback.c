#include "../../include/scop.h"

/* Escapte Key : ESC */
void act_escape(t_obj_model *model) {
    glfwSetWindowShouldClose(model->win_ptr, GL_TRUE);
}



/* Zoom : W */
void act_zoom(t_obj_model *model) {
    move_camera_forward(&model->cam, CAM_ZOOM);
}

/* Unzoom : S */
void act_unzoom(t_obj_model *model) {
    move_camera_backward(&model->cam, CAM_ZOOM);
}

/* Rotate camera left : A */
void act_rotate_camera_left(t_obj_model *model) {
	rotate_camera(&model->cam, CAM_MOVE_HORIZONTAL, VEC3_ROTATEY);
}

/* Rotate camera right D */
void act_rotate_camera_right(t_obj_model *model) {
    rotate_camera(&model->cam, -CAM_MOVE_HORIZONTAL, VEC3_ROTATEY);
}

/* Up camera : E */
void act_up_camera(t_obj_model *model) {
    move_camera_up(&model->cam, CAM_UP_DOWN);
}

/* Down camera : Q */
void act_down_camera(t_obj_model *model) {
    move_camera_up(&model->cam, -CAM_UP_DOWN);
}

/* Reset cam : ENTER */
void act_reset_camera(t_obj_model *model) {
    reset_camera(model);
}

/* Display cam data : SPACE */
void act_display_camera_value(t_obj_model *model) {
    display_camera_value(&model->cam);
}

/* Change polygon mode : P */
void act_change_polygon_mode(t_obj_model *model) {
	/* To store in context structure */
    static u8 fill_mode = 1;
	
	(void)model;
    fill_mode = !fill_mode;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE + fill_mode);
}

/* Rotate object left : LEFT */
void act_rotate_object_left(t_obj_model *model) {
	rotate_object_around_center(model, VEC3_ROTATEX, ROTATE_ANGLE, model->shader_id);
}

/* Rotate object right : RIGHT */
void act_rotate_object_right(t_obj_model *model) {
	rotate_object_around_center(model, VEC3_ROTATEX, -ROTATE_ANGLE, model->shader_id);
}

/* Rotate object up : UP */
void act_rotate_object_up(t_obj_model *model) {
	rotate_object_around_center(model, VEC3_ROTATEY, ROTATE_ANGLE, model->shader_id);
}

/* Rotate object down : DOWN */
void act_rotate_object_down(t_obj_model *model) {
	rotate_object_around_center(model, VEC3_ROTATEY, -ROTATE_ANGLE, model->shader_id);
}

/* Rotate object Z up : PAGE UP*/
void act_rotate_object_z_up(t_obj_model *model) {
	rotate_object_around_center(model, VEC3_ROTATEZ, ROTATE_ANGLE, model->shader_id);
}

/* Stop/Start auto X rotation : R */
void act_stop_start_x_rotation(t_obj_model *model) {
	reverse_flag(&model->status, STATUS_ROTATE_X);
}

/* Stop/Start auto Y rotation : T */
void act_stop_start_y_rotation(t_obj_model *model) {
	reverse_flag(&model->status, STATUS_ROTATE_Y);
}

/* Stop/Start auto Z rotation : Y */
void act_stop_start_z_rotation(t_obj_model *model) {
	reverse_flag(&model->status, STATUS_ROTATE_Z);
}


/**
 * @brief Set key callback for the window
 * @param window 
 * @param key keycode receive from glfw
 * @param scancode unused
 * @param action action receive from glfw	
 * @param mode unused
*/
void handle_input(t_obj_model *model)
{
	/* To store in context structure */
	static u8		previous_state[GLFW_KEY_LAST] = {0};
	t_key_action	key_actions[] = {
		{GLFW_KEY_ESCAPE, act_escape, SINGLE_PRESS},
		{GLFW_KEY_W, act_zoom, REPEAT},
		{GLFW_KEY_S, act_unzoom, REPEAT},
		{GLFW_KEY_A, act_rotate_camera_left, REPEAT},
		{GLFW_KEY_D, act_rotate_camera_right, REPEAT},
		{GLFW_KEY_Q, act_up_camera, REPEAT},
		{GLFW_KEY_E, act_down_camera, REPEAT},
		{GLFW_KEY_ENTER, act_reset_camera, SINGLE_PRESS},
		{GLFW_KEY_SPACE, act_display_camera_value, SINGLE_PRESS},
		{GLFW_KEY_P, act_change_polygon_mode, SINGLE_PRESS},
		{GLFW_KEY_LEFT, act_rotate_object_left, REPEAT},
		{GLFW_KEY_RIGHT, act_rotate_object_right, REPEAT},
		{GLFW_KEY_UP, act_rotate_object_up, REPEAT},
		{GLFW_KEY_DOWN, act_rotate_object_down, REPEAT},
		{GLFW_KEY_PAGE_UP, act_rotate_object_z_up, REPEAT},
		{GLFW_KEY_R, act_stop_start_y_rotation, SINGLE_PRESS},
		{GLFW_KEY_T, act_stop_start_x_rotation, SINGLE_PRESS},
		{GLFW_KEY_Y, act_stop_start_z_rotation, SINGLE_PRESS},
	};
	u32 			max = (sizeof(key_actions) / sizeof(t_key_action));
	s32				state = GLFW_RELEASE;
 	
	for (u32 i = 0; i < max; i++) {
        state = glfwGetKey(model->win_ptr, key_actions[i].key);

		if (!key_actions[i].repeat) { /* if not repeat key */ 
			if (state == GLFW_PRESS \
				&& previous_state[key_actions[i].key] != (u8)GLFW_PRESS) { /* and key pressed and previous state not key_pressed */
				key_actions[i].action(model);
			}
		} else if (state == GLFW_PRESS) { /* If repeat and key pressed */
			key_actions[i].action(model);
		}
        previous_state[key_actions[i].key] = state;
	}
}