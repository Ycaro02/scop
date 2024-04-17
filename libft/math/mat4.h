#ifndef MAT4_HEADER_H
#define MAT4_HEADER_H

#include "ft_math.h"

void	*ft_memcpy(void *dest, const void *src, size_t n);

typedef vec4_float mat4_float[4];

/* Identity mat4 init */
#define MAT4_IDENTITY_INIT		{{1.0f, 0.0f, 0.0f, 0.0f},                   \
								{0.0f, 1.0f, 0.0f, 0.0f},                    \
								{0.0f, 0.0f, 1.0f, 0.0f},                    \
								{0.0f, 0.0f, 0.0f, 1.0f}}

/* Copy identity mat4 in mat */
FT_INLINE void mat_identity(vec4_float *mat) {
	mat4_float tmp = MAT4_IDENTITY_INIT;
	ft_memcpy(mat, &tmp, (sizeof(vec4_float) * 4));
}

FT_INLINE void mat_zero(mat4_float mat) {
	ft_bzero(mat, sizeof(mat4_float));
}

#endif /* MAT_HEADER_H */