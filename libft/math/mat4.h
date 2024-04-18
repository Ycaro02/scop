#ifndef MAT4_HEADER_H
#define MAT4_HEADER_H

#include "ft_math.h"

extern void	*ft_memcpy(void *dest, const void *src, size_t n);

typedef vec4_f32 mat4_f32[4];

/* Identity mat4 init */
#define MAT4_IDENTITY_INIT		{{1.0f, 0.0f, 0.0f, 0.0f},                   \
								{0.0f, 1.0f, 0.0f, 0.0f},                    \
								{0.0f, 0.0f, 1.0f, 0.0f},                    \
								{0.0f, 0.0f, 0.0f, 1.0f}}

/* Copy identity mat4 in mat */
FT_INLINE void mat_identity(vec4_f32 *mat) {
	mat4_f32 tmp = MAT4_IDENTITY_INIT;
	ft_memcpy(mat, &tmp, (sizeof(vec4_f32) * 4));
}

FT_INLINE void mat_zero(mat4_f32 mat) {
	ft_bzero(mat, sizeof(mat4_f32));
}

/**
 * @brief Update view mattrice 4x4
 * @param[in] eye position of the eye
 * @param[in] center position of the center
 * @param[in] up up vector
 * @param[out] dest Destination matrix
*/
FT_INLINE void update_view_matrice(vec3_f32 eye, vec3_f32 center, vec3_f32 up, mat4_f32 dest) {
    vec3_f32 f, u, s;

    vec3_sub(center, eye, f);
    vec3_normalize(f);

    vec3_cross(f, up, s);
    vec3_normalize(s);

    vec3_cross(s, f, u);

    dest[0][0] = s[0]; dest[0][1] = u[0]; dest[0][2] =-f[0]; 
    dest[1][0] = s[1]; dest[1][1] = u[1]; dest[1][2] =-f[1]; 
    dest[2][0] = s[2]; dest[2][1] = u[2]; dest[2][2] =-f[2]; 
    dest[3][0] =-VEC3_DOT(s, eye);
    dest[3][1] =-VEC3_DOT(u, eye);
    dest[3][2] = VEC3_DOT(f, eye);
    dest[0][3] = dest[1][3] = dest[2][3] = 0.0f;
    dest[3][3] = 1.0f;  
}


/**
 * @brief Get perspective matrice 4x4
 * @param[in] fovy field of view
 * @param[in] aspect aspect ratio
 * @param[in] nearZ near plane
 * @param[in] farZ far plane
 * @param[out] dest Destination matrix
*/
FT_INLINE void get_perspective_mat4(float fovy, float aspect, float nearZ, float farZ, mat4_f32 dest) {
    float f, fn;

    mat_zero(dest);
    f  = 1.0f / tanf(fovy * 0.5f);
    fn = 1.0f / (nearZ - farZ);
    dest[0][0] = f / aspect;
    dest[1][1] = f;
    dest[2][2] = (nearZ + farZ) * fn;
    dest[2][3] =-1.0f;
    dest[3][2] = 2.0f * nearZ * farZ * fn;
}

/**
 *	@brief make rotation matrix
 *	@param m[out] Destination matrix
 *	@param angle[in] angle of rotation
 *	@param axis[in] axis of rotation
*/
FT_INLINE void make_rotatation(mat4_f32 m, float angle, vec3_f32 axis) {
  vec3_f32 axisn, v, vs;
  float c;

  c = cosf(angle);

  vec3_normalize_to(axis, axisn);
  vec3_scale(axisn, 1.0f - c, v);
  vec3_scale(axisn, sinf(angle), vs);

  vec3_scale(axisn, v[0], m[0]);
  vec3_scale(axisn, v[1], m[1]);
  vec3_scale(axisn, v[2], m[2]);

  m[0][0] += c;       m[1][0] -= vs[2];   m[2][0] += vs[1];
  m[0][1] += vs[2];   m[1][1] += c;       m[2][1] -= vs[0];
  m[0][2] -= vs[1];   m[1][2] += vs[0];   m[2][2] += c;

  m[0][3] = m[1][3] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0.0f;
  m[3][3] = 1.0f;
}


/**
 * @brief Mult mat4 by vector 4
 * @param m[in] operand matrix
 * @param v[in] operand vector
 * @param dest[out] Destination vec4
*/

FT_INLINE void mat4_mult_vec4(mat4_f32 m, vec4_f32 v, vec4_f32 dest)
{
    vec4_f32 res;
    res[0] = m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0] * v[3];
    res[1] = m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1] * v[3];
    res[2] = m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2] * v[3];
    res[3] = m[0][3] * v[0] + m[1][3] * v[1] + m[2][3] * v[2] + m[3][3] * v[3];
    ft_vec_copy(dest, res, sizeof(vec4_f32));
}

/**
 * @brief Mult mat4 by vector 3
 * @param m[in] operand matrix
 * @param v[in] operand vector
 * @param last[in] last element of vector
 * @param dest[out] Destination vec3
*/
FT_INLINE void mat4_mult_vec3(mat4_f32 mat, vec3_f32 v, float last, vec3_f32 dest) {
    vec4_f32 res;
    
    CREATE_VEC4(v[0], v[1], v[2], last, res);
    mat4_mult_vec4(mat, res, res);
    CREATE_VEC3(res[0], res[1], res[2], dest);
}

#endif /* MAT_HEADER_H */