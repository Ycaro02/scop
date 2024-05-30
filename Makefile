include libft/rsc/mk/color.mk
include rsc/mk/source.mk

NAME			=	scop
CC				=	gcc
CFLAGS			=	-Wall -Wextra -Werror -O3 -g 
# ASCII_ART		=	./rsc/mk/ascii.sh
ASCII_NAME		=	${NAME}


OPENGL_LIB		= -lglfw -lGL -lm -Lrsc/lib_deps/

MODEL_42		= rsc/model/42.obj
VALGRIND_TEST	= valgrind --suppressions=rsc/vsupp/42_vsupp.supp --leak-check=full ./scop rsc/model/42.obj aa

ARGS			=	rsc/42.obj

all:		$(NAME)

%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME):	$(LIBFT) $(LIST) $(OBJ_DIR) $(OBJS) $(DISPLAY_NAME)
	@printf "$(CYAN)Compiling ${NAME} ...$(RESET)\n"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(LIST) ${OPENGL_LIB}
	@printf "$(GREEN)Compiling $(NAME) done$(RESET)\n"

$(LIST):
ifeq ($(shell [ -f ${LIST} ] && echo 0 || echo 1), 1)
	@printf "$(CYAN)Compiling list...$(RESET)\n"
	@$(MAKE_LIST)
	@printf "$(GREEN)Compiling list done$(RESET)\n"
endif

$(LIBFT):
ifeq ($(shell [ -f ${LIBFT} ] && echo 0 || echo 1), 1)
	@printf "$(CYAN)Compiling libft...$(RESET)\n"
	@$(MAKE_LIBFT)
	@printf "$(GREEN)Compiling libft done$(RESET)\n"
endif

$(OBJ_DIR):
	mkdir $(ALL_SRC_DIR)
# @$(ASCII_ART) $(ASCII_NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@printf "$(YELLOW)Compile $<$(RESET)\n"
	@$(CC) -o $@ -c $< $(CFLAGS)

bonus: clear_mandatory ${NAME}

clear_mandatory:
ifeq ($(shell [ -f ${OBJ_DIR}/main.o ] && echo 0 || echo 1), 0)
	@printf "$(RED)Clean mandatory obj $(RESET)\n"
	@rm -rf ${OBJ_DIR}
endif

clean:
ifeq ($(shell [ -d ${OBJ_DIR} ] && echo 0 || echo 1), 0)
	@$(RM) $(OBJ_DIR)
	@printf "$(RED)Clean $(OBJ_DIR)/test output$(RESET)\n"
	@$(RM) ${TESTER_OUT_FILES}
endif

fclean:		clean
	@printf "$(RED)Clean $(NAME)/lib$(RESET)\n"
	@$(RM) $(NAME) ${TESTER_OUT_FILES} ${DISPLAY_NAME}

clean_lib:
	@$(MAKE_LIBFT) fclean
	@$(MAKE_LIST) fclean

test: $(NAME)
	@printf "$(YELLOW)Test $(NAME)$(RESET)\n"
	@./$(NAME) $(MODEL_42)

vtest: $(NAME)
	@$(VALGRIND_TEST)

re:			fclean all

.PHONY:		all clean fclean re bonus
