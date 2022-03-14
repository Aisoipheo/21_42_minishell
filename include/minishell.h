/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 12:30:22 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/14 17:40:46 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "linked_list.h"
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>

// DEBUG
# define DEBUG_COLOUR "\033[1;30m"
# define RESET_COLOUR "\033[0m"

void	debug_log(const char *fmt, ...);

# define MINIS_PROMPT "(ง •̀_•́)ง "

# define CMD_APPEND 0b001
# define CMD_INSOURCE 0b010
# define CMD_SUBSHELL 0b100

int			g_exit;

typedef struct s_info	t_info;
typedef int				(*t_builtin_ptr)(t_llist *, t_info *);

struct s_info
{
	char						exit_f;
	char						envp_f;
	char						**envp;
	char						*g_exit_str;
	char						*reserved_words[7];
	t_builtin_ptr				f_ptrs[7];
	t_llist						*envp_list;
};

typedef struct s_cmd_info
{
	char	*in_file;
	char	*out_file;
	char	*delim;
	int		flags;
	int		_shlvl;
}	t_cmd_info;

typedef struct s_group
{
	int			type;
	t_llist		*cmds;
	t_llist		*files;
}	t_group;

typedef struct s_fd
{
	int		pfd[2];
	int		fds[2];
}	t_fd;

typedef struct s_expi
{
	unsigned int	size;
	int				f;
}	t_expi;

t_group	*ft_group_new(int type);
void	ft_group_free(void *group);

/* ============ Environment ============ */
int		ft_parse_envp(t_llist *list, char *envp[]);
char	**ft_compose_envp(t_llist *list);
/* ============ /Environqment ============ */

void	handler(int sig);
void	handler_term(t_info *info);
void	handler_in_executor(int sig);
void	handler_in_heredoc(int sig);

pid_t	executor(t_group *cmds, t_info *info);
pid_t	pipeline(t_group *cmds, t_info *info);
int		ft_common(t_group *cmds, t_info *info);
int		ft_subshell(t_group *cmds, t_info *info);
int		ft_acces(t_ll_elem *cmd, char *path, char **filepath);
int		create_argv(t_ll_elem *cmd, char ***args, char *path);
int		check_if_builtins(t_ll_elem *cmd, t_info *info);
int		ft_callbuiltin(int idx, t_ll_elem *cmd, t_info *info, t_fd *fd);
int		ft_execsubshell(t_ll_elem *cmd, t_info *info, t_fd *fd);
int		ft_execcommon(t_ll_elem *cmd, t_info *info, t_fd *fd, int mode);
int		ft_execve(t_ll_elem *cmd, t_info *info, t_fd *fd);
int		create_heredoc(t_cmd_info *c_info, t_llist *files);

int		ft_strncmp(const char *str1, const char *str2, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);

int		ft_init(t_info *info, char *envp[]);
int		ft_destroy(t_info *info);

/* ============ Built-ins ============ */
int		ft_echo(t_llist *args, t_info *info);
int		ft_cd(t_llist *args, t_info *info);
int		ft_pwd(t_llist *args, t_info *info);
int		ft_export(t_llist *args, t_info *info);
int		ft_unset(t_llist *args, t_info *info);
int		ft_env(t_llist *args, t_info *info);
int		ft_exit(t_llist *args, t_info *info);
/* ============ /Built-ins ============ */

int		get_line(const char *prompt, char **line);

#endif
