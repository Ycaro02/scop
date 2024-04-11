#ifndef SCOP_HEADER_H
#define SCOP_HEADER_H

#include "../libft/libft.h"


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
#define TOKEN_F	"f"

typedef struct s_idxvertice_node {
	t_vec3_u32 	vec;				/* vec3 uint32 for 3 first val */
	u32			*other;				/* uin32_t int array aloc for other value if needed */
} t_idxvertice_node;


typedef t_idxvertice_node t_fnode;

typedef struct s_obj_file {
	char	*o;				/* The name of the object */
	char	*mtllib;		/* The name of the material file associated with the object */
	char	*usemtl;		/* The name of the material to be used for the subsequent faces of the object */
	t_list	*v;				/* The coordinates of a 3D vertex, list of t_vec3_float */
	t_list	*vt;			/* The texture coordinates associated with a vertex, list of vec2/vec3 float */
	t_list	*vn;			/* The coordinates of the normal vector associated with a vertex, list of t_vec3_float */
	t_list	*f;				/* The indices of the vertices composing a face, list t_fnode */
	u8		smooth;			/* The smoothing group state. 'on' to activate, 'off' to deactivate. 1 for true, otherwise 0*/
} t_obj_file;


/* Basic load file function */
char	**ft_split_trim(char const *str, char c);
char	**load_file(char *path);

#endif /* SCOP_HEADER_H */