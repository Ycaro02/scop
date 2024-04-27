#ifndef SCOP_HEADER_H
#define SCOP_HEADER_H

#include <math.h>

#include "../glad_gen/include/glad/gl.h" /* include glad header before glfw3 */
#include "../include/glfw3.h"
#include "../libft/libft.h"
#include "../libft/parse_flag/parse_flag.h"
#include "../libft/stack_string/stack_string.h"
#include "../src/parser/parse_bmp.h"

/* Vertex shader source path */
#define VERTEX_SHADER_PATH "rsc/shaders/vertex_shader.glsl"

/* Fragment shader source path */
#define FRAGMENT_SHADER_PATH "rsc/shaders/fragment_shader.glsl"

#define TEXTURE_MANDATORY_PATH "rsc/texture/kitten.bmp"
#define TEXTURE_BRICK_PATH "rsc/texture/brick.bmp"

#define MODEL_PATH "rsc/model/"

/* Window size */
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 1080

#define CAM_ZOOM 0.3f				/* Zoom/Unzoom value */
#define CAM_MOVE_HORIZONTAL 1.0f	/* Move camera horizontal value */
#define CAM_UP_DOWN 0.05f			/* Move camera up/down value */

#define ROTATE_ANGLE 4.0f			/* Rotate obj angle when arrow pressed */

#define VEC3_ROTATEX (vec3_f32){1.0f, 0.0f, 0.0f}
#define VEC3_ROTATEY (vec3_f32){0.0f, 1.0f, 0.0f}
#define VEC3_ROTATEZ (vec3_f32){0.0f, 0.0f, 1.0f}

/* Triangle data size */
#define TRIANGLE_DSIZE(model) (u32)(sizeof(vec3_u32) * model->tri_size)

/* Texture handle define */

#define X_UNUSED 1U
#define Y_UNUSED 2U
#define Z_UNUSED 4U

#define X_FIELD 0U
#define Y_FIELD 1U
#define Z_FIELD 2U


/* Enum for obj token accepted */
enum e_obj_token {
	ENUM_UNKNOWN=0,
	ENUM_COMMENT=(1u << 0),
	ENUM_OBJ=(1u << 1),
	ENUM_SMOOTH=(1u << 2),
	ENUM_VERTEX=(1u << 3),
	ENUM_VT=(1u << 4),
	ENUM_VN=(1u << 5),
	ENUM_MTLLIB=(1u << 6),
	ENUM_USEMTL=(1u << 7),
	ENUM_F=(1u << 8)
};

/* Token accepted in OBJ file */
#define TOKEN_UNKNOWN	"unknown"	/* Type: string Nb Value: 1 Description: Unknown token. */
#define TOKEN_COMMENT	"#"			/* Type: string Nb Value: 1 Description: A comment line. */
#define TOKEN_OBJ 		"o"			/* Type: string Nb Value: 1 Description: The name of the object */
#define TOKEN_SMOOTH	"s"			/* Type: bool (true or false) Nb Value: 1 Description: The smoothing group state. on to activate, off to deactivate.*/
#define TOKEN_VERTEX	"v"			/* Type: float Nb Value: 3 Description: The coordinates of a 3D vertex.*/
#define TOKEN_VT		"vt"		/* Type: float Nb Value: 2 or 3 Description: The texture coordinates associated with a vertex.*/
#define TOKEN_VN		"vn"		/* Type: float Nb Value: 3 Description: The coordinates of the normal vector associated with a vertex.*/
#define TOKEN_MTLLIB	"mtllib"	/* Type: string Nb Value: 1 Description: The name of the material file associated with the object.*/
#define TOKEN_USEMT		"usemtl"	/* Type: string Nb Value: 1 Description: The name of the material to be used for the subsequent faces of the object */

/*
 *	Description: The indices of the vertices composing a face. 
 *	It may also include indices of texture coordinates (vt) and normal vectors (vn)
 *	depending on the structure of the OBJ file.
 *	Type: uint32_t or int 
 *	Nb Value: variable (3 or more)
*/
#define TOKEN_FACE	"f"

/* Token array for obj file  */
#define OBJ_TOKEN_ARRAY {TOKEN_COMMENT, TOKEN_OBJ, TOKEN_SMOOTH, TOKEN_VERTEX, TOKEN_VT, TOKEN_VN, TOKEN_MTLLIB, TOKEN_USEMT, TOKEN_FACE, NULL}

/* Node used only for parse */
typedef struct s_face_node {
	vec3_u32 		vec;				/* vec3 uint32 for 3 first val */
	u32				*other;				/* uin32_t int array aloc for other value if needed otherwise NULL */
} t_face_node;

/* Brut parsing structure */
typedef struct s_obj_file {
	char			*o;				/* The name of the object */
	char			*mtllib;		/* The name of the material file associated with the object */
	char			*usemtl;		/* The name of the material to be used for the subsequent faces of the object */
	t_list			*vertex;		/* The coordinates of a 3D vertex, list of vec3_f32 */
	t_list			*face;			/* The indices of the vertices composing a face, list t_fnode */
	t_list			*vt;			/* The texture coordinates associated with a vertex, list of vec2/vec3 float */
	t_list			*vn;			/* The coordinates of the normal vector associated with a vertex, list of vec3_f32 */
	u8				smooth;			/* The smoothing group state. 'on' to activate, 'off' to deactivate. 1 for true, otherwise 0*/
} t_obj_file;

/* Material parse */

enum e_material_token {
	ENUM_MTL_UNKNOWN=0,
	ENUM_MTL_COMMENT=(1u << 0),
	ENUM_MTL_NEWMTL=(1u << 1),
	ENUM_MTL_KA=(1u << 2),
	ENUM_MTL_KD=(1u << 3),
	ENUM_MTL_KS=(1u << 4),
	ENUM_MTL_NS=(1u << 5),
	ENUM_MTL_NI=(1u << 6),
	ENUM_MTL_D=(1u << 7),
	ENUM_MTL_ILLUM=(1u << 8)
};

#define TOKEN_MTL_NEWMTL	"newmtl"	/* Type: string Nb Value: 1 Description: The name of the material */
#define TOKEN_MTL_KA		"Ka"		/* Type: float Nb Value: 3 Description: Ambient color */
#define TOKEN_MTL_KD		"Kd"		/* Type: float Nb Value: 3 Description: Diffuse color */
#define TOKEN_MTL_KS		"Ks"		/* Type: float Nb Value: 3 Description: Specular color */
#define TOKEN_MTL_NS		"Ns"		/* Type: float Nb Value: 1 Description: Specular exponent */
#define TOKEN_MTL_NI		"Ni"		/* Type: float Nb Value: 1 Description: Optical density */
#define TOKEN_MTL_D			"d"			/* Type: float Nb Value: 1 Description: Dissolve factor */
#define TOKEN_MTL_ILLUM		"illum"		/* Type: int Nb Value: 1 Description: Illumination model */

#define MATERIAL_TOKEN_ARRAY {TOKEN_COMMENT, TOKEN_MTL_NEWMTL, TOKEN_MTL_KA, TOKEN_MTL_KD, TOKEN_MTL_KS, TOKEN_MTL_NS, TOKEN_MTL_NI, TOKEN_MTL_D, TOKEN_MTL_ILLUM, NULL}

typedef struct s_material_file {
	char			*newmtl;		/* The name of the material */
	vec3_f32		ka;				/* Ambient color */
	vec3_f32		kd;				/* Diffuse color */
	vec3_f32		ks;				/* Specular color */
	float			ns;				/* Specular exponent */
	float			ni;				/* Optical density */
	float			d;				/* Dissolve factor */
	u32				illum;			/* Illumination model */
} t_material_file;

/* Camera structure */
typedef struct t_camera {
    vec3_f32		position;			/* position vector */
    vec3_f32		target;				/* target vector */
    vec3_f32		up;					/* up vector */
    mat4_f32		view;				/* view matrix */
    mat4_f32		projection;			/* projection matrix */
} t_camera;

typedef struct s_obj_face {
	t_list		*vertex;			/* list of vec3_f32, list of vertex for this face, all triangle vertex for each face */
	t_list		*idx;				/* list of u32, list of index for each vertex */
	u32			size;				/* number of vertex in this face */
	u32			id;					/* face id */
} t_obj_face;

/* Model structure */
typedef struct s_obj_model {
	t_camera		cam;			/* camera structure */
	vec3_f32		*vertex;		/* vertex array, give to openGL context */
	u32				v_size;			/* vertex size */
	vec3_u32		*tri_face;		/* face array, give to openGL context, each vector is a triangle, each point represent index of vertex to link */
	u32				tri_size;		/* face size */
	t_list			*obj_face;		/* list of obj face structure */
	t_material_file	*material;		/* material file structure */
	vec3_f32		*colors;		/* color array, give to openGL context, one for each triangle vertex */
	vec2_f32		*texture_coord;	/* texture coordinates associated with a vertex */
	GLFWwindow		*win_ptr;		/* window pointer */
	mat4_f32		rotation;		/* model rotation matrix */
	vec3_f32		center;			/* model center */
	vec3_f32		min;			/* model min */
	vec3_f32		max;			/* model max */
	GLuint			vao;			/* vertex array object */
	GLuint			vbo;			/* vertex buffer object */
	GLuint			ebo;			/* element buffer object */
	GLuint			shader_id;		/* shader program id */
	u32				status;			/* model rotation state */
	f32				tex_intensity;	/* texture intensity */
	s8				texture_mod;	/* texture mod, 1 for true, -1 for false */
}	t_obj_model;


/* Key repeat, key_action->repeat field */
#define SINGLE_PRESS	0				/* key action will be done only once */
#define REPEAT			1				/* key action will be done until key release */


/* Texture color scale factor */
#define TEXTURE_COLOR_SCALE_FACTOR 0.0075f


/* Key action structure */
typedef struct s_key_action {
    int key;							/* key code */
    void (*action)(t_obj_model *model);	/* action to do, function ptr void (t_obj_model *model)*/
	u8	repeat;							/* auto repeat until key release */
} t_key_action;

/* Enum for model status */
enum model_status {
	STATUS_NONE=0,
	STATUS_ROTATE_X=(1 << 0),
	STATUS_ROTATE_Y=(1 << 1),
	STATUS_ROTATE_Z=(1 << 2),
};

// t_obj_file	*obj;		/* obj file structure */
// u32			vt_size;	/* texture size */
// u32			vn_size;	/* normal size */
// vec3_f32	*vt;			/* texture array */
// vec3_f32	*vn;			/* normal array */
// u8			smooth;		/* The smoothing group state. 'on' to activate, 'off' to deactivate. 1 for true, otherwise 0*/


void			get_obj_center(t_obj_model* m, vec3_f32 center);
void set_shader_var_float(GLuint shader_id, char *name, float value);

/* parser/material_parse.c */
t_material_file *parse_mtl_file(char *path);
void 			display_material_data(t_material_file *file);

/* parser/obj_parse.c */
t_obj_model		*parse_obj_file(char *path);
void			free_obj_file(t_obj_file *obj);

/* parser/utils */
u16				is_valid_token(char **valid_tokens, char *to_check);
s8				get_str_after_token(char **to_fill_ptr, char **line);

/* builder/build_obj_model.c */
void			display_vertex_lst(t_list *lst);
void			free_obj_model(t_obj_model *model);
t_obj_model		*init_obj_model(t_obj_file *obj_file);
t_list			*quadra_to_triangle(t_list *face_node_lst);
void			init_gl_triangle_array(t_obj_model *model);

/* parser/parse_line.c  */
u8				handle_smooth_str(char *str);
void			free_obj_model(t_obj_model *model);
u8				line_to_face(t_obj_file *file, char **line);

/* to revork */
void			line_to_vec3_u32(char **line, u32 *other_val, vec3_u32 *vec, u8 *error);
vec3_f32		*line_to_vertex_node(char **line);


/* main to move */
void			check_struct_size(char *str_test, u32 struct_size, u32 wanted_size);

/* parser/build_triangle_lst.c */
t_list			*quadra_to_triangle(t_list *face_node_lst);
t_list 			*get_all_triangle_vertex(t_obj_model *model, t_list **idx_lst_ptr);

/* parser/face_handling.c */
t_list			*get_all_face_vertex(t_obj_file *file, t_obj_model *model);
void			free_obj_face_lst(t_list **face_lst);

/* render/camera */
t_camera		create_camera(float fov, float aspect_ratio, float near, float far);
void			update_camera(t_camera* camera, GLuint shader_id);
void			display_camera_value(t_camera *cam);
void			move_camera_forward(t_camera* camera, float distance);
void			move_camera_backward(t_camera* camera, float distance);
void 			rotate_camera(t_camera* camera, float angle, vec3_f32 axis);
void 			reset_camera(t_obj_model *model);
void			move_camera_up(t_camera* camera, float distance) ;
void			rotate_object_around_center(t_obj_model* m, vec3_f32 rotate_vec, float angle, GLuint shader_id);

/* window/openGL_glw_init.c */
GLFWwindow		*init_openGL_context();
void			glfw_destroy(GLFWwindow *win, t_obj_model *model);

/* render/shader_utils */
void			set_shader_var_mat4(GLuint shader_id, char *var_name, mat4_f32 data);
void			set_shader_var_vec4(GLuint shader_id, char *var_name, vec4_f32 vec);
GLuint			load_shader(t_obj_model *model);

/* render/main_loop.c */
void			main_loop(t_obj_model *model, GLFWwindow *win);

/* win_event/key_callback.c */
void			handle_input(t_obj_model *model);

/* builder/texture.c */
u8				build_material_texture(t_obj_model *model);

/* builder.colors.c */
u8				hard_build_color(t_obj_model *model);

/* builder/load_texture.c */
GLuint			init_openGL_texture(t_obj_model* model, u8 *data, u32 width, u32 height, u16 texture_type);
s8				brut_load_texture(char *path, t_obj_model *model);

#endif /* SCOP_HEADER_H */
