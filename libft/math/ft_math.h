#include "../basic_define.h"
#include <math.h>

#define FT_INLINE   static inline __attribute((always_inline))
#define FT_EPSILON  1e-5f

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

/* Cast macro */
#define FT_CAST(type, x) ((type)x)

/* Important PI val */
#define FT_PI        3.14159265358979323846264338327950288   /* pi          */
#define FT_PI_2      1.57079632679489661923132169163975144   /* pi/2        */
#define FT_PI_4      0.785398163397448309615660845819875721  /* pi/4        */
#define FT_1_PI      0.318309886183790671537767526745028724  /* 1/pi        */
#define FT_2_PI      0.636619772367581343075535053490057448  /* 2/pi        */
#define FT_2_SQRTPI  1.12837916709551257389615890312154517   /* 2/sqrt(pi)  */
#define FT_SQRT2     1.41421356237309504880168872420969808   /* sqrt(2)     */
#define FT_SQRT1_2   0.707106781186547524400844362104849039  /* 1/sqrt(2)   */

/* maybe not mandatory */
#define FT_E         2.71828182845904523536028747135266250   /* e           */
#define FT_LOG2E     1.44269504088896340735992468100189214   /* log2(e)     */
#define FT_LOG10E    0.434294481903251827651128918916605082  /* log10(e)    */
#define FT_LN2       0.693147180559945309417232121458176568  /* loge(2)     */
#define FT_LN10      2.30258509299404568401799145468436421   /* loge(10)    */