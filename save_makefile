NAME = scop

OPENGL_LIB= -lglfw -lGL -lm -L//usr/lib/x86_64-linux-gnu/

LIBFT = libft/libft.a

MAKE_LIBFT = make -s -C libft

all: ${NAME}

${NAME}: lib
	gcc src/main_opengl.c -o scop ${OPENGL_LIB} ${LIBFT}

lib:
	${MAKE_LIBFT}

fclean:
	rm -rf scop

re: fclean all

.PHONY: clean
