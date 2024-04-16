#include "../libft.h"
#include "vector_template.h"

typedef vec4_float mat4_float[4];

/* Identity mat4 init */
#define MAT4_IDENTITY_INIT		{{1.0f, 0.0f, 0.0f, 0.0f},                   \
								{0.0f, 1.0f, 0.0f, 0.0f},                    \
								{0.0f, 0.0f, 1.0f, 0.0f},                    \
								{0.0f, 0.0f, 0.0f, 1.0f}}

FT_INLINE void mat4_copy(t_vec4_float *dest, t_vec4_float src) {
	ft_memcpy(dest, &src, (sizeof(t_vec4_float) * 4));
}

/* Copy identity mat4 in mat */
FT_INLINE void mat_identity(t_vec4_float *mat) {
	mat4_float tmp = MAT4_IDENTITY_INIT;
	ft_memcpy(mat, &tmp, (sizeof(t_vec4_float) * 4));
}

/* Multiply two mat4 */
FT_INLINE void mat4_mul(t_vec4_float *a, t_vec4_float *b, t_vec4_float *dest) {
	t_vec4_float tmp;
	int i;

	i = 0;
	while (i < 4) {
		tmp.x = a[i].x * b[0].x + a[i].y * b[1].x + a[i].z * b[2].x + a[i].w * b[3].x;
		tmp.y = a[i].x * b[0].y + a[i].y * b[1].y + a[i].z * b[2].y + a[i].w * b[3].y;
		tmp.z = a[i].x * b[0].z + a[i].y * b[1].z + a[i].z * b[2].z + a[i].w * b[3].z;
		tmp.w = a[i].x * b[0].w + a[i].y * b[1].w + a[i].z * b[2].w + a[i].w * b[3].w;
		dest[i] = tmp;
		i++;
	}
}