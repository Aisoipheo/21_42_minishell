NAME				= minishell

HDRS_DIR			= ./include
SRCS_DIR			= ./src
OBJS_DIR			= ./obj

C_FILES				= main.c\
					linked_list.c\
					linked_list2.c\
					linked_list3.c\
					environment.c\
					norm_hacks.c\
					utils.c\
					utils2.c\
					utils3.c\
					utils4.c\
					utils5.c\
					lexer.c\
					lexer2.c\
					parser.c\
					parser2.c\
					parser3.c\
					parser4.c\
					parser5.c\
					parser6.c\
					parser7.c\
					parser8.c\
					parser9.c\
					parser10.c\
					parser11.c\
					group.c\
					signal.c\
					utils_libft.c\
					executor.c\
					executor2.c\
					executor3.c\
					pipeline.c\
					subshell.c\
					builtin.c\
					builtin2.c\
					builtin3.c\
					destroy.c\
					init.c\
					heredoc.c

SRCS				= $(addprefix $(SRCS_DIR)/, $(C_FILES))
OBJS				= $(addprefix $(OBJS_DIR)/, $(C_FILES:.c=.o))
DEPENDENCIES		= $(OBJS:.o=.d)

RL_DIR				= /Users/$(USER)/.brew/Cellar/readline/8.1.?

CC					= gcc
CFLAGS				= -g -Wall -Werror -Wextra -I $(HDRS_DIR) -I $(RL_DIR)/include/
LFLAGS				= $(CFLAGS) -lreadline -L $(RL_DIR)/lib/

AR					= ar
ARFLAGS				= rcs

RM					= rm
RMFLAGS				= -rf

MKDIR				= mkdir
MKDIRFLAGS			= -p

.PHONY : all clean fclean re bonus norm
.DEFAULT_GOAL := all

-include $(DEPENDENCIES)

$(OBJS_DIR)/%.o : $(SRCS_DIR)/%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@ -MD

$(OBJS_DIR) :
	$(MKDIR) $(MKDIRFLAGS) $(OBJS_DIR)

$(NAME) : $(SRCS) $(OBJS_DIR) $(OBJS) Makefile
	$(CC) $(LFLAGS) -o $@ $(OBJS)

all : $(NAME)

bonus : $(NAME)

clean :
	$(RM) $(RMFLAGS) $(OBJS) $(DEPENDENCIES)

fclean : clean
	$(RM) $(RMFLAGS) $(NAME) $(OBJS_DIR)

norm :
	norminette $(SRCS_DIR) $(HDRS_DIR)

val : $(NAME)
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./$(NAME)

re: fclean all
