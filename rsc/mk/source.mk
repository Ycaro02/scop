OBJ_DIR			=	obj

SRC_DIR 		=	src

SUB_SRC_DIR		=	parser\
					render\
					glad\
					win_event

ALL_SRC_DIR		=	$(OBJ_DIR) $(addprefix $(OBJ_DIR)/, $(SUB_SRC_DIR))

MAIN_MANDATORY 	=	main_opengl.c

SRCS			=	parser/obj_parse.c\
					parser/parse_line.c\
					parser/build_obj_model.c\
					parser/face_handling.c\
					render/camera.c\
					render/shader_utils.c\
					win_event/key_callback.c\
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