OBJ_DIR			=	obj

SRC_DIR 		=	src

SUB_SRC_DIR		=	parser\
					render\
					glad\
					window\
					win_event\
					builder

ALL_SRC_DIR		=	$(OBJ_DIR) $(addprefix $(OBJ_DIR)/, $(SUB_SRC_DIR))

MAIN_MANDATORY 	=	main_opengl.c

SRCS			=	parser/obj_parse.c\
					parser/parse_line.c\
					parser/parse_utils.c\
					parser/material_parse.c\
					parser/parse_bmp.c\
					window/openGL_glw_init.c\
					builder/face_handling.c\
					builder/build_triangle_lst.c\
					builder/build_obj_model.c\
					builder/texture.c\
					builder/colors.c\
					win_event/key_callback.c\
					render/shader_utils.c\
					render/camera.c\
					glad/gl.c\


# SRCS_BONUS		=	main_bonus.c

MAKE_LIBFT		=	make -s -C libft -j

MAKE_LIST		=	make -s -C libft/list -j

LIBFT			= 	libft/libft.a

LIST			= 	libft/list/linked_list.a

OBJS 			= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

RM			=	rm -rf

ifeq ($(findstring bonus, $(MAKECMDGOALS)), bonus)
ASCII_NAME	= "bonus"
SRCS += $(SRCS_BONUS)
else
SRCS += $(MAIN_MANDATORY)
endif