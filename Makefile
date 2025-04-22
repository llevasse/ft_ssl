#---COMMON_VAR-----------------------------------
NAME					=	ft_ssl
NAME_SAN	=	ft_ssl_san
CC						=	cc
RM						=	rm -rf

RED						=	\033[0;31m
GREEN					=	\033[0;32m
YELLOW					=	\033[0;33m
NC						=	\033[0m

#---FT_SSL_VAR-----------------------------------
SRC						=	srcs/main.c \
											srcs/utils.c \
											srcs/md5.c \
											srcs/sha256.c \
											srcs/base64.c \
											srcs/rsa.c
							

OBJS_DIR				=	.OBJS/
OBJS_DIR_SAN				=	.OBJS_SAN/
OBJS					=	$(addprefix $(OBJS_DIR), $(SRC:.c=.o))
OBJS_SAN					=	$(addprefix $(OBJS_DIR_SAN), $(SRC:.c=_san.o))

INC_DIR					=	include/
INC_FILE				=	include/ft_ssl.h \
									include/ft_md5.h \
									include/ft_sha256.h

FLAGS					=	-Wall -Werror -Wextra -g
INCLUDES				=	-I $(INC_DIR) 

#---RULES----------------------------------------

all:			$(NAME)

$(NAME):		$(OBJS_DIR) Makefile $(INC_FILE) $(OBJS) $(OBJS_SAN)
				@$(CC) $(FLAGS) $(INCLUDES) $(OBJS) -D FT_SSL_MD5=1 -lm -o $@
				@$(CC) $(FLAGS) -fsanitize=address $(INCLUDES) $(OBJS_SAN) -D FT_SSL_MD5=1 -lm -o ${NAME_SAN}
				@echo "\33[2K\r$(GREEN)$(NAME) compiled :D$(NC)"

$(OBJS_DIR)%.o:	%.c $(INC_FILE)
				@mkdir -p $(shell dirname $@)
				@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@
				@echo "\33[2K\r$(YELLOW)Compiled $<$(NC)"

$(OBJS_DIR_SAN)%_san.o:	%.c $(INC_FILE)
				@mkdir -p $(shell dirname $@)
				@$(CC) $(FLAGS) -fsanitize=address $(INCLUDES) -c $< -o $@
				@echo "\33[2K\r$(YELLOW)Compiled $< with -fsanitize=address$(NC)"

$(OBJS_DIR):
				mkdir -p $(OBJS_DIR)

clean:
				@echo "$(RED)Deleting $(NAME) object...\n"
				$(RM) $(OBJS_DIR) $(OBJS_DIR_SAN)
				@echo "$(GREEN)Done\n$(ND)"

fclean:			clean
				@echo "$(RED)Deleting $(NAME) executable..."
				rm -f $(NAME) $(NAME_SAN)
				@echo "$(GREEN)Done\n$(ND)"

re:				fclean all

test:	all
				bash tester.sh "test" "this should work" "a" "" "42"

.PHONY:			all clean fclean re
