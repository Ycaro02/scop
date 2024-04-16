#include "../libft.h"
#include "vector_template.h"

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