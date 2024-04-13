#ifndef SCOP_HEADER_H
#define SCOP_HEADER_H

#include "../libft/libft.h"

#include "../glad_gen/include/glad/gl.h" /* include glad header before glfw3 */

#include "../include/glfw3.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

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

typedef struct s_face_node {
	t_vec3_u32 	vec;				/* vec3 uint32 for 3 first val */
	u32			*other;				/* uin32_t int array aloc for other value if needed otherwise NULL */
} t_face_node;

typedef struct s_obj_file {
	char	*o;				/* The name of the object */
	char	*mtllib;		/* The name of the material file associated with the object */
	char	*usemtl;		/* The name of the material to be used for the subsequent faces of the object */
	t_list	*vertex;		/* The coordinates of a 3D vertex, list of t_vec3_float */
	t_list	*vt;			/* The texture coordinates associated with a vertex, list of vec2/vec3 float */
	t_list	*vn;			/* The coordinates of the normal vector associated with a vertex, list of t_vec3_float */
	t_list	*face;				/* The indices of the vertices composing a face, list t_fnode */
	u8		smooth;			/* The smoothing group state. 'on' to activate, 'off' to deactivate. 1 for true, otherwise 0*/
} t_obj_file;


typedef struct s_obj_model {
	t_vec3_float	*vertex;	/* vertex array */
	u32				v_size;		/* vertex size */
	t_vec3_u32		*tri_face;	/* face array */
	u32				tri_size;		/* face size */
	// t_obj_file	*obj;		/* obj file structure */
	// u32			vt_size;		/* texture size */
	// u32			vn_size;		/* normal size */
	// t_vec3_float	*vt;		/* texture array */
	// t_vec3_float	*vn;		/* normal array */
	// u8			smooth;		/* The smoothing group state. 'on' to activate, 'off' to deactivate. 1 for true, otherwise 0*/
}	t_obj_model;

/* parser/load_file.c */
char			**ft_split_trim(char const *str, char c);
char			**load_file(char *path);

/* parser/obj_parse.c */
t_obj_model		*parse_obj_file(char *path);
t_vec3_float	*vertex_list_toarray(t_list *lst, u32 lst_size);
void			free_obj_file(t_obj_file *obj);

/* parser/build_obj_model.c */
t_vec3_float	*vertex_list_toarray(t_list *lst, u32 lst_size);
void			display_vertex_lst(t_list *lst);
void			free_obj_model(t_obj_model *model);
t_obj_model		*init_obj_model(t_obj_file *obj_file);
void init_gl_vertex_buffer(t_obj_model *model);
t_list *quadra_to_triangle(t_list *face_node_lst);

/* parser/parse_line.c  */
u8				handle_smooth_str(char *str);
void			free_obj_model(t_obj_model *model);
t_vec3_float	*line_to_vertex_node(char **line);
u8				line_to_face(t_obj_file *file, char **line);
t_vec3_u32		line_to_vec3_u32(char **line, u32 *other_val);




#endif /* SCOP_HEADER_H */