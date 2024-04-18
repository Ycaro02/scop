#ifndef SCOP_HEADER_H
#define SCOP_HEADER_H


#include <math.h>

// #include <cglm/cglm.h> /* need to remove */

#include "../glad_gen/include/glad/gl.h" /* include glad header before glfw3 */
#include "../include/glfw3.h"
#include "../libft/libft.h"

#define VERTEX_SHADER_PATH "rsc/shaders/vertex_shader.glsl"
#define FRAGMENT_SHADER_PATH "rsc/shaders/fragment_shader.glsl"

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700

enum e_obj_token {
	ENUM_UNKNOWN=0,
	ENUM_COMMENT=(1 << 0),
	ENUM_OBJ=(1 << 1),
	ENUM_SMOOTH=(1 << 2),
	ENUM_VERTEX=(1 << 3),
	ENUM_VT=(1 << 4),
	ENUM_VN=(1 << 5),
	ENUM_MTLLIB=(1 << 6),
	ENUM_USEMTL=(1 << 7),
	ENUM_F=(1 << 8)
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

/* Triangle data size */
#define TRIANGLE_DSIZE(model) (u32)(sizeof(vec3_u32) * model->tri_size)


/* Camera in implementation */
typedef struct t_camera {
    vec3_f32 position;				/* position vector */
    vec3_f32 target;				/* target vector */
    vec3_f32 up;					/* up vector */
    mat4_f32 view;					/* view matrix */
    mat4_f32 projection;			/* projection matrix */
	mat4_f32 model;					/* model matrix */
} t_camera;

/* Node used only for parse */
typedef struct s_face_node {
	vec3_u32 	vec;				/* vec3 uint32 for 3 first val */
	u32			*other;				/* uin32_t int array aloc for other value if needed otherwise NULL */
} t_face_node;

/* Brut parsing structure */
typedef struct s_obj_file {
	char	*o;				/* The name of the object */
	char	*mtllib;		/* The name of the material file associated with the object */
	char	*usemtl;		/* The name of the material to be used for the subsequent faces of the object */
	t_list	*vertex;		/* The coordinates of a 3D vertex, list of vec3_f32 */
	t_list	*vt;			/* The texture coordinates associated with a vertex, list of vec2/vec3 float */
	t_list	*vn;			/* The coordinates of the normal vector associated with a vertex, list of vec3_f32 */
	t_list	*face;				/* The indices of the vertices composing a face, list t_fnode */
	u8		smooth;			/* The smoothing group state. 'on' to activate, 'off' to deactivate. 1 for true, otherwise 0*/
} t_obj_file;

typedef struct s_obj_model {
	t_camera		cam;			/* camera structure */
	vec3_f32	*vertex;		/* vertex array */
	u32				v_size;			/* vertex size */
	vec3_u32		*tri_face;		/* face array */
	u32				tri_size;		/* face size */
	GLuint			vao;			/* vertex array object */
	GLuint			vbo;			/* vertex buffer object */
	GLuint			ebo;			/* element buffer object */
	GLuint			shader_id;		/* shader program id */
	vec3_f32		*colors;		/* color array */
	// t_obj_file	*obj;		/* obj file structure */
	// u32			vt_size;		/* texture size */
	// u32			vn_size;		/* normal size */
	// vec3_f32	*vt;		/* texture array */
	// vec3_f32	*vn;		/* normal array */
	// u8			smooth;		/* The smoothing group state. 'on' to activate, 'off' to deactivate. 1 for true, otherwise 0*/
}	t_obj_model;

#define S_ANGLE 5.0f
#define VEC3_ROTATEX (vec3_f32){0.0f, 1.0f, 0.0f}
#define VEC3_ROTATEY (vec3_f32){1.0f, 0.0f, 0.0f}
#define VEC3_ROTATEZ (vec3_f32){0.0f, 0.0f, 1.0f}


/* parser/load_file.c */
char			**ft_split_trim(char const *str, char c);
char			**load_file(char *path);

/* parser/obj_parse.c */
t_obj_model		*parse_obj_file(char *path);
void			free_obj_file(t_obj_file *obj);

/* parser/build_obj_model.c */
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
t_list			*quadra_to_triangle(t_list *face_node_lst);

/* render/camera */
t_camera		create_camera(float fov, float aspect_ratio, float near, float far);
void			update_camera(t_camera* camera, GLuint shader_id);
void			move_camera_forward(t_camera* camera, float distance);
void			move_camera_backward(t_camera* camera, float distance);
void			rotate_camera(t_camera* camera, float angle, vec3_f32 axis);
t_camera		init_custom_camera();
void			rotate_object(t_camera* camera, vec3_f32 rotate_vec, float angle, GLuint shader_id);

/*render/mat4*/
// vec4_f32	*create_mat4(vec4_f32 a, vec4_f32 b, vec4_f32 c, vec4_f32 d);
// vec4_f32	*create_mat4_identity();

/* render/shader_utils */
void			set_shader_var_mat4(GLuint shader_id, char *var_name, mat4_f32 data);
void			set_shader_var_vec4(GLuint shader_id, char *var_name, vec4_f32 vec);
GLuint			load_shader(t_obj_model *model);

#endif /* SCOP_HEADER_H */
