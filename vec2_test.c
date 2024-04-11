#include "libft/libft.h"


int main (void)
{
	t_vec2_s16 a = CREATE_VEC2(s16, 1, 2);

	t_vec2_u16 b = CREATE_VEC2(u16, 3, 4);

	t_vec2_float c = CREATE_VEC2(float, 5.443299, 6.223411);

	DISPLAY_VEC2(s16, a);
	DISPLAY_VEC2(u16, b);
	DISPLAY_VEC2(float, c);

	return (0);
}
