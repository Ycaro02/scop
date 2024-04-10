#ifndef DEFINE_VECTOR_HEADER
# define DEFINE_VECTOR_HEADER

# include <sys/types.h>

 /********************************************************************
 *                        DATA STRUCTURES                           *
 ********************************************************************/

/* Macro for write 2d point structure */
#define DEFINE_VEC2_STRUCT(type) \
typedef struct s_vec2_##type { \
    type x; \
    type y; \
} t_vec2_##type;

/* Define structure for all type */
DEFINE_VEC2_STRUCT(u8)
DEFINE_VEC2_STRUCT(u16)
DEFINE_VEC2_STRUCT(u32)
DEFINE_VEC2_STRUCT(u64)

DEFINE_VEC2_STRUCT(s8)
DEFINE_VEC2_STRUCT(s16)
DEFINE_VEC2_STRUCT(s32)
DEFINE_VEC2_STRUCT(s64)

/* Macro for easy access */
#define VEC2_GET_X(vec) (vec.x)
#define VEC2_GET_Y(vec) (vec.y)

/* Macro for create vector */
#define CREATE_VECTOR(type, y, x) ((t_vec2_##type){ .x = x, .y = y })

/* Macro for add vector */
#define ADD_VECTOR(type, a, b) ((t_vec2_##type){ .x = ((t_vec2_##type))a.x + ((t_vec2_##type))b.x, .y = ((t_vec2_##type))a.y + ((t_vec2_##type))b.y })

/* Macro for sub vector */
#define SUB_VECTOR(type, a, b) ((t_vec2_##type){ .x = ((t_vec2_##type))a.x - ((t_vec2_##type))b.x, .y = ((t_vec2_##type))a.y - ((t_vec2_##type))b.y })

/* Macro for multiply vector */
#define MULT_VEC2(type, a, b) ((t_vec2_##type){ .x = ((t_vec2_##type))a.x * ((t_vec2_##type))b.x, .y = ((t_vec2_##type))a.y * ((t_vec2_##type))b.y })

/* Macro for divide vector */
#define DIV_VECTOR(type, a, b) ((t_vec2_##type){ .x = ((t_vec2_##type))a.x / ((t_vec2_##type))b.x, .y = ((t_vec2_##type))a.y / ((t_vec2_##type))b.y })

/* Macro for compare two vectors */
#define VECTOR_CMP(type, a, b) (((t_vec2_##type))a.y == ((t_vec2_##type))b.y && ((t_vec2_##type))a.x == ((t_vec2_##type))b.x)


// ADD_VECTOR(u8, CREATE_VECTOR(u8, 1, 2), CREATE_VECTOR(u8, 3, 4))

# endif /* DEFINE_VECTOR_HEADER */
