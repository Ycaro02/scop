OBJ_DIR			=	obj

SRC_DIR 		=	src

PARSER_DIR		=	parser

GLAD_DIR		=	glad

MAIN_MANDATORY 	=	main_opengl.c

SRCS			=	parser/obj_parse.c\
					parser/load_file.c\
					parser/parse_line.c\
					parser/build_obj_model.c\
					glad/gl.c\


# SRCS_BONUS		=	main_bonus.c

MAKE_LIBFT		=	make -s -C libft

MAKE_LIST		=	make -s -C libft/list

LIBFT			= 	libft/libft.a

LIST			= 	libft/list/linked_list.a

OBJS 			= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

RM			=	rm -rf

ALL_DIR		= $(OBJ_DIR) $(OBJ_DIR)/$(PARSER_DIR) $(OBJ_DIR)/${GLAD_DIR}

ifeq ($(findstring bonus, $(MAKECMDGOALS)), bonus)
ASCII_NAME	= "bonus"
SRCS += $(SRCS_BONUS)
else
SRCS += $(MAIN_MANDATORY)
endif