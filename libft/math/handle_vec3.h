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
FT_INLINE void vec3_normalise(vec3_float v) {
    float norm = VEC3_NORM(v);
    if (norm <= FT_EPSILON) {
        ft_vec_zero(v, sizeof(vec3_float));
        return;
    }
    vec3_scale(v, (1.0f / norm), v);
}

/* cross product of vec3 */
FT_INLINE void vec3_cross(vec3_float a, vec3_float b, vec3_float dest) {
    dest[0] = a[1] * b[2] - a[2] * b[1];
    dest[1] = a[2] * b[0] - a[0] * b[2];
    dest[2] = a[0] * b[1] - a[1] * b[0];
}


#endif /* HANDLE_VEC3_H */