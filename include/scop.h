#ifndef SCOP_HEADER_H
#define SCOP_HEADER_H

#define TOKEN_V 'v'


/*

Token 'v' :

Number of values: 3
Data type: float
Description: The coordinates of a 3D vertex.
Token 'vt' :

Number of values: 2 or 3
Data type: float
Description: The texture coordinates associated with a vertex.
Token 'vn' :

Number of values: 3
Data type: float
Description: The coordinates of the normal vector associated with a vertex.
Token 'f' :

Number of values: variable (3 or more)
Data type: uint32_t or int
Description: The indices of the vertices composing a face. It may also include indices of texture coordinates (vt) and normal vectors (vn) depending on the structure of your OBJ file.
Token 'mtllib' :

Number of values: 1
Data type: string
Description: The name of the material file associated with the object.
Token 'usemtl' :

Number of values: 1
Data type: string
Description: The name of the material to be used for the subsequent faces of the object.
Token 'o' :

Number of values: 1
Data type: string
Description: The name of the object.
Token 's' :

Number of values: 1
Data type: bool (true or false)
Description: The smoothing group state. 'on' to activate, 'off' to deactivate.
*/
#endif /* SCOP_HEADER_H */