/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat4.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfour <nfour@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 12:14:49 by nfour             #+#    #+#             */
/*   Updated: 2024/04/18 12:14:49 by nfour            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAT4_HEADER_H
#define MAT4_HEADER_H

#include "ft_math.h"

/* Used function from libft */
extern void	*ft_memcpy(void *dest, const void *src, size_t n);

/* Matrice float 4x4 */
typedef vec4_f32 mat4_f32[4];

/* Identity mat4 init */
#define MAT4_IDENTITY_INIT		{{1.0f, 0.0f, 0.0f, 0.0f},                   \
								{0.0f, 1.0f, 0.0f, 0.0f},                    \
								{0.0f, 0.0f, 1.0f, 0.0f},                    \
								{0.0f, 0.0f, 0.0f, 1.0f}}

/* Get identity matrice in mat */
FT_INLINE void mat_identity(vec4_f32 *mat) {
	mat4_f32 tmp = MAT4_IDENTITY_INIT;
	ft_memcpy(mat, &tmp, (sizeof(vec4_f32) * 4));
}

/* Call ft_bzero on mat4x4 float */
FT_INLINE void mat_zero(mat4_f32 mat) {
	ft_bzero(mat, sizeof(mat4_f32));
}

/**
 * @brief	Update view mattrice 4x4
 *
 * This function calculates a view matrix that 
 * transforms points from world space to camera space in 3D graphics.
 * The view matrix is based on the camera's position (eye),
 * the point the camera is looking at (center), and the camera's up direction (up)
 *
 * @param[in] eye position of the eye
 * @param[in] center position of the center
 * @param[in] up up vector
 * @param[out] dest Destination matrix
*/
FT_INLINE void update_view_matrice(vec3_f32 eye, vec3_f32 center, vec3_f32 up, mat4_f32 dest) {
    vec3_f32 f, u, s;

	/*	Compute the the forward vector f by subtracting the eye vector 
		(representing the camera's position) from the center vector 
		(representing the point the camera is looking at) */
    vec3_sub(center, eye, f);

	/*	Normalizes the forward vector f.
		Normalizing a vector scales it so that its length (or magnitude) is 1.*/
    vec3_normalize(f);

	/*	Compute the right vector s as the cross product of
		(the forward vector f and the up vector up), and normalize it */
    vec3_cross(f, up, s);
    vec3_normalize(s);

	/* Compute the up vector u as the cross product of the right vector s and the forward vector f*/
    vec3_cross(s, f, u);

    dest[0][0] = s[0]; dest[0][1] = u[0]; dest[0][2] =-f[0]; 
    dest[1][0] = s[1]; dest[1][1] = u[1]; dest[1][2] =-f[1]; 
    dest[2][0] = s[2]; dest[2][1] = u[2]; dest[2][2] =-f[2];

    /* Compute the translation part of the view matrix */
	dest[3][0] =-VEC3_DOT(s, eye);
    dest[3][1] =-VEC3_DOT(u, eye);
    dest[3][2] = VEC3_DOT(f, eye);
	
	/* Last column is usualy [0,0,0,1] for affine transformation */
    dest[0][3] = dest[1][3] = dest[2][3] = 0.0f;
    dest[3][3] = 1.0f;  
}

/**
 * @brief Get perspective matrice 4x4.
 *
 * This function calculates a perspective projection matrix for 3D graphics.
 * The perspective matrix is based on the field of view (fovy), aspect ratio (aspect),
 * and near and far clipping planes (nearZ, farZ).
 *
 * @param[in] fovy Field of view in radians.
 * @param[in] aspect Aspect ratio (width / height).
 * @param[in] nearZ Near clipping plane distance.
 * @param[in] farZ Far clipping plane distance.
 * @param[out] dest Destination matrix to store the resulting perspective matrix.
 */
FT_INLINE void get_perspective_mat4(float fovy, float aspect, float nearZ, float farZ, mat4_f32 dest) {
    float f, fn;

    /* Initialize the destination matrix to zero */
    mat_zero(dest);

    /* Calculate the cotangent of the field of view divided by 2 */
    f  = 1.0f / tanf(fovy * 0.5f);

    /* Calculate the reciprocal of the difference between the near and far clipping planes */
    fn = 1.0f / (nearZ - farZ);

    dest[0][0] = f / aspect;                /* Scale factor for x-coordinate */
    dest[1][1] = f;                         /* Scale factor for y-coordinate */
    dest[2][2] = (nearZ + farZ) * fn;       /* Depth range adjustment */
    dest[2][3] = -1.0f;                     /* Homogeneous division for z-coordinate */
    dest[3][2] = 2.0f * nearZ * farZ * fn;  /* Depth range adjustment */
}

/**
 * @brief Make rotation matrix.
 *
 * This function creates a rotation matrix around a specified axis.
 *
 * @param[out] m Destination matrix to store the resulting rotation matrix.
 * @param[in] angle Angle of rotation in radians.
 * @param[in] axis Axis of rotation.
 */
FT_INLINE void make_rotatation(mat4_f32 m, float angle, vec3_f32 axis) {
    vec3_f32 axisn, v, vs;
    float c;

    /* Calculate the cosine of the angle */
    c = cosf(angle);

    /* Normalize the axis of rotation */ 
    vec3_normalize_to(axis, axisn);

    /* Precompute values for the rotation calculation */
    vec3_scale(axisn, 1.0f - c, v);
    vec3_scale(axisn, sinf(angle), vs);

    /* Populate the rotation matrix */
    vec3_scale(axisn, v[0], m[0]);
    vec3_scale(axisn, v[1], m[1]);
    vec3_scale(axisn, v[2], m[2]);

    m[0][0] += c;       m[1][0] -= vs[2];   m[2][0] += vs[1];
    m[0][1] += vs[2];   m[1][1] += c;       m[2][1] -= vs[0];
    m[0][2] -= vs[1];   m[1][2] += vs[0];   m[2][2] += c;
 
    /* Set the fourth row and column for homogenous coordinates */
    m[0][3] = m[1][3] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0.0f;
    m[3][3] = 1.0f;
}

/**
 * @brief Mult mat4 by vector 4.
 *
 * This function multiplies a 4x4 matrix by a 4-dimensional vector.
 *
 * @param[in] m Operand matrix.
 * @param[in] v Operand vector.
 * @param[out] dest Destination vector to store the result.
 */
FT_INLINE void mat4_mult_vec4(mat4_f32 m, vec4_f32 v, vec4_f32 dest) {
    vec4_f32 res;

    /* Perform matrix-vector multiplication */
    res[0] = m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0] * v[3];
    res[1] = m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1] * v[3];
    res[2] = m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2] * v[3];
    res[3] = m[0][3] * v[0] + m[1][3] * v[1] + m[2][3] * v[2] + m[3][3] * v[3];

    ft_vec_copy(dest, res, sizeof(vec4_f32));
}

/**
 * @brief Mult mat4 by vector 3.
 *
 * This function multiplies a 4x4 matrix by a 3-dimensional vector.
 *
 * @param[in] m Operand matrix.
 * @param[in] v Operand vector.
 * @param[in] last Last element of the vector.
 * @param[out] dest Destination vector to store the result.
 */
FT_INLINE void mat4_mult_vec3(mat4_f32 mat, vec3_f32 v, float last, vec3_f32 dest) {
    vec4_f32 res;

    /* Create a 4-dimensional vector from the input 3-dimensional vector and last element */
    CREATE_VEC4(v[0], v[1], v[2], last, res);

    /* Perform matrix-vector multiplication */
    mat4_mult_vec4(mat, res, res);

    /* Extract the resulting 3-dimensional vector from the 4-dimensional vector */
    CREATE_VEC3(res[0], res[1], res[2], dest);
}


#endif /* MAT_HEADER_H */