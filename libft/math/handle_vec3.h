#ifndef HANDLE_VEC3_H
#define HANDLE_VEC3_H 1

#include "vector_template.h"

/* Vec3 float manipulation*/

/**
 * @brief add vec3_float
 * @param a first operand vector
 * @param b second operand vector
 * @param dest Destination vector
*/
FT_INLINE void vec3_add(vec3_float a, vec3_float b, vec3_float dest) {
    dest[0] = a[0] + b[0];
    dest[1] = a[1] + b[1];
    dest[2] = a[2] + b[2];
}

/**
 * @brief sub vec3_float
 * @param a first operand vector
 * @param b second operand vector
 * @param dest Destination vector
*/
FT_INLINE void vec3_sub(vec3_float a, vec3_float b, vec3_float dest) {
    dest[0] = a[0] - b[0];
    dest[1] = a[1] - b[1];
    dest[2] = a[2] - b[2];
}

/* Dot product */
#define VEC3_DOT(a, b) (float)((a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]))

/* Dot on same target */
#define VEC3_NORM2(a) VEC3_DOT(a, a)

/* Vector norm */
#define VEC3_NORM(a)  sqrtf(VEC3_NORM2(a))

FT_INLINE void vec3_scale(vec3_float v, float s, vec3_float dest) {
  dest[0] = v[0] * s;
  dest[1] = v[1] * s;
  dest[2] = v[2] * s;
}

/* Normalise the v vector */
FT_INLINE void vec3_normalize(vec3_float v) {
    float norm = VEC3_NORM(v);
    if (norm <= FT_EPSILON) {
        ft_vec_zero(v, sizeof(vec3_float));
        return;
    }
    vec3_scale(v, (1.0f / norm), v);
}

/* Normalise the v vector */
FT_INLINE void vec3_normalize_to(vec3_float v, vec3_float dest) {
    float norm = VEC3_NORM(v);
    if (norm <= FT_EPSILON) {
        ft_vec_zero(dest, sizeof(vec3_float));
        return;
    }
    vec3_scale(v, (1.0f / norm), dest);
}

/* cross product of vec3 */
FT_INLINE void vec3_cross(vec3_float a, vec3_float b, vec3_float dest) {
    dest[0] = a[1] * b[2] - a[2] * b[1];
    dest[1] = a[2] * b[0] - a[0] * b[2];
    dest[2] = a[0] * b[1] - a[1] * b[0];
}


FT_INLINE void update_view_matrice(vec3_float eye, vec3_float center, vec3_float up, mat4_float dest) {
    vec3_float f, u, s;

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

FT_INLINE void get_perspective_mat4(float fovy, float aspect, float nearZ, float farZ, mat4_float dest) {
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

FT_INLINE void make_rotatation(mat4_float m, float angle, vec3_float axis) {
  vec3_float axisn, v, vs;
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

#endif /* HANDLE_VEC3_H */