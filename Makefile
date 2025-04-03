#---COMMON_VAR-----------------------------------
NAME					=	ft_ssl
CC						=	cc
RM						=	rm -rf

RED						=	\033[0;31m
GREEN					=	\033[0;32m
YELLOW					=	\033[0;33m
NC						=	\033[0m

#---FT_SSL_VAR-----------------------------------
SRC						=	srcs/main.c \
							

OBJS_DIR				=	.OBJS/
OBJS					=	$(addprefix $(OBJS_DIR), $(SRC:.c=.o))

INC_DIR					=	include/
INC_FILE				=	include/ft_ssl.h 

FLAGS					=	-Wall -Werror -Wextra -g
INCLUDES				=	-I $(INC_DIR)

#---RULES----------------------------------------

all:			$(NAME)

$(NAME):		$(OBJS_DIR) Makefile $(INC_FILE) $(OBJS)
				$(CC) $(FLAGS) $(INCLUDES) $(OBJS) -o $@
				@echo "\33[2K\r$(GREEN)$(NAME) compiled :D$(NC)"


$(OBJS_DIR)%.o:	%.c $(INC_FILE)
				@mkdir -p $(shell dirname $@)
				@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@
				@echo "\33[2K\r$(YELLOW)Compiled $<$(NC)"

$(OBJS_DIR):
				mkdir -p $(OBJS_DIR)

clean:
				@echo "$(RED)Deleting $(NAME) object...\n"
				$(RM) $(OBJS_DIR)
				@echo "$(GREEN)Done\n$(ND)"

fclean:			clean
				@echo "$(RED)Deleting $(NAME) executable..."
				rm -f $(NAME)
				@echo "$(GREEN)Done\n$(ND)"

re:				fclean all

.PHONY:			all clean fclean re
