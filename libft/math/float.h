#include "ft_math.h"

/* Check float eqauel without epsilon */
FT_INLINE int8_t float_equal(float a, float b) {
	return (fabs(a - b) < FT_EPSILON);
}

/* Convert degres to radian  */
FT_INLINE float deg_to_rad(float deg) {
	return (float)(deg * (float)M_PI / 180.0f);
}

/* Convert radian to degres */
FT_INLINE float rad_to_deg(float rad) {
	return (float)(rad * 180.0f / (float)M_PI);
}