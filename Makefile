NAME				= minishell

HDRS_DIR			= ./include
SRCS_DIR			= ./src
OBJS_DIR			= ./obj

C_FILES				= main.c\
					linked_list.c\
					linked_list2.c\
					environment.c\
					norm_hacks.c\
					utils.c\
					utils2.c\
					utils3.c\
					lexer.c\
					lexer2.c

SRCS				= $(addprefix $(SRCS_DIR)/, $(C_FILES))
OBJS				= $(addprefix $(OBJS_DIR)/, $(C_FILES:.c=.o))
DEPENDENCIES		= $(OBJS:.o=.d)

RL_DIR				= /Users/$(USER)/.brew/Cellar/readline/8.1.1

CC					= gcc
CFLAGS				= -Wall -Werror -Wextra -I$(HDRS_DIR) -I$(RL_DIR)/include
LFLAGS				= $(CFLAGS) -L$(RL_DIR)/lib/ -lreadline

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

re: fclean all
