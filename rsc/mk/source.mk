OBJ_DIR			=	obj

SRC_DIR 		=	src

PARSER_DIR		=	parser

MAIN_MANDATORY 	=	main_opengl.c

SRCS			=	parser/obj_parse.c\
					parser/load_file.c\


# SRCS_BONUS		=	main_bonus.c

MAKE_LIBFT		=	make -s -C libft

MAKE_LIST		=	make -s -C libft/list

LIBFT			= 	libft/libft.a

LIST			= 	libft/list/linked_list.a

OBJS 			= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

RM			=	rm -rf

ALL_DIR		= $(OBJ_DIR) $(OBJ_DIR)/$(PARSER_DIR)

# ifeq ($(findstring bonus, $(MAKECMDGOALS)), bonus)
# ASCII_NAME	= "bonus"
# SRCS += $(SRCS_BONUS)
# else
# SRCS += $(MAIN_MANDATORY)
# endif